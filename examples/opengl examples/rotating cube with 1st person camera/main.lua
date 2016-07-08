Graphics.setFont('C:/Windows/Fonts/DejaVuSerif.ttf')

local myWindow = Window({
	width = 640,
	height = 480,
	title = "OpenGL 1 Person Camera!",
	depthbits = 24
})

local camera = {
	position = {0, 2.5, 5},
	view = {0, 2.5, 0},
	upvector = {0, 1, 0},
	deltaspeed = .25,
	deltaangle = math.pi/180,
	vspeed = 0,
	hspeed = 0,
	vertical_angle = 0,
	horizont_angle = 0,
	distance = 5
}

function Camera_Move()
	if Keys.isKeyDown(Keys.W) then
		camera.vspeed = camera.vspeed - camera.deltaspeed
	end
	if Keys.isKeyDown(Keys.S) then
		camera.vspeed = camera.vspeed + camera.deltaspeed
	end
	if Keys.isKeyDown(Keys.D) then
		camera.hspeed = camera.hspeed + camera.deltaspeed
	end
	if Keys.isKeyDown(Keys.A) then
		camera.hspeed = camera.hspeed - camera.deltaspeed
	end
	if Keys.isKeyDown(Keys.Up) then
		camera.vertical_angle = camera.vertical_angle + camera.deltaangle
		camera.vertical_angle = math.min(0.9902, camera.vertical_angle)
	end
	if Keys.isKeyDown(Keys.Down) then
		camera.vertical_angle = camera.vertical_angle - camera.deltaangle
		camera.vertical_angle = math.max(-0.7071, camera.vertical_angle)
	end
	if Keys.isKeyDown(Keys.Left) then
		camera.horizont_angle = camera.horizont_angle - camera.deltaangle
	end
	if Keys.isKeyDown(Keys.Right) then
		camera.horizont_angle = camera.horizont_angle + camera.deltaangle
	end
	if camera.horizont_angle > math.pi * 2 then
		camera.horizont_angle = camera.horizont_angle - math.pi * 2
	elseif camera.horizont_angle < 0 then
		camera.horizont_angle = camera.horizont_angle + math.pi * 2
	end
	local cosv, cosh, sinv, sinh = math.cos(camera.vertical_angle),
		math.cos(camera.horizont_angle), math.sin(camera.vertical_angle),
		math.sin(camera.horizont_angle)
	camera.position = {
		camera.position[1] - sinh * camera.vspeed + cosh * camera.hspeed,
		camera.position[2],
		camera.position[3] + cosh * camera.vspeed + sinh * camera.hspeed
	}
	camera.view = {
		camera.position[1] + camera.distance * sinh * cosv,
		camera.position[2] + camera.distance * sinv,
		camera.position[3] - camera.distance * cosh * cosv
	}
	camera.upvector[2] = cosv >= 0 and 1 or -1
	camera.hspeed = 0
	camera.vspeed = 0	
end

function OpenGL_Init()
	glShadeModel(GL_SMOOTH)
	glClearColor(0, 0, 0, 0.5)
	glClearDepth(1)
end

local View2D = View()

function OpenGL_Resize(width, height)
	View2D:setSize(width, height)
	View2D:setCenter(width/2, height/2)
	myWindow:setView(View2D)
	height = math.max(1, height)
end

local rotation = 0

function OpenGL_Render()
    local width, height = myWindow:getSize()
	glEnable(GL_DEPTH_TEST)
	glDepthFunc(GL_LEQUAL)
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST)
	glMatrixMode(GL_PROJECTION)
	glLoadIdentity()
	gluPerspective(45, width/height, 1, 1000)
	--glFrustum(-10, 10, -10, 10, 1, 1000)
	glMatrixMode(GL_MODELVIEW)
	glClear(GL_COLOR_BUFFER_BIT + GL_DEPTH_BUFFER_BIT)
	glLoadIdentity()
	gluLookAt(camera.position[1], camera.position[2], camera.position[3],
		camera.view[1], camera.view[2], camera.view[3],
		camera.upvector[1], camera.upvector[2], camera.upvector[3])
	glColor3f(150/255, 190/255, 150/255)
	glBegin(GL_LINES)
	local glVertex3f = glVertex3f
	for i = -500, 500, 5 do
		  glVertex3f(-500, 0, i)
		  glVertex3f(500, 0, i)
		  glVertex3f(i, 0, -500)
		  glVertex3f(i, 0, 500)
	end
	glEnd()

	glPushMatrix()

	glTranslated(0, 2, -5)
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
	  glColor3f(1, 1, 1)
	  glVertex3f(1, 1, -1)
	  glVertex3f(1, 1, 1)
	  glVertex3f(1, -1, 1)
	  glVertex3f(1, -1, -1)
	glEnd()
	rotation = rotation + 0.9

	glPopMatrix()

	glColor3f(75/255, 128/255, 1)
	glBegin(GL_LINES)
	for i = -500, 500, 5 do
		  glVertex3f(-500, 20, i)
		  glVertex3f(500, 20, i)
		  glVertex3f(i, 20, -500)
		  glVertex3f(i, 20, 500)
	end
	glEnd()

end

OpenGL_Resize(myWindow:getSize())
OpenGL_Init()

local running = true
local previous_time = os.clock()
local fps = 0
local hasfocus = true

while running do

	for _, event in ipairs (myWindow:getEvents()) do
		if event.type == Events.Closed then
			running = false
		elseif event.type == Events.KeyPressed then
			if event.keycode == Keys.Escape or
				event.keycode == Keys.F4 and event.alt then
				running = false
			end
		elseif event.type == Events.FocusOff then
			hasfocus = false
		elseif event.type == Events.FocusOn then
			hasfocus = true
		elseif event.type == Events.Resized then
			OpenGL_Resize(event.width, event.height)
		end
	end

	if hasfocus then
		Camera_Move()
	end

	OpenGL_Render()
	
	myWindow:resetGLStates()

	Graphics.setColor(0, 0, 25, 120)
	Graphics.rectangle(true, 12, 12, 272, 48)
	Graphics.setColor(25, 75, 44)
	Graphics.rectangle(false, 12, 12, 272, 48)
	Graphics.setColor({255, 255, 255})
	Graphics.print("Use W, A, S, D to walk",
		17, 14, {size = 15})
	Graphics.print("Use arrow keys to look around",
		17, 34, {size = 15})
	
	myWindow:display()

	fps = fps + 1

	if os.clock() - previous_time > 1 then
		previous_time = os.clock()
		myWindow:setTitle("OpenGL 1 Person Camera! FPS: "..fps..".")
		fps = 0
	end

	System.sleep(16)
end

myWindow:close()