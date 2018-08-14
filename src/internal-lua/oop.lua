local sub = string.sub
local unpack = table.unpack or unpack
local rawget, rawset = rawget, rawset
local setmetatable = setmetatable
local getmetatable = getmetatable
local type = type
local tostring = tostring
local newproxy = newproxy
local LUA_VERSION = tonumber((__VERSION or ''):gsub('[^%d.]', ''), 10) or 0
local debug_getinfo = debug.getinfo
local string_format = string.format

local lastCreatedClass = nil

local function class__call (class, ...)
    return class.new(...)
end

local function class__newindex (class, index, value)
    if type(index) == 'string' then
        if index == 'destructor' or index == '__gc' then
            if LUA_VERSION >= 5.2 then
                rawset(class._internal_instance_meta, '__gc', value)
            end
            rawset(class, 'destructor', value)
        elseif sub(index, 1, 2) == '__' then
            rawset(class._internal_instance_meta, index, value)
            if index == '__tostring' then
                rawset(class, 'toString', value)
            end
        else
            rawset(class, index, value)
        end
    else
        rawset(class, index, value)
    end
end

local function class__tostring (class)
    return string_format('[class %s%s]', class.name, class.src)
end

local function extends (class_or_parent, parent)
    local obj = parent and class_or_parent or lastCreatedClass
    local class = parent or class_or_parent
    if type(class) == 'string' then
        class = _G[class]
    elseif type(class) ~= 'table' or _G[class.name] == class then
        error('Invalid parent class inheritance')
    end
    obj._internal_baseClass = class
    if class._internal_instance_meta.__tostring then
        obj._internal_instance_meta.__tostring = class._internal_instance_meta.__tostring
    end
    return obj, class
end

local propagationMeta = {}

local PROP_INDEX_LEVEL_CLASS = 1
local PROP_INDEX_INSTANCE = 2
local PROP_INDEX_LEVEL = 3
local PROP_INDEX_METHOD_LEVEL = 4

local function getSuper (instance, level, levelClass, methodLevel)
    return setmetatable({
        levelClass,
        instance,
        level,
        methodLevel
    }, propagationMeta)
end

local function getClassParent (class, levels)
    if not levels or levels <= 0 then
        return class
    end
    while levels > 0 and class do
        levels = levels - 1
        class = class._internal_baseClass
    end
    return class
end

propagationMeta.__index = function (propagationTable, index)
    local instance = rawget(propagationTable, PROP_INDEX_INSTANCE)

    local ret = rawget(instance, index)

    if ret then
        return ret
    end

    local thisClass = rawget(propagationTable, PROP_INDEX_LEVEL_CLASS)
    local currentLevel = rawget(propagationTable, PROP_INDEX_LEVEL)
    local methodLevel = rawget(propagationTable, PROP_INDEX_METHOD_LEVEL)

    if index == 'isPropagation' then
        return currentLevel
    elseif index == 'super' then
        local base = thisClass._internal_baseClass
        if not base then
            return
        end
        return getSuper(instance, currentLevel + 1, base, methodLevel)
    end

    local indexLevel = methodLevel[index]

    if not indexLevel then
        if methodLevel[1] then
            methodLevel[1] = false
            indexLevel = currentLevel
        else
            indexLevel = 0
        end
    end

    if indexLevel < currentLevel then
        local class = getClassParent(instance._class, indexLevel)
        propagationTable = getSuper(instance, indexLevel, class, methodLevel)
        return propagationTable[index]
    end

    ret = thisClass[index]

    if ret then
        if type(ret) == 'function' then
            return function (self, ...)
                methodLevel[index] = indexLevel + 1
                local results = { ret(propagationTable, ...) }
                methodLevel[index] = indexLevel
                return unpack(results)
            end
        else
            return ret
        end
    end

    if not thisClass._internal_baseClass then
        return
    end

    methodLevel[index] = (methodLevel[index] or 0) + 1
    propagationTable = getSuper(instance, currentLevel + 1, thisClass._internal_baseClass, methodLevel)

    return propagationTable[index]
end

propagationMeta.__call = function (propagationTable, ...)
    local thisClass = rawget(propagationTable, 1)
    local constructor = thisClass and rawget(thisClass, 'constructor')
    return constructor and constructor(propagationTable, ...)
end

