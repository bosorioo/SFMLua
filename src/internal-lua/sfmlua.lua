lua_path = {
	WorkDir .. "?.lua",
	WorkDir .. "?/main.lua",
	WorkDir .. "libs/?.lua",
	WorkDir .. "libs/?/main.lua"
}

dll_path = {
	WorkDir .. "?.dll",
	WorkDir .. "libs/dlls/?.dll"
}

if WorkDir ~= ExecDir then
	for index = 1, #lua_path do
		lpath = lua_path[index]
		lua_path[#lua_path + 1] = ExecDir .. string.sub(lpath, #WorkDir + 1)
	end
	for index = 1, #dll_path do
		cpath = dll_path[index]
		dll_path[#dll_path + 1] = ExecDir .. string.sub(cpath, #WorkDir + 1)
	end
end

package.path = table.concat(lua_path, ';')
package.cpath = table.concat(dll_path, ';')

local print = print

function printn(...)
	print()
	print(...)
	local t = {...}
	if type(t[#t]) == 'string' then
		print '\n'
	end
	io.flush()
end

function printf(str, ...)
	return print(string.format(str, ...))
end

function printfn(str, ...)
	return printn(string.format(str, ...))
end
