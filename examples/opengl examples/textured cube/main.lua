Console.open()

local myWindow = Window({
	width = 640,
	height = 480,
	title = "OpenGL Textured Cube!",
	depthbits = 24,
	antialiasing = 8
})

function OpenGL_Resize(width, height)
	height = math.max(1, height)
	glViewport(0, 0, width, height)
	glMatrixMode(GL_PROJECTION)
	glLoadIdentity()
	gluPerspective(45, width / height, 1, 1000)
	--glFrustum(-2, 2, -1.5, 1.5, 1, 40)
	--glOrtho(-10, 1, -1, 1, 1, 100)
	glMatrixMode(GL_MODELVIEW)
	glLoadIdentity()
end

glShadeModel(GL_SMOOTH)
glClearColor(0, 0, 0, 0.5)
glClearDepth(1)
glEnable(GL_TEXTURE_2D)
glEnable(GL_DEPTH_TEST)
glDepthFunc(GL_LEQUAL)
glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST)
OpenGL_Resize(640, 480)

local rotation = 0
local texture = Texture(WorkDir .. "texture.png")

function OpenGL_Render()

	glClear(GL_COLOR_BUFFER_BIT + GL_DEPTH_BUFFER_BIT)
	glLoadIdentity()

	glTranslated(0, 0, -4)
	glRotated(rotation, 0, 1, 0)
	glRotated(rotation, 1, 1, 1)
	Texture.bind(texture)

	glBegin(GL_QUADS)

	  glTexCoord2f(1, 1)
	  glVertex3f(1, 1, -1)
	  glTexCoord2f(0, 1)
	  glVertex3f(-1, 1, -1)
	  glTexCoord2f(0, 0)
	  glVertex3f(-1, 1, 1)
	  glTexCoord2f(1, 0)
	  glVertex3f(1, 1, 1)

	  glTexCoord2f(0, 1)
	  glVertex3f(1, -1, 1)
	  glTexCoord2f(1, 1)
	  glVertex3f(-1, -1, 1)
	  glTexCoord2f(1, 0)
	  glVertex3f(-1, -1, -1)
	  glTexCoord2f(0, 0)
	  glVertex3f(1, -1, -1)

	  glTexCoord2f(1, 0)
	  glVertex3f(1, 1, 1)
	  glTexCoord2f(0, 0)
	  glVertex3f(-1, 1, 1)
	  glTexCoord2f(0, 1)
	  glVertex3f(-1, -1, 1)
	  glTexCoord2f(1, 1)
	  glVertex3f(1, -1, 1)

	  glTexCoord2f(0, 0)
	  glVertex3f(1, -1, -1)
	  glTexCoord2f(1, 0)
	  glVertex3f(-1, -1, -1)
	  glTexCoord2f(1, 1)
	  glVertex3f(-1, 1, -1)
	  glTexCoord2f(0, 1)
	  glVertex3f(1, 1, -1)


	  glTexCoord2f(0, 0)
	  glVertex3f(-1, 1, 1)
	  glTexCoord2f(1, 0)
	  glVertex3f(-1, 1, -1)
	  glTexCoord2f(1, 1)
	  glVertex3f(-1, -1, -1)
	  glTexCoord2f(0, 1)
	  glVertex3f(-1, -1, 1)


	  glTexCoord2f(0, 0)
	  glVertex3f(1, 1, -1)
	  glTexCoord2f(1, 0)
	  glVertex3f(1, 1, 1)
	  glTexCoord2f(1, 1)
	  glVertex3f(1, -1, 1)
	  glTexCoord2f(0, 1)
	  glVertex3f(1, -1, -1)

	glEnd()

	Texture.bind()
	rotation = rotation + 0.5
end

local running = true
local previous_time = os.clock()
local fps = 0

while running do

	for _, event in ipairs (myWindow:getEvents()) do
		if event.type == Events.Closed then
			running = false
		elseif event.type == Events.KeyPressed then
			if event.keycode == Keys.Escape or
				event.keycode == Keys.F4 and event.alt then
				running = false
			end
		elseif event.type == Events.Resized then
			OpenGL_Resize(event.width, event.height)
		end
	end

	OpenGL_Render()
	myWindow:display()

	fps = fps + 1
	local clock = os.clock()

	if clock - previous_time > 1 then
		previous_time = os.clock()
		myWindow:setTitle("OpenGL Textured Cube! FPS: "..fps..".")
		fps = 0
	end

	--System.sleep(10)

end

myWindow:close()