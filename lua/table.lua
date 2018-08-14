local function quote(str)
	str = string.gsub(str, '\\', '\\\\')
	str = string.gsub(str, '\n', '\\n')
	str = string.gsub(str, '\t', '\\t')
	return '"' .. string.gsub(str, '"', '\\"') .. '"'
end

local dumps_ident_level = 0
local dumps_new_line = false
local ident = function () return string.rep('\t', dumps_ident_level) end
local add_dump = function (dump_table, data)
	if dumps_new_line then
		dump_table[#dump_table + 1] = '\n'
		if dumps_ident_level > 0 then
			dump_table[#dump_table + 1] = ident()
		end
		dumps_new_line = false
	end
	dump_table[#dump_table + 1] = data
end

-- Dump table to string
table.dumps = function (t, pretty)

	pretty = pretty and true

	local type = type

	if not t or type(t) ~= 'table' then
		return ''
	end

	local dump_data = {'{'}
	local first = true
	local eq = pretty and ' = ' or '='

	if pretty then
		dumps_ident_level = dumps_ident_level + 1
		dumps_new_line = true
	end

	for key, value in pairs (t) do

		if value == nil then
			goto CONTINUE
		end

		if not first then
			add_dump(dump_data, ',')
			if pretty then
				dumps_new_line = true
			end
		end

		if type(key) == 'string' then
			if key:match('^[a-zA-Z_][a-zA-Z_0-9]*$') then
				add_dump(dump_data, key .. eq)
			else
				add_dump(dump_data, '[' .. quote(key) .. ']' .. eq)
			end
		elseif type(key) == 'number' then
			add_dump(dump_data, '[' .. key .. ']' .. eq)
		end

		if type(value) == 'string' then
			add_dump(dump_data, quote(value))
		elseif type(value) == 'number' then
			add_dump(dump_data, value)
		elseif type(value) == 'table' then
			add_dump(dump_data, table.dumps(value, pretty))
		else
			add_dump(dump_data, 'nil')
		end

		first = false
		::CONTINUE::
	end

	if pretty then
		dumps_ident_level = dumps_ident_level - 1
		dumps_new_line = true
	end

	add_dump(dump_data, '}')

	return table.concat(dump_data)
end

table.loads = function(dump_str)
	local func, err = load(dump_str)
	if func then
		local loaded_table = func()
		if type(loaded_table) == 'table' then
			return loaded_table
		end
	end
end

-- insert number indexed values from one table to another
table.iinsert = function(dst, src)
	local b = #dst
	for i, info in ipairs (src) do
		dst[b + i] = info
	end
end

table.swap = function (tab, index_a, index_b)
	tab[index_a], tab[index_b] = tab[index_b], tab[index_a]
end

table.shuffle = function(tab, n)
	local n = n or #tab
	local s = #tab
	local random = math.random
	local remove = table.remove
	local insert = table.insert
	for times = 1, n do
		local index1 = random(s)
		local index2 = random(s)
		local value1 = tab[index1]
		local value2 = tab[index2]
		tab[index1] = value2
		tab[index2] = value1
	end
end

table.find = function (tab, value)
	return table.contains(tab, value) or -1
end

table.contains = function(array, value)
	for index, v in pairs (array) do
		if v == value then
			return index
		end
	end
	return false
end

table.copy = function(tab)
	local ret = {}
	for a, b in pairs (tab) do
		ret[a] = type(b) == 'table' and table.copy(b) or b
	end
	return ret
end

table.icopy = function(tab)
	local ret = {}
	for a, b in ipairs (tab) do
		ret[a] = type(b) == 'table' and table.copy(b) or b
	end
	return ret
end

table.binarysearch = function(vector, look_up, ...)
	local indexBegin, indexEnd = 1, #vector
	local middle, thing
	local floor = math.floor
	if type(look_up) == 'function' then
		repeat
			if indexBegin > indexEnd then
				return nil, indexBegin
			end
			middle = indexBegin + floor((indexEnd - indexBegin) / 2)
			thing = look_up(vector[middle], middle, ...)
			if thing == nil then
				return middle
			elseif thing == true then
				indexEnd = middle - 1
			else
				indexBegin = middle + 1
			end
		until false
	else
		repeat
			if indexBegin > indexEnd then
				return nil, indexBegin
			end
			middle = indexBegin + floor((indexEnd - indexBegin) / 2)
			thing = vector[middle]
			if thing == look_up then
				return middle
			elseif thing > look_up then
				indexEnd = middle - 1
			else
				indexBegin = middle + 1
			end
		until false
	end
end