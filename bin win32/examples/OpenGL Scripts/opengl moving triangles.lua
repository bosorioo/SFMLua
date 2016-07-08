local myWindow = Window({
	width = 640,
	height = 480,
	title = "OpenGL Moving Triangles!",
	depthbits = 8
})

function OpenGL_Init()
	glShadeModel(GL_SMOOTH)
	glClearColor(0, 0, 0, 0)
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

function draw_triangle()
	glBegin(GL_TRIANGLES)
		glColor3f(1, 0, 0)
		glVertex3f(0, 1, 0)
		glColor3f(0, 1, 0)
		glVertex3f(-1, -1, 0)
		glColor3f(0, 0, 1)
		glVertex3f(1, -1, 0)
	glEnd()
end

local translation, rotation, scale = 0, 0, 1
local deltaT, deltaS = 0.005, -0.005

function OpenGL_Render()
	glClear(GL_COLOR_BUFFER_BIT + GL_DEPTH_BUFFER_BIT)
	glLoadIdentity()
	glPushMatrix()
	glTranslated(-2, 0, -6)
	glTranslated(0, translation, 0)
	draw_triangle()
	glPopMatrix()
	glPushMatrix()
	glTranslated(0, 0, -6)
	glRotated(rotation, 0, 1, 0)
	draw_triangle()
	glPopMatrix()
	glPushMatrix()
	glTranslated(2, 0, -6)
	glScaled(scale, scale, scale)
	draw_triangle()
	glPopMatrix()
	translation = translation + deltaT
	rotation = rotation + 0.5
	scale = scale + deltaS
	if translation > 0.9 or translation < -0.9 then deltaT = -deltaT end
	if scale < 0.75 or scale > 1.25 then deltaS = -deltaS end
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
		myWindow:setTitle("OpenGL Moving Triangles! FPS: "..fps..".")
		fps = 0
	end

	System.sleep(15)

end

myWindow:close()