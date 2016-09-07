local sub = string.sub
local unpack = table.unpack
local rawget = rawget
local setmetatable = setmetatable

local function class__call(class, ...)
	return class.new(...)
end

local function extends(derived, baseName)
	derived._internal_baseClass = _G[baseName]
end

local propagationMeta = {}

propagationMeta.__index = function(propagationTable, index)
	local thisClass = rawget(propagationTable, 1)
	local instance = rawget(propagationTable, 2)
	
	local ret = rawget(instance, index) or (thisClass and thisClass[index])
	if ret then return ret end
	
	if index == 'super' and thisClass and thisClass._internal_baseClass then
		return setmetatable({thisClass._internal_baseClass, instance}, propagationMeta)
	end
end

propagationMeta.__call = function(propagationTable, ...)
	local thisClass = rawget(propagationTable, 1)
	local ctor = thisClass and rawget(thisClass, 'constructor')
	return ctor and ctor(propagationTable, ...)
end

propagationMeta.__newindex = function(propagationTable, index, value)
	rawget(propagationTable, 2)[index] = value
end

function Class (className)
	local class = {_internal_baseClass = nil, extends = extends}
	class._internal_instance_meta = {
		__index = function (instance, index)
			local ret = rawget(instance, index) or class[index]
			if ret then return ret end
			local base = class._internal_baseClass
			while base do
				ret = base[index]
				if ret then
					return ret
				end
				base = base._internal_baseClass
			end
			if index == 'super' and class._internal_baseClass then
				return setmetatable({class._internal_baseClass, instance}, propagationMeta)
			end
		end,
		__gc = function (instance)
			if class.destructor then 
				class.destructor(instance) 
				if class._internal_baseClass and class._internal_baseClass.destructor then
					class._internal_baseClass.destructor(instance)
				end
			end 
		end} 
	class.new = function(...)
		local args = {...} 
		local instance = --[[class.baseClass and class.baseClass.new({}) or]] {}
		setmetatable(instance, class._internal_instance_meta) 
		if class.constructor then
			local construction = class.constructor(instance, unpack(args, args[1] == class and 2 or 1))
			if construction ~= nil then
				instance = construction
			end
		end 
		return instance 
	end
	local function getName() return className or '?' end
	class.getClassName = getName class.typeof = className
	setmetatable(class, {__newindex = function (class, index, value)
		if type(index) == 'string' and sub(index, 1, 2) == '__' then
			rawset(class._internal_instance_meta, index, value)
		else 
			rawset(class, index, value)
		end 
	end, __call = class__call})
	if className then 
		_G[className] = class 
	end 
	return class 
end

function typeof(thing, check)

	local t = type(thing)

	if check then
		if t == 'table' then
			return check == 'table' or thing.getClassName and
			thing.getClassName() == check or thing.baseClass and
			typeof(thing.baseClass, check)
		end
		return t == check
	end
	
	return t == 'table' and thing.getClassName and thing.getClassName() or t
end