local myWindow = Window({
	width = 640,
	height = 480,
	title = "OpenGL Rotating Cube!"
})

function OpenGL_Init()
	glShadeModel(GL_SMOOTH)
	glClearColor(0, 0, 0, 0.5)
	glClearDepth(1)
	glEnable(GL_DEPTH_TEST)
	glDepthFunc(GL_LEQUAL)
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST)
end

function OpenGL_Resize(width, height)
	height = math.max(1, height)
	glViewport(0, 0, width, height)
	glMatrixMode(GL_PROJECTION)
	glLoadIdentity()
	gluPerspective(45, width/height, 0.1, 100)
	glMatrixMode(GL_MODELVIEW)
	glLoadIdentity()
end

local rotation = 0

function OpenGL_Render()
	glClear(GL_COLOR_BUFFER_BIT + GL_DEPTH_BUFFER_BIT)
	glLoadIdentity()
	glTranslated(0, 0, -7)
	glRotated(rotation, 0, 1, 0)
	glRotated(rotation, 1, 1, 1)
	glBegin(GL_QUADS)
	  glColor3f(0, 1, 0)
	  glVertex3f(1, 1, -1)
	  glVertex3f(-1, 1, -1)
	  glVertex3f(-1, 1, 1)
	  glVertex3f(1, 1, 1)
	  glColor3f(1, 0.5, 0)
	  glVertex3f(1, -1, 1)
	  glVertex3f(-1, -1, 1)
	  glVertex3f(-1, -1, -1)
	  glVertex3f(1, -1, -1)
	  glColor3f(1, 0, 0)
	  glVertex3f(1, 1, 1)
	  glVertex3f(-1, 1, 1)
	  glVertex3f(-1, -1, 1)
	  glVertex3f(1, -1, 1)
	  glColor3f(1, 1, 0)
	  glVertex3f(1, -1, -1)
	  glVertex3f(-1, -1, -1)
	  glVertex3f(-1, 1, -1)
	  glVertex3f(1, 1, -1)
	  glColor3f(0, 0, 1)
	  glVertex3f(-1, 1, 1)
	  glVertex3f(-1, 1, -1)
	  glVertex3f(-1, -1, -1)
	  glVertex3f(-1, -1, 1)
	  glColor3f(1, 0, 1)
	  glVertex3f(1, 1, -1)
	  glVertex3f(1, 1, 1)
	  glVertex3f(1, -1, 1)
	  glVertex3f(1, -1, -1)
	glEnd()
	rotation = rotation + 0.9
end

OpenGL_Resize(640, 480)
OpenGL_Init()

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

	if os.clock() - previous_time > 1 then
		previous_time = os.clock()
		myWindow:setTitle("OpenGL Rotating Cube! FPS: "..fps..".")
		fps = 0
	end

	System.sleep(15)

end

myWindow:close()