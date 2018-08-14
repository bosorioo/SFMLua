local function prepare(word, before, after)
	word = string.gsub(word, '[\\/]', '/')
	if before and word:sub(1, 1) ~= '/' then
		word = '/' .. word
	end
	if after and word:sub(-1) ~= '/' then
		word = word .. '/'
	end
	return word
end

local function pathShort(epath, base, rep)
	local path, file = epath:match('(.+[\\/])(.+)$')
	if not path or not file then return epath, '', false end
	if path:sub(1, 3) == '...' then
			path = path:sub(4)
	end
	path, base = prepare(path, false, true), prepare(base, false, true)
	for index = 1, #base - 1 do
		local bg, en = base:find(path:sub(1, #base - index + 1), index, true)
		if bg and en then
			return (rep or '') .. path:sub(en - bg + 2), file, true
		end
	end
	return path, file, false
end

function onTrace(trace, lasterr)

	local level = 0
	local extra
	local rep = function(n)
		return string.rep(' ', n)
	end

	Console.setNotification(true, true)

	for msg in trace:gmatch('[^\n]+') do

		local C = msg:match('%[C%]: (.+)')

		if C then
			extra = C .. ' '
		end

		if level > 0 then

			if level == 1 then
				msg = lasterr
			end

			local file, line, detail = msg:match('%s*(.+):(%d+):%s+(.+)$')

			if file and line and detail then

				local path, lua, ok = pathShort(file, WorkDir, '')

				if not ok then
					path, lua = pathShort(file, ExecDir, 'exec dir/')
				end

				if level == 1 then
					Console.notify 'Traceback:\n'
				end

				if detail:match('module [\'"].-[\'"] not found') then

					detail = detail:gsub("no file '(.-)'", function(dir)
						local path, file, ok = pathShort(dir, WorkDir, '')
						if not ok then
							path, file = pathShort(dir, ExecDir, 'exec dir/')
						end
						return rep(4) .. 'no file \'' .. (path or '') .. (file or '?') .. '\''
					end):gsub("no field package", rep(4) .. "no field package")

				end

				Console.notify(string.format('\t[%d] %s%s\n\t\t\t%s%s on line %d',
						level, extra or '', detail, path, lua, line))

				extra = nil

				if level == 8 then
					break
				end

				level = level + 1
			end

		else level = 1
		end
	end

	local file, line, detail = lasterr:match('%s*(.+):(%d+):%s+(.+)$')

	if file and line and detail then
		local path, lua, ok = pathShort(file, WorkDir, '')
		if not ok then
			path, lua = pathShort(file, ExecDir, 'exec dir/')
		end
		if detail:match('module [\'"].-[\'"] not found') then
			detail = detail:match('module [^\n]+')
		end
		Console.notify(string.format('%sError:\n\t%s\n\t%s%s on line %d',
				level > 0 and '\n' or '', detail, path, lua, line))
	end

	print '\n'

end
