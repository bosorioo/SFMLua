/************
* Generated with lua2header.lua
* Original source code:
*
* 1 - local sub = string.sub
* 2 - local unpack = table.unpack
* 3 - local rawget = rawget
* 4 - local setmetatable = setmetatable

* 5 - local function class__call(class, ...)
* 6 - 	return class.new(...)
* 7 - end

* 8 - local function extends(derived, baseName)
* 9 - 	derived._internal_baseClass = _G[baseName]
* 10 - end

* 11 - local propagationMeta = {}

* 12 - propagationMeta.__index = function(propagationTable, index)
* 13 - 	local thisClass = rawget(propagationTable, 1)
* 14 - 	local instance = rawget(propagationTable, 2)
* 15 - 	
* 16 - 	local ret = rawget(instance, index) or (thisClass and thisClass[index])
* 17 - 	if ret then return ret end
* 18 - 	
* 19 - 	if index == 'super' and thisClass and thisClass._internal_baseClass then
* 20 - 		return setmetatable({thisClass._internal_baseClass, instance}, propagationMeta)
* 21 - 	end
* 22 - end

* 23 - propagationMeta.__call = function(propagationTable, ...)
* 24 - 	local thisClass = rawget(propagationTable, 1)
* 25 - 	local ctor = thisClass and rawget(thisClass, 'constructor')
* 26 - 	return ctor and ctor(propagationTable, ...)
* 27 - end

* 28 - propagationMeta.__newindex = function(propagationTable, index, value)
* 29 - 	rawget(propagationTable, 2)[index] = value
* 30 - end

* 31 - function Class (className)
* 32 - 	local class = {_internal_baseClass = nil, extends = extends}
* 33 - 	class._internal_instance_meta = {
* 34 - 		__index = function (instance, index)
* 35 - 			local ret = rawget(instance, index) or class[index]
* 36 - 			if ret then return ret end
* 37 - 			local base = class._internal_baseClass
* 38 - 			while base do
* 39 - 				ret = base[index]
* 40 - 				if ret then
* 41 - 					return ret
* 42 - 				end
* 43 - 				base = base._internal_baseClass
* 44 - 			end
* 45 - 			if index == 'super' and class._internal_baseClass then
* 46 - 				return setmetatable({class._internal_baseClass, instance}, propagationMeta)
* 47 - 			end
* 48 - 		end,
* 49 - 		__gc = function (instance)
* 50 - 			if class.destructor then 
* 51 - 				class.destructor(instance) 
* 52 - 				if class._internal_baseClass and class._internal_baseClass.destructor then
* 53 - 					class._internal_baseClass.destructor(instance)
* 54 - 				end
* 55 - 			end 
* 56 - 		end} 
* 57 - 	class.new = function(...)
* 58 - 		local args = {...} 
* 59 - 		local instance = --[[class.baseClass and class.baseClass.new({}) or]] {}
* 60 - 		setmetatable(instance, class._internal_instance_meta) 
* 61 - 		if class.constructor then
* 62 - 			local construction = class.constructor(instance, unpack(args, args[1] == class and 2 or 1))
* 63 - 			if construction ~= nil then
* 64 - 				instance = construction
* 65 - 			end
* 66 - 		end 
* 67 - 		return instance 
* 68 - 	end
* 69 - 	local function getName() return className or '?' end
* 70 - 	class.getClassName = getName class.typeof = className
* 71 - 	setmetatable(class, {__newindex = function (class, index, value)
* 72 - 		if type(index) == 'string' and sub(index, 1, 2) == '__' then
* 73 - 			rawset(class._internal_instance_meta, index, value)
* 74 - 		else 
* 75 - 			rawset(class, index, value)
* 76 - 		end 
* 77 - 	end, __call = class__call})
* 78 - 	if className then 
* 79 - 		_G[className] = class 
* 80 - 	end 
* 81 - 	return class 
* 82 - end

* 83 - function typeof(thing, check)

* 84 - 	local t = type(thing)

* 85 - 	if check then
* 86 - 		if t == 'table' then
* 87 - 			return check == 'table' or thing.getClassName and
* 88 - 			thing.getClassName() == check or thing.baseClass and
* 89 - 			typeof(thing.baseClass, check)
* 90 - 		end
* 91 - 		return t == check
* 92 - 	end
* 93 - 	
* 94 - 	return t == 'table' and thing.getClassName and thing.getClassName() or t
* 95 - end
*
************/

#ifndef SIMPLE_OOP_LUA_H
#define SIMPLE_OOP_LUA_H

