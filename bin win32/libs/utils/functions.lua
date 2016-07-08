local upper, lower, find = string.upper, string.lower, string.find
local sub, gsub, upper, lower = string.sub, string.gsub, string.upper, string.lower
local match = string.match
local insert, remove, unpack = table.insert, table.remove, table.unpack
local abs, cos, sin, random = math.abs, math.cos, math.sin, math.random
local floor, ceil, min, max = math.floor, math.ceil, math.min, math.max
local huge, pi = math.huge, math.pi

-- [[ Drawing ]]

getEllipsePoints = function(ox, oy, a, b, points, from, to, m, n)
	local ret = {}
	from, m, n = from or 0, m or 2, n or 2
	to = to or pi * 2 - from
	local delta = (to - from) / points
	local cosine, sine = 0, 0
	local signal = math.signal
	local s = 0
	for t = from, to, delta do
		cosine = cos(t)
		sine = sin(t)
		ret[s + 1] = ox + a * abs(cosine)^(2/m) * signal(cosine)
		ret[s + 2] = oy - b * abs(sine)^(2/n) * signal(sine)
		s = s + 2
	end
	return ret, {ox - a, oy - b, 2 * a, 2 * b}
end

getSuperPoints = function (ox, oy, a, b, m, n1, n2, n3, points, from, to)
	from = from or 0
	to = to or pi * 2 - from
	local delta = (to - from) / points
	local ret, r, boundbox = {}, 0, {huge, 0, huge, 0}
	local s = 1
	for t = from, to, delta do
		r = (abs(cos(m*t/4))^n2 +
			abs(sin(m*t/4))^n3)^(-1/n1)
		ret[s] = ox + r * cos(t) * a
		boundbox[1] = min(boundbox[1], ret[s])
		boundbox[3] = max(boundbox[3], ret[s])
		ret[s + 1] = oy - r * sin(t) * b
		boundbox[2] = min(boundbox[2], ret[s + 1])
		boundbox[4] = max(boundbox[4], ret[s + 1])
		s = s + 2
	end
	boundbox[3] = boundbox[3] - boundbox[1]
	boundbox[4] = boundbox[4] - boundbox[2]
	return ret, boundbox
end

getCircularPoints = function(ox, oy, radius, number_of_points, from, to)
	local ret = {}
	from = from or 0
	to = to or pi * 2 - from
	local delta = (to - from) / number_of_points
	local s = 1
	for t = from, to, delta do
		ret[s] = ox + radius * cos(t)
		ret[s + 1] = oy - radius * sin(t)
		s = s + 2
	end
	return ret, {ox - radius, oy - radius, 2 * radius, 2 * radius}
end

getRectangularPoints = function (ox, oy, w, h)
	local ret = {ox-w/2,oy-h/2,ox+w/2,oy-h/2,ox+w/2,oy+h/2,ox-w/2,oy+h/2}
	return ret, {ret[1], ret[2], w, h}
end

RGBA = function(r, g, b, a)
	return r * 65536 + g * 256 + b + (a or 255) * 16777216
end

RGB = RGBA

toRGB = function(color)
	return (color / 65536) % 256, (color / 256) % 256, color % 256, (color / 16777216) % 256
end

toRGBA = toRGB

if Graphics then
	Graphics.setLineWidth = function (width)
		glLineWidth(width or 1)
	end
end

--[[ Memory ]]

getMemory = function ()
	return collectgarbage("count")
end

getUsedMemory = get_used_memory

clearMemory = function ()
	return collectgarbage()
end

--[[ OpenGL Stencil Testing]]

local scissorsSet = {}
local stencilsCount = 0
local GL_DECR, GL_INCR, GL_KEEP = GL_DECR, GL_INCR, GL_KEEP
local GL_NEVER, GL_EQUAL, GL_TRUE = GL_NEVER, GL_EQUAL, GL_TRUE
local GL_FALSE = GL_FALSE
local glStencilFunc = glStencilFunc
local glStencilOp = glStencilOp

function setScissor(x, y, width, height)
	local scissorCount = #scissorsSet
	if not x then
		if scissorCount + stencilsCount == 0 then
			return
		end
		if scissorCount + stencilsCount == 1 then
			glDisable(GL_STENCIL_TEST)
			scissorsSet[scissorCount] = nil
			return
		end
		local id, x, y, width, height = table.unpack(scissorsSet[scissorCount])
		scissorsSet[scissorCount] = nil
		glStencilFunc(GL_NEVER, 0, 0xFF)
		glStencilOp(GL_DECR, GL_DECR, GL_DECR)
		Graphics.rectangle(true, x, y, width, height)
		glStencilFunc(GL_EQUAL, id - 1, 0xFF)
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP)
	else
		if scissorCount + stencilsCount == 0 then
			glClear(GL_STENCIL_BUFFER_BIT)
			glEnable(GL_STENCIL_TEST)
		end
		scissorsSet[scissorCount + 1] = {scissorCount + stencilsCount + 1, x,  y, width, height}
		glStencilFunc(GL_NEVER, 0, 0xFF)
		glStencilOp(GL_INCR, GL_INCR, GL_INCR)
		Graphics.rectangle(true, x, y, width, height)
		glStencilFunc(GL_EQUAL, scissorCount + stencilsCount + 1, 0xFF)
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP)
	end
end

function stencilOpen(inverted)
	local ref = #scissorsSet + stencilsCount
	if ref == 0 then
		glClear(GL_STENCIL_BUFFER_BIT)
		glEnable(GL_STENCIL_TEST)
	end
	stencilsCount = stencilsCount + 1
	glStencilFunc(GL_EQUAL, ref, 0xFF)
	glStencilOp(GL_KEEP, GL_INCR, GL_INCR)
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE)
	if inverted then
		Graphics.rectangle(true, 0, 0, 4096, 4096)
		glStencilFunc(GL_EQUAL, ref + 1, 0xFF)
		glStencilOp(GL_KEEP, GL_DECR, GL_DECR)
	end
