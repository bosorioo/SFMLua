/************
* Generated with lua2header.lua
* Original source code:
*
* 1 - local sub = string.sub
* 2 - local unpack = table.unpack or unpack
* 3 - local rawget, rawset = rawget, rawset
* 4 - local setmetatable = setmetatable
* 5 - local getmetatable = getmetatable
* 6 - local type = type
* 7 - local tostring = tostring
* 8 - local newproxy = newproxy
* 9 - local LUA_VERSION = tonumber((__VERSION or ''):gsub('[^%d.]', ''), 10) or 0
* 10 - local debug_getinfo = debug.getinfo
* 11 - local string_format = string.format

* 12 - local lastCreatedClass = nil

* 13 - local function class__call (class, ...)
* 14 -     return class.new(...)
* 15 - end

* 16 - local function class__newindex (class, index, value)
* 17 -     if type(index) == 'string' then
* 18 -         if index == 'destructor' or index == '__gc' then
* 19 -             if LUA_VERSION >= 5.2 then
* 20 -                 rawset(class._internal_instance_meta, '__gc', value)
* 21 -             end
* 22 -             rawset(class, 'destructor', value)
* 23 -         elseif sub(index, 1, 2) == '__' then
* 24 -             rawset(class._internal_instance_meta, index, value)
* 25 -             if index == '__tostring' then
* 26 -                 rawset(class, 'toString', value)
* 27 -             end
* 28 -         else
* 29 -             rawset(class, index, value)
* 30 -         end
* 31 -     else
* 32 -         rawset(class, index, value)
* 33 -     end
* 34 - end

* 35 - local function class__tostring (class)
* 36 -     return string_format('[class %s%s]', class.name, class.src)
* 37 - end

* 38 - local function extends (class_or_parent, parent)
* 39 -     local obj = parent and class_or_parent or lastCreatedClass
* 40 -     local class = parent or class_or_parent
* 41 -     if type(class) == 'string' then
* 42 -         class = _G[class]
* 43 -     elseif type(class) ~= 'table' or _G[class.name] == class then
* 44 -         error('Invalid parent class inheritance')
* 45 -     end
* 46 -     obj._internal_baseClass = class
* 47 -     if class._internal_instance_meta.__tostring then
* 48 -         obj._internal_instance_meta.__tostring = class._internal_instance_meta.__tostring
* 49 -     end
* 50 -     return obj, class
* 51 - end

* 52 - local propagationMeta = {}

* 53 - local PROP_INDEX_LEVEL_CLASS = 1
* 54 - local PROP_INDEX_INSTANCE = 2
* 55 - local PROP_INDEX_LEVEL = 3
* 56 - local PROP_INDEX_METHOD_LEVEL = 4

* 57 - local function getSuper (instance, level, levelClass, methodLevel)
* 58 -     return setmetatable({
* 59 -         levelClass,
* 60 -         instance,
* 61 -         level,
* 62 -         methodLevel
* 63 -     }, propagationMeta)
* 64 - end

* 65 - local function getClassParent (class, levels)
* 66 -     if not levels or levels <= 0 then
* 67 -         return class
* 68 -     end
* 69 -     while levels > 0 and class do
* 70 -         levels = levels - 1
* 71 -         class = class._internal_baseClass
* 72 -     end
* 73 -     return class
* 74 - end

* 75 - propagationMeta.__index = function (propagationTable, index)
* 76 -     local instance = rawget(propagationTable, PROP_INDEX_INSTANCE)

* 77 -     local ret = rawget(instance, index)

* 78 -     if ret then
* 79 -         return ret
* 80 -     end

* 81 -     local thisClass = rawget(propagationTable, PROP_INDEX_LEVEL_CLASS)
* 82 -     local currentLevel = rawget(propagationTable, PROP_INDEX_LEVEL)
* 83 -     local methodLevel = rawget(propagationTable, PROP_INDEX_METHOD_LEVEL)

* 84 -     if index == 'isPropagation' then
* 85 -         return currentLevel
* 86 -     elseif index == 'super' then
* 87 -         local base = thisClass._internal_baseClass
* 88 -         if not base then
* 89 -             return
* 90 -         end
* 91 -         return getSuper(instance, currentLevel + 1, base, methodLevel)
* 92 -     end

* 93 -     local indexLevel = methodLevel[index]

* 94 -     if not indexLevel then
* 95 -         if methodLevel[1] then
* 96 -             methodLevel[1] = false
* 97 -             indexLevel = currentLevel
* 98 -         else
* 99 -             indexLevel = 0
* 100 -         end
* 101 -     end

* 102 -     if indexLevel < currentLevel then
* 103 -         local class = getClassParent(instance._class, indexLevel)
* 104 -         propagationTable = getSuper(instance, indexLevel, class, methodLevel)
* 105 -         return propagationTable[index]
* 106 -     end

* 107 -     ret = thisClass[index]

* 108 -     if ret then
* 109 -         if type(ret) == 'function' then
* 110 -             return function (self, ...)
* 111 -                 methodLevel[index] = indexLevel + 1
* 112 -                 local results = { ret(propagationTable, ...) }
* 113 -                 methodLevel[index] = indexLevel
* 114 -                 return unpack(results)
* 115 -             end
* 116 -         else
* 117 -             return ret
* 118 -         end
* 119 -     end

* 120 -     if not thisClass._internal_baseClass then
* 121 -         return
* 122 -     end

* 123 -     methodLevel[index] = (methodLevel[index] or 0) + 1
* 124 -     propagationTable = getSuper(instance, currentLevel + 1, thisClass._internal_baseClass, methodLevel)

* 125 -     return propagationTable[index]
* 126 - end

* 127 - propagationMeta.__call = function (propagationTable, ...)
* 128 -     local thisClass = rawget(propagationTable, 1)
* 129 -     local constructor = thisClass and rawget(thisClass, 'constructor')
* 130 -     return constructor and constructor(propagationTable, ...)
* 131 - end

* 132 - propagationMeta.__newindex = function (propagationTable, index, value)
* 133 -     rawget(propagationTable, PROP_INDEX_INSTANCE)[index] = value
* 134 - end

* 135 - propagationMeta.__tostring = function (propagationTable)
* 136 -     local instance = rawget(propagationTable, PROP_INDEX_INSTANCE)
* 137 -     return tostring(instance) .. '_' .. rawget(propagationTable, PROP_INDEX_LEVEL)
* 138 - end

* 139 - function getInstanceSuperLevel (instance)
* 140 -     local meta = getmetatable(instance)
* 141 -     if meta == propagationMeta then
* 142 -         return rawget(instance, PROP_INDEX_LEVEL)
* 143 -     else
* 144 -         return 0
* 145 -     end
* 146 - end

* 147 - function getInstanceAsBase (instance)
* 148 -     local meta = getmetatable(instance)
* 149 -     if meta == propagationMeta then
* 150 -         return rawget(instance, PROP_INDEX_INSTANCE)
* 151 -     else
* 152 -         return instance
* 153 -     end
* 154 - end

* 155 - function Class (className)
* 156 -     if className and _G[className] then
* 157 -         return error('Redefining class ' .. className, 0)
* 158 -     end

* 159 -     local classDeclarationInfo = debug_getinfo(2)

* 160 -     local class = {
* 161 -         _internal_baseClass = nil,
* 162 -         extends = extends,
* 163 -         name = className or '<anonymous>'
* 164 -     }

* 165 -     if classDeclarationInfo then
* 166 -         class.src = classDeclarationInfo.source .. ':' .. classDeclarationInfo.currentline
* 167 -     end

* 168 -     class.__index = function (instance, index)

* 169 -         local metamethodLookup = index == '__tostring'
* 170 -         local ret = rawget(instance, index) or class[index]

* 171 -         if metamethodLookup and not ret then
* 172 -             ret = class._internal_instance_meta[index]
* 173 -         end

* 174 -         if ret then
* 175 -             return ret
* 176 -         elseif index == 'isPropagation' then
* 177 -             return false
* 178 -         end

* 179 -         if not class._internal_baseClass then
* 180 -             return
* 181 -         end

* 182 -         local methodLevel = { true }
* 183 -         local super = getSuper(instance, 1, class._internal_baseClass, methodLevel)

* 184 -         if index == 'super' then
* 185 -             return super
* 186 -         else
* 187 -             return super[index]
* 188 -         end
* 189 -     end

* 190 -     local function cast (instance)
* 191 -         rawset(instance, '_class', class)
* 192 -         instance = setmetatable(instance, class._internal_instance_meta)
* 193 -         if class.destructor and newproxy then
* 194 -             local proxy = newproxy()
* 195 -             rawset(instance, '__gc_proxy', proxy)
* 196 -             getmetatable(proxy).__gc = function()
* 197 -                 local baseClass = class._internal_baseClass
* 198 -                 while baseClass do
* 199 -                     if baseClass.destructor then
* 200 -                         baseClass.destructor(instance)
* 201 -                     end
* 202 -                     baseClass = baseClass._internal_baseClass
* 203 -                 end
* 204 -                 class.destructor(instance)
* 205 -             end
* 206 -         end
* 207 -         return instance
* 208 -     end

* 209 -     local function create (instance, ...)
* 210 -         local args = { ... }
* 211 -         local info = debug_getinfo(2)
* 212 -         if info and not instance.src then
* 213 -             instance.src = info.source .. ':' .. info.currentline
* 214 -         end
* 215 -         instance = cast(instance)
* 216 -         if class.constructor then
* 217 -             local construction = class.constructor(instance, unpack(args, args[1] == class and 2 or 1))
* 218 -             if construction ~= nil then
* 219 -                 instance = construction
* 220 -             end
* 221 -         else
* 222 -             local base = class._internal_baseClass
* 223 -             while base do
* 224 -                 if base.constructor then
* 225 -                     local construction = base.constructor(instance, unpack(args, args[1] == class and 2 or 1))
* 226 -                     if construction ~= nil then
* 227 -                         instance = construction
* 228 -                     end
* 229 -                     break
* 230 -                 end
* 231 -                 base = base._internal_baseClass
* 232 -             end
* 233 -         end
* 234 -         return instance
* 235 -     end

* 236 -     class.cast = cast
* 237 -     class.create = create
* 238 -     class.class = class
* 239 -     class.new = function (...) return create({}, ...) end
* 240 -     class._internal_instance_meta = { __index = class.__index }
* 241 -     class.getClassName = function () return className end
* 242 -     setmetatable(class, { __newindex = class__newindex, __call = class__call, __tostring = class__tostring })
* 243 -     if className then
* 244 -         _G[className] = class
* 245 -     end
* 246 -     lastCreatedClass = class
* 247 -     return lastCreatedClass
* 248 - end

* 249 - function instanceof (thing, class)
* 250 -     if type(thing) ~= 'table' then
* 251 -         return
* 252 -     end
* 253 -     if type(class) == 'string' then
* 254 -         class = _G[class]
* 255 -         if not class then
* 256 -             return
* 257 -         end
* 258 -     end
* 259 -     local c = thing._class
* 260 -     while c do
* 261 -         if c.name == class.name then
* 262 -             return true
* 263 -         end
* 264 -         c = c._internal_baseClass
* 265 -     end
* 266 - end

* 267 - function typeof (thing, check)

* 268 -     local t = type(thing)

* 269 -     if check then
* 270 -         if t == 'table' then
* 271 -             if check == 'table' then
* 272 -                 return true
* 273 -             end
* 274 -             local class = thing._class
* 275 -             if class and class.name == check then
* 276 -                 return true
* 277 -             end
* 278 -             local getClassName = thing.getClassName
* 279 -             if getClassName and getClassName() == check then
* 280 -                 return true
* 281 -             end
* 282 -             local baseClass = thing.baseClass
* 283 -             if baseClass then
* 284 -                 return typeof(baseClass, check)
* 285 -             end
* 286 -         end
* 287 -         return t == check
* 288 -     end

* 289 -     if t == 'table' then
* 290 -         local class = thing._class
* 291 -         if class then
* 292 -             return class.name
* 293 -         end
* 294 -         class = thing.getClassName
* 295 -         if class then
* 296 -             return class()
* 297 -         end
* 298 -     end
* 299 - end

*
************/

