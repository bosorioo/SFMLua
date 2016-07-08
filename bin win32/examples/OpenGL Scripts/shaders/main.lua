Console.open()

HEIGHT = 600
WIDTH = 800
SHADER_FADE_TIME = 5
SHADER_CHANGE_TIME = 15
SHADER_CHANGE_RANDOMLY = true

_SHADERS = {}
_SHADER_NOW = 0
_SHADER_NEXT = 0
_SHADER_SWITCHING = false
_SHADER_SWITCH_TIME = 0
_SHADER_LAST_CHANGE = 0
_SHADER_SWITCHER_TIME = 0

function onStart()
	math.randomseed(os.time())
	math.random()math.random()math.random()
	_SHADERS = {}
	local i = 1
	repeat
		local path = string.format(WorkDir.."shaders/sh%.2d.txt", i)
		local content = getFileContent(path)
		if not content then break end
		printn("Loading shader #".. i .. ".")
		local s = Shader(content)
		if s then
			s:setParameter("resolution", WIDTH, HEIGHT)
			table.insert(_SHADERS, s)
			if i ~= 1 then
				s:setParameter("alpha", 1)
			end
		end
		i = i + 1
	until false
	print ("Loaded "..(i-1).." shaders.\n")
	_SHADER_NOW = 0
	_SHADER_NEXT = 0
	_SHADER_SWITCHING = false
	_SHADER_SWITCH_TIME = 0
	_SHADER_LAST_CHANGE = 0
	_SHADER_SWITCHER_TIME = 0
	changeShader(true)
	printn("Currently showing shader #".._SHADER_NOW..".")
end

function getFileContent(path)
	local r = io.open(path, "r")
	if not r then return end
	local t = r:read("*a")
	r:close()
	return t
end

function onResize(width, height)
	WIDTH, HEIGHT = width, height
	for index, shader in ipairs (_SHADERS) do
		shader:setParameter("resolution", width, height)
	end
end

function drawShader()
	local s = _SHADERS[_SHADER_NOW]
	if s then
		local time = os.clock()
		if _SHADER_SWITCHING then
			local next = _SHADERS[_SHADER_NEXT]
			local value = (time-_SHADER_SWITCH_TIME)/SHADER_FADE_TIME
			next:setParameter("time", time)
			next:setParameter("alpha", value)
			Shader.bind(next)
			Graphics.rectangle(true, 0, 0, WIDTH, HEIGHT)
			if value >= 1 then
				s:setParameter("alpha", 1)
				_SHADER_SWITCHING = false
				_SHADER_NOW = _SHADER_NEXT
				s = _SHADERS[_SHADER_NOW]
				s:setParameter("alpha", 1)
			else
				s:setParameter("alpha", 1 - value)
				s:setParameter("time", time)
				Shader.bind(s)
				Graphics.rectangle(true, 0, 0, WIDTH, HEIGHT)
			end
			if time - _SHADER_SWITCH_TIME <= 2.5 then
				Shader.bind(_SHADERS[1])
				_SHADERS[1]:setParameter("time", time - _SHADER_SWITCH_TIME)
				Graphics.rectangle(true, 0, 0, WIDTH, HEIGHT)
			end
		else
			s:setParameter("time", time)
			Shader.bind(s)
			Graphics.rectangle(true, 0, 0, WIDTH, HEIGHT)
		end
	end
	Shader.bind()
end

function changeShader(first, next)
	if not next then
		repeat
			_SHADER_NEXT = math.random(#_SHADERS)
		until _SHADER_NEXT ~= 1 and _SHADER_NEXT ~= _SHADER_NOW
	else
		_SHADER_NEXT = math.max(1, math.min(#_SHADERS, tonumber(next)))
	end
	if first then _SHADER_NOW = _SHADER_NEXT return end
	if not next then
		_SHADER_SWITCHING = true
		_SHADER_SWITCH_TIME = os.clock()
	else
		_SHADER_NOW = _SHADER_NEXT
	end
	_SHADER_LAST_CHANGE = os.clock()
	print ("Changing to ".._SHADER_NEXT..".\n")
end

local window = Window({height = HEIGHT, width = WIDTH, title = "Shaders"})

onStart()

while window:isOpen() do

	for index, event in ipairs (window:getEvents()) do
		if event.type == Events.Closed then
			window:close()
			return
		elseif event.type == Events.Resized then
			onResize(event.width, event.height)
		elseif event.type == Events.KeyPressed then
			if event.keycode == Keys.Right then
				SHADER_CHANGE_RANDOMLY = false
				changeShader(false, (_SHADER_NOW + 1) % (#_SHADERS + 1))
			elseif event.keycode == Keys.Left then
				SHADER_CHANGE_RANDOMLY = false
				if _SHADER_NOW == 1 then
					changeShader(false, #_SHADERS)
				else
					changeShader(false, (_SHADER_NOW - 1) % (#_SHADERS + 1))
				end
			end
		end
	end
	
	window:clear()

	drawShader()

	if os.clock() - _SHADER_LAST_CHANGE >
	   (SHADER_CHANGE_RANDOMLY and SHADER_CHANGE_TIME or 5 * SHADER_CHANGE_TIME) then
		SHADER_CHANGE_RANDOMLY = true
		changeShader()
	end

	window:display()
end
