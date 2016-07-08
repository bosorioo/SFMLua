tablecompiler = {

	safequote = function(str)
		return str:gsub('"', '\\"')
	end,

	dumptable = function(t)
		if not t then return false end
		local dump = "{"
		local first = true
		for key, value in pairs (t) do
			if value == nil then
				goto CONTINUE
			end
			if not first then
				dump = dump .. ","
			end
			if type(key) == "string" then
				dump = dump .. "[\""..tablecompiler.safequote(key).."\"]="
			elseif type(key) == "number" then
				dump = dump .. "["..key.."]="
			end
			if type(value) == "string" then
				dump = dump .. "\""..tablecompiler.safequote(value).."\""
			elseif type(value) == "number" then	
				dump = dump .. value
			elseif type(value) == "table" then
				dump = dump .. tablecompiler.dumptable(value)
			else
				dump = dump .. "nil"
			end
			first = false
			::CONTINUE::
		end
		return dump .. "}"
	end,

	savetablestring = function(t, filepath, format)
		local file = io.open(filepath, "w")
		if not file then return false end
		file:write("return " .. tablecompiler.dumptable(t, format))
		file:close()
		return true
	end,

	compiletablestring = function(t)
		if not t then return false end
		local funcstr = "return function () return "..t.." end"
		local tmp = io.open("@", "w")
		tmp:write(funcstr)
		tmp:close()
		local compile = dofile("@")
		os.remove("@")
		if not compile then return false end
		return string.dump(compile)
	end,

	savecompilation = function(file, compilation)
		if not compilation then return false end
		local f = io.open(file, "wb")
		if not f then
			print ("Failed to save compilation on file "..file..".")
			return false
		end
		f:write(compilation)
		f:close()
	end,

	compileandsave = function (file, tab)
		tablecompiler.savecompilation(file,
		  tablecompiler.compiletablestring(
		    tablecompiler.dumptable(tab)
		  )
		)
	end,

	loadcompiledtablestring = function(str)
		return loadstring(str)()
	end,

	loadfile = function (file)
		local f = io.open(file, "r")
		if not f then return false end
		local ret = loadstring(f:read("*a"))()
		f:close()
		return ret
	end,

	loadtablestring = function (str)
		return loadstring(str)()
	end,

	loadcompiledfile = function(file)
		local f = io.open(file, "rb")
		if not f then return false end
		local ret = loadstring(f:read("*a"))()
		f:close()
		return ret
	end
}
	