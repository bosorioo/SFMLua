Graphics.setFont('C:/Windows/Fonts/DejaVuSerif.ttf')
BLOCK_SIZE = 20
FRUIT_RADIUS = 4
WINDOW_SIZE = 520
BLOCKS = WINDOW_SIZE/BLOCK_SIZE
BLOCK_EMPTY = 0
BLOCK_SNAKE = 1
BLOCK_FRUIT = 2
DIRECTION_NONE  =  0
DIRECTION_NORTH =  1
DIRECTION_EAST  =  2
DIRECTION_SOUTH = -1
DIRECTION_WEST  = -2
UPDATE_INTERVAL = 260
UPDATE_ACCEL = 7
UPDATE_FASTEST = 20
SNAKE_COLOR = {83, 146, 237}
DEAD_SNAKE_COLOR = {220, 25, 45}
FRUIT_COLOR = {24, 193, 72}
FPS_LIMIT = 60

_GAME_OVER = false
_STARTED = false
_LAST_UPDATED = 0
_TIME_TO_UPDATE = UPDATE_INTERVAL
_BLOCKED_DIRECTION = false
_MAP = false
_SNAKE = false

math.randomseed(os.time())
math.random();math.random();math.random();math.random()

function getCenterPosition()
	return math.floor(WINDOW_SIZE/BLOCK_SIZE/2)
end

function mapAddFruit(map)
	local row, column = math.random(0, BLOCKS - 1), math.random(0, BLOCKS - 1)
	while map[row + 1][column + 1] ~= BLOCK_EMPTY do
		row, column = math.random(0, BLOCKS - 1), math.random(0, BLOCKS - 1)
	end
	map[row + 1][column + 1] = BLOCK_FRUIT
end

function newMap()
	local map = {}
	for row = 0, BLOCKS do
		map[row + 1] = {}
		for column = 0, BLOCKS do
			map[row + 1][column + 1] = BLOCK_EMPTY
		end
	end
	mapAddFruit(map)
return map
end

function mapSetType(map, row, column, type)
	if row < 1 or row > BLOCKS or column < 1 or column > BLOCKS then
		return
	end
	map[row][column] = type
end

function mapGetType(map, row, column)
	if row < 1 or row > BLOCKS or column < 1 or column > BLOCKS then
		return
	end
	return map[row][column]
end

function getPositionByDirection(x, y, direction)
	if direction == DIRECTION_NORTH then
		y = y - 1
	elseif direction == DIRECTION_SOUTH then
		y = y + 1
	elseif direction == DIRECTION_EAST then
		x = x + 1
	elseif direction == DIRECTION_WEST then
		x = x - 1
	end
	if x < 0 then
		x = BLOCKS - x - 2
	elseif x >= BLOCKS then
		x = x - BLOCKS
	end
	if y < 0 then
		y = BLOCKS - y - 2
	elseif y >= BLOCKS then
		y = y - BLOCKS
	end
	return x, y
end

function grayscaleColor(color)
	local c = color[1] * .16 + color[2] * .63 + color[3] * .21
	return {c, c, c}
end

window = Window({width = WINDOW_SIZE, height = WINDOW_SIZE, title = "Snake"})
window:setVerticalSync(false)

_SNAKE = {
	row = getCenterPosition(),
	column = getCenterPosition(),
	direction = DIRECTION_NONE,
	size = 0,
	body_positions = {}
}

function onStart()
	_MAP = newMap()
	_SNAKE = {
		row = getCenterPosition(),
		column = getCenterPosition(),
		direction = DIRECTION_NONE,
		size = 0,
		body_positions = {}
	}
	_BLOCKED_DIRECTION = false
	_STARTED = false
	_LAST_UPDATED = 0
	_TIME_TO_UPDATE = UPDATE_INTERVAL
	_GAME_OVER = false
end



