-- Opens jack.png
local jack = Image(WorkDir .. "jack.png")

-- If jack couldn't be created...
if not jack then 
	-- Oh no! Jack couldn't be loaded
	return
end

-- Make white pixels invisible!
jack:alphaMask({255, 255, 255})

-- Create a texture to make possible drawing images to window
local texture = Texture()
-- Get the pixels from jack and store in the texture
texture:loadImage(jack)

-- Creates a sprite (our jack!) from the texture
local Jack = Sprite(texture)
-- Set the center (also called origin) of transformations (rotations etc) at jack's head
Jack:setOrigin(16, 4)

-- Create our window
local window = Window({height = 420, width = 600, title = "Jack is saying hi!", antialiasing = 8})
-- Set the icon to jack's picture
window:setIcon(jack)
window:setVerticalSync(true)

-- Variables to control background color and jack's details
local last = 0
local d = 3
local background = {255, 255, 255}
local backchange = {math.random(-20, 20)/d, math.random(-20, 20)/d, math.random(-20, 20)/d}

-- Main loop, executing until window is closed
while window:isOpen() do

	-- Get events so the window will be responsive
	for _, event in ipairs (window:getEvents()) do
		if event.type == Events.Closed then
			window:close()
		end
	end

	-- If program run time reached the last jack's modification, it's time to modify it again
	if last <= os.clock() then
		-- Set a random position for jack in the screen
		Jack:setPosition(math.random(600), math.random(420))
		-- At 33% of time
		if math.random(3) == 1 then
			-- His rotation will be 0 (he will look just like the original image)
			Jack:setRotation(0)
			-- His size will vary from 5x to 10x (jack is small, 32x32 pixels, we have to make it big!)
			Jack:setScale(math.random(5, 10))
		-- At 66% of time
		else
			-- His rotation will be any!
			Jack:setRotation(math.random(360))
			-- His size may vary from small to big
			Jack:setScale(math.random(2, 35))
		end
		-- Schedule the next time jack needs to be modified, that may range from 0 seconds to 1.2 seconds
		last = os.clock() + math.random(12)/10
	end

	-- Clear the window with the color stored in background table
	window:clear(background)

	-- For the RGB of the background:
	for i = 1, 3 do
		-- The background color changes according to the backchange table
		background[i] = backchange[i] + background[i]
		-- What if the backchange table holds very low values? Color changing will be very slow, and that's boring,
		-- so lets generate new values for it!
		if math.abs(backchange[i]) < 5/d then
			backchange[i] = math.random(-20, 20)/d
		end
		-- Color cannot go further than 255!
		if background[i] > 255 then
			-- Set it to an acceptable value
			background[i] = 255
			-- Update it's varying value
			backchange[i] = math.random(-20, 20)/d
		-- Same here, except that pixels can't be darker than black (0)
		elseif background[i] < 0 then
			background[i] = 0
			backchange[i] = math.random(-20, 20)/d
		end
	end

	-- Draw jack
	Jack:draw(window)
	-- Display contents drawn
	window:display()

end