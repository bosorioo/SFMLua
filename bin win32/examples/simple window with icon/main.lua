-- Define a table that contains the window's information
local attributes = {
	height = 320,
	width  = 450,
	title  = "Example Window"
}

-- Create the window and store it in variable "window"
local window = Window(attributes)

-- Loads the image at a given directory
local icon = Image(WorkDir .. "/icon.png")

if not icon then
	return error("Icon.png not found! Looked on "..WorkDir.."/icon.png.\n")
end

-- Make white pixels transparent
icon:alphaMask({255, 255, 255})

-- Changes the window icon
window:setIcon(icon)

-- Main loop of the program, it runs while the window isn't closed
while window:isOpen() do

	-- Refreshes the window. This is a substitute for the method getEvents. While getEvents returns
	-- a table with the users inputs, this function drops and ignores that. Both make the window
	-- responsive. Anyway, at least one of them is required, using both result in the second
	-- being just useless. The refresh function automatically closes the window when the user
	-- press the close button or send any other message to close the program (like alt + f4),
	-- while the getEvents function tells the program that the user wants to close it, so it
	-- can be handled manually.
	window:refresh()

	-- Paint the entire window to a tone of blue
	window:clear(0, 0, 125)

	-- Drawing operations stay on buffer, the display method has to be called so everything will
	-- be drawn on the window.
	window:display()

end