const char lua_script_simple_oop[] = {
	0x6C, 0x6F, 0x63, 0x61, 0x6C, 0x20, 0x73, 0x75, 0x62, 0x20, 0x3D, 0x20,
	0x73, 0x74, 0x72, 0x69, 0x6E, 0x67, 0x2E, 0x73, 0x75, 0x62, 0x0A, 0x6C,
	0x6F, 0x63, 0x61, 0x6C, 0x20, 0x75, 0x6E, 0x70, 0x61, 0x63, 0x6B, 0x20,
	0x3D, 0x20, 0x74, 0x61, 0x62, 0x6C, 0x65, 0x2E, 0x75, 0x6E, 0x70, 0x61,
	0x63, 0x6B, 0x0A, 0x6C, 0x6F, 0x63, 0x61, 0x6C, 0x20, 0x72, 0x61, 0x77,
	0x67, 0x65, 0x74, 0x20, 0x3D, 0x20, 0x72, 0x61, 0x77, 0x67, 0x65, 0x74,
	0x0A, 0x6C, 0x6F, 0x63, 0x61, 0x6C, 0x20, 0x73, 0x65, 0x74, 0x6D, 0x65,
	0x74, 0x61, 0x74, 0x61, 0x62, 0x6C, 0x65, 0x20, 0x3D, 0x20, 0x73, 0x65,
	0x74, 0x6D, 0x65, 0x74, 0x61, 0x74, 0x61, 0x62, 0x6C, 0x65, 0x0A, 0x0A,
	0x6C, 0x6F, 0x63, 0x61, 0x6C, 0x20, 0x66, 0x75, 0x6E, 0x63, 0x74, 0x69,
	0x6F, 0x6E, 0x20, 0x63, 0x6C, 0x61, 0x73, 0x73, 0x5F, 0x5F, 0x63, 0x61,
	0x6C, 0x6C, 0x28, 0x63, 0x6C, 0x61, 0x73, 0x73, 0x2C, 0x20, 0x2E, 0x2E,
	0x2E, 0x29, 0x0A, 0x09, 0x72, 0x65, 0x74, 0x75, 0x72, 0x6E, 0x20, 0x63,
	0x6C, 0x61, 0x73, 0x73, 0x2E, 0x6E, 0x65, 0x77, 0x28, 0x2E, 0x2E, 0x2E,
	0x29, 0x0A, 0x65, 0x6E, 0x64, 0x0A, 0x0A, 0x6C, 0x6F, 0x63, 0x61, 0x6C,
	0x20, 0x66, 0x75, 0x6E, 0x63, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x65, 0x78,
	0x74, 0x65, 0x6E, 0x64, 0x73, 0x28, 0x64, 0x65, 0x72, 0x69, 0x76, 0x65,
	0x64, 0x2C, 0x20, 0x62, 0x61, 0x73, 0x65, 0x4E, 0x61, 0x6D, 0x65, 0x29,
	0x0A, 0x09, 0x64, 0x65, 0x72, 0x69, 0x76, 0x65, 0x64, 0x2E, 0x5F, 0x69,
	0x6E, 0x74, 0x65, 0x72, 0x6E, 0x61, 0x6C, 0x5F, 0x62, 0x61, 0x73, 0x65,
	0x43, 0x6C, 0x61, 0x73, 0x73, 0x20, 0x3D, 0x20, 0x5F, 0x47, 0x5B, 0x62,
	0x61, 0x73, 0x65, 0x4E, 0x61, 0x6D, 0x65, 0x5D, 0x0A, 0x65, 0x6E, 0x64,
	0x0A, 0x0A, 0x6C, 0x6F, 0x63, 0x61, 0x6C, 0x20, 0x70, 0x72, 0x6F, 0x70,
	0x61, 0x67, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x4D, 0x65, 0x74, 0x61, 0x20,
	0x3D, 0x20, 0x7B, 0x7D, 0x0A, 0x0A, 0x70, 0x72, 0x6F, 0x70, 0x61, 0x67,
	0x61, 0x74, 0x69, 0x6F, 0x6E, 0x4D, 0x65, 0x74, 0x61, 0x2E, 0x5F, 0x5F,
	0x69, 0x6E, 0x64, 0x65, 0x78, 0x20, 0x3D, 0x20, 0x66, 0x75, 0x6E, 0x63,
	0x74, 0x69, 0x6F, 0x6E, 0x28, 0x70, 0x72, 0x6F, 0x70, 0x61, 0x67, 0x61,
	0x74, 0x69, 0x6F, 0x6E, 0x54, 0x61, 0x62, 0x6C, 0x65, 0x2C, 0x20, 0x69,
	0x6E, 0x64, 0x65, 0x78, 0x29, 0x0A, 0x09, 0x6C, 0x6F, 0x63, 0x61, 0x6C,
	0x20, 0x74, 0x68, 0x69, 0x73, 0x43, 0x6C, 0x61, 0x73, 0x73, 0x20, 0x3D,
	0x20, 0x72, 0x61, 0x77, 0x67, 0x65, 0x74, 0x28, 0x70, 0x72, 0x6F, 0x70,
	0x61, 0x67, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x54, 0x61, 0x62, 0x6C, 0x65,
	0x2C, 0x20, 0x31, 0x29, 0x0A, 0x09, 0x6C, 0x6F, 0x63, 0x61, 0x6C, 0x20,
	0x69, 0x6E, 0x73, 0x74, 0x61, 0x6E, 0x63, 0x65, 0x20, 0x3D, 0x20, 0x72,
	0x61, 0x77, 0x67, 0x65, 0x74, 0x28, 0x70, 0x72, 0x6F, 0x70, 0x61, 0x67,
	0x61, 0x74, 0x69, 0x6F, 0x6E, 0x54, 0x61, 0x62, 0x6C, 0x65, 0x2C, 0x20,
	0x32, 0x29, 0x0A, 0x09, 0x0A, 0x09, 0x6C, 0x6F, 0x63, 0x61, 0x6C, 0x20,
	0x72, 0x65, 0x74, 0x20, 0x3D, 0x20, 0x72, 0x61, 0x77, 0x67, 0x65, 0x74,
	0x28, 0x69, 0x6E, 0x73, 0x74, 0x61, 0x6E, 0x63, 0x65, 0x2C, 0x20, 0x69,
	0x6E, 0x64, 0x65, 0x78, 0x29, 0x20, 0x6F, 0x72, 0x20, 0x28, 0x74, 0x68,
	0x69, 0x73, 0x43, 0x6C, 0x61, 0x73, 0x73, 0x20, 0x61, 0x6E, 0x64, 0x20,
	0x74, 0x68, 0x69, 0x73, 0x43, 0x6C, 0x61, 0x73, 0x73, 0x5B, 0x69, 0x6E,
	0x64, 0x65, 0x78, 0x5D, 0x29, 0x0A, 0x09, 0x69, 0x66, 0x20, 0x72, 0x65,
	0x74, 0x20, 0x74, 0x68, 0x65, 0x6E, 0x20, 0x72, 0x65, 0x74, 0x75, 0x72,
	0x6E, 0x20, 0x72, 0x65, 0x74, 0x20, 0x65, 0x6E, 0x64, 0x0A, 0x09, 0x0A,
	0x09, 0x69, 0x66, 0x20, 0x69, 0x6E, 0x64, 0x65, 0x78, 0x20, 0x3D, 0x3D,
	0x20, 0x27, 0x73, 0x75, 0x70, 0x65, 0x72, 0x27, 0x20, 0x61, 0x6E, 0x64,
	0x20, 0x74, 0x68, 0x69, 0x73, 0x43, 0x6C, 0x61, 0x73, 0x73, 0x20, 0x61,
	0x6E, 0x64, 0x20, 0x74, 0x68, 0x69, 0x73, 0x43, 0x6C, 0x61, 0x73, 0x73,
	0x2E, 0x5F, 0x69, 0x6E, 0x74, 0x65, 0x72, 0x6E, 0x61, 0x6C, 0x5F, 0x62,
	0x61, 0x73, 0x65, 0x43, 0x6C, 0x61, 0x73, 0x73, 0x20, 0x74, 0x68, 0x65,
	0x6E, 0x0A, 0x09, 0x09, 0x72, 0x65, 0x74, 0x75, 0x72, 0x6E, 0x20, 0x73,
	0x65, 0x74, 0x6D, 0x65, 0x74, 0x61, 0x74, 0x61, 0x62, 0x6C, 0x65, 0x28,
	0x7B, 0x74, 0x68, 0x69, 0x73, 0x43, 0x6C, 0x61, 0x73, 0x73, 0x2E, 0x5F,
	0x69, 0x6E, 0x74, 0x65, 0x72, 0x6E, 0x61, 0x6C, 0x5F, 0x62, 0x61, 0x73,
	0x65, 0x43, 0x6C, 0x61, 0x73, 0x73, 0x2C, 0x20, 0x69, 0x6E, 0x73, 0x74,
	0x61, 0x6E, 0x63, 0x65, 0x7D, 0x2C, 0x20, 0x70, 0x72, 0x6F, 0x70, 0x61,
	0x67, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x4D, 0x65, 0x74, 0x61, 0x29, 0x0A,
	0x09, 0x65, 0x6E, 0x64, 0x0A, 0x65, 0x6E, 0x64, 0x0A, 0x0A, 0x70, 0x72,
	0x6F, 0x70, 0x61, 0x67, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x4D, 0x65, 0x74,
	0x61, 0x2E, 0x5F, 0x5F, 0x63, 0x61, 0x6C, 0x6C, 0x20, 0x3D, 0x20, 0x66,
	0x75, 0x6E, 0x63, 0x74, 0x69, 0x6F, 0x6E, 0x28, 0x70, 0x72, 0x6F, 0x70,
	0x61, 0x67, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x54, 0x61, 0x62, 0x6C, 0x65,
	0x2C, 0x20, 0x2E, 0x2E, 0x2E, 0x29, 0x0A, 0x09, 0x6C, 0x6F, 0x63, 0x61,
	0x6C, 0x20, 0x74, 0x68, 0x69, 0x73, 0x43, 0x6C, 0x61, 0x73, 0x73, 0x20,
	0x3D, 0x20, 0x72, 0x61, 0x77, 0x67, 0x65, 0x74, 0x28, 0x70, 0x72, 0x6F,
	0x70, 0x61, 0x67, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x54, 0x61, 0x62, 0x6C,
	0x65, 0x2C, 0x20, 0x31, 0x29, 0x0A, 0x09, 0x6C, 0x6F, 0x63, 0x61, 0x6C,
	0x20, 0x63, 0x74, 0x6F, 0x72, 0x20, 0x3D, 0x20, 0x74, 0x68, 0x69, 0x73,
	0x43, 0x6C, 0x61, 0x73, 0x73, 0x20, 0x61, 0x6E, 0x64, 0x20, 0x72, 0x61,
	0x77, 0x67, 0x65, 0x74, 0x28, 0x74, 0x68, 0x69, 0x73, 0x43, 0x6C, 0x61,
	0x73, 0x73, 0x2C, 0x20, 0x27, 0x63, 0x6F, 0x6E, 0x73, 0x74, 0x72, 0x75,
	0x63, 0x74, 0x6F, 0x72, 0x27, 0x29, 0x0A, 0x09, 0x72, 0x65, 0x74, 0x75,
	0x72, 0x6E, 0x20, 0x63, 0x74, 0x6F, 0x72, 0x20, 0x61, 0x6E, 0x64, 0x20,
	0x63, 0x74, 0x6F, 0x72, 0x28, 0x70, 0x72, 0x6F, 0x70, 0x61, 0x67, 0x61,
	0x74, 0x69, 0x6F, 0x6E, 0x54, 0x61, 0x62, 0x6C, 0x65, 0x2C, 0x20, 0x2E,
	0x2E, 0x2E, 0x29, 0x0A, 0x65, 0x6E, 0x64, 0x0A, 0x0A, 0x70, 0x72, 0x6F,
	0x70, 0x61, 0x67, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x4D, 0x65, 0x74, 0x61,
	0x2E, 0x5F, 0x5F, 0x6E, 0x65, 0x77, 0x69, 0x6E, 0x64, 0x65, 0x78, 0x20,
	0x3D, 0x20, 0x66, 0x75, 0x6E, 0x63, 0x74, 0x69, 0x6F, 0x6E, 0x28, 0x70,
	0x72, 0x6F, 0x70, 0x61, 0x67, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x54, 0x61,
	0x62, 0x6C, 0x65, 0x2C, 0x20, 0x69, 0x6E, 0x64, 0x65, 0x78, 0x2C, 0x20,
	0x76, 0x61, 0x6C, 0x75, 0x65, 0x29, 0x0A, 0x09, 0x72, 0x61, 0x77, 0x67,
	0x65, 0x74, 0x28, 0x70, 0x72, 0x6F, 0x70, 0x61, 0x67, 0x61, 0x74, 0x69,
	0x6F, 0x6E, 0x54, 0x61, 0x62, 0x6C, 0x65, 0x2C, 0x20, 0x32, 0x29, 0x5B,
	0x69, 0x6E, 0x64, 0x65, 0x78, 0x5D, 0x20, 0x3D, 0x20, 0x76, 0x61, 0x6C,
	0x75, 0x65, 0x0A, 0x65, 0x6E, 0x64, 0x0A, 0x0A, 0x66, 0x75, 0x6E, 0x63,
	0x74, 0x69, 0x6F, 0x6E, 0x20, 0x43, 0x6C, 0x61, 0x73, 0x73, 0x20, 0x28,
	0x63, 0x6C, 0x61, 0x73, 0x73, 0x4E, 0x61, 0x6D, 0x65, 0x29, 0x0A, 0x09,
	0x6C, 0x6F, 0x63, 0x61, 0x6C, 0x20, 0x63, 0x6C, 0x61, 0x73, 0x73, 0x20,
	0x3D, 0x20, 0x7B, 0x5F, 0x69, 0x6E, 0x74, 0x65, 0x72, 0x6E, 0x61, 0x6C,
	0x5F, 0x62, 0x61, 0x73, 0x65, 0x43, 0x6C, 0x61, 0x73, 0x73, 0x20, 0x3D,
	0x20, 0x6E, 0x69, 0x6C, 0x2C, 0x20, 0x65, 0x78, 0x74, 0x65, 0x6E, 0x64,
	0x73, 0x20, 0x3D, 0x20, 0x65, 0x78, 0x74, 0x65, 0x6E, 0x64, 0x73, 0x7D,
	0x0A, 0x09, 0x63, 0x6C, 0x61, 0x73, 0x73, 0x2E, 0x5F, 0x69, 0x6E, 0x74,
	0x65, 0x72, 0x6E, 0x61, 0x6C, 0x5F, 0x69, 0x6E, 0x73, 0x74, 0x61, 0x6E,
	0x63, 0x65, 0x5F, 0x6D, 0x65, 0x74, 0x61, 0x20, 0x3D, 0x20, 0x7B, 0x0A,
	0x09, 0x09, 0x5F, 0x5F, 0x69, 0x6E, 0x64, 0x65, 0x78, 0x20, 0x3D, 0x20,
	0x66, 0x75, 0x6E, 0x63, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x28, 0x69, 0x6E,
	0x73, 0x74, 0x61, 0x6E, 0x63, 0x65, 0x2C, 0x20, 0x69, 0x6E, 0x64, 0x65,
	0x78, 0x29, 0x0A, 0x09, 0x09, 0x09, 0x6C, 0x6F, 0x63, 0x61, 0x6C, 0x20,
	0x72, 0x65, 0x74, 0x20, 0x3D, 0x20, 0x72, 0x61, 0x77, 0x67, 0x65, 0x74,
	0x28, 0x69, 0x6E, 0x73, 0x74, 0x61, 0x6E, 0x63, 0x65, 0x2C, 0x20, 0x69,
	0x6E, 0x64, 0x65, 0x78, 0x29, 0x20, 0x6F, 0x72, 0x20, 0x63, 0x6C, 0x61,
	0x73, 0x73, 0x5B, 0x69, 0x6E, 0x64, 0x65, 0x78, 0x5D, 0x0A, 0x09, 0x09,
	0x09, 0x69, 0x66, 0x20, 0x72, 0x65, 0x74, 0x20, 0x74, 0x68, 0x65, 0x6E,
	0x20, 0x72, 0x65, 0x74, 0x75, 0x72, 0x6E, 0x20, 0x72, 0x65, 0x74, 0x20,
	0x65, 0x6E, 0x64, 0x0A, 0x09, 0x09, 0x09, 0x6C, 0x6F, 0x63, 0x61, 0x6C,
	0x20, 0x62, 0x61, 0x73, 0x65, 0x20, 0x3D, 0x20, 0x63, 0x6C, 0x61, 0x73,
	0x73, 0x2E, 0x5F, 0x69, 0x6E, 0x74, 0x65, 0x72, 0x6E, 0x61, 0x6C, 0x5F,
	0x62, 0x61, 0x73, 0x65, 0x43, 0x6C, 0x61, 0x73, 0x73, 0x0A, 0x09, 0x09,
	0x09, 0x77, 0x68, 0x69, 0x6C, 0x65, 0x20, 0x62, 0x61, 0x73, 0x65, 0x20,
	0x64, 0x6F, 0x0A, 0x09, 0x09, 0x09, 0x09, 0x72, 0x65, 0x74, 0x20, 0x3D,
	0x20, 0x62, 0x61, 0x73, 0x65, 0x5B, 0x69, 0x6E, 0x64, 0x65, 0x78, 0x5D,
	0x0A, 0x09, 0x09, 0x09, 0x09, 0x69, 0x66, 0x20, 0x72, 0x65, 0x74, 0x20,
	0x74, 0x68, 0x65, 0x6E, 0x0A, 0x09, 0x09, 0x09, 0x09, 0x09, 0x72, 0x65,
	0x74, 0x75, 0x72, 0x6E, 0x20, 0x72, 0x65, 0x74, 0x0A, 0x09, 0x09, 0x09,
	0x09, 0x65, 0x6E, 0x64, 0x0A, 0x09, 0x09, 0x09, 0x09, 0x62, 0x61, 0x73,
	0x65, 0x20, 0x3D, 0x20, 0x62, 0x61, 0x73, 0x65, 0x2E, 0x5F, 0x69, 0x6E,
	0x74, 0x65, 0x72, 0x6E, 0x61, 0x6C, 0x5F, 0x62, 0x61, 0x73, 0x65, 0x43,
	0x6C, 0x61, 0x73, 0x73, 0x0A, 0x09, 0x09, 0x09, 0x65, 0x6E, 0x64, 0x0A,
	0x09, 0x09, 0x09, 0x69, 0x66, 0x20, 0x69, 0x6E, 0x64, 0x65, 0x78, 0x20,
	0x3D, 0x3D, 0x20, 0x27, 0x73, 0x75, 0x70, 0x65, 0x72, 0x27, 0x20, 0x61,
	0x6E, 0x64, 0x20, 0x63, 0x6C, 0x61, 0x73, 0x73, 0x2E, 0x5F, 0x69, 0x6E,
	0x74, 0x65, 0x72, 0x6E, 0x61, 0x6C, 0x5F, 0x62, 0x61, 0x73, 0x65, 0x43,
	0x6C, 0x61, 0x73, 0x73, 0x20, 0x74, 0x68, 0x65, 0x6E, 0x0A, 0x09, 0x09,
	0x09, 0x09, 0x72, 0x65, 0x74, 0x75, 0x72, 0x6E, 0x20, 0x73, 0x65, 0x74,
	0x6D, 0x65, 0x74, 0x61, 0x74, 0x61, 0x62, 0x6C, 0x65, 0x28, 0x7B, 0x63,
	0x6C, 0x61, 0x73, 0x73, 0x2E, 0x5F, 0x69, 0x6E, 0x74, 0x65, 0x72, 0x6E,
	0x61, 0x6C, 0x5F, 0x62, 0x61, 0x73, 0x65, 0x43, 0x6C, 0x61, 0x73, 0x73,
	0x2C, 0x20, 0x69, 0x6E, 0x73, 0x74, 0x61, 0x6E, 0x63, 0x65, 0x7D, 0x2C,
	0x20, 0x70, 0x72, 0x6F, 0x70, 0x61, 0x67, 0x61, 0x74, 0x69, 0x6F, 0x6E,
	0x4D, 0x65, 0x74, 0x61, 0x29, 0x0A, 0x09, 0x09, 0x09, 0x65, 0x6E, 0x64,
	0x0A, 0x09, 0x09, 0x65, 0x6E, 0x64, 0x2C, 0x0A, 0x09, 0x09, 0x5F, 0x5F,
	0x67, 0x63, 0x20, 0x3D, 0x20, 0x66, 0x75, 0x6E, 0x63, 0x74, 0x69, 0x6F,
	0x6E, 0x20, 0x28, 0x69, 0x6E, 0x73, 0x74, 0x61, 0x6E, 0x63, 0x65, 0x29,
	0x0A, 0x09, 0x09, 0x09, 0x69, 0x66, 0x20, 0x63, 0x6C, 0x61, 0x73, 0x73,
	0x2E, 0x64, 0x65, 0x73, 0x74, 0x72, 0x75, 0x63, 0x74, 0x6F, 0x72, 0x20,
	0x74, 0x68, 0x65, 0x6E, 0x20, 0x0A, 0x09, 0x09, 0x09, 0x09, 0x63, 0x6C,
	0x61, 0x73, 0x73, 0x2E, 0x64, 0x65, 0x73, 0x74, 0x72, 0x75, 0x63, 0x74,
	0x6F, 0x72, 0x28, 0x69, 0x6E, 0x73, 0x74, 0x61, 0x6E, 0x63, 0x65, 0x29,
	0x20, 0x0A, 0x09, 0x09, 0x09, 0x09, 0x69, 0x66, 0x20, 0x63, 0x6C, 0x61,
	0x73, 0x73, 0x2E, 0x5F, 0x69, 0x6E, 0x74, 0x65, 0x72, 0x6E, 0x61, 0x6C,
	0x5F, 0x62, 0x61, 0x73, 0x65, 0x43, 0x6C, 0x61, 0x73, 0x73, 0x20, 0x61,
	0x6E, 0x64, 0x20, 0x63, 0x6C, 0x61, 0x73, 0x73, 0x2E, 0x5F, 0x69, 0x6E,
	0x74, 0x65, 0x72, 0x6E, 0x61, 0x6C, 0x5F, 0x62, 0x61, 0x73, 0x65, 0x43,
	0x6C, 0x61, 0x73, 0x73, 0x2E, 0x64, 0x65, 0x73, 0x74, 0x72, 0x75, 0x63,
	0x74, 0x6F, 0x72, 0x20, 0x74, 0x68, 0x65, 0x6E, 0x0A, 0x09, 0x09, 0x09,
	0x09, 0x09, 0x63, 0x6C, 0x61, 0x73, 0x73, 0x2E, 0x5F, 0x69, 0x6E, 0x74,
	0x65, 0x72, 0x6E, 0x61, 0x6C, 0x5F, 0x62, 0x61, 0x73, 0x65, 0x43, 0x6C,
	0x61, 0x73, 0x73, 0x2E, 0x64, 0x65, 0x73, 0x74, 0x72, 0x75, 0x63, 0x74,
	0x6F, 0x72, 0x28, 0x69, 0x6E, 0x73, 0x74, 0x61, 0x6E, 0x63, 0x65, 0x29,
	0x0A, 0x09, 0x09, 0x09, 0x09, 0x65, 0x6E, 0x64, 0x0A, 0x09, 0x09, 0x09,
	0x65, 0x6E, 0x64, 0x20, 0x0A, 0x09, 0x09, 0x65, 0x6E, 0x64, 0x7D, 0x20,
	0x0A, 0x09, 0x63, 0x6C, 0x61, 0x73, 0x73, 0x2E, 0x6E, 0x65, 0x77, 0x20,
	0x3D, 0x20, 0x66, 0x75, 0x6E, 0x63, 0x74, 0x69, 0x6F, 0x6E, 0x28, 0x2E,
	0x2E, 0x2E, 0x29, 0x0A, 0x09, 0x09, 0x6C, 0x6F, 0x63, 0x61, 0x6C, 0x20,
	0x61, 0x72, 0x67, 0x73, 0x20, 0x3D, 0x20, 0x7B, 0x2E, 0x2E, 0x2E, 0x7D,
	0x20, 0x0A, 0x09, 0x09, 0x6C, 0x6F, 0x63, 0x61, 0x6C, 0x20, 0x69, 0x6E,
	0x73, 0x74, 0x61, 0x6E, 0x63, 0x65, 0x20, 0x3D, 0x20, 0x2D, 0x2D, 0x5B,
	0x5B, 0x63, 0x6C, 0x61, 0x73, 0x73, 0x2E, 0x62, 0x61, 0x73, 0x65, 0x43,
	0x6C, 0x61, 0x73, 0x73, 0x20, 0x61, 0x6E, 0x64, 0x20, 0x63, 0x6C, 0x61,
	0x73, 0x73, 0x2E, 0x62, 0x61, 0x73, 0x65, 0x43, 0x6C, 0x61, 0x73, 0x73,
	0x2E, 0x6E, 0x65, 0x77, 0x28, 0x7B, 0x7D, 0x29, 0x20, 0x6F, 0x72, 0x5D,
	0x5D, 0x20, 0x7B, 0x7D, 0x0A, 0x09, 0x09, 0x73, 0x65, 0x74, 0x6D, 0x65,
	0x74, 0x61, 0x74, 0x61, 0x62, 0x6C, 0x65, 0x28, 0x69, 0x6E, 0x73, 0x74,
	0x61, 0x6E, 0x63, 0x65, 0x2C, 0x20, 0x63, 0x6C, 0x61, 0x73, 0x73, 0x2E,
	0x5F, 0x69, 0x6E, 0x74, 0x65, 0x72, 0x6E, 0x61, 0x6C, 0x5F, 0x69, 0x6E,
	0x73, 0x74, 0x61, 0x6E, 0x63, 0x65, 0x5F, 0x6D, 0x65, 0x74, 0x61, 0x29,
	0x20, 0x0A, 0x09, 0x09, 0x69, 0x66, 0x20, 0x63, 0x6C, 0x61, 0x73, 0x73,
	0x2E, 0x63, 0x6F, 0x6E, 0x73, 0x74, 0x72, 0x75, 0x63, 0x74, 0x6F, 0x72,
	0x20, 0x74, 0x68, 0x65, 0x6E, 0x0A, 0x09, 0x09, 0x09, 0x6C, 0x6F, 0x63,
	0x61, 0x6C, 0x20, 0x63, 0x6F, 0x6E, 0x73, 0x74, 0x72, 0x75, 0x63, 0x74,
	0x69, 0x6F, 0x6E, 0x20, 0x3D, 0x20, 0x63, 0x6C, 0x61, 0x73, 0x73, 0x2E,
	0x63, 0x6F, 0x6E, 0x73, 0x74, 0x72, 0x75, 0x63, 0x74, 0x6F, 0x72, 0x28,
	0x69, 0x6E, 0x73, 0x74, 0x61, 0x6E, 0x63, 0x65, 0x2C, 0x20, 0x75, 0x6E,
	0x70, 0x61, 0x63, 0x6B, 0x28, 0x61, 0x72, 0x67, 0x73, 0x2C, 0x20, 0x61,
	0x72, 0x67, 0x73, 0x5B, 0x31, 0x5D, 0x20, 0x3D, 0x3D, 0x20, 0x63, 0x6C,
	0x61, 0x73, 0x73, 0x20, 0x61, 0x6E, 0x64, 0x20, 0x32, 0x20, 0x6F, 0x72,
	0x20, 0x31, 0x29, 0x29, 0x0A, 0x09, 0x09, 0x09, 0x69, 0x66, 0x20, 0x63,
	0x6F, 0x6E, 0x73, 0x74, 0x72, 0x75, 0x63, 0x74, 0x69, 0x6F, 0x6E, 0x20,
	0x7E, 0x3D, 0x20, 0x6E, 0x69, 0x6C, 0x20, 0x74, 0x68, 0x65, 0x6E, 0x0A,
	0x09, 0x09, 0x09, 0x09, 0x69, 0x6E, 0x73, 0x74, 0x61, 0x6E, 0x63, 0x65,
	0x20, 0x3D, 0x20, 0x63, 0x6F, 0x6E, 0x73, 0x74, 0x72, 0x75, 0x63, 0x74,
	0x69, 0x6F, 0x6E, 0x0A, 0x09, 0x09, 0x09, 0x65, 0x6E, 0x64, 0x0A, 0x09,
	0x09, 0x65, 0x6E, 0x64, 0x20, 0x0A, 0x09, 0x09, 0x72, 0x65, 0x74, 0x75,
	0x72, 0x6E, 0x20, 0x69, 0x6E, 0x73, 0x74, 0x61, 0x6E, 0x63, 0x65, 0x20,
	0x0A, 0x09, 0x65, 0x6E, 0x64, 0x0A, 0x09, 0x6C, 0x6F, 0x63, 0x61, 0x6C,
	0x20, 0x66, 0x75, 0x6E, 0x63, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x67, 0x65,
	0x74, 0x4E, 0x61, 0x6D, 0x65, 0x28, 0x29, 0x20, 0x72, 0x65, 0x74, 0x75,
	0x72, 0x6E, 0x20, 0x63, 0x6C, 0x61, 0x73, 0x73, 0x4E, 0x61, 0x6D, 0x65,
	0x20, 0x6F, 0x72, 0x20, 0x27, 0x3F, 0x27, 0x20, 0x65, 0x6E, 0x64, 0x0A,
	0x09, 0x63, 0x6C, 0x61, 0x73, 0x73, 0x2E, 0x67, 0x65, 0x74, 0x43, 0x6C,
	0x61, 0x73, 0x73, 0x4E, 0x61, 0x6D, 0x65, 0x20, 0x3D, 0x20, 0x67, 0x65,
	0x74, 0x4E, 0x61, 0x6D, 0x65, 0x20, 0x63, 0x6C, 0x61, 0x73, 0x73, 0x2E,
	0x74, 0x79, 0x70, 0x65, 0x6F, 0x66, 0x20, 0x3D, 0x20, 0x63, 0x6C, 0x61,
	0x73, 0x73, 0x4E, 0x61, 0x6D, 0x65, 0x0A, 0x09, 0x73, 0x65, 0x74, 0x6D,
	0x65, 0x74, 0x61, 0x74, 0x61, 0x62, 0x6C, 0x65, 0x28, 0x63, 0x6C, 0x61,
	0x73, 0x73, 0x2C, 0x20, 0x7B, 0x5F, 0x5F, 0x6E, 0x65, 0x77, 0x69, 0x6E,
	0x64, 0x65, 0x78, 0x20, 0x3D, 0x20, 0x66, 0x75, 0x6E, 0x63, 0x74, 0x69,
	0x6F, 0x6E, 0x20, 0x28, 0x63, 0x6C, 0x61, 0x73, 0x73, 0x2C, 0x20, 0x69,
	0x6E, 0x64, 0x65, 0x78, 0x2C, 0x20, 0x76, 0x61, 0x6C, 0x75, 0x65, 0x29,
	0x0A, 0x09, 0x09, 0x69, 0x66, 0x20, 0x74, 0x79, 0x70, 0x65, 0x28, 0x69,
	0x6E, 0x64, 0x65, 0x78, 0x29, 0x20, 0x3D, 0x3D, 0x20, 0x27, 0x73, 0x74,
	0x72, 0x69, 0x6E, 0x67, 0x27, 0x20, 0x61, 0x6E, 0x64, 0x20, 0x73, 0x75,
	0x62, 0x28, 0x69, 0x6E, 0x64, 0x65, 0x78, 0x2C, 0x20, 0x31, 0x2C, 0x20,
	0x32, 0x29, 0x20, 0x3D, 0x3D, 0x20, 0x27, 0x5F, 0x5F, 0x27, 0x20, 0x74,
	0x68, 0x65, 0x6E, 0x0A, 0x09, 0x09, 0x09, 0x72, 0x61, 0x77, 0x73, 0x65,
	0x74, 0x28, 0x63, 0x6C, 0x61, 0x73, 0x73, 0x2E, 0x5F, 0x69, 0x6E, 0x74,
	0x65, 0x72, 0x6E, 0x61, 0x6C, 0x5F, 0x69, 0x6E, 0x73, 0x74, 0x61, 0x6E,
	0x63, 0x65, 0x5F, 0x6D, 0x65, 0x74, 0x61, 0x2C, 0x20, 0x69, 0x6E, 0x64,
	0x65, 0x78, 0x2C, 0x20, 0x76, 0x61, 0x6C, 0x75, 0x65, 0x29, 0x0A, 0x09,
	0x09, 0x65, 0x6C, 0x73, 0x65, 0x20, 0x0A, 0x09, 0x09, 0x09, 0x72, 0x61,
	0x77, 0x73, 0x65, 0x74, 0x28, 0x63, 0x6C, 0x61, 0x73, 0x73, 0x2C, 0x20,
	0x69, 0x6E, 0x64, 0x65, 0x78, 0x2C, 0x20, 0x76, 0x61, 0x6C, 0x75, 0x65,
	0x29, 0x0A, 0x09, 0x09, 0x65, 0x6E, 0x64, 0x20, 0x0A, 0x09, 0x65, 0x6E,
	0x64, 0x2C, 0x20, 0x5F, 0x5F, 0x63, 0x61, 0x6C, 0x6C, 0x20, 0x3D, 0x20,
	0x63, 0x6C, 0x61, 0x73, 0x73, 0x5F, 0x5F, 0x63, 0x61, 0x6C, 0x6C, 0x7D,
	0x29, 0x0A, 0x09, 0x69, 0x66, 0x20, 0x63, 0x6C, 0x61, 0x73, 0x73, 0x4E,
	0x61, 0x6D, 0x65, 0x20, 0x74, 0x68, 0x65, 0x6E, 0x20, 0x0A, 0x09, 0x09,
	0x5F, 0x47, 0x5B, 0x63, 0x6C, 0x61, 0x73, 0x73, 0x4E, 0x61, 0x6D, 0x65,
	0x5D, 0x20, 0x3D, 0x20, 0x63, 0x6C, 0x61, 0x73, 0x73, 0x20, 0x0A, 0x09,
	0x65, 0x6E, 0x64, 0x20, 0x0A, 0x09, 0x72, 0x65, 0x74, 0x75, 0x72, 0x6E,
	0x20, 0x63, 0x6C, 0x61, 0x73, 0x73, 0x20, 0x0A, 0x65, 0x6E, 0x64, 0x0A,
	0x0A, 0x66, 0x75, 0x6E, 0x63, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x74, 0x79,
	0x70, 0x65, 0x6F, 0x66, 0x28, 0x74, 0x68, 0x69, 0x6E, 0x67, 0x2C, 0x20,
	0x63, 0x68, 0x65, 0x63, 0x6B, 0x29, 0x0A, 0x0A, 0x09, 0x6C, 0x6F, 0x63,
	0x61, 0x6C, 0x20, 0x74, 0x20, 0x3D, 0x20, 0x74, 0x79, 0x70, 0x65, 0x28,
	0x74, 0x68, 0x69, 0x6E, 0x67, 0x29, 0x0A, 0x0A, 0x09, 0x69, 0x66, 0x20,
	0x63, 0x68, 0x65, 0x63, 0x6B, 0x20, 0x74, 0x68, 0x65, 0x6E, 0x0A, 0x09,
	0x09, 0x69, 0x66, 0x20, 0x74, 0x20, 0x3D, 0x3D, 0x20, 0x27, 0x74, 0x61,
	0x62, 0x6C, 0x65, 0x27, 0x20, 0x74, 0x68, 0x65, 0x6E, 0x0A, 0x09, 0x09,
	0x09, 0x72, 0x65, 0x74, 0x75, 0x72, 0x6E, 0x20, 0x63, 0x68, 0x65, 0x63,
	0x6B, 0x20, 0x3D, 0x3D, 0x20, 0x27, 0x74, 0x61, 0x62, 0x6C, 0x65, 0x27,
	0x20, 0x6F, 0x72, 0x20, 0x74, 0x68, 0x69, 0x6E, 0x67, 0x2E, 0x67, 0x65,
	0x74, 0x43, 0x6C, 0x61, 0x73, 0x73, 0x4E, 0x61, 0x6D, 0x65, 0x20, 0x61,
	0x6E, 0x64, 0x0A, 0x09, 0x09, 0x09, 0x74, 0x68, 0x69, 0x6E, 0x67, 0x2E,
	0x67, 0x65, 0x74, 0x43, 0x6C, 0x61, 0x73, 0x73, 0x4E, 0x61, 0x6D, 0x65,
	0x28, 0x29, 0x20, 0x3D, 0x3D, 0x20, 0x63, 0x68, 0x65, 0x63, 0x6B, 0x20,
	0x6F, 0x72, 0x20, 0x74, 0x68, 0x69, 0x6E, 0x67, 0x2E, 0x62, 0x61, 0x73,
	0x65, 0x43, 0x6C, 0x61, 0x73, 0x73, 0x20, 0x61, 0x6E, 0x64, 0x0A, 0x09,
	0x09, 0x09, 0x74, 0x79, 0x70, 0x65, 0x6F, 0x66, 0x28, 0x74, 0x68, 0x69,
	0x6E, 0x67, 0x2E, 0x62, 0x61, 0x73, 0x65, 0x43, 0x6C, 0x61, 0x73, 0x73,
	0x2C, 0x20, 0x63, 0x68, 0x65, 0x63, 0x6B, 0x29, 0x0A, 0x09, 0x09, 0x65,
	0x6E, 0x64, 0x0A, 0x09, 0x09, 0x72, 0x65, 0x74, 0x75, 0x72, 0x6E, 0x20,
	0x74, 0x20, 0x3D, 0x3D, 0x20, 0x63, 0x68, 0x65, 0x63, 0x6B, 0x0A, 0x09,
	0x65, 0x6E, 0x64, 0x0A, 0x09, 0x0A, 0x09, 0x72, 0x65, 0x74, 0x75, 0x72,
	0x6E, 0x20, 0x74, 0x20, 0x3D, 0x3D, 0x20, 0x27, 0x74, 0x61, 0x62, 0x6C,
	0x65, 0x27, 0x20, 0x61, 0x6E, 0x64, 0x20, 0x74, 0x68, 0x69, 0x6E, 0x67,
	0x2E, 0x67, 0x65, 0x74, 0x43, 0x6C, 0x61, 0x73, 0x73, 0x4E, 0x61, 0x6D,
	0x65, 0x20, 0x61, 0x6E, 0x64, 0x20, 0x74, 0x68, 0x69, 0x6E, 0x67, 0x2E,
	0x67, 0x65, 0x74, 0x43, 0x6C, 0x61, 0x73, 0x73, 0x4E, 0x61, 0x6D, 0x65,
	0x28, 0x29, 0x20, 0x6F, 0x72, 0x20, 0x74, 0x0A, 0x65, 0x6E, 0x64
};

#endif