propagationMeta.__newindex = function (propagationTable, index, value)
    rawget(propagationTable, PROP_INDEX_INSTANCE)[index] = value
end

propagationMeta.__tostring = function (propagationTable)
    local instance = rawget(propagationTable, PROP_INDEX_INSTANCE)
    return tostring(instance) .. '_' .. rawget(propagationTable, PROP_INDEX_LEVEL)
end

function getInstanceSuperLevel (instance)
    local meta = getmetatable(instance)
    if meta == propagationMeta then
        return rawget(instance, PROP_INDEX_LEVEL)
    else
        return 0
    end
end

function getInstanceAsBase (instance)
    local meta = getmetatable(instance)
    if meta == propagationMeta then
        return rawget(instance, PROP_INDEX_INSTANCE)
    else
        return instance
    end
end

function Class (className)
    if className and _G[className] then
        return error('Redefining class ' .. className, 0)
    end

    local classDeclarationInfo = debug_getinfo(2)

    local class = {
        _internal_baseClass = nil,
        extends = extends,
        name = className or '<anonymous>'
    }

    if classDeclarationInfo then
        class.src = classDeclarationInfo.source .. ':' .. classDeclarationInfo.currentline
    end

    class.__index = function (instance, index)

        local metamethodLookup = index == '__tostring'
        local ret = rawget(instance, index) or class[index]

        if metamethodLookup and not ret then
            ret = class._internal_instance_meta[index]
        end

        if ret then
            return ret
        elseif index == 'isPropagation' then
            return false
        end

        if not class._internal_baseClass then
            return
        end

        local methodLevel = { true }
        local super = getSuper(instance, 1, class._internal_baseClass, methodLevel)

        if index == 'super' then
            return super
        else
            return super[index]
        end
    end

    local function cast (instance)
        rawset(instance, '_class', class)
        instance = setmetatable(instance, class._internal_instance_meta)
        if class.destructor and newproxy then
            local proxy = newproxy()
            rawset(instance, '__gc_proxy', proxy)
            getmetatable(proxy).__gc = function()
                local baseClass = class._internal_baseClass
                while baseClass do
                    if baseClass.destructor then
                        baseClass.destructor(instance)
                    end
                    baseClass = baseClass._internal_baseClass
                end
                class.destructor(instance)
            end
        end
        return instance
    end

    local function create (instance, ...)
        local args = { ... }
        local info = debug_getinfo(2)
        if info and not instance.src then
            instance.src = info.source .. ':' .. info.currentline
        end
        instance = cast(instance)
        if class.constructor then
            local construction = class.constructor(instance, unpack(args, args[1] == class and 2 or 1))
            if construction ~= nil then
                instance = construction
            end
        else
            local base = class._internal_baseClass
            while base do
                if base.constructor then
                    local construction = base.constructor(instance, unpack(args, args[1] == class and 2 or 1))
                    if construction ~= nil then
                        instance = construction
                    end
                    break
                end
                base = base._internal_baseClass
            end
        end
        return instance
    end

    class.cast = cast
    class.create = create
    class.class = class
    class.new = function (...) return create({}, ...) end
    class._internal_instance_meta = { __index = class.__index }
    class.getClassName = function () return className end
    setmetatable(class, { __newindex = class__newindex, __call = class__call, __tostring = class__tostring })
    if className then
        _G[className] = class
    end
    lastCreatedClass = class
    return lastCreatedClass
end

function instanceof (thing, class)
    if type(thing) ~= 'table' then
        return
    end
    if type(class) == 'string' then
        class = _G[class]
        if not class then
            return
        end
    end
    local c = thing._class
    while c do
        if c.name == class.name then
            return true
        end
        c = c._internal_baseClass
    end
end

function typeof (thing, check)

    local t = type(thing)

    if check then
        if t == 'table' then
            if check == 'table' then
                return true
            end
            local class = thing._class
            if class and class.name == check then
                return true
            end
            local getClassName = thing.getClassName
            if getClassName and getClassName() == check then
                return true
            end
            local baseClass = thing.baseClass
            if baseClass then
                return typeof(baseClass, check)
            end
        end
        return t == check
    end

    if t == 'table' then
        local class = thing._class
        if class then
            return class.name
        end
        class = thing.getClassName
        if class then
            return class()
        end
    end
end