#ifndef OOP_LUA_H
#define OOP_LUA_H

const char lua_script_oop[] = {
	'l', 'o', 'c', 'a', 'l', ' ', 's', 'u', 'b', ' ', '=', ' ',
	's', 't', 'r', 'i', 'n', 'g', '.', 's', 'u', 'b', '\n', 'l',
	'o', 'c', 'a', 'l', ' ', 'u', 'n', 'p', 'a', 'c', 'k', ' ',
	'=', ' ', 't', 'a', 'b', 'l', 'e', '.', 'u', 'n', 'p', 'a',
	'c', 'k', ' ', 'o', 'r', ' ', 'u', 'n', 'p', 'a', 'c', 'k',
	'\n', 'l', 'o', 'c', 'a', 'l', ' ', 'r', 'a', 'w', 'g', 'e',
	't', ',', ' ', 'r', 'a', 'w', 's', 'e', 't', ' ', '=', ' ',
	'r', 'a', 'w', 'g', 'e', 't', ',', ' ', 'r', 'a', 'w', 's',
	'e', 't', '\n', 'l', 'o', 'c', 'a', 'l', ' ', 's', 'e', 't',
	'm', 'e', 't', 'a', 't', 'a', 'b', 'l', 'e', ' ', '=', ' ',
	's', 'e', 't', 'm', 'e', 't', 'a', 't', 'a', 'b', 'l', 'e',
	'\n', 'l', 'o', 'c', 'a', 'l', ' ', 'g', 'e', 't', 'm', 'e',
	't', 'a', 't', 'a', 'b', 'l', 'e', ' ', '=', ' ', 'g', 'e',
	't', 'm', 'e', 't', 'a', 't', 'a', 'b', 'l', 'e', '\n', 'l',
	'o', 'c', 'a', 'l', ' ', 't', 'y', 'p', 'e', ' ', '=', ' ',
	't', 'y', 'p', 'e', '\n', 'l', 'o', 'c', 'a', 'l', ' ', 't',
	'o', 's', 't', 'r', 'i', 'n', 'g', ' ', '=', ' ', 't', 'o',
	's', 't', 'r', 'i', 'n', 'g', '\n', 'l', 'o', 'c', 'a', 'l',
	' ', 'n', 'e', 'w', 'p', 'r', 'o', 'x', 'y', ' ', '=', ' ',
	'n', 'e', 'w', 'p', 'r', 'o', 'x', 'y', '\n', 'l', 'o', 'c',
	'a', 'l', ' ', 'L', 'U', 'A', '_', 'V', 'E', 'R', 'S', 'I',
	'O', 'N', ' ', '=', ' ', 't', 'o', 'n', 'u', 'm', 'b', 'e',
	'r', '(', '(', '_', '_', 'V', 'E', 'R', 'S', 'I', 'O', 'N',
	' ', 'o', 'r', ' ', '\'', '\'', ')', ':', 'g', 's', 'u', 'b',
	'(', '\'', '[', '^', '%', 'd', '.', ']', '\'', ',', ' ', '\'',
	'\'', ')', ',', ' ', '1', '0', ')', ' ', 'o', 'r', ' ', '0',
	'\n', 'l', 'o', 'c', 'a', 'l', ' ', 'd', 'e', 'b', 'u', 'g',
	'_', 'g', 'e', 't', 'i', 'n', 'f', 'o', ' ', '=', ' ', 'd',
	'e', 'b', 'u', 'g', '.', 'g', 'e', 't', 'i', 'n', 'f', 'o',
	'\n', 'l', 'o', 'c', 'a', 'l', ' ', 's', 't', 'r', 'i', 'n',
	'g', '_', 'f', 'o', 'r', 'm', 'a', 't', ' ', '=', ' ', 's',
	't', 'r', 'i', 'n', 'g', '.', 'f', 'o', 'r', 'm', 'a', 't',
	'\n', '\n', 'l', 'o', 'c', 'a', 'l', ' ', 'l', 'a', 's', 't',
	'C', 'r', 'e', 'a', 't', 'e', 'd', 'C', 'l', 'a', 's', 's',
	' ', '=', ' ', 'n', 'i', 'l', '\n', '\n', 'l', 'o', 'c', 'a',
	'l', ' ', 'f', 'u', 'n', 'c', 't', 'i', 'o', 'n', ' ', 'c',
	'l', 'a', 's', 's', '_', '_', 'c', 'a', 'l', 'l', ' ', '(',
	'c', 'l', 'a', 's', 's', ',', ' ', '.', '.', '.', ')', '\n',
	' ', ' ', ' ', ' ', 'r', 'e', 't', 'u', 'r', 'n', ' ', 'c',
	'l', 'a', 's', 's', '.', 'n', 'e', 'w', '(', '.', '.', '.',
	')', '\n', 'e', 'n', 'd', '\n', '\n', 'l', 'o', 'c', 'a', 'l',
	' ', 'f', 'u', 'n', 'c', 't', 'i', 'o', 'n', ' ', 'c', 'l',
	'a', 's', 's', '_', '_', 'n', 'e', 'w', 'i', 'n', 'd', 'e',
	'x', ' ', '(', 'c', 'l', 'a', 's', 's', ',', ' ', 'i', 'n',
	'd', 'e', 'x', ',', ' ', 'v', 'a', 'l', 'u', 'e', ')', '\n',
	' ', ' ', ' ', ' ', 'i', 'f', ' ', 't', 'y', 'p', 'e', '(',
	'i', 'n', 'd', 'e', 'x', ')', ' ', '=', '=', ' ', '\'', 's',
	't', 'r', 'i', 'n', 'g', '\'', ' ', 't', 'h', 'e', 'n', '\n',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'i', 'f', ' ', 'i',
	'n', 'd', 'e', 'x', ' ', '=', '=', ' ', '\'', 'd', 'e', 's',
	't', 'r', 'u', 'c', 't', 'o', 'r', '\'', ' ', 'o', 'r', ' ',
	'i', 'n', 'd', 'e', 'x', ' ', '=', '=', ' ', '\'', '_', '_',
	'g', 'c', '\'', ' ', 't', 'h', 'e', 'n', '\n', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'i', 'f', ' ',
	'L', 'U', 'A', '_', 'V', 'E', 'R', 'S', 'I', 'O', 'N', ' ',
	'>', '=', ' ', '5', '.', '2', ' ', 't', 'h', 'e', 'n', '\n',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', 'r', 'a', 'w', 's', 'e', 't', '(', 'c',
	'l', 'a', 's', 's', '.', '_', 'i', 'n', 't', 'e', 'r', 'n',
	'a', 'l', '_', 'i', 'n', 's', 't', 'a', 'n', 'c', 'e', '_',
	'm', 'e', 't', 'a', ',', ' ', '\'', '_', '_', 'g', 'c', '\'',
	',', ' ', 'v', 'a', 'l', 'u', 'e', ')', '\n', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'e', 'n', 'd',
	'\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', 'r', 'a', 'w', 's', 'e', 't', '(', 'c', 'l', 'a', 's',
	's', ',', ' ', '\'', 'd', 'e', 's', 't', 'r', 'u', 'c', 't',
	'o', 'r', '\'', ',', ' ', 'v', 'a', 'l', 'u', 'e', ')', '\n',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'e', 'l', 's', 'e',
	'i', 'f', ' ', 's', 'u', 'b', '(', 'i', 'n', 'd', 'e', 'x',
	',', ' ', '1', ',', ' ', '2', ')', ' ', '=', '=', ' ', '\'',
	'_', '_', '\'', ' ', 't', 'h', 'e', 'n', '\n', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'r', 'a', 'w',
	's', 'e', 't', '(', 'c', 'l', 'a', 's', 's', '.', '_', 'i',
	'n', 't', 'e', 'r', 'n', 'a', 'l', '_', 'i', 'n', 's', 't',
	'a', 'n', 'c', 'e', '_', 'm', 'e', 't', 'a', ',', ' ', 'i',
	'n', 'd', 'e', 'x', ',', ' ', 'v', 'a', 'l', 'u', 'e', ')',
	'\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', 'i', 'f', ' ', 'i', 'n', 'd', 'e', 'x', ' ', '=', '=',
	' ', '\'', '_', '_', 't', 'o', 's', 't', 'r', 'i', 'n', 'g',
	'\'', ' ', 't', 'h', 'e', 'n', '\n', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'r',
	'a', 'w', 's', 'e', 't', '(', 'c', 'l', 'a', 's', 's', ',',
	' ', '\'', 't', 'o', 'S', 't', 'r', 'i', 'n', 'g', '\'', ',',
	' ', 'v', 'a', 'l', 'u', 'e', ')', '\n', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'e', 'n', 'd', '\n',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'e', 'l', 's', 'e',
	'\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', 'r', 'a', 'w', 's', 'e', 't', '(', 'c', 'l', 'a', 's',
	's', ',', ' ', 'i', 'n', 'd', 'e', 'x', ',', ' ', 'v', 'a',
	'l', 'u', 'e', ')', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', 'e', 'n', 'd', '\n', ' ', ' ', ' ', ' ', 'e', 'l', 's',
	'e', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'r', 'a',
	'w', 's', 'e', 't', '(', 'c', 'l', 'a', 's', 's', ',', ' ',
	'i', 'n', 'd', 'e', 'x', ',', ' ', 'v', 'a', 'l', 'u', 'e',
	')', '\n', ' ', ' ', ' ', ' ', 'e', 'n', 'd', '\n', 'e', 'n',
	'd', '\n', '\n', 'l', 'o', 'c', 'a', 'l', ' ', 'f', 'u', 'n',
	'c', 't', 'i', 'o', 'n', ' ', 'c', 'l', 'a', 's', 's', '_',
	'_', 't', 'o', 's', 't', 'r', 'i', 'n', 'g', ' ', '(', 'c',
	'l', 'a', 's', 's', ')', '\n', ' ', ' ', ' ', ' ', 'r', 'e',
	't', 'u', 'r', 'n', ' ', 's', 't', 'r', 'i', 'n', 'g', '_',
	'f', 'o', 'r', 'm', 'a', 't', '(', '\'', '[', 'c', 'l', 'a',
	's', 's', ' ', '%', 's', '%', 's', ']', '\'', ',', ' ', 'c',
	'l', 'a', 's', 's', '.', 'n', 'a', 'm', 'e', ',', ' ', 'c',
	'l', 'a', 's', 's', '.', 's', 'r', 'c', ')', '\n', 'e', 'n',
	'd', '\n', '\n', 'l', 'o', 'c', 'a', 'l', ' ', 'f', 'u', 'n',
	'c', 't', 'i', 'o', 'n', ' ', 'e', 'x', 't', 'e', 'n', 'd',
	's', ' ', '(', 'c', 'l', 'a', 's', 's', '_', 'o', 'r', '_',
	'p', 'a', 'r', 'e', 'n', 't', ',', ' ', 'p', 'a', 'r', 'e',
	'n', 't', ')', '\n', ' ', ' ', ' ', ' ', 'l', 'o', 'c', 'a',
	'l', ' ', 'o', 'b', 'j', ' ', '=', ' ', 'p', 'a', 'r', 'e',
	'n', 't', ' ', 'a', 'n', 'd', ' ', 'c', 'l', 'a', 's', 's',
	'_', 'o', 'r', '_', 'p', 'a', 'r', 'e', 'n', 't', ' ', 'o',
	'r', ' ', 'l', 'a', 's', 't', 'C', 'r', 'e', 'a', 't', 'e',
	'd', 'C', 'l', 'a', 's', 's', '\n', ' ', ' ', ' ', ' ', 'l',
	'o', 'c', 'a', 'l', ' ', 'c', 'l', 'a', 's', 's', ' ', '=',
	' ', 'p', 'a', 'r', 'e', 'n', 't', ' ', 'o', 'r', ' ', 'c',
	'l', 'a', 's', 's', '_', 'o', 'r', '_', 'p', 'a', 'r', 'e',
	'n', 't', '\n', ' ', ' ', ' ', ' ', 'i', 'f', ' ', 't', 'y',
	'p', 'e', '(', 'c', 'l', 'a', 's', 's', ')', ' ', '=', '=',
	' ', '\'', 's', 't', 'r', 'i', 'n', 'g', '\'', ' ', 't', 'h',
	'e', 'n', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'c',
	'l', 'a', 's', 's', ' ', '=', ' ', '_', 'G', '[', 'c', 'l',
	'a', 's', 's', ']', '\n', ' ', ' ', ' ', ' ', 'e', 'l', 's',
	'e', 'i', 'f', ' ', 't', 'y', 'p', 'e', '(', 'c', 'l', 'a',
	's', 's', ')', ' ', '~', '=', ' ', '\'', 't', 'a', 'b', 'l',
	'e', '\'', ' ', 'o', 'r', ' ', '_', 'G', '[', 'c', 'l', 'a',
	's', 's', '.', 'n', 'a', 'm', 'e', ']', ' ', '=', '=', ' ',
	'c', 'l', 'a', 's', 's', ' ', 't', 'h', 'e', 'n', '\n', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', 'e', 'r', 'r', 'o', 'r',
	'(', '\'', 'I', 'n', 'v', 'a', 'l', 'i', 'd', ' ', 'p', 'a',
	'r', 'e', 'n', 't', ' ', 'c', 'l', 'a', 's', 's', ' ', 'i',
	'n', 'h', 'e', 'r', 'i', 't', 'a', 'n', 'c', 'e', '\'', ')',
	'\n', ' ', ' ', ' ', ' ', 'e', 'n', 'd', '\n', ' ', ' ', ' ',
	' ', 'o', 'b', 'j', '.', '_', 'i', 'n', 't', 'e', 'r', 'n',
	'a', 'l', '_', 'b', 'a', 's', 'e', 'C', 'l', 'a', 's', 's',
	' ', '=', ' ', 'c', 'l', 'a', 's', 's', '\n', ' ', ' ', ' ',
	' ', 'i', 'f', ' ', 'c', 'l', 'a', 's', 's', '.', '_', 'i',
	'n', 't', 'e', 'r', 'n', 'a', 'l', '_', 'i', 'n', 's', 't',
	'a', 'n', 'c', 'e', '_', 'm', 'e', 't', 'a', '.', '_', '_',
	't', 'o', 's', 't', 'r', 'i', 'n', 'g', ' ', 't', 'h', 'e',
	'n', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'o', 'b',
	'j', '.', '_', 'i', 'n', 't', 'e', 'r', 'n', 'a', 'l', '_',
	'i', 'n', 's', 't', 'a', 'n', 'c', 'e', '_', 'm', 'e', 't',
	'a', '.', '_', '_', 't', 'o', 's', 't', 'r', 'i', 'n', 'g',
	' ', '=', ' ', 'c', 'l', 'a', 's', 's', '.', '_', 'i', 'n',
	't', 'e', 'r', 'n', 'a', 'l', '_', 'i', 'n', 's', 't', 'a',
	'n', 'c', 'e', '_', 'm', 'e', 't', 'a', '.', '_', '_', 't',
	'o', 's', 't', 'r', 'i', 'n', 'g', '\n', ' ', ' ', ' ', ' ',
	'e', 'n', 'd', '\n', ' ', ' ', ' ', ' ', 'r', 'e', 't', 'u',
	'r', 'n', ' ', 'o', 'b', 'j', ',', ' ', 'c', 'l', 'a', 's',
	's', '\n', 'e', 'n', 'd', '\n', '\n', 'l', 'o', 'c', 'a', 'l',
	' ', 'p', 'r', 'o', 'p', 'a', 'g', 'a', 't', 'i', 'o', 'n',
	'M', 'e', 't', 'a', ' ', '=', ' ', '{', '}', '\n', '\n', 'l',
	'o', 'c', 'a', 'l', ' ', 'P', 'R', 'O', 'P', '_', 'I', 'N',
	'D', 'E', 'X', '_', 'L', 'E', 'V', 'E', 'L', '_', 'C', 'L',
	'A', 'S', 'S', ' ', '=', ' ', '1', '\n', 'l', 'o', 'c', 'a',
	'l', ' ', 'P', 'R', 'O', 'P', '_', 'I', 'N', 'D', 'E', 'X',
	'_', 'I', 'N', 'S', 'T', 'A', 'N', 'C', 'E', ' ', '=', ' ',
	'2', '\n', 'l', 'o', 'c', 'a', 'l', ' ', 'P', 'R', 'O', 'P',
	'_', 'I', 'N', 'D', 'E', 'X', '_', 'L', 'E', 'V', 'E', 'L',
	' ', '=', ' ', '3', '\n', 'l', 'o', 'c', 'a', 'l', ' ', 'P',
	'R', 'O', 'P', '_', 'I', 'N', 'D', 'E', 'X', '_', 'M', 'E',
	'T', 'H', 'O', 'D', '_', 'L', 'E', 'V', 'E', 'L', ' ', '=',
	' ', '4', '\n', '\n', 'l', 'o', 'c', 'a', 'l', ' ', 'f', 'u',
	'n', 'c', 't', 'i', 'o', 'n', ' ', 'g', 'e', 't', 'S', 'u',
	'p', 'e', 'r', ' ', '(', 'i', 'n', 's', 't', 'a', 'n', 'c',
	'e', ',', ' ', 'l', 'e', 'v', 'e', 'l', ',', ' ', 'l', 'e',
	'v', 'e', 'l', 'C', 'l', 'a', 's', 's', ',', ' ', 'm', 'e',
	't', 'h', 'o', 'd', 'L', 'e', 'v', 'e', 'l', ')', '\n', ' ',
	' ', ' ', ' ', 'r', 'e', 't', 'u', 'r', 'n', ' ', 's', 'e',
	't', 'm', 'e', 't', 'a', 't', 'a', 'b', 'l', 'e', '(', '{',
	'\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'l', 'e', 'v',
	'e', 'l', 'C', 'l', 'a', 's', 's', ',', '\n', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', 'i', 'n', 's', 't', 'a', 'n', 'c',
	'e', ',', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'l',
	'e', 'v', 'e', 'l', ',', '\n', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', 'm', 'e', 't', 'h', 'o', 'd', 'L', 'e', 'v', 'e',
	'l', '\n', ' ', ' ', ' ', ' ', '}', ',', ' ', 'p', 'r', 'o',
	'p', 'a', 'g', 'a', 't', 'i', 'o', 'n', 'M', 'e', 't', 'a',
	')', '\n', 'e', 'n', 'd', '\n', '\n', 'l', 'o', 'c', 'a', 'l',
	' ', 'f', 'u', 'n', 'c', 't', 'i', 'o', 'n', ' ', 'g', 'e',
	't', 'C', 'l', 'a', 's', 's', 'P', 'a', 'r', 'e', 'n', 't',
	' ', '(', 'c', 'l', 'a', 's', 's', ',', ' ', 'l', 'e', 'v',
	'e', 'l', 's', ')', '\n', ' ', ' ', ' ', ' ', 'i', 'f', ' ',
	'n', 'o', 't', ' ', 'l', 'e', 'v', 'e', 'l', 's', ' ', 'o',
	'r', ' ', 'l', 'e', 'v', 'e', 'l', 's', ' ', '<', '=', ' ',
	'0', ' ', 't', 'h', 'e', 'n', '\n', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', 'r', 'e', 't', 'u', 'r', 'n', ' ', 'c', 'l',
	'a', 's', 's', '\n', ' ', ' ', ' ', ' ', 'e', 'n', 'd', '\n',
	' ', ' ', ' ', ' ', 'w', 'h', 'i', 'l', 'e', ' ', 'l', 'e',
	'v', 'e', 'l', 's', ' ', '>', ' ', '0', ' ', 'a', 'n', 'd',
	' ', 'c', 'l', 'a', 's', 's', ' ', 'd', 'o', '\n', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', 'l', 'e', 'v', 'e', 'l', 's',
	' ', '=', ' ', 'l', 'e', 'v', 'e', 'l', 's', ' ', '-', ' ',
	'1', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'c', 'l',
	'a', 's', 's', ' ', '=', ' ', 'c', 'l', 'a', 's', 's', '.',
	'_', 'i', 'n', 't', 'e', 'r', 'n', 'a', 'l', '_', 'b', 'a',
	's', 'e', 'C', 'l', 'a', 's', 's', '\n', ' ', ' ', ' ', ' ',
	'e', 'n', 'd', '\n', ' ', ' ', ' ', ' ', 'r', 'e', 't', 'u',
	'r', 'n', ' ', 'c', 'l', 'a', 's', 's', '\n', 'e', 'n', 'd',
	'\n', '\n', 'p', 'r', 'o', 'p', 'a', 'g', 'a', 't', 'i', 'o',
	'n', 'M', 'e', 't', 'a', '.', '_', '_', 'i', 'n', 'd', 'e',
	'x', ' ', '=', ' ', 'f', 'u', 'n', 'c', 't', 'i', 'o', 'n',
	' ', '(', 'p', 'r', 'o', 'p', 'a', 'g', 'a', 't', 'i', 'o',
	'n', 'T', 'a', 'b', 'l', 'e', ',', ' ', 'i', 'n', 'd', 'e',
	'x', ')', '\n', ' ', ' ', ' ', ' ', 'l', 'o', 'c', 'a', 'l',
	' ', 'i', 'n', 's', 't', 'a', 'n', 'c', 'e', ' ', '=', ' ',
	'r', 'a', 'w', 'g', 'e', 't', '(', 'p', 'r', 'o', 'p', 'a',
	'g', 'a', 't', 'i', 'o', 'n', 'T', 'a', 'b', 'l', 'e', ',',
	' ', 'P', 'R', 'O', 'P', '_', 'I', 'N', 'D', 'E', 'X', '_',
	'I', 'N', 'S', 'T', 'A', 'N', 'C', 'E', ')', '\n', '\n', ' ',
	' ', ' ', ' ', 'l', 'o', 'c', 'a', 'l', ' ', 'r', 'e', 't',
	' ', '=', ' ', 'r', 'a', 'w', 'g', 'e', 't', '(', 'i', 'n',
	's', 't', 'a', 'n', 'c', 'e', ',', ' ', 'i', 'n', 'd', 'e',
	'x', ')', '\n', '\n', ' ', ' ', ' ', ' ', 'i', 'f', ' ', 'r',
	'e', 't', ' ', 't', 'h', 'e', 'n', '\n', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', 'r', 'e', 't', 'u', 'r', 'n', ' ', 'r',
	'e', 't', '\n', ' ', ' ', ' ', ' ', 'e', 'n', 'd', '\n', '\n',
	' ', ' ', ' ', ' ', 'l', 'o', 'c', 'a', 'l', ' ', 't', 'h',
	'i', 's', 'C', 'l', 'a', 's', 's', ' ', '=', ' ', 'r', 'a',
	'w', 'g', 'e', 't', '(', 'p', 'r', 'o', 'p', 'a', 'g', 'a',
	't', 'i', 'o', 'n', 'T', 'a', 'b', 'l', 'e', ',', ' ', 'P',
	'R', 'O', 'P', '_', 'I', 'N', 'D', 'E', 'X', '_', 'L', 'E',
	'V', 'E', 'L', '_', 'C', 'L', 'A', 'S', 'S', ')', '\n', ' ',
	' ', ' ', ' ', 'l', 'o', 'c', 'a', 'l', ' ', 'c', 'u', 'r',
	'r', 'e', 'n', 't', 'L', 'e', 'v', 'e', 'l', ' ', '=', ' ',
	'r', 'a', 'w', 'g', 'e', 't', '(', 'p', 'r', 'o', 'p', 'a',
	'g', 'a', 't', 'i', 'o', 'n', 'T', 'a', 'b', 'l', 'e', ',',
	' ', 'P', 'R', 'O', 'P', '_', 'I', 'N', 'D', 'E', 'X', '_',
	'L', 'E', 'V', 'E', 'L', ')', '\n', ' ', ' ', ' ', ' ', 'l',
	'o', 'c', 'a', 'l', ' ', 'm', 'e', 't', 'h', 'o', 'd', 'L',
	'e', 'v', 'e', 'l', ' ', '=', ' ', 'r', 'a', 'w', 'g', 'e',
	't', '(', 'p', 'r', 'o', 'p', 'a', 'g', 'a', 't', 'i', 'o',
	'n', 'T', 'a', 'b', 'l', 'e', ',', ' ', 'P', 'R', 'O', 'P',
	'_', 'I', 'N', 'D', 'E', 'X', '_', 'M', 'E', 'T', 'H', 'O',
	'D', '_', 'L', 'E', 'V', 'E', 'L', ')', '\n', '\n', ' ', ' ',
	' ', ' ', 'i', 'f', ' ', 'i', 'n', 'd', 'e', 'x', ' ', '=',
	'=', ' ', '\'', 'i', 's', 'P', 'r', 'o', 'p', 'a', 'g', 'a',
	't', 'i', 'o', 'n', '\'', ' ', 't', 'h', 'e', 'n', '\n', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', 'r', 'e', 't', 'u', 'r',
	'n', ' ', 'c', 'u', 'r', 'r', 'e', 'n', 't', 'L', 'e', 'v',
	'e', 'l', '\n', ' ', ' ', ' ', ' ', 'e', 'l', 's', 'e', 'i',
	'f', ' ', 'i', 'n', 'd', 'e', 'x', ' ', '=', '=', ' ', '\'',
	's', 'u', 'p', 'e', 'r', '\'', ' ', 't', 'h', 'e', 'n', '\n',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'l', 'o', 'c', 'a',
	'l', ' ', 'b', 'a', 's', 'e', ' ', '=', ' ', 't', 'h', 'i',
	's', 'C', 'l', 'a', 's', 's', '.', '_', 'i', 'n', 't', 'e',
	'r', 'n', 'a', 'l', '_', 'b', 'a', 's', 'e', 'C', 'l', 'a',
	's', 's', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'i',
	'f', ' ', 'n', 'o', 't', ' ', 'b', 'a', 's', 'e', ' ', 't',
	'h', 'e', 'n', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', 'r', 'e', 't', 'u', 'r', 'n', '\n', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', 'e', 'n', 'd', '\n', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', 'r', 'e', 't', 'u', 'r',
	'n', ' ', 'g', 'e', 't', 'S', 'u', 'p', 'e', 'r', '(', 'i',
	'n', 's', 't', 'a', 'n', 'c', 'e', ',', ' ', 'c', 'u', 'r',
	'r', 'e', 'n', 't', 'L', 'e', 'v', 'e', 'l', ' ', '+', ' ',
	'1', ',', ' ', 'b', 'a', 's', 'e', ',', ' ', 'm', 'e', 't',
	'h', 'o', 'd', 'L', 'e', 'v', 'e', 'l', ')', '\n', ' ', ' ',
	' ', ' ', 'e', 'n', 'd', '\n', '\n', ' ', ' ', ' ', ' ', 'l',
	'o', 'c', 'a', 'l', ' ', 'i', 'n', 'd', 'e', 'x', 'L', 'e',
	'v', 'e', 'l', ' ', '=', ' ', 'm', 'e', 't', 'h', 'o', 'd',
	'L', 'e', 'v', 'e', 'l', '[', 'i', 'n', 'd', 'e', 'x', ']',
	'\n', '\n', ' ', ' ', ' ', ' ', 'i', 'f', ' ', 'n', 'o', 't',
	' ', 'i', 'n', 'd', 'e', 'x', 'L', 'e', 'v', 'e', 'l', ' ',
	't', 'h', 'e', 'n', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', 'i', 'f', ' ', 'm', 'e', 't', 'h', 'o', 'd', 'L', 'e',
	'v', 'e', 'l', '[', '1', ']', ' ', 't', 'h', 'e', 'n', '\n',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	'm', 'e', 't', 'h', 'o', 'd', 'L', 'e', 'v', 'e', 'l', '[',
	'1', ']', ' ', '=', ' ', 'f', 'a', 'l', 's', 'e', '\n', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'i',
	'n', 'd', 'e', 'x', 'L', 'e', 'v', 'e', 'l', ' ', '=', ' ',
	'c', 'u', 'r', 'r', 'e', 'n', 't', 'L', 'e', 'v', 'e', 'l',
	'\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'e', 'l', 's',
	'e', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', 'i', 'n', 'd', 'e', 'x', 'L', 'e', 'v', 'e', 'l',
	' ', '=', ' ', '0', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', 'e', 'n', 'd', '\n', ' ', ' ', ' ', ' ', 'e', 'n', 'd',
	'\n', '\n', ' ', ' ', ' ', ' ', 'i', 'f', ' ', 'i', 'n', 'd',
	'e', 'x', 'L', 'e', 'v', 'e', 'l', ' ', '<', ' ', 'c', 'u',
	'r', 'r', 'e', 'n', 't', 'L', 'e', 'v', 'e', 'l', ' ', 't',
	'h', 'e', 'n', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	'l', 'o', 'c', 'a', 'l', ' ', 'c', 'l', 'a', 's', 's', ' ',
	'=', ' ', 'g', 'e', 't', 'C', 'l', 'a', 's', 's', 'P', 'a',
	'r', 'e', 'n', 't', '(', 'i', 'n', 's', 't', 'a', 'n', 'c',
	'e', '.', '_', 'c', 'l', 'a', 's', 's', ',', ' ', 'i', 'n',
	'd', 'e', 'x', 'L', 'e', 'v', 'e', 'l', ')', '\n', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', 'p', 'r', 'o', 'p', 'a', 'g',
	'a', 't', 'i', 'o', 'n', 'T', 'a', 'b', 'l', 'e', ' ', '=',
	' ', 'g', 'e', 't', 'S', 'u', 'p', 'e', 'r', '(', 'i', 'n',
	's', 't', 'a', 'n', 'c', 'e', ',', ' ', 'i', 'n', 'd', 'e',
	'x', 'L', 'e', 'v', 'e', 'l', ',', ' ', 'c', 'l', 'a', 's',
	's', ',', ' ', 'm', 'e', 't', 'h', 'o', 'd', 'L', 'e', 'v',
	'e', 'l', ')', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	'r', 'e', 't', 'u', 'r', 'n', ' ', 'p', 'r', 'o', 'p', 'a',
	'g', 'a', 't', 'i', 'o', 'n', 'T', 'a', 'b', 'l', 'e', '[',
	'i', 'n', 'd', 'e', 'x', ']', '\n', ' ', ' ', ' ', ' ', 'e',
	'n', 'd', '\n', '\n', ' ', ' ', ' ', ' ', 'r', 'e', 't', ' ',
	'=', ' ', 't', 'h', 'i', 's', 'C', 'l', 'a', 's', 's', '[',
	'i', 'n', 'd', 'e', 'x', ']', '\n', '\n', ' ', ' ', ' ', ' ',
	'i', 'f', ' ', 'r', 'e', 't', ' ', 't', 'h', 'e', 'n', '\n',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'i', 'f', ' ', 't',
	'y', 'p', 'e', '(', 'r', 'e', 't', ')', ' ', '=', '=', ' ',
	'\'', 'f', 'u', 'n', 'c', 't', 'i', 'o', 'n', '\'', ' ', 't',
	'h', 'e', 'n', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', 'r', 'e', 't', 'u', 'r', 'n', ' ', 'f',
	'u', 'n', 'c', 't', 'i', 'o', 'n', ' ', '(', 's', 'e', 'l',
	'f', ',', ' ', '.', '.', '.', ')', '\n', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	'm', 'e', 't', 'h', 'o', 'd', 'L', 'e', 'v', 'e', 'l', '[',
	'i', 'n', 'd', 'e', 'x', ']', ' ', '=', ' ', 'i', 'n', 'd',
	'e', 'x', 'L', 'e', 'v', 'e', 'l', ' ', '+', ' ', '1', '\n',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', 'l', 'o', 'c', 'a', 'l', ' ', 'r', 'e',
	's', 'u', 'l', 't', 's', ' ', '=', ' ', '{', ' ', 'r', 'e',
	't', '(', 'p', 'r', 'o', 'p', 'a', 'g', 'a', 't', 'i', 'o',
	'n', 'T', 'a', 'b', 'l', 'e', ',', ' ', '.', '.', '.', ')',
	' ', '}', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', 'm', 'e', 't', 'h', 'o',
	'd', 'L', 'e', 'v', 'e', 'l', '[', 'i', 'n', 'd', 'e', 'x',
	']', ' ', '=', ' ', 'i', 'n', 'd', 'e', 'x', 'L', 'e', 'v',
	'e', 'l', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', 'r', 'e', 't', 'u', 'r',
	'n', ' ', 'u', 'n', 'p', 'a', 'c', 'k', '(', 'r', 'e', 's',
	'u', 'l', 't', 's', ')', '\n', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', 'e', 'n', 'd', '\n', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', 'e', 'l', 's', 'e', '\n', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'r',
	'e', 't', 'u', 'r', 'n', ' ', 'r', 'e', 't', '\n', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', 'e', 'n', 'd', '\n', ' ', ' ',
	' ', ' ', 'e', 'n', 'd', '\n', '\n', ' ', ' ', ' ', ' ', 'i',
	'f', ' ', 'n', 'o', 't', ' ', 't', 'h', 'i', 's', 'C', 'l',
	'a', 's', 's', '.', '_', 'i', 'n', 't', 'e', 'r', 'n', 'a',
	'l', '_', 'b', 'a', 's', 'e', 'C', 'l', 'a', 's', 's', ' ',
	't', 'h', 'e', 'n', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', 'r', 'e', 't', 'u', 'r', 'n', '\n', ' ', ' ', ' ', ' ',
	'e', 'n', 'd', '\n', '\n', ' ', ' ', ' ', ' ', 'm', 'e', 't',
	'h', 'o', 'd', 'L', 'e', 'v', 'e', 'l', '[', 'i', 'n', 'd',
	'e', 'x', ']', ' ', '=', ' ', '(', 'm', 'e', 't', 'h', 'o',
	'd', 'L', 'e', 'v', 'e', 'l', '[', 'i', 'n', 'd', 'e', 'x',
	']', ' ', 'o', 'r', ' ', '0', ')', ' ', '+', ' ', '1', '\n',
	' ', ' ', ' ', ' ', 'p', 'r', 'o', 'p', 'a', 'g', 'a', 't',
	'i', 'o', 'n', 'T', 'a', 'b', 'l', 'e', ' ', '=', ' ', 'g',
	'e', 't', 'S', 'u', 'p', 'e', 'r', '(', 'i', 'n', 's', 't',
	'a', 'n', 'c', 'e', ',', ' ', 'c', 'u', 'r', 'r', 'e', 'n',
	't', 'L', 'e', 'v', 'e', 'l', ' ', '+', ' ', '1', ',', ' ',
	't', 'h', 'i', 's', 'C', 'l', 'a', 's', 's', '.', '_', 'i',
	'n', 't', 'e', 'r', 'n', 'a', 'l', '_', 'b', 'a', 's', 'e',
	'C', 'l', 'a', 's', 's', ',', ' ', 'm', 'e', 't', 'h', 'o',
	'd', 'L', 'e', 'v', 'e', 'l', ')', '\n', '\n', ' ', ' ', ' ',
	' ', 'r', 'e', 't', 'u', 'r', 'n', ' ', 'p', 'r', 'o', 'p',
	'a', 'g', 'a', 't', 'i', 'o', 'n', 'T', 'a', 'b', 'l', 'e',
	'[', 'i', 'n', 'd', 'e', 'x', ']', '\n', 'e', 'n', 'd', '\n',
	'\n', 'p', 'r', 'o', 'p', 'a', 'g', 'a', 't', 'i', 'o', 'n',
	'M', 'e', 't', 'a', '.', '_', '_', 'c', 'a', 'l', 'l', ' ',
	'=', ' ', 'f', 'u', 'n', 'c', 't', 'i', 'o', 'n', ' ', '(',
	'p', 'r', 'o', 'p', 'a', 'g', 'a', 't', 'i', 'o', 'n', 'T',
	'a', 'b', 'l', 'e', ',', ' ', '.', '.', '.', ')', '\n', ' ',
	' ', ' ', ' ', 'l', 'o', 'c', 'a', 'l', ' ', 't', 'h', 'i',
	's', 'C', 'l', 'a', 's', 's', ' ', '=', ' ', 'r', 'a', 'w',
	'g', 'e', 't', '(', 'p', 'r', 'o', 'p', 'a', 'g', 'a', 't',
	'i', 'o', 'n', 'T', 'a', 'b', 'l', 'e', ',', ' ', '1', ')',
	'\n', ' ', ' ', ' ', ' ', 'l', 'o', 'c', 'a', 'l', ' ', 'c',
	'o', 'n', 's', 't', 'r', 'u', 'c', 't', 'o', 'r', ' ', '=',
	' ', 't', 'h', 'i', 's', 'C', 'l', 'a', 's', 's', ' ', 'a',
	'n', 'd', ' ', 'r', 'a', 'w', 'g', 'e', 't', '(', 't', 'h',
	'i', 's', 'C', 'l', 'a', 's', 's', ',', ' ', '\'', 'c', 'o',
	'n', 's', 't', 'r', 'u', 'c', 't', 'o', 'r', '\'', ')', '\n',
	' ', ' ', ' ', ' ', 'r', 'e', 't', 'u', 'r', 'n', ' ', 'c',
	'o', 'n', 's', 't', 'r', 'u', 'c', 't', 'o', 'r', ' ', 'a',
	'n', 'd', ' ', 'c', 'o', 'n', 's', 't', 'r', 'u', 'c', 't',
	'o', 'r', '(', 'p', 'r', 'o', 'p', 'a', 'g', 'a', 't', 'i',
	'o', 'n', 'T', 'a', 'b', 'l', 'e', ',', ' ', '.', '.', '.',
	')', '\n', 'e', 'n', 'd', '\n', '\n', 'p', 'r', 'o', 'p', 'a',
	'g', 'a', 't', 'i', 'o', 'n', 'M', 'e', 't', 'a', '.', '_',
	'_', 'n', 'e', 'w', 'i', 'n', 'd', 'e', 'x', ' ', '=', ' ',
	'f', 'u', 'n', 'c', 't', 'i', 'o', 'n', ' ', '(', 'p', 'r',
	'o', 'p', 'a', 'g', 'a', 't', 'i', 'o', 'n', 'T', 'a', 'b',
	'l', 'e', ',', ' ', 'i', 'n', 'd', 'e', 'x', ',', ' ', 'v',
	'a', 'l', 'u', 'e', ')', '\n', ' ', ' ', ' ', ' ', 'r', 'a',
	'w', 'g', 'e', 't', '(', 'p', 'r', 'o', 'p', 'a', 'g', 'a',
	't', 'i', 'o', 'n', 'T', 'a', 'b', 'l', 'e', ',', ' ', 'P',
	'R', 'O', 'P', '_', 'I', 'N', 'D', 'E', 'X', '_', 'I', 'N',
	'S', 'T', 'A', 'N', 'C', 'E', ')', '[', 'i', 'n', 'd', 'e',
	'x', ']', ' ', '=', ' ', 'v', 'a', 'l', 'u', 'e', '\n', 'e',
	'n', 'd', '\n', '\n', 'p', 'r', 'o', 'p', 'a', 'g', 'a', 't',
	'i', 'o', 'n', 'M', 'e', 't', 'a', '.', '_', '_', 't', 'o',
	's', 't', 'r', 'i', 'n', 'g', ' ', '=', ' ', 'f', 'u', 'n',
	'c', 't', 'i', 'o', 'n', ' ', '(', 'p', 'r', 'o', 'p', 'a',
	'g', 'a', 't', 'i', 'o', 'n', 'T', 'a', 'b', 'l', 'e', ')',
	'\n', ' ', ' ', ' ', ' ', 'l', 'o', 'c', 'a', 'l', ' ', 'i',
	'n', 's', 't', 'a', 'n', 'c', 'e', ' ', '=', ' ', 'r', 'a',
	'w', 'g', 'e', 't', '(', 'p', 'r', 'o', 'p', 'a', 'g', 'a',
	't', 'i', 'o', 'n', 'T', 'a', 'b', 'l', 'e', ',', ' ', 'P',
	'R', 'O', 'P', '_', 'I', 'N', 'D', 'E', 'X', '_', 'I', 'N',
	'S', 'T', 'A', 'N', 'C', 'E', ')', '\n', ' ', ' ', ' ', ' ',
	'r', 'e', 't', 'u', 'r', 'n', ' ', 't', 'o', 's', 't', 'r',
	'i', 'n', 'g', '(', 'i', 'n', 's', 't', 'a', 'n', 'c', 'e',
	')', ' ', '.', '.', ' ', '\'', '_', '\'', ' ', '.', '.', ' ',
	'r', 'a', 'w', 'g', 'e', 't', '(', 'p', 'r', 'o', 'p', 'a',
	'g', 'a', 't', 'i', 'o', 'n', 'T', 'a', 'b', 'l', 'e', ',',
	' ', 'P', 'R', 'O', 'P', '_', 'I', 'N', 'D', 'E', 'X', '_',
	'L', 'E', 'V', 'E', 'L', ')', '\n', 'e', 'n', 'd', '\n', '\n',
	'f', 'u', 'n', 'c', 't', 'i', 'o', 'n', ' ', 'g', 'e', 't',
	'I', 'n', 's', 't', 'a', 'n', 'c', 'e', 'S', 'u', 'p', 'e',
	'r', 'L', 'e', 'v', 'e', 'l', ' ', '(', 'i', 'n', 's', 't',
	'a', 'n', 'c', 'e', ')', '\n', ' ', ' ', ' ', ' ', 'l', 'o',
	'c', 'a', 'l', ' ', 'm', 'e', 't', 'a', ' ', '=', ' ', 'g',
	'e', 't', 'm', 'e', 't', 'a', 't', 'a', 'b', 'l', 'e', '(',
	'i', 'n', 's', 't', 'a', 'n', 'c', 'e', ')', '\n', ' ', ' ',
	' ', ' ', 'i', 'f', ' ', 'm', 'e', 't', 'a', ' ', '=', '=',
	' ', 'p', 'r', 'o', 'p', 'a', 'g', 'a', 't', 'i', 'o', 'n',
	'M', 'e', 't', 'a', ' ', 't', 'h', 'e', 'n', '\n', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', 'r', 'e', 't', 'u', 'r', 'n',
	' ', 'r', 'a', 'w', 'g', 'e', 't', '(', 'i', 'n', 's', 't',
	'a', 'n', 'c', 'e', ',', ' ', 'P', 'R', 'O', 'P', '_', 'I',
	'N', 'D', 'E', 'X', '_', 'L', 'E', 'V', 'E', 'L', ')', '\n',
	' ', ' ', ' ', ' ', 'e', 'l', 's', 'e', '\n', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', 'r', 'e', 't', 'u', 'r', 'n', ' ',
	'0', '\n', ' ', ' ', ' ', ' ', 'e', 'n', 'd', '\n', 'e', 'n',
	'd', '\n', '\n', 'f', 'u', 'n', 'c', 't', 'i', 'o', 'n', ' ',
	'g', 'e', 't', 'I', 'n', 's', 't', 'a', 'n', 'c', 'e', 'A',
	's', 'B', 'a', 's', 'e', ' ', '(', 'i', 'n', 's', 't', 'a',
	'n', 'c', 'e', ')', '\n', ' ', ' ', ' ', ' ', 'l', 'o', 'c',
	'a', 'l', ' ', 'm', 'e', 't', 'a', ' ', '=', ' ', 'g', 'e',
	't', 'm', 'e', 't', 'a', 't', 'a', 'b', 'l', 'e', '(', 'i',
	'n', 's', 't', 'a', 'n', 'c', 'e', ')', '\n', ' ', ' ', ' ',
	' ', 'i', 'f', ' ', 'm', 'e', 't', 'a', ' ', '=', '=', ' ',
	'p', 'r', 'o', 'p', 'a', 'g', 'a', 't', 'i', 'o', 'n', 'M',
	'e', 't', 'a', ' ', 't', 'h', 'e', 'n', '\n', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', 'r', 'e', 't', 'u', 'r', 'n', ' ',
	'r', 'a', 'w', 'g', 'e', 't', '(', 'i', 'n', 's', 't', 'a',
	'n', 'c', 'e', ',', ' ', 'P', 'R', 'O', 'P', '_', 'I', 'N',
	'D', 'E', 'X', '_', 'I', 'N', 'S', 'T', 'A', 'N', 'C', 'E',
	')', '\n', ' ', ' ', ' ', ' ', 'e', 'l', 's', 'e', '\n', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', 'r', 'e', 't', 'u', 'r',
	'n', ' ', 'i', 'n', 's', 't', 'a', 'n', 'c', 'e', '\n', ' ',
	' ', ' ', ' ', 'e', 'n', 'd', '\n', 'e', 'n', 'd', '\n', '\n',
	'f', 'u', 'n', 'c', 't', 'i', 'o', 'n', ' ', 'C', 'l', 'a',
	's', 's', ' ', '(', 'c', 'l', 'a', 's', 's', 'N', 'a', 'm',
	'e', ')', '\n', ' ', ' ', ' ', ' ', 'i', 'f', ' ', 'c', 'l',
	'a', 's', 's', 'N', 'a', 'm', 'e', ' ', 'a', 'n', 'd', ' ',
	'_', 'G', '[', 'c', 'l', 'a', 's', 's', 'N', 'a', 'm', 'e',
	']', ' ', 't', 'h', 'e', 'n', '\n', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', 'r', 'e', 't', 'u', 'r', 'n', ' ', 'e', 'r',
	'r', 'o', 'r', '(', '\'', 'R', 'e', 'd', 'e', 'f', 'i', 'n',
	'i', 'n', 'g', ' ', 'c', 'l', 'a', 's', 's', ' ', '\'', ' ',
	'.', '.', ' ', 'c', 'l', 'a', 's', 's', 'N', 'a', 'm', 'e',
	',', ' ', '0', ')', '\n', ' ', ' ', ' ', ' ', 'e', 'n', 'd',
	'\n', '\n', ' ', ' ', ' ', ' ', 'l', 'o', 'c', 'a', 'l', ' ',
	'c', 'l', 'a', 's', 's', 'D', 'e', 'c', 'l', 'a', 'r', 'a',
	't', 'i', 'o', 'n', 'I', 'n', 'f', 'o', ' ', '=', ' ', 'd',
	'e', 'b', 'u', 'g', '_', 'g', 'e', 't', 'i', 'n', 'f', 'o',
	'(', '2', ')', '\n', '\n', ' ', ' ', ' ', ' ', 'l', 'o', 'c',
	'a', 'l', ' ', 'c', 'l', 'a', 's', 's', ' ', '=', ' ', '{',
	'\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '_', 'i', 'n',
	't', 'e', 'r', 'n', 'a', 'l', '_', 'b', 'a', 's', 'e', 'C',
	'l', 'a', 's', 's', ' ', '=', ' ', 'n', 'i', 'l', ',', '\n',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'e', 'x', 't', 'e',
	'n', 'd', 's', ' ', '=', ' ', 'e', 'x', 't', 'e', 'n', 'd',
	's', ',', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'n',
	'a', 'm', 'e', ' ', '=', ' ', 'c', 'l', 'a', 's', 's', 'N',
	'a', 'm', 'e', ' ', 'o', 'r', ' ', '\'', '<', 'a', 'n', 'o',
	'n', 'y', 'm', 'o', 'u', 's', '>', '\'', '\n', ' ', ' ', ' ',
	' ', '}', '\n', '\n', ' ', ' ', ' ', ' ', 'i', 'f', ' ', 'c',
	'l', 'a', 's', 's', 'D', 'e', 'c', 'l', 'a', 'r', 'a', 't',
	'i', 'o', 'n', 'I', 'n', 'f', 'o', ' ', 't', 'h', 'e', 'n',
	'\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'c', 'l', 'a',
	's', 's', '.', 's', 'r', 'c', ' ', '=', ' ', 'c', 'l', 'a',
	's', 's', 'D', 'e', 'c', 'l', 'a', 'r', 'a', 't', 'i', 'o',
	'n', 'I', 'n', 'f', 'o', '.', 's', 'o', 'u', 'r', 'c', 'e',
	' ', '.', '.', ' ', '\'', ':', '\'', ' ', '.', '.', ' ', 'c',
	'l', 'a', 's', 's', 'D', 'e', 'c', 'l', 'a', 'r', 'a', 't',
	'i', 'o', 'n', 'I', 'n', 'f', 'o', '.', 'c', 'u', 'r', 'r',
	'e', 'n', 't', 'l', 'i', 'n', 'e', '\n', ' ', ' ', ' ', ' ',
	'e', 'n', 'd', '\n', '\n', ' ', ' ', ' ', ' ', 'c', 'l', 'a',
	's', 's', '.', '_', '_', 'i', 'n', 'd', 'e', 'x', ' ', '=',
	' ', 'f', 'u', 'n', 'c', 't', 'i', 'o', 'n', ' ', '(', 'i',
	'n', 's', 't', 'a', 'n', 'c', 'e', ',', ' ', 'i', 'n', 'd',
	'e', 'x', ')', '\n', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', 'l', 'o', 'c', 'a', 'l', ' ', 'm', 'e', 't', 'a', 'm',
	'e', 't', 'h', 'o', 'd', 'L', 'o', 'o', 'k', 'u', 'p', ' ',
	'=', ' ', 'i', 'n', 'd', 'e', 'x', ' ', '=', '=', ' ', '\'',
	'_', '_', 't', 'o', 's', 't', 'r', 'i', 'n', 'g', '\'', '\n',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'l', 'o', 'c', 'a',
	'l', ' ', 'r', 'e', 't', ' ', '=', ' ', 'r', 'a', 'w', 'g',
	'e', 't', '(', 'i', 'n', 's', 't', 'a', 'n', 'c', 'e', ',',
	' ', 'i', 'n', 'd', 'e', 'x', ')', ' ', 'o', 'r', ' ', 'c',
	'l', 'a', 's', 's', '[', 'i', 'n', 'd', 'e', 'x', ']', '\n',
	'\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'i', 'f', ' ',
	'm', 'e', 't', 'a', 'm', 'e', 't', 'h', 'o', 'd', 'L', 'o',
	'o', 'k', 'u', 'p', ' ', 'a', 'n', 'd', ' ', 'n', 'o', 't',
	' ', 'r', 'e', 't', ' ', 't', 'h', 'e', 'n', '\n', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'r', 'e',
	't', ' ', '=', ' ', 'c', 'l', 'a', 's', 's', '.', '_', 'i',
	'n', 't', 'e', 'r', 'n', 'a', 'l', '_', 'i', 'n', 's', 't',
	'a', 'n', 'c', 'e', '_', 'm', 'e', 't', 'a', '[', 'i', 'n',
	'd', 'e', 'x', ']', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', 'e', 'n', 'd', '\n', '\n', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', 'i', 'f', ' ', 'r', 'e', 't', ' ', 't', 'h', 'e',
	'n', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', 'r', 'e', 't', 'u', 'r', 'n', ' ', 'r', 'e', 't',
	'\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'e', 'l', 's',
	'e', 'i', 'f', ' ', 'i', 'n', 'd', 'e', 'x', ' ', '=', '=',
	' ', '\'', 'i', 's', 'P', 'r', 'o', 'p', 'a', 'g', 'a', 't',
	'i', 'o', 'n', '\'', ' ', 't', 'h', 'e', 'n', '\n', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'r', 'e',
	't', 'u', 'r', 'n', ' ', 'f', 'a', 'l', 's', 'e', '\n', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', 'e', 'n', 'd', '\n', '\n',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'i', 'f', ' ', 'n',
	'o', 't', ' ', 'c', 'l', 'a', 's', 's', '.', '_', 'i', 'n',
	't', 'e', 'r', 'n', 'a', 'l', '_', 'b', 'a', 's', 'e', 'C',
	'l', 'a', 's', 's', ' ', 't', 'h', 'e', 'n', '\n', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'r', 'e',
	't', 'u', 'r', 'n', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', 'e', 'n', 'd', '\n', '\n', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', 'l', 'o', 'c', 'a', 'l', ' ', 'm', 'e', 't', 'h',
	'o', 'd', 'L', 'e', 'v', 'e', 'l', ' ', '=', ' ', '{', ' ',
	't', 'r', 'u', 'e', ' ', '}', '\n', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', 'l', 'o', 'c', 'a', 'l', ' ', 's', 'u', 'p',
	'e', 'r', ' ', '=', ' ', 'g', 'e', 't', 'S', 'u', 'p', 'e',
	'r', '(', 'i', 'n', 's', 't', 'a', 'n', 'c', 'e', ',', ' ',
	'1', ',', ' ', 'c', 'l', 'a', 's', 's', '.', '_', 'i', 'n',
	't', 'e', 'r', 'n', 'a', 'l', '_', 'b', 'a', 's', 'e', 'C',
	'l', 'a', 's', 's', ',', ' ', 'm', 'e', 't', 'h', 'o', 'd',
	'L', 'e', 'v', 'e', 'l', ')', '\n', '\n', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', 'i', 'f', ' ', 'i', 'n', 'd', 'e', 'x',
	' ', '=', '=', ' ', '\'', 's', 'u', 'p', 'e', 'r', '\'', ' ',
	't', 'h', 'e', 'n', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', 'r', 'e', 't', 'u', 'r', 'n', ' ',
	's', 'u', 'p', 'e', 'r', '\n', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', 'e', 'l', 's', 'e', '\n', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', 'r', 'e', 't', 'u', 'r',
	'n', ' ', 's', 'u', 'p', 'e', 'r', '[', 'i', 'n', 'd', 'e',
	'x', ']', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'e',
	'n', 'd', '\n', ' ', ' ', ' ', ' ', 'e', 'n', 'd', '\n', '\n',
	' ', ' ', ' ', ' ', 'l', 'o', 'c', 'a', 'l', ' ', 'f', 'u',
	'n', 'c', 't', 'i', 'o', 'n', ' ', 'c', 'a', 's', 't', ' ',
	'(', 'i', 'n', 's', 't', 'a', 'n', 'c', 'e', ')', '\n', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', 'r', 'a', 'w', 's', 'e',
	't', '(', 'i', 'n', 's', 't', 'a', 'n', 'c', 'e', ',', ' ',
	'\'', '_', 'c', 'l', 'a', 's', 's', '\'', ',', ' ', 'c', 'l',
	'a', 's', 's', ')', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', 'i', 'n', 's', 't', 'a', 'n', 'c', 'e', ' ', '=', ' ',
	's', 'e', 't', 'm', 'e', 't', 'a', 't', 'a', 'b', 'l', 'e',
	'(', 'i', 'n', 's', 't', 'a', 'n', 'c', 'e', ',', ' ', 'c',
	'l', 'a', 's', 's', '.', '_', 'i', 'n', 't', 'e', 'r', 'n',
	'a', 'l', '_', 'i', 'n', 's', 't', 'a', 'n', 'c', 'e', '_',
	'm', 'e', 't', 'a', ')', '\n', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', 'i', 'f', ' ', 'c', 'l', 'a', 's', 's', '.', 'd',
	'e', 's', 't', 'r', 'u', 'c', 't', 'o', 'r', ' ', 'a', 'n',
	'd', ' ', 'n', 'e', 'w', 'p', 'r', 'o', 'x', 'y', ' ', 't',
	'h', 'e', 'n', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', 'l', 'o', 'c', 'a', 'l', ' ', 'p', 'r',
	'o', 'x', 'y', ' ', '=', ' ', 'n', 'e', 'w', 'p', 'r', 'o',
	'x', 'y', '(', ')', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', 'r', 'a', 'w', 's', 'e', 't', '(',
	'i', 'n', 's', 't', 'a', 'n', 'c', 'e', ',', ' ', '\'', '_',
	'_', 'g', 'c', '_', 'p', 'r', 'o', 'x', 'y', '\'', ',', ' ',
	'p', 'r', 'o', 'x', 'y', ')', '\n', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', 'g', 'e', 't', 'm', 'e',
	't', 'a', 't', 'a', 'b', 'l', 'e', '(', 'p', 'r', 'o', 'x',
	'y', ')', '.', '_', '_', 'g', 'c', ' ', '=', ' ', 'f', 'u',
	'n', 'c', 't', 'i', 'o', 'n', '(', ')', '\n', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', 'l', 'o', 'c', 'a', 'l', ' ', 'b', 'a', 's', 'e', 'C',
	'l', 'a', 's', 's', ' ', '=', ' ', 'c', 'l', 'a', 's', 's',
	'.', '_', 'i', 'n', 't', 'e', 'r', 'n', 'a', 'l', '_', 'b',
	'a', 's', 'e', 'C', 'l', 'a', 's', 's', '\n', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', 'w', 'h', 'i', 'l', 'e', ' ', 'b', 'a', 's', 'e', 'C',
	'l', 'a', 's', 's', ' ', 'd', 'o', '\n', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', 'i', 'f', ' ', 'b', 'a', 's', 'e', 'C',
	'l', 'a', 's', 's', '.', 'd', 'e', 's', 't', 'r', 'u', 'c',
	't', 'o', 'r', ' ', 't', 'h', 'e', 'n', '\n', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'b', 'a', 's',
	'e', 'C', 'l', 'a', 's', 's', '.', 'd', 'e', 's', 't', 'r',
	'u', 'c', 't', 'o', 'r', '(', 'i', 'n', 's', 't', 'a', 'n',
	'c', 'e', ')', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	'e', 'n', 'd', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	'b', 'a', 's', 'e', 'C', 'l', 'a', 's', 's', ' ', '=', ' ',
	'b', 'a', 's', 'e', 'C', 'l', 'a', 's', 's', '.', '_', 'i',
	'n', 't', 'e', 'r', 'n', 'a', 'l', '_', 'b', 'a', 's', 'e',
	'C', 'l', 'a', 's', 's', '\n', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'e', 'n',
	'd', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', 'c', 'l', 'a', 's', 's', '.',
	'd', 'e', 's', 't', 'r', 'u', 'c', 't', 'o', 'r', '(', 'i',
	'n', 's', 't', 'a', 'n', 'c', 'e', ')', '\n', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'e', 'n', 'd',
	'\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'e', 'n', 'd',
	'\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'r', 'e', 't',
	'u', 'r', 'n', ' ', 'i', 'n', 's', 't', 'a', 'n', 'c', 'e',
	'\n', ' ', ' ', ' ', ' ', 'e', 'n', 'd', '\n', '\n', ' ', ' ',
	' ', ' ', 'l', 'o', 'c', 'a', 'l', ' ', 'f', 'u', 'n', 'c',
	't', 'i', 'o', 'n', ' ', 'c', 'r', 'e', 'a', 't', 'e', ' ',
	'(', 'i', 'n', 's', 't', 'a', 'n', 'c', 'e', ',', ' ', '.',
	'.', '.', ')', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	'l', 'o', 'c', 'a', 'l', ' ', 'a', 'r', 'g', 's', ' ', '=',
	' ', '{', ' ', '.', '.', '.', ' ', '}', '\n', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', 'l', 'o', 'c', 'a', 'l', ' ', 'i',
	'n', 'f', 'o', ' ', '=', ' ', 'd', 'e', 'b', 'u', 'g', '_',
	'g', 'e', 't', 'i', 'n', 'f', 'o', '(', '2', ')', '\n', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', 'i', 'f', ' ', 'i', 'n',
	'f', 'o', ' ', 'a', 'n', 'd', ' ', 'n', 'o', 't', ' ', 'i',
	'n', 's', 't', 'a', 'n', 'c', 'e', '.', 's', 'r', 'c', ' ',
	't', 'h', 'e', 'n', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', 'i', 'n', 's', 't', 'a', 'n', 'c',
	'e', '.', 's', 'r', 'c', ' ', '=', ' ', 'i', 'n', 'f', 'o',
	'.', 's', 'o', 'u', 'r', 'c', 'e', ' ', '.', '.', ' ', '\'',
	':', '\'', ' ', '.', '.', ' ', 'i', 'n', 'f', 'o', '.', 'c',
	'u', 'r', 'r', 'e', 'n', 't', 'l', 'i', 'n', 'e', '\n', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', 'e', 'n', 'd', '\n', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', 'i', 'n', 's', 't', 'a',
	'n', 'c', 'e', ' ', '=', ' ', 'c', 'a', 's', 't', '(', 'i',
	'n', 's', 't', 'a', 'n', 'c', 'e', ')', '\n', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', 'i', 'f', ' ', 'c', 'l', 'a', 's',
	's', '.', 'c', 'o', 'n', 's', 't', 'r', 'u', 'c', 't', 'o',
	'r', ' ', 't', 'h', 'e', 'n', '\n', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', 'l', 'o', 'c', 'a', 'l',
	' ', 'c', 'o', 'n', 's', 't', 'r', 'u', 'c', 't', 'i', 'o',
	'n', ' ', '=', ' ', 'c', 'l', 'a', 's', 's', '.', 'c', 'o',
	'n', 's', 't', 'r', 'u', 'c', 't', 'o', 'r', '(', 'i', 'n',
	's', 't', 'a', 'n', 'c', 'e', ',', ' ', 'u', 'n', 'p', 'a',
	'c', 'k', '(', 'a', 'r', 'g', 's', ',', ' ', 'a', 'r', 'g',
	's', '[', '1', ']', ' ', '=', '=', ' ', 'c', 'l', 'a', 's',
	's', ' ', 'a', 'n', 'd', ' ', '2', ' ', 'o', 'r', ' ', '1',
	')', ')', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', 'i', 'f', ' ', 'c', 'o', 'n', 's', 't', 'r',
	'u', 'c', 't', 'i', 'o', 'n', ' ', '~', '=', ' ', 'n', 'i',
	'l', ' ', 't', 'h', 'e', 'n', '\n', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'i',
	'n', 's', 't', 'a', 'n', 'c', 'e', ' ', '=', ' ', 'c', 'o',
	'n', 's', 't', 'r', 'u', 'c', 't', 'i', 'o', 'n', '\n', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'e',
	'n', 'd', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'e',
	'l', 's', 'e', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', 'l', 'o', 'c', 'a', 'l', ' ', 'b', 'a',
	's', 'e', ' ', '=', ' ', 'c', 'l', 'a', 's', 's', '.', '_',
	'i', 'n', 't', 'e', 'r', 'n', 'a', 'l', '_', 'b', 'a', 's',
	'e', 'C', 'l', 'a', 's', 's', '\n', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', 'w', 'h', 'i', 'l', 'e',
	' ', 'b', 'a', 's', 'e', ' ', 'd', 'o', '\n', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', 'i', 'f', ' ', 'b', 'a', 's', 'e', '.', 'c', 'o', 'n',
	's', 't', 'r', 'u', 'c', 't', 'o', 'r', ' ', 't', 'h', 'e',
	'n', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'l', 'o',
	'c', 'a', 'l', ' ', 'c', 'o', 'n', 's', 't', 'r', 'u', 'c',
	't', 'i', 'o', 'n', ' ', '=', ' ', 'b', 'a', 's', 'e', '.',
	'c', 'o', 'n', 's', 't', 'r', 'u', 'c', 't', 'o', 'r', '(',
	'i', 'n', 's', 't', 'a', 'n', 'c', 'e', ',', ' ', 'u', 'n',
	'p', 'a', 'c', 'k', '(', 'a', 'r', 'g', 's', ',', ' ', 'a',
	'r', 'g', 's', '[', '1', ']', ' ', '=', '=', ' ', 'c', 'l',
	'a', 's', 's', ' ', 'a', 'n', 'd', ' ', '2', ' ', 'o', 'r',
	' ', '1', ')', ')', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', 'i', 'f', ' ', 'c', 'o', 'n', 's', 't', 'r', 'u', 'c',
	't', 'i', 'o', 'n', ' ', '~', '=', ' ', 'n', 'i', 'l', ' ',
	't', 'h', 'e', 'n', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', 'i', 'n', 's', 't', 'a', 'n', 'c',
	'e', ' ', '=', ' ', 'c', 'o', 'n', 's', 't', 'r', 'u', 'c',
	't', 'i', 'o', 'n', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', 'e', 'n', 'd', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', 'b', 'r', 'e', 'a', 'k', '\n', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'e',
	'n', 'd', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', 'b', 'a', 's', 'e', ' ',
	'=', ' ', 'b', 'a', 's', 'e', '.', '_', 'i', 'n', 't', 'e',
	'r', 'n', 'a', 'l', '_', 'b', 'a', 's', 'e', 'C', 'l', 'a',
	's', 's', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', 'e', 'n', 'd', '\n', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', 'e', 'n', 'd', '\n', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', 'r', 'e', 't', 'u', 'r', 'n', ' ', 'i', 'n',
	's', 't', 'a', 'n', 'c', 'e', '\n', ' ', ' ', ' ', ' ', 'e',
	'n', 'd', '\n', '\n', ' ', ' ', ' ', ' ', 'c', 'l', 'a', 's',
	's', '.', 'c', 'a', 's', 't', ' ', '=', ' ', 'c', 'a', 's',
	't', '\n', ' ', ' ', ' ', ' ', 'c', 'l', 'a', 's', 's', '.',
	'c', 'r', 'e', 'a', 't', 'e', ' ', '=', ' ', 'c', 'r', 'e',
	'a', 't', 'e', '\n', ' ', ' ', ' ', ' ', 'c', 'l', 'a', 's',
	's', '.', 'c', 'l', 'a', 's', 's', ' ', '=', ' ', 'c', 'l',
	'a', 's', 's', '\n', ' ', ' ', ' ', ' ', 'c', 'l', 'a', 's',
	's', '.', 'n', 'e', 'w', ' ', '=', ' ', 'f', 'u', 'n', 'c',
	't', 'i', 'o', 'n', ' ', '(', '.', '.', '.', ')', ' ', 'r',
	'e', 't', 'u', 'r', 'n', ' ', 'c', 'r', 'e', 'a', 't', 'e',
	'(', '{', '}', ',', ' ', '.', '.', '.', ')', ' ', 'e', 'n',
	'd', '\n', ' ', ' ', ' ', ' ', 'c', 'l', 'a', 's', 's', '.',
	'_', 'i', 'n', 't', 'e', 'r', 'n', 'a', 'l', '_', 'i', 'n',
	's', 't', 'a', 'n', 'c', 'e', '_', 'm', 'e', 't', 'a', ' ',
	'=', ' ', '{', ' ', '_', '_', 'i', 'n', 'd', 'e', 'x', ' ',
	'=', ' ', 'c', 'l', 'a', 's', 's', '.', '_', '_', 'i', 'n',
	'd', 'e', 'x', ' ', '}', '\n', ' ', ' ', ' ', ' ', 'c', 'l',
	'a', 's', 's', '.', 'g', 'e', 't', 'C', 'l', 'a', 's', 's',
	'N', 'a', 'm', 'e', ' ', '=', ' ', 'f', 'u', 'n', 'c', 't',
	'i', 'o', 'n', ' ', '(', ')', ' ', 'r', 'e', 't', 'u', 'r',
	'n', ' ', 'c', 'l', 'a', 's', 's', 'N', 'a', 'm', 'e', ' ',
	'e', 'n', 'd', '\n', ' ', ' ', ' ', ' ', 's', 'e', 't', 'm',
	'e', 't', 'a', 't', 'a', 'b', 'l', 'e', '(', 'c', 'l', 'a',
	's', 's', ',', ' ', '{', ' ', '_', '_', 'n', 'e', 'w', 'i',
	'n', 'd', 'e', 'x', ' ', '=', ' ', 'c', 'l', 'a', 's', 's',
	'_', '_', 'n', 'e', 'w', 'i', 'n', 'd', 'e', 'x', ',', ' ',
	'_', '_', 'c', 'a', 'l', 'l', ' ', '=', ' ', 'c', 'l', 'a',
	's', 's', '_', '_', 'c', 'a', 'l', 'l', ',', ' ', '_', '_',
	't', 'o', 's', 't', 'r', 'i', 'n', 'g', ' ', '=', ' ', 'c',
	'l', 'a', 's', 's', '_', '_', 't', 'o', 's', 't', 'r', 'i',
	'n', 'g', ' ', '}', ')', '\n', ' ', ' ', ' ', ' ', 'i', 'f',
	' ', 'c', 'l', 'a', 's', 's', 'N', 'a', 'm', 'e', ' ', 't',
	'h', 'e', 'n', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	'_', 'G', '[', 'c', 'l', 'a', 's', 's', 'N', 'a', 'm', 'e',
	']', ' ', '=', ' ', 'c', 'l', 'a', 's', 's', '\n', ' ', ' ',
	' ', ' ', 'e', 'n', 'd', '\n', ' ', ' ', ' ', ' ', 'l', 'a',
	's', 't', 'C', 'r', 'e', 'a', 't', 'e', 'd', 'C', 'l', 'a',
	's', 's', ' ', '=', ' ', 'c', 'l', 'a', 's', 's', '\n', ' ',
	' ', ' ', ' ', 'r', 'e', 't', 'u', 'r', 'n', ' ', 'l', 'a',
	's', 't', 'C', 'r', 'e', 'a', 't', 'e', 'd', 'C', 'l', 'a',
	's', 's', '\n', 'e', 'n', 'd', '\n', '\n', 'f', 'u', 'n', 'c',
	't', 'i', 'o', 'n', ' ', 'i', 'n', 's', 't', 'a', 'n', 'c',
	'e', 'o', 'f', ' ', '(', 't', 'h', 'i', 'n', 'g', ',', ' ',
	'c', 'l', 'a', 's', 's', ')', '\n', ' ', ' ', ' ', ' ', 'i',
	'f', ' ', 't', 'y', 'p', 'e', '(', 't', 'h', 'i', 'n', 'g',
	')', ' ', '~', '=', ' ', '\'', 't', 'a', 'b', 'l', 'e', '\'',
	' ', 't', 'h', 'e', 'n', '\n', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', 'r', 'e', 't', 'u', 'r', 'n', '\n', ' ', ' ', ' ',
	' ', 'e', 'n', 'd', '\n', ' ', ' ', ' ', ' ', 'i', 'f', ' ',
	't', 'y', 'p', 'e', '(', 'c', 'l', 'a', 's', 's', ')', ' ',
	'=', '=', ' ', '\'', 's', 't', 'r', 'i', 'n', 'g', '\'', ' ',
	't', 'h', 'e', 'n', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', 'c', 'l', 'a', 's', 's', ' ', '=', ' ', '_', 'G', '[',
	'c', 'l', 'a', 's', 's', ']', '\n', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', 'i', 'f', ' ', 'n', 'o', 't', ' ', 'c', 'l',
	'a', 's', 's', ' ', 't', 'h', 'e', 'n', '\n', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'r', 'e', 't',
	'u', 'r', 'n', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	'e', 'n', 'd', '\n', ' ', ' ', ' ', ' ', 'e', 'n', 'd', '\n',
	' ', ' ', ' ', ' ', 'l', 'o', 'c', 'a', 'l', ' ', 'c', ' ',
	'=', ' ', 't', 'h', 'i', 'n', 'g', '.', '_', 'c', 'l', 'a',
	's', 's', '\n', ' ', ' ', ' ', ' ', 'w', 'h', 'i', 'l', 'e',
	' ', 'c', ' ', 'd', 'o', '\n', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', 'i', 'f', ' ', 'c', '.', 'n', 'a', 'm', 'e', ' ',
	'=', '=', ' ', 'c', 'l', 'a', 's', 's', '.', 'n', 'a', 'm',
	'e', ' ', 't', 'h', 'e', 'n', '\n', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', 'r', 'e', 't', 'u', 'r',
	'n', ' ', 't', 'r', 'u', 'e', '\n', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', 'e', 'n', 'd', '\n', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', 'c', ' ', '=', ' ', 'c', '.', '_', 'i', 'n',
	't', 'e', 'r', 'n', 'a', 'l', '_', 'b', 'a', 's', 'e', 'C',
	'l', 'a', 's', 's', '\n', ' ', ' ', ' ', ' ', 'e', 'n', 'd',
	'\n', 'e', 'n', 'd', '\n', '\n', 'f', 'u', 'n', 'c', 't', 'i',
	'o', 'n', ' ', 't', 'y', 'p', 'e', 'o', 'f', ' ', '(', 't',
	'h', 'i', 'n', 'g', ',', ' ', 'c', 'h', 'e', 'c', 'k', ')',
	'\n', '\n', ' ', ' ', ' ', ' ', 'l', 'o', 'c', 'a', 'l', ' ',
	't', ' ', '=', ' ', 't', 'y', 'p', 'e', '(', 't', 'h', 'i',
	'n', 'g', ')', '\n', '\n', ' ', ' ', ' ', ' ', 'i', 'f', ' ',
	'c', 'h', 'e', 'c', 'k', ' ', 't', 'h', 'e', 'n', '\n', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', 'i', 'f', ' ', 't', ' ',
	'=', '=', ' ', '\'', 't', 'a', 'b', 'l', 'e', '\'', ' ', 't',
	'h', 'e', 'n', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', 'i', 'f', ' ', 'c', 'h', 'e', 'c', 'k',
	' ', '=', '=', ' ', '\'', 't', 'a', 'b', 'l', 'e', '\'', ' ',
	't', 'h', 'e', 'n', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'r', 'e', 't',
	'u', 'r', 'n', ' ', 't', 'r', 'u', 'e', '\n', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'e', 'n', 'd',
	'\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', 'l', 'o', 'c', 'a', 'l', ' ', 'c', 'l', 'a', 's', 's',
	' ', '=', ' ', 't', 'h', 'i', 'n', 'g', '.', '_', 'c', 'l',
	'a', 's', 's', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', 'i', 'f', ' ', 'c', 'l', 'a', 's', 's',
	' ', 'a', 'n', 'd', ' ', 'c', 'l', 'a', 's', 's', '.', 'n',
	'a', 'm', 'e', ' ', '=', '=', ' ', 'c', 'h', 'e', 'c', 'k',
	' ', 't', 'h', 'e', 'n', '\n', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'r', 'e',
	't', 'u', 'r', 'n', ' ', 't', 'r', 'u', 'e', '\n', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'e', 'n',
	'd', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', 'l', 'o', 'c', 'a', 'l', ' ', 'g', 'e', 't', 'C',
	'l', 'a', 's', 's', 'N', 'a', 'm', 'e', ' ', '=', ' ', 't',
	'h', 'i', 'n', 'g', '.', 'g', 'e', 't', 'C', 'l', 'a', 's',
	's', 'N', 'a', 'm', 'e', '\n', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', 'i', 'f', ' ', 'g', 'e', 't',
	'C', 'l', 'a', 's', 's', 'N', 'a', 'm', 'e', ' ', 'a', 'n',
	'd', ' ', 'g', 'e', 't', 'C', 'l', 'a', 's', 's', 'N', 'a',
	'm', 'e', '(', ')', ' ', '=', '=', ' ', 'c', 'h', 'e', 'c',
	'k', ' ', 't', 'h', 'e', 'n', '\n', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'r',
	'e', 't', 'u', 'r', 'n', ' ', 't', 'r', 'u', 'e', '\n', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'e',
	'n', 'd', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', 'l', 'o', 'c', 'a', 'l', ' ', 'b', 'a', 's',
	'e', 'C', 'l', 'a', 's', 's', ' ', '=', ' ', 't', 'h', 'i',
	'n', 'g', '.', 'b', 'a', 's', 'e', 'C', 'l', 'a', 's', 's',
	'\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	' ', 'i', 'f', ' ', 'b', 'a', 's', 'e', 'C', 'l', 'a', 's',
	's', ' ', 't', 'h', 'e', 'n', '\n', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'r',
	'e', 't', 'u', 'r', 'n', ' ', 't', 'y', 'p', 'e', 'o', 'f',
	'(', 'b', 'a', 's', 'e', 'C', 'l', 'a', 's', 's', ',', ' ',
	'c', 'h', 'e', 'c', 'k', ')', '\n', ' ', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', 'e', 'n', 'd', '\n', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', 'e', 'n', 'd', '\n', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', 'r', 'e', 't', 'u', 'r',
	'n', ' ', 't', ' ', '=', '=', ' ', 'c', 'h', 'e', 'c', 'k',
	'\n', ' ', ' ', ' ', ' ', 'e', 'n', 'd', '\n', '\n', ' ', ' ',
	' ', ' ', 'i', 'f', ' ', 't', ' ', '=', '=', ' ', '\'', 't',
	'a', 'b', 'l', 'e', '\'', ' ', 't', 'h', 'e', 'n', '\n', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', 'l', 'o', 'c', 'a', 'l',
	' ', 'c', 'l', 'a', 's', 's', ' ', '=', ' ', 't', 'h', 'i',
	'n', 'g', '.', '_', 'c', 'l', 'a', 's', 's', '\n', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', 'i', 'f', ' ', 'c', 'l', 'a',
	's', 's', ' ', 't', 'h', 'e', 'n', '\n', ' ', ' ', ' ', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'r', 'e', 't', 'u',
	'r', 'n', ' ', 'c', 'l', 'a', 's', 's', '.', 'n', 'a', 'm',
	'e', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'e', 'n',
	'd', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'c', 'l',
	'a', 's', 's', ' ', '=', ' ', 't', 'h', 'i', 'n', 'g', '.',
	'g', 'e', 't', 'C', 'l', 'a', 's', 's', 'N', 'a', 'm', 'e',
	'\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'i', 'f', ' ',
	'c', 'l', 'a', 's', 's', ' ', 't', 'h', 'e', 'n', '\n', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'r',
	'e', 't', 'u', 'r', 'n', ' ', 'c', 'l', 'a', 's', 's', '(',
	')', '\n', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'e', 'n',
	'd', '\n', ' ', ' ', ' ', ' ', 'e', 'n', 'd', '\n', 'e', 'n',
	'd', '\n'
};

#endif