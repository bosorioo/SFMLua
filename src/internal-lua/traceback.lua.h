/************
* Generated with lua2header.lua
* Original source code:
*
* 1 - local function prepare(word, before, after)
* 2 - 	word = string.gsub(word, '[\\/]', '/')
* 3 - 	if before and word:sub(1, 1) ~= '/' then
* 4 - 		word = '/' .. word
* 5 - 	end
* 6 - 	if after and word:sub(-1) ~= '/' then
* 7 - 		word = word .. '/'
* 8 - 	end
* 9 - 	return word
* 10 - end

* 11 - local function pathShort(epath, base, rep)
* 12 - 	local path, file = epath:match('(.+[\\/])(.+)$')
* 13 - 	if not path or not file then return epath, '', false end
* 14 - 	if path:sub(1, 3) == '...' then
* 15 - 			path = path:sub(4)
* 16 - 	end
* 17 - 	path, base = prepare(path, false, true), prepare(base, false, true)
* 18 - 	for index = 1, #base - 1 do
* 19 - 		local bg, en = base:find(path:sub(1, #base - index + 1), index, true)
* 20 - 		if bg and en then
* 21 - 			return (rep or '') .. path:sub(en - bg + 2), file, true
* 22 - 		end
* 23 - 	end
* 24 - 	return path, file, false
* 25 - end

* 26 - function onTrace(trace, lasterr)

* 27 - 	local level = 0
* 28 - 	local extra
* 29 - 	local rep = function(n)
* 30 - 		return string.rep(' ', n)
* 31 - 	end
* 32 - 	Console.setNotification(true, true)

* 33 - 	for msg in trace:gmatch('[^\n]+') do

* 34 - 		local C = msg:match('%[C%]: (.+)')

* 35 - 		if C then
* 36 - 			extra = C .. ' '
* 37 - 		end

* 38 - 		if level > 0 then

* 39 - 			if level == 1 then
* 40 - 				msg = lasterr
* 41 - 			end

* 42 - 			local file, line, detail = msg:match('%s*(.+):(%d+):%s+(.+)$')

* 43 - 			if file and line and detail then

* 44 - 				local path, lua, ok = pathShort(file, WorkDir, '')

* 45 - 				if not ok then
* 46 - 					path, lua = pathShort(file, ExecDir, 'exec dir/')
* 47 - 				end

* 48 - 				if level == 1 then
* 49 - 					Console.notify 'Traceback:\n'
* 50 - 				end

* 51 - 				if detail:match('module [\'"].-[\'"] not found') then

* 52 - 					detail = detail:gsub("no file '(.-)'", function(dir)
* 53 - 						local path, file, ok = pathShort(dir, WorkDir, '')
* 54 - 						if not ok then
* 55 - 							path, file = pathShort(dir, ExecDir, 'exec dir/')
* 56 - 						end
* 57 - 						return rep(4) .. 'no file \'' .. (path or '') .. (file or '?') .. '\''
* 58 - 					end):gsub("no field package", rep(4) .. "no field package")

* 59 - 				end

* 60 - 				Console.notify(string.format('\t[%d] %s%s\n\t\t\t%s%s on line %d',
* 61 - 						level, extra or '', detail, path, lua, line))

* 62 - 				extra = nil

* 63 - 				if level == 8 then
* 64 - 					break
* 65 - 				end

* 66 - 				level = level + 1
* 67 - 			end

* 68 - 		else level = 1
* 69 - 		end
* 70 - 	end

* 71 - 	local file, line, detail = lasterr:match('%s*(.+):(%d+):%s+(.+)$')

* 72 - 	if file and line and detail then
* 73 - 		local path, lua, ok = pathShort(file, WorkDir, '')
* 74 - 		if not ok then
* 75 - 			path, lua = pathShort(file, ExecDir, 'exec dir/')
* 76 - 		end
* 77 - 		if detail:match('module [\'"].-[\'"] not found') then
* 78 - 			detail = detail:match('module [^\n]+')
* 79 - 		end
* 80 - 		Console.notify(string.format('%sError:\n\t%s\n\t%s%s on line %d',
* 81 - 				level > 0 and '\n' or '', detail, path, lua, line))
* 82 - 	end

* 83 - 	print'\n'

* 84 - end
*
************/

#ifndef TRACEBACK_LUA_H
#define TRACEBACK_LUA_H

