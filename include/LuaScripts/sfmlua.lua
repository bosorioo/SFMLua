package.path = WorkDir .. "?.lua;" .. WorkDir .. "?/main.lua;" .. 
	WorkDir .. "libs/?.lua;" .. WorkDir .. "libs/?/main.lua;"
package.cpath = WorkDir .. "?.dll;" .. WorkDir .. "libs/dlls/?.dll;"

if WorkDir ~= ExecDir then
	package.path = package.path .. ExecDir .. "?.lua;" .. ExecDir .. "?/main.lua;" .. 
		ExecDir .. "libs/?.lua;" .. ExecDir .. "libs/?/main.lua;"
	package.cpath = package.cpath .. ExecDir .. "?.dll;" .. ExecDir .. "libs/dlls/?.dll;"
end

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