function onDraw()

	for row = 0, BLOCKS do
		for column = 0, BLOCKS do
			local block_type = _MAP[row + 1][column + 1]
			if block_type == BLOCK_SNAKE then
				if _GAME_OVER then
					Graphics.setColor(grayscaleColor(SNAKE_COLOR))
				else
					Graphics.setColor(SNAKE_COLOR)
				end
				Graphics.rectangle(true, 1 + row * BLOCK_SIZE,
						1 + column * BLOCK_SIZE,
						BLOCK_SIZE - 2, BLOCK_SIZE - 2)
			elseif block_type == BLOCK_FRUIT and not _GAME_OVER then
				Graphics.setColor(FRUIT_COLOR)
				Graphics.circle(true, (row + .5) * BLOCK_SIZE,
						(column + .5) * BLOCK_SIZE,
						FRUIT_RADIUS, FRUIT_RADIUS * 5)
			end
		end
	end

	if _GAME_OVER then
		Graphics.setColor(DEAD_SNAKE_COLOR)
	else
		Graphics.setColor(SNAKE_COLOR)
	end

	if _GAME_OVER and (os.clock() % 1) < .5 then
		local row, column = getPositionByDirection(_SNAKE.row, _SNAKE.column, -_SNAKE.direction)
		Graphics.rectangle(true, 1 + row * BLOCK_SIZE,
			1 + column * BLOCK_SIZE,
			BLOCK_SIZE - 2, BLOCK_SIZE - 2)
	else
		Graphics.rectangle(true, 1 + _SNAKE.row * BLOCK_SIZE,
			1 + _SNAKE.column * BLOCK_SIZE,
			BLOCK_SIZE - 2, BLOCK_SIZE - 2)
	end

	if _GAME_OVER then
		Graphics.setColor(255, 0, 0)
		Graphics.print("GAME OVER", WINDOW_SIZE/2 - 162, WINDOW_SIZE/2 - 120,
			{size = 56})
		Graphics.setColor(255, 255, 255)
		Graphics.print("Press ENTER to restart", WINDOW_SIZE/2 - 172, WINDOW_SIZE/2 - 60,
			{size = 25})
	end

	Graphics.setColor(255, 255, 255)
	for i = 1, 3 do
		Graphics.print("Snake length: " .. _SNAKE.size + 1, WINDOW_SIZE/2 - 74, 12,
			{size = 15})
	end
end

function onUpdate()
	if _GAME_OVER then return end
	local row, column = _SNAKE.row, _SNAKE.column
	_SNAKE.row, _SNAKE.column = getPositionByDirection(_SNAKE.row, _SNAKE.column, _SNAKE.direction)
	if mapGetType(_MAP, _SNAKE.row + 1, _SNAKE.column + 1) == BLOCK_FRUIT then
		table.insert(_SNAKE.body_positions, 1, {row, column})
		_SNAKE.size = _SNAKE.size + 1
		mapSetType(_MAP, row + 1, column + 1, BLOCK_SNAKE)
		mapAddFruit(_MAP)
		_TIME_TO_UPDATE = math.max(UPDATE_FASTEST, _TIME_TO_UPDATE - UPDATE_ACCEL)
		_BLOCKED_DIRECTION = -_SNAKE.direction
	elseif _SNAKE.size > 0 then
		_BLOCKED_DIRECTION = -_SNAKE.direction
		mapSetType(_MAP, row + 1, column + 1, BLOCK_SNAKE)
		mapSetType(_MAP, _SNAKE.body_positions[_SNAKE.size][1] + 1,
			_SNAKE.body_positions[_SNAKE.size][2] + 1, BLOCK_EMPTY)
		table.remove(_SNAKE.body_positions, _SNAKE.size)
		table.insert(_SNAKE.body_positions, 1, {row, column})
	end
	if mapGetType(_MAP, _SNAKE.row + 1, _SNAKE.column + 1) == BLOCK_SNAKE then
		_GAME_OVER = true
	end
end

onStart()

local frames = 0
local timing = 0

while window:isOpen() do

	window:clear()
	onDraw()
	window:display()

	for _, event in pairs (window:getEvents()) do
		if event.type == Events.Closed then
			window:close()
			return
		elseif event.type == Events.KeyPressed then
			if not _STARTED then
				_STARTED = true
			end
			if _GAME_OVER then
				if event.keycode == Keys.Return then
					onStart()
				end
			else
				if event.keycode == Keys.Left and
				   DIRECTION_WEST ~= _BLOCKED_DIRECTION then
					_SNAKE.direction = DIRECTION_WEST
				elseif event.keycode == Keys.Right and
				   DIRECTION_EAST ~= _BLOCKED_DIRECTION then
					_SNAKE.direction = DIRECTION_EAST
				elseif event.keycode == Keys.Up and
				   DIRECTION_NORTH ~= _BLOCKED_DIRECTION then
					_SNAKE.direction = DIRECTION_NORTH
				elseif event.keycode == Keys.Down and
				   DIRECTION_SOUTH ~= _BLOCKED_DIRECTION then
					_SNAKE.direction = DIRECTION_SOUTH
				end
			end
		end
	end

	local clock = os.clock()

	if _STARTED and clock - _LAST_UPDATED >= _TIME_TO_UPDATE/1000 then
			_LAST_UPDATED = clock
			onUpdate()
	end

	frames = frames + 1

	if clock - timing >= 1 then
		window:setTitle("Snake - "..frames.." fps")
		frames = 0
		timing = clock
	end

	System.sleep(1000/FPS_LIMIT)

end