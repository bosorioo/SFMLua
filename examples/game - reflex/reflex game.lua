--[[highscore: 134.316 159]]
Graphics.setFont('C:/Windows/Fonts/DejaVuSerif.ttf')
local w_width = 400
local w_height = 120
local window = Window({height = w_height, width = w_width, styles = Styles.Titlebar + Styles.Close, title = "Reflex"})
window:setVerticalSync(true)
local enemy_width = 9
local enemies = {}
local enemy_speed = 5.2
local shot_radius = 3
local shot_speed = 5
local shots = {}
local lives = 3
local time_started = 0
local killed = 0
local started = false
local highscore = {time = 0, kills = 0}
local font = {size = 16}
local highscore_load = io.open(WorkDir .. "reflex.lua", "r")
if highscore_load then
	local score = highscore_load:read("*l")
	if score then
		highscore.time, highscore.kills = score:match("%-%-%[%[highscore: (%d+%.?%d*)%s(%d+%.?%d*)%]%]")
		highscore.time = tonumber(highscore.time) or 0
		highscore.kills = tonumber(highscore.kills) or 0
	end
	highscore_load:close()
end

math.randomseed(os.time())
math.random()math.random()math.random()

function precision(n, p)
	p = p or 4
	local d = math.floor((n - math.floor(n)) * 10^p) / 10^p
	if d == 0 then
		return tostring(math.floor(n)) .. "." .. string.rep("0", p)
	else
		return tostring(math.floor(n)) .. "." .. tostring(d):sub(3)
	end
return n
end

function getTime()
	return math.random(0, 250) / 100
end

function newShot(right)
	local shot = {right, pos = right and 220 or 180}
	table.insert(shots, shot)
end

function newEnemy(right, delay, deep)
	 local enemy = {os.clock() + delay, right, pos = 0}
	 table.insert(enemies, enemy)
	 if math.random(3) == 1 and deep and deep < 3 then
		 newEnemy(math.random(2) == 1, getTime(), 1)
	 end
end

function Reset()
	enemies = {}
	shots = {}
	killed = 0
end

function Begin()
	time_started = os.clock()
	lives = 3
	newEnemy(true, getTime() + 1.2)
	newEnemy(false, getTime() + 1.2)
	started = true
end

function Highscore()
	if highscore.time < os.clock() - time_started then
		highscore.time = os.clock() - time_started
	end
	if highscore.kills < killed then
		highscore.kills = killed
	end
end

function SaveHighscore()
	local file = io.open(WorkDir .. "reflex.lua", "r+")
	if not file then return end
	local text = file:read("*a")
	text = text:match("[^\n]+.(.+)")
	file:close()
	if text then
		text = "--[[highscore: "..highscore.time.." "..highscore.kills.."]]\n" .. text
		file = io.open(WorkDir .. "reflex.lua", "w")
		file:write(text)
		file:close()
	end
end
	  

function TakeLife()
	lives = lives - 1
	if lives < 0 then
		started = false
		Highscore()
		Reset()
	end
end

local view = View()
view:setSize(w_width, w_height)
view:setCenter(w_width/2, w_height/2)

while window:isOpen() do
	for _, event in pairs(window:getEvents()) do
		if event.type == Events.Closed then
			window:close()
			SaveHighscore()
			return
		elseif event.type == Events.KeyPressed then
			if started and os.clock() - time_started >= 1 then
				if event.keycode == Keys.Left then
					newShot(false)
				elseif event.keycode == Keys.Right then
					newShot(true)
				end
			elseif not started and event.keycode == Keys.Return then
				Begin()
			end
		end
	end
	
	local width, height = window:getSize()
	local ratio = math.min(width/w_width, height/w_height)
	local wr, hr = math.abs(w_width*ratio - width)/2/width,  math.abs(w_height*ratio - height)/2/height
	window:clear()
	
	view:setViewport(wr, hr, 1 - 2 * wr, 1 - 2 * hr)
	
	window:setView(view)
	
	Graphics.setColor(220, 220, 220)
	Graphics.rectangle(true, 0, 0, w_width, w_height)
	Graphics.setColor(210, 210, 210)
	Graphics.rectangle(true, 40, 40, 320, 40)
	Graphics.setColor(0, 0, 0)
	Graphics.rectangle(false, 40, 40, 320, 40)
	Graphics.setColor(25, 255, 65)
	Graphics.rectangle(true, 180, 40, 40, 40)
	Graphics.setColor(0, 0, 0)
	Graphics.rectangle(false, 180, 40, 40, 40)
	
	for index, shot in pairs (shots) do
		if shot.pos <= 180 then
			shot.pos = shot.pos - shot_speed
		else
			shot.pos = shot.pos + shot_speed
		end
		if shot[1] and shot.pos + shot_radius/2 >= 360 or
		   not shot[1] and shot.pos - shot_radius/2 <= 40 then
			TakeLife()
			table.remove(shots, index)
		else
			Graphics.setColor(40, 40, 220)
			Graphics.circle(true, shot.pos, 60, shot_radius, 30)
			Graphics.setColor(0, 0, 0)
			Graphics.circle(false, shot.pos, 60, shot_radius, 30)
		end
	end
	
	for index, enemy in pairs (enemies) do
		if type(enemy[1]) == "number" then
			if enemy[1] > os.clock() then
				goto Continue
			else
				enemy.pos = enemy[2] and 40 or 360
				enemy[1] = true
			end
		end
		if enemy.pos < 200 then
			enemy.pos = enemy.pos + enemy_speed
		else
			enemy.pos = enemy.pos - enemy_speed
		end
		if enemy[2] and enemy.pos + enemy_width/2 >= 180 or
		   not enemy[2] and enemy.pos - enemy_width/2 <= 220 then
			table.remove(enemies, index)
			newEnemy(enemy[2], getTime())
			TakeLife()
		else
			Graphics.setColor(255, 0, 0)
			Graphics.rectangle(true, enemy.pos - enemy_width/2, 40, enemy_width, 40)
			Graphics.setColor(0, 0, 0)
			Graphics.rectangle(false, enemy.pos - enemy_width/2, 40, enemy_width, 40)
		end
		::Continue::
	end
	
	for i, shot in pairs (shots) do
		for j, enemy in pairs (enemies) do
			if math.abs(enemy.pos - shot.pos) <= shot_radius/2 + enemy_width/2 then
				table.remove(enemies, j)
				table.remove(shots, i)
				newEnemy(enemy[2], getTime())
				killed = killed + 1
			end
		end
	end
	
	if started then
		Graphics.print("Lives: ".. lives, 40, 8, font)
		Graphics.print("Time Survived: ".. precision(os.clock() - time_started, 1), 170, 8, font)
		Graphics.print("Killed: "..killed, 145, 90, font)
	else
		Graphics.print("Use left/right arrows to shoot!", 44, 8, font)
		Graphics.print("Press ENTER to start.", 104, 90, font)
	end
	window:display()
end