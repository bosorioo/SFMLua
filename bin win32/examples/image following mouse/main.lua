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

-- Main loop, executing until window is closed
while window:isOpen() do

	-- Get events so the window will be responsive
	for _, event in ipairs (window:getEvents(true)) do
		if event.type == Events.Closed then
			window:close()
		elseif event.type == Events.MouseMoved then
			Jack:setPosition(event.x, event.y)
		elseif event.type == Events.Resized then
			local view = View()
			view:setSize(event.width, event.height)
			view:setCenter(event.width / 2, event.height / 2)
			window:setView(view)
		end
	end

	-- Clear the window to white
	window:clear(255, 255, 255)

	-- Draw jack
	Jack:draw(window)
	-- Display contents drawn
	window:display()

end