const char lua_script_traceback[] = {
	0x6C, 0x6F, 0x63, 0x61, 0x6C, 0x20, 0x66, 0x75, 0x6E, 0x63, 0x74, 0x69,
	0x6F, 0x6E, 0x20, 0x70, 0x72, 0x65, 0x70, 0x61, 0x72, 0x65, 0x28, 0x77,
	0x6F, 0x72, 0x64, 0x2C, 0x20, 0x62, 0x65, 0x66, 0x6F, 0x72, 0x65, 0x2C,
	0x20, 0x61, 0x66, 0x74, 0x65, 0x72, 0x29, 0x20, 0x77, 0x6F, 0x72, 0x64,
	0x20, 0x3D, 0x20, 0x73, 0x74, 0x72, 0x69, 0x6E, 0x67, 0x2E, 0x67, 0x73,
	0x75, 0x62, 0x28, 0x77, 0x6F, 0x72, 0x64, 0x2C, 0x20, 0x27, 0x5B, 0x5C,
	0x5C, 0x2F, 0x5D, 0x27, 0x2C, 0x20, 0x27, 0x2F, 0x27, 0x29, 0x20, 0x69,
	0x66, 0x20, 0x62, 0x65, 0x66, 0x6F, 0x72, 0x65, 0x20, 0x61, 0x6E, 0x64,
	0x20, 0x77, 0x6F, 0x72, 0x64, 0x3A, 0x73, 0x75, 0x62, 0x28, 0x31, 0x2C,
	0x20, 0x31, 0x29, 0x20, 0x7E, 0x3D, 0x20, 0x27, 0x2F, 0x27, 0x20, 0x74,
	0x68, 0x65, 0x6E, 0x20, 0x77, 0x6F, 0x72, 0x64, 0x20, 0x3D, 0x20, 0x27,
	0x2F, 0x27, 0x20, 0x2E, 0x2E, 0x20, 0x77, 0x6F, 0x72, 0x64, 0x20, 0x65,
	0x6E, 0x64, 0x20, 0x69, 0x66, 0x20, 0x61, 0x66, 0x74, 0x65, 0x72, 0x20,
	0x61, 0x6E, 0x64, 0x20, 0x77, 0x6F, 0x72, 0x64, 0x3A, 0x73, 0x75, 0x62,
	0x28, 0x2D, 0x31, 0x29, 0x20, 0x7E, 0x3D, 0x20, 0x27, 0x2F, 0x27, 0x20,
	0x74, 0x68, 0x65, 0x6E, 0x20, 0x77, 0x6F, 0x72, 0x64, 0x20, 0x3D, 0x20,
	0x77, 0x6F, 0x72, 0x64, 0x20, 0x2E, 0x2E, 0x20, 0x27, 0x2F, 0x27, 0x20,
	0x65, 0x6E, 0x64, 0x20, 0x72, 0x65, 0x74, 0x75, 0x72, 0x6E, 0x20, 0x77,
	0x6F, 0x72, 0x64, 0x20, 0x65, 0x6E, 0x64, 0x20, 0x6C, 0x6F, 0x63, 0x61,
	0x6C, 0x20, 0x66, 0x75, 0x6E, 0x63, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x70,
	0x61, 0x74, 0x68, 0x53, 0x68, 0x6F, 0x72, 0x74, 0x28, 0x65, 0x70, 0x61,
	0x74, 0x68, 0x2C, 0x20, 0x62, 0x61, 0x73, 0x65, 0x2C, 0x20, 0x72, 0x65,
	0x70, 0x29, 0x20, 0x6C, 0x6F, 0x63, 0x61, 0x6C, 0x20, 0x70, 0x61, 0x74,
	0x68, 0x2C, 0x20, 0x66, 0x69, 0x6C, 0x65, 0x20, 0x3D, 0x20, 0x65, 0x70,
	0x61, 0x74, 0x68, 0x3A, 0x6D, 0x61, 0x74, 0x63, 0x68, 0x28, 0x27, 0x28,
	0x2E, 0x2B, 0x5B, 0x5C, 0x5C, 0x2F, 0x5D, 0x29, 0x28, 0x2E, 0x2B, 0x29,
	0x24, 0x27, 0x29, 0x20, 0x69, 0x66, 0x20, 0x6E, 0x6F, 0x74, 0x20, 0x70,
	0x61, 0x74, 0x68, 0x20, 0x6F, 0x72, 0x20, 0x6E, 0x6F, 0x74, 0x20, 0x66,
	0x69, 0x6C, 0x65, 0x20, 0x74, 0x68, 0x65, 0x6E, 0x20, 0x72, 0x65, 0x74,
	0x75, 0x72, 0x6E, 0x20, 0x65, 0x70, 0x61, 0x74, 0x68, 0x2C, 0x20, 0x27,
	0x27, 0x2C, 0x20, 0x66, 0x61, 0x6C, 0x73, 0x65, 0x20, 0x65, 0x6E, 0x64,
	0x20, 0x69, 0x66, 0x20, 0x70, 0x61, 0x74, 0x68, 0x3A, 0x73, 0x75, 0x62,
	0x28, 0x31, 0x2C, 0x20, 0x33, 0x29, 0x20, 0x3D, 0x3D, 0x20, 0x27, 0x2E,
	0x2E, 0x2E, 0x27, 0x20, 0x74, 0x68, 0x65, 0x6E, 0x20, 0x70, 0x61, 0x74,
	0x68, 0x20, 0x3D, 0x20, 0x70, 0x61, 0x74, 0x68, 0x3A, 0x73, 0x75, 0x62,
	0x28, 0x34, 0x29, 0x20, 0x65, 0x6E, 0x64, 0x20, 0x70, 0x61, 0x74, 0x68,
	0x2C, 0x20, 0x62, 0x61, 0x73, 0x65, 0x20, 0x3D, 0x20, 0x70, 0x72, 0x65,
	0x70, 0x61, 0x72, 0x65, 0x28, 0x70, 0x61, 0x74, 0x68, 0x2C, 0x20, 0x66,
	0x61, 0x6C, 0x73, 0x65, 0x2C, 0x20, 0x74, 0x72, 0x75, 0x65, 0x29, 0x2C,
	0x20, 0x70, 0x72, 0x65, 0x70, 0x61, 0x72, 0x65, 0x28, 0x62, 0x61, 0x73,
	0x65, 0x2C, 0x20, 0x66, 0x61, 0x6C, 0x73, 0x65, 0x2C, 0x20, 0x74, 0x72,
	0x75, 0x65, 0x29, 0x20, 0x66, 0x6F, 0x72, 0x20, 0x69, 0x6E, 0x64, 0x65,
	0x78, 0x20, 0x3D, 0x20, 0x31, 0x2C, 0x20, 0x23, 0x62, 0x61, 0x73, 0x65,
	0x20, 0x2D, 0x20, 0x31, 0x20, 0x64, 0x6F, 0x20, 0x6C, 0x6F, 0x63, 0x61,
	0x6C, 0x20, 0x62, 0x67, 0x2C, 0x20, 0x65, 0x6E, 0x20, 0x3D, 0x20, 0x62,
	0x61, 0x73, 0x65, 0x3A, 0x66, 0x69, 0x6E, 0x64, 0x28, 0x70, 0x61, 0x74,
	0x68, 0x3A, 0x73, 0x75, 0x62, 0x28, 0x31, 0x2C, 0x20, 0x23, 0x62, 0x61,
	0x73, 0x65, 0x20, 0x2D, 0x20, 0x69, 0x6E, 0x64, 0x65, 0x78, 0x20, 0x2B,
	0x20, 0x31, 0x29, 0x2C, 0x20, 0x69, 0x6E, 0x64, 0x65, 0x78, 0x2C, 0x20,
	0x74, 0x72, 0x75, 0x65, 0x29, 0x20, 0x69, 0x66, 0x20, 0x62, 0x67, 0x20,
	0x61, 0x6E, 0x64, 0x20, 0x65, 0x6E, 0x20, 0x74, 0x68, 0x65, 0x6E, 0x20,
	0x72, 0x65, 0x74, 0x75, 0x72, 0x6E, 0x20, 0x28, 0x72, 0x65, 0x70, 0x20,
	0x6F, 0x72, 0x20, 0x27, 0x27, 0x29, 0x20, 0x2E, 0x2E, 0x20, 0x70, 0x61,
	0x74, 0x68, 0x3A, 0x73, 0x75, 0x62, 0x28, 0x65, 0x6E, 0x20, 0x2D, 0x20,
	0x62, 0x67, 0x20, 0x2B, 0x20, 0x32, 0x29, 0x2C, 0x20, 0x66, 0x69, 0x6C,
	0x65, 0x2C, 0x20, 0x74, 0x72, 0x75, 0x65, 0x20, 0x65, 0x6E, 0x64, 0x20,
	0x65, 0x6E, 0x64, 0x20, 0x72, 0x65, 0x74, 0x75, 0x72, 0x6E, 0x20, 0x70,
	0x61, 0x74, 0x68, 0x2C, 0x20, 0x66, 0x69, 0x6C, 0x65, 0x2C, 0x20, 0x66,
	0x61, 0x6C, 0x73, 0x65, 0x20, 0x65, 0x6E, 0x64, 0x20, 0x66, 0x75, 0x6E,
	0x63, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x6F, 0x6E, 0x54, 0x72, 0x61, 0x63,
	0x65, 0x28, 0x74, 0x72, 0x61, 0x63, 0x65, 0x2C, 0x20, 0x6C, 0x61, 0x73,
	0x74, 0x65, 0x72, 0x72, 0x29, 0x20, 0x6C, 0x6F, 0x63, 0x61, 0x6C, 0x20,
	0x6C, 0x65, 0x76, 0x65, 0x6C, 0x20, 0x3D, 0x20, 0x30, 0x20, 0x6C, 0x6F,
	0x63, 0x61, 0x6C, 0x20, 0x65, 0x78, 0x74, 0x72, 0x61, 0x20, 0x6C, 0x6F,
	0x63, 0x61, 0x6C, 0x20, 0x72, 0x65, 0x70, 0x20, 0x3D, 0x20, 0x66, 0x75,
	0x6E, 0x63, 0x74, 0x69, 0x6F, 0x6E, 0x28, 0x6E, 0x29, 0x20, 0x72, 0x65,
	0x74, 0x75, 0x72, 0x6E, 0x20, 0x73, 0x74, 0x72, 0x69, 0x6E, 0x67, 0x2E,
	0x72, 0x65, 0x70, 0x28, 0x27, 0x20, 0x27, 0x2C, 0x20, 0x6E, 0x29, 0x20,
	0x65, 0x6E, 0x64, 0x20, 0x43, 0x6F, 0x6E, 0x73, 0x6F, 0x6C, 0x65, 0x2E,
	0x73, 0x65, 0x74, 0x4E, 0x6F, 0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74,
	0x69, 0x6F, 0x6E, 0x28, 0x74, 0x72, 0x75, 0x65, 0x2C, 0x20, 0x74, 0x72,
	0x75, 0x65, 0x29, 0x20, 0x66, 0x6F, 0x72, 0x20, 0x6D, 0x73, 0x67, 0x20,
	0x69, 0x6E, 0x20, 0x74, 0x72, 0x61, 0x63, 0x65, 0x3A, 0x67, 0x6D, 0x61,
	0x74, 0x63, 0x68, 0x28, 0x27, 0x5B, 0x5E, 0x5C, 0x6E, 0x5D, 0x2B, 0x27,
	0x29, 0x20, 0x64, 0x6F, 0x20, 0x6C, 0x6F, 0x63, 0x61, 0x6C, 0x20, 0x43,
	0x20, 0x3D, 0x20, 0x6D, 0x73, 0x67, 0x3A, 0x6D, 0x61, 0x74, 0x63, 0x68,
	0x28, 0x27, 0x25, 0x5B, 0x43, 0x25, 0x5D, 0x3A, 0x20, 0x28, 0x2E, 0x2B,
	0x29, 0x27, 0x29, 0x20, 0x69, 0x66, 0x20, 0x43, 0x20, 0x74, 0x68, 0x65,
	0x6E, 0x20, 0x65, 0x78, 0x74, 0x72, 0x61, 0x20, 0x3D, 0x20, 0x43, 0x20,
	0x2E, 0x2E, 0x20, 0x27, 0x20, 0x27, 0x20, 0x65, 0x6E, 0x64, 0x20, 0x69,
	0x66, 0x20, 0x6C, 0x65, 0x76, 0x65, 0x6C, 0x20, 0x3E, 0x20, 0x30, 0x20,
	0x74, 0x68, 0x65, 0x6E, 0x20, 0x69, 0x66, 0x20, 0x6C, 0x65, 0x76, 0x65,
	0x6C, 0x20, 0x3D, 0x3D, 0x20, 0x31, 0x20, 0x74, 0x68, 0x65, 0x6E, 0x20,
	0x6D, 0x73, 0x67, 0x20, 0x3D, 0x20, 0x6C, 0x61, 0x73, 0x74, 0x65, 0x72,
	0x72, 0x20, 0x65, 0x6E, 0x64, 0x20, 0x6C, 0x6F, 0x63, 0x61, 0x6C, 0x20,
	0x66, 0x69, 0x6C, 0x65, 0x2C, 0x20, 0x6C, 0x69, 0x6E, 0x65, 0x2C, 0x20,
	0x64, 0x65, 0x74, 0x61, 0x69, 0x6C, 0x20, 0x3D, 0x20, 0x6D, 0x73, 0x67,
	0x3A, 0x6D, 0x61, 0x74, 0x63, 0x68, 0x28, 0x27, 0x25, 0x73, 0x2A, 0x28,
	0x2E, 0x2B, 0x29, 0x3A, 0x28, 0x25, 0x64, 0x2B, 0x29, 0x3A, 0x25, 0x73,
	0x2B, 0x28, 0x2E, 0x2B, 0x29, 0x24, 0x27, 0x29, 0x20, 0x69, 0x66, 0x20,
	0x66, 0x69, 0x6C, 0x65, 0x20, 0x61, 0x6E, 0x64, 0x20, 0x6C, 0x69, 0x6E,
	0x65, 0x20, 0x61, 0x6E, 0x64, 0x20, 0x64, 0x65, 0x74, 0x61, 0x69, 0x6C,
	0x20, 0x74, 0x68, 0x65, 0x6E, 0x20, 0x6C, 0x6F, 0x63, 0x61, 0x6C, 0x20,
	0x70, 0x61, 0x74, 0x68, 0x2C, 0x20, 0x6C, 0x75, 0x61, 0x2C, 0x20, 0x6F,
	0x6B, 0x20, 0x3D, 0x20, 0x70, 0x61, 0x74, 0x68, 0x53, 0x68, 0x6F, 0x72,
	0x74, 0x28, 0x66, 0x69, 0x6C, 0x65, 0x2C, 0x20, 0x57, 0x6F, 0x72, 0x6B,
	0x44, 0x69, 0x72, 0x2C, 0x20, 0x27, 0x27, 0x29, 0x20, 0x69, 0x66, 0x20,
	0x6E, 0x6F, 0x74, 0x20, 0x6F, 0x6B, 0x20, 0x74, 0x68, 0x65, 0x6E, 0x20,
	0x70, 0x61, 0x74, 0x68, 0x2C, 0x20, 0x6C, 0x75, 0x61, 0x20, 0x3D, 0x20,
	0x70, 0x61, 0x74, 0x68, 0x53, 0x68, 0x6F, 0x72, 0x74, 0x28, 0x66, 0x69,
	0x6C, 0x65, 0x2C, 0x20, 0x45, 0x78, 0x65, 0x63, 0x44, 0x69, 0x72, 0x2C,
	0x20, 0x27, 0x65, 0x78, 0x65, 0x63, 0x20, 0x64, 0x69, 0x72, 0x2F, 0x27,
	0x29, 0x20, 0x65, 0x6E, 0x64, 0x20, 0x69, 0x66, 0x20, 0x6C, 0x65, 0x76,
	0x65, 0x6C, 0x20, 0x3D, 0x3D, 0x20, 0x31, 0x20, 0x74, 0x68, 0x65, 0x6E,
	0x20, 0x43, 0x6F, 0x6E, 0x73, 0x6F, 0x6C, 0x65, 0x2E, 0x6E, 0x6F, 0x74,
	0x69, 0x66, 0x79, 0x20, 0x27, 0x54, 0x72, 0x61, 0x63, 0x65, 0x62, 0x61,
	0x63, 0x6B, 0x3A, 0x5C, 0x6E, 0x27, 0x20, 0x65, 0x6E, 0x64, 0x20, 0x69,
	0x66, 0x20, 0x64, 0x65, 0x74, 0x61, 0x69, 0x6C, 0x3A, 0x6D, 0x61, 0x74,
	0x63, 0x68, 0x28, 0x27, 0x6D, 0x6F, 0x64, 0x75, 0x6C, 0x65, 0x20, 0x5B,
	0x5C, 0x27, 0x22, 0x5D, 0x2E, 0x2D, 0x5B, 0x5C, 0x27, 0x22, 0x5D, 0x20,
	0x6E, 0x6F, 0x74, 0x20, 0x66, 0x6F, 0x75, 0x6E, 0x64, 0x27, 0x29, 0x20,
	0x74, 0x68, 0x65, 0x6E, 0x20, 0x64, 0x65, 0x74, 0x61, 0x69, 0x6C, 0x20,
	0x3D, 0x20, 0x64, 0x65, 0x74, 0x61, 0x69, 0x6C, 0x3A, 0x67, 0x73, 0x75,
	0x62, 0x28, 0x22, 0x6E, 0x6F, 0x20, 0x66, 0x69, 0x6C, 0x65, 0x20, 0x27,
	0x28, 0x2E, 0x2D, 0x29, 0x27, 0x22, 0x2C, 0x20, 0x66, 0x75, 0x6E, 0x63,
	0x74, 0x69, 0x6F, 0x6E, 0x28, 0x64, 0x69, 0x72, 0x29, 0x20, 0x6C, 0x6F,
	0x63, 0x61, 0x6C, 0x20, 0x70, 0x61, 0x74, 0x68, 0x2C, 0x20, 0x66, 0x69,
	0x6C, 0x65, 0x2C, 0x20, 0x6F, 0x6B, 0x20, 0x3D, 0x20, 0x70, 0x61, 0x74,
	0x68, 0x53, 0x68, 0x6F, 0x72, 0x74, 0x28, 0x64, 0x69, 0x72, 0x2C, 0x20,
	0x57, 0x6F, 0x72, 0x6B, 0x44, 0x69, 0x72, 0x2C, 0x20, 0x27, 0x27, 0x29,
	0x20, 0x69, 0x66, 0x20, 0x6E, 0x6F, 0x74, 0x20, 0x6F, 0x6B, 0x20, 0x74,
	0x68, 0x65, 0x6E, 0x20, 0x70, 0x61, 0x74, 0x68, 0x2C, 0x20, 0x66, 0x69,
	0x6C, 0x65, 0x20, 0x3D, 0x20, 0x70, 0x61, 0x74, 0x68, 0x53, 0x68, 0x6F,
	0x72, 0x74, 0x28, 0x64, 0x69, 0x72, 0x2C, 0x20, 0x45, 0x78, 0x65, 0x63,
	0x44, 0x69, 0x72, 0x2C, 0x20, 0x27, 0x65, 0x78, 0x65, 0x63, 0x20, 0x64,
	0x69, 0x72, 0x2F, 0x27, 0x29, 0x20, 0x65, 0x6E, 0x64, 0x20, 0x72, 0x65,
	0x74, 0x75, 0x72, 0x6E, 0x20, 0x72, 0x65, 0x70, 0x28, 0x34, 0x29, 0x20,
	0x2E, 0x2E, 0x20, 0x27, 0x6E, 0x6F, 0x20, 0x66, 0x69, 0x6C, 0x65, 0x20,
	0x5C, 0x27, 0x27, 0x20, 0x2E, 0x2E, 0x20, 0x28, 0x70, 0x61, 0x74, 0x68,
	0x20, 0x6F, 0x72, 0x20, 0x27, 0x27, 0x29, 0x20, 0x2E, 0x2E, 0x20, 0x28,
	0x66, 0x69, 0x6C, 0x65, 0x20, 0x6F, 0x72, 0x20, 0x27, 0x3F, 0x27, 0x29,
	0x20, 0x2E, 0x2E, 0x20, 0x27, 0x5C, 0x27, 0x27, 0x20, 0x65, 0x6E, 0x64,
	0x29, 0x3A, 0x67, 0x73, 0x75, 0x62, 0x28, 0x22, 0x6E, 0x6F, 0x20, 0x66,
	0x69, 0x65, 0x6C, 0x64, 0x20, 0x70, 0x61, 0x63, 0x6B, 0x61, 0x67, 0x65,
	0x22, 0x2C, 0x20, 0x72, 0x65, 0x70, 0x28, 0x34, 0x29, 0x20, 0x2E, 0x2E,
	0x20, 0x22, 0x6E, 0x6F, 0x20, 0x66, 0x69, 0x65, 0x6C, 0x64, 0x20, 0x70,
	0x61, 0x63, 0x6B, 0x61, 0x67, 0x65, 0x22, 0x29, 0x20, 0x65, 0x6E, 0x64,
	0x20, 0x43, 0x6F, 0x6E, 0x73, 0x6F, 0x6C, 0x65, 0x2E, 0x6E, 0x6F, 0x74,
	0x69, 0x66, 0x79, 0x28, 0x73, 0x74, 0x72, 0x69, 0x6E, 0x67, 0x2E, 0x66,
	0x6F, 0x72, 0x6D, 0x61, 0x74, 0x28, 0x27, 0x5C, 0x74, 0x5B, 0x25, 0x64,
	0x5D, 0x20, 0x25, 0x73, 0x25, 0x73, 0x5C, 0x6E, 0x5C, 0x74, 0x5C, 0x74,
	0x5C, 0x74, 0x25, 0x73, 0x25, 0x73, 0x20, 0x6F, 0x6E, 0x20, 0x6C, 0x69,
	0x6E, 0x65, 0x20, 0x25, 0x64, 0x27, 0x2C, 0x20, 0x6C, 0x65, 0x76, 0x65,
	0x6C, 0x2C, 0x20, 0x65, 0x78, 0x74, 0x72, 0x61, 0x20, 0x6F, 0x72, 0x20,
	0x27, 0x27, 0x2C, 0x20, 0x64, 0x65, 0x74, 0x61, 0x69, 0x6C, 0x2C, 0x20,
	0x70, 0x61, 0x74, 0x68, 0x2C, 0x20, 0x6C, 0x75, 0x61, 0x2C, 0x20, 0x6C,
	0x69, 0x6E, 0x65, 0x29, 0x29, 0x20, 0x65, 0x78, 0x74, 0x72, 0x61, 0x20,
	0x3D, 0x20, 0x6E, 0x69, 0x6C, 0x20, 0x69, 0x66, 0x20, 0x6C, 0x65, 0x76,
	0x65, 0x6C, 0x20, 0x3D, 0x3D, 0x20, 0x38, 0x20, 0x74, 0x68, 0x65, 0x6E,
	0x20, 0x62, 0x72, 0x65, 0x61, 0x6B, 0x20, 0x65, 0x6E, 0x64, 0x20, 0x6C,
	0x65, 0x76, 0x65, 0x6C, 0x20, 0x3D, 0x20, 0x6C, 0x65, 0x76, 0x65, 0x6C,
	0x20, 0x2B, 0x20, 0x31, 0x20, 0x65, 0x6E, 0x64, 0x20, 0x65, 0x6C, 0x73,
	0x65, 0x20, 0x6C, 0x65, 0x76, 0x65, 0x6C, 0x20, 0x3D, 0x20, 0x31, 0x20,
	0x65, 0x6E, 0x64, 0x20, 0x65, 0x6E, 0x64, 0x20, 0x6C, 0x6F, 0x63, 0x61,
	0x6C, 0x20, 0x66, 0x69, 0x6C, 0x65, 0x2C, 0x20, 0x6C, 0x69, 0x6E, 0x65,
	0x2C, 0x20, 0x64, 0x65, 0x74, 0x61, 0x69, 0x6C, 0x20, 0x3D, 0x20, 0x6C,
	0x61, 0x73, 0x74, 0x65, 0x72, 0x72, 0x3A, 0x6D, 0x61, 0x74, 0x63, 0x68,
	0x28, 0x27, 0x25, 0x73, 0x2A, 0x28, 0x2E, 0x2B, 0x29, 0x3A, 0x28, 0x25,
	0x64, 0x2B, 0x29, 0x3A, 0x25, 0x73, 0x2B, 0x28, 0x2E, 0x2B, 0x29, 0x24,
	0x27, 0x29, 0x20, 0x69, 0x66, 0x20, 0x66, 0x69, 0x6C, 0x65, 0x20, 0x61,
	0x6E, 0x64, 0x20, 0x6C, 0x69, 0x6E, 0x65, 0x20, 0x61, 0x6E, 0x64, 0x20,
	0x64, 0x65, 0x74, 0x61, 0x69, 0x6C, 0x20, 0x74, 0x68, 0x65, 0x6E, 0x20,
	0x6C, 0x6F, 0x63, 0x61, 0x6C, 0x20, 0x70, 0x61, 0x74, 0x68, 0x2C, 0x20,
	0x6C, 0x75, 0x61, 0x2C, 0x20, 0x6F, 0x6B, 0x20, 0x3D, 0x20, 0x70, 0x61,
	0x74, 0x68, 0x53, 0x68, 0x6F, 0x72, 0x74, 0x28, 0x66, 0x69, 0x6C, 0x65,
	0x2C, 0x20, 0x57, 0x6F, 0x72, 0x6B, 0x44, 0x69, 0x72, 0x2C, 0x20, 0x27,
	0x27, 0x29, 0x20, 0x69, 0x66, 0x20, 0x6E, 0x6F, 0x74, 0x20, 0x6F, 0x6B,
	0x20, 0x74, 0x68, 0x65, 0x6E, 0x20, 0x70, 0x61, 0x74, 0x68, 0x2C, 0x20,
	0x6C, 0x75, 0x61, 0x20, 0x3D, 0x20, 0x70, 0x61, 0x74, 0x68, 0x53, 0x68,
	0x6F, 0x72, 0x74, 0x28, 0x66, 0x69, 0x6C, 0x65, 0x2C, 0x20, 0x45, 0x78,
	0x65, 0x63, 0x44, 0x69, 0x72, 0x2C, 0x20, 0x27, 0x65, 0x78, 0x65, 0x63,
	0x20, 0x64, 0x69, 0x72, 0x2F, 0x27, 0x29, 0x20, 0x65, 0x6E, 0x64, 0x20,
	0x69, 0x66, 0x20, 0x64, 0x65, 0x74, 0x61, 0x69, 0x6C, 0x3A, 0x6D, 0x61,
	0x74, 0x63, 0x68, 0x28, 0x27, 0x6D, 0x6F, 0x64, 0x75, 0x6C, 0x65, 0x20,
	0x5B, 0x5C, 0x27, 0x22, 0x5D, 0x2E, 0x2D, 0x5B, 0x5C, 0x27, 0x22, 0x5D,
	0x20, 0x6E, 0x6F, 0x74, 0x20, 0x66, 0x6F, 0x75, 0x6E, 0x64, 0x27, 0x29,
	0x20, 0x74, 0x68, 0x65, 0x6E, 0x20, 0x64, 0x65, 0x74, 0x61, 0x69, 0x6C,
	0x20, 0x3D, 0x20, 0x64, 0x65, 0x74, 0x61, 0x69, 0x6C, 0x3A, 0x6D, 0x61,
	0x74, 0x63, 0x68, 0x28, 0x27, 0x6D, 0x6F, 0x64, 0x75, 0x6C, 0x65, 0x20,
	0x5B, 0x5E, 0x5C, 0x6E, 0x5D, 0x2B, 0x27, 0x29, 0x20, 0x65, 0x6E, 0x64,
	0x20, 0x43, 0x6F, 0x6E, 0x73, 0x6F, 0x6C, 0x65, 0x2E, 0x6E, 0x6F, 0x74,
	0x69, 0x66, 0x79, 0x28, 0x73, 0x74, 0x72, 0x69, 0x6E, 0x67, 0x2E, 0x66,
	0x6F, 0x72, 0x6D, 0x61, 0x74, 0x28, 0x27, 0x25, 0x73, 0x45, 0x72, 0x72,
	0x6F, 0x72, 0x3A, 0x5C, 0x6E, 0x5C, 0x74, 0x25, 0x73, 0x5C, 0x6E, 0x5C,
	0x74, 0x25, 0x73, 0x25, 0x73, 0x20, 0x6F, 0x6E, 0x20, 0x6C, 0x69, 0x6E,
	0x65, 0x20, 0x25, 0x64, 0x27, 0x2C, 0x20, 0x6C, 0x65, 0x76, 0x65, 0x6C,
	0x20, 0x3E, 0x20, 0x30, 0x20, 0x61, 0x6E, 0x64, 0x20, 0x27, 0x5C, 0x6E,
	0x27, 0x20, 0x6F, 0x72, 0x20, 0x27, 0x27, 0x2C, 0x20, 0x64, 0x65, 0x74,
	0x61, 0x69, 0x6C, 0x2C, 0x20, 0x70, 0x61, 0x74, 0x68, 0x2C, 0x20, 0x6C,
	0x75, 0x61, 0x2C, 0x20, 0x6C, 0x69, 0x6E, 0x65, 0x29, 0x29, 0x20, 0x65,
	0x6E, 0x64, 0x20, 0x70, 0x72, 0x69, 0x6E, 0x74, 0x27, 0x5C, 0x6E, 0x27,
	0x20, 0x65, 0x6E, 0x64
};

#endif