end

function stencilClose()
	local GL_TRUE, GL_KEEP = GL_TRUE, GL_KEEP
	local ref = #scissorsSet + stencilsCount
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE)
	glStencilFunc(GL_EQUAL, ref, 0xFF)
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP)
end

function stencilDone()
	local ref = #scissorsSet + stencilsCount
	stencilsCount = stencilsCount - 1
	if ref == 1 then
		glDisable(GL_STENCIL_TEST)
	else
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE)
		glStencilFunc(GL_EQUAL, ref, 0xFF)
		glStencilOp(GL_KEEP, GL_DECR, GL_DECR)
		Graphics.rectangle(true, 0, 0, 4096, 4096)
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE)
		glStencilFunc(GL_EQUAL, ref - 1, 0xFF)
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP)
	end
end

Scissor = setScissor



--[[ Tables ]]

table.tinsert = function(ret, insertion)
	local b = #ret
	for i, info in ipairs (insertion) do
		ret[b + i] = info
	end
end

table.clear = function (tab)
	for a, b in pairs (tab) do
		tab[a] = nil
	end
end

table.swap = function (tab, index_a, index_b)
	tab[index_a], tab[index_b] = tab[index_b], tab[index_a]
end

table.shuffle = function(tab, n)
	local n = n or #tab
	for times = 1, n do
		local position = random(#tab)
		local value = tab[position]
		remove(tab, position)
		insert(tab, random(n), value)
	end
end

table.find = function (tab, value)
	return isInArray(tab, value)
end

isInArray = function(array, value)
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
	if type(look_up) == 'function' then repeat
		if indexBegin > indexEnd then return nil, indexBegin end
			middle = indexBegin + floor((indexEnd - indexBegin) / 2)
			thing = look_up(vector[middle], middle, ...)
			if thing == nil then return middle
			elseif thing == true then indexEnd = middle - 1
			else indexBegin = middle + 1 end
		until false
	else repeat if indexBegin > indexEnd then return nil, indexBegin end
			middle = indexBegin + floor((indexEnd - indexBegin) / 2)
			thing = vector[middle]
			if thing == look_up then return middle
			elseif thing > look_up then indexEnd = middle - 1
			else indexBegin = middle + 1 end
		until false
	end
end

--[[ Math ]]

math.min_nil = function (a, b)
	if not a or not b then return a or b end
	return min(a, b)
end

math.max_nil = function (a, b)
	if not a or not b then return a or b end
	return max(a, b)
end


math.limit = function (lower, number, upper)
	return min(upper, math.max(lower, number))
end

math.radians = function (degrees)
	return degrees / 180 * math.pi
end

math.degrees = function (radians)
	return radians * 180 / math.pi
end

math.signal = function (number)
	return number >= 0 and 1 or -1
end

-- [[ Strings ]]

local capitalize = function(a, b)
	return upper(a) .. lower(b)
end

string.capitalize = function(a)
	return gsub(a, "(%w)(%w+)", capitalize)
end

string.trim = function (str)
	return gsub(str, "^%s*(.-)%s*$", "%1")
end

string.count = function (str, p, plain)
	local count = -1
	local begin = 0
	repeat
		begin = find(str, p, begin + 1, plain)
		count = count + 1
	until not begin
	return count
end

string.split = function (str, sep, plain, empty)
	local result, t, size = {}, 1, #str
	local st, en, mark  = 1
	repeat mark = st st, en = string.find(str, sep, st, plain)
		if st then if st > mark or empty then
			result[t] = string.sub(str, mark, st - 1)
		end st = en + 1 t = t + 1 else if size > mark then
		result[t] = string.sub(str, mark) end return result
	end until st >= size
	return result
end

string.alphanum = function (char)
	return match(char, '[a-zA-Z0-9]+')
end

local str = getmetatable("")

str.__add = function(a, b) -- the + operator
	return tostring(a) .. tostring(b)
	end

str.__sub = function(a, b) -- the - operator
	return gsub(a, b, "")
end

str.__index = function(a, b) -- if you attempt to do string[id]
	if type(b) ~= "number" then
		return string[b]
	end
	return sub(a, b, b)
end

-- [[ Connection ]]

httpGet = function (url, uri, includeHeaders)
	local _url, _uri = match(url, "http://([^/]+)/?(.*)")

	url = _url or url
	uri = _uri or uri

	local ip = Network.getIp(url)

	if ip == 0 then return nil, "couldn't resolve ip" end

	local tcp = TcpSocket()
	local success, err = tcp:connect(ip, 80)

	if not success then return nil, "on connect", err end

	success, err = tcp:send("GET /" .. (uri or "") .. 
	" HTTP/1.0\r\nFrom: nobody@sorry.org\r\n" ..
	"User-Agent: sfmlua/2.x\r\nHost: ".. url .. "\r\n" ..
	"Content-Length: 0\r\nContent-Type: " ..
	"application/x-www-form-urlencoded\r\n\r\n")

	if not success then return nil, "on send", err end

	local ret = {}

	repeat
		success, err = tcp:receive()
		if success then
			ret[#ret + 1] = success
		end
	until not success

	tcp:connect()

	if success then
		return table.concat(ret, "")
	end

	data = table.concat(ret, '')

	if not includeHeaders then
		for i = 1, #data do
			if sub(data, i, i + 3) == '\r\n\r\n' then
				data = data:sub(i + 4)
				break
			end
		end
	end

	return data
end