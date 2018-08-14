require "libs.classes.gui manager"

Class "Application"

function Application:constructor(width, height, title)
	self.width, self.height = width or 800, height or 600
	self.window = nil
	self.gcCounter = 0
	self.frames = 0
	self.fpsClock = 0
	self.lastSecondFrames = 0
	self.maxFps = 60
	self.waitForInputs = false
	self.lastClock = os.clock()
	self.title = title
	self.running = false
	self.onCloseOperation = nil
	self.onRunFunctions = nil
	self.frameTime = 0
	self.lastMouseX = 0
	self.lastMouseY = 0
	self.gui = GuiManager()
end

function Application:__index(a)
	local method = rawget(self, a) or rawget(Application, a)
	if method then
		return method
	end
	method = rawget(self, 'window') and rawget(self, 'window')[a]
	if method then
		return function(...)
			method(rawget(self, 'window'), select(2, ...))
		end
	end
end

function Application:setTitle(title)
	self.title = title
	if self.window then
		self.window:setTitle(title)
	end
end

function Application:setWaitForInputs(boolean)
	self.waitForInputs = boolean
end

function Application:getTitle()
	return self.title
end

function Application:getSize()
	if self.window then
		return self.window:getSize()
	end
	return self.width, self.height
end

function Application:setIcon(image)
	if self.window then
		self.window:setIcon(image)
	else
		self.icon = image
	end
end

function Application:setOnCloseOperation(func)
	self.onCloseOperation = func
end

function Application:addOnRun(func)
	if not self.onRunFunctions then
		self.onRunFunctions = {}
	end
	self.onRunFunctions[#self.onRunFunctions + 1] = func
end

function Application:addEventCallback(event, callback)
	return self.gui:addEventCallback(event, callback)
end

function Application:removeEventCallback(event, index_or_callback)
	return self.gui:removeEventCallback(event, index_or_callback)
end

function Application:getWindow()
	return self and self.window or Application.lastWindow
end

function Application:isWindowOpen()
	return self.window:isOpen()
end

function Application:getMousePosition()
	return Mouse.getPosition(self and self.window or Application.lastWindow)
end

function Application:getOSHandle()
	if self and self.window then
		return self.window:getHandle()
	else
		return Application.lastWindow:getHandle()
	end
end

function Application:asyncTask(thread, afterFunc, ...)

	if not self.threads then
		self.threads = {thread, afterFunc}
	else
		self.threads[#self.threads + 1] = thread
		self.threads[#self.threads + 1] = afterFunc
	end

	thread:launch()
end

function Application:createWindow()

	local window = self.window

	if window then
		window:close()
	end

	window = Window({
		height = self.height,
		width = self.width,
		title = self.title,
		depthbits = 24,
		stencilbits = 8
	})
	
	if self.icon then
		window:setIcon(self.icon)
	end

	self.gui:setWindow(window)
	self.window = window
end

function Application:step()

	if not self.window then
		self:createWindow()
		self:onEvent("onResize", self.window:getSize())
	else
		self.gcCounter = self.gcCounter + 1
		
		if self.gcCounter >= 120 then
			collectgarbage()
			self.gcCounter = 0
		end
	end
	
	self:processEvents()

	if self.threads then
		local tIndex = 1
		repeat
			local thread = self.threads[tIndex]
			local afterF = self.threads[tIndex + 1]
			if thread and not thread:isRunning() then
				if afterF then afterF(thread) end
				table.remove(self.threads, 1)
				table.remove(self.threads, 1)
				if #self.threads == 0 then
					self.threads = nil
					break
				end
			elseif not thread then
				break
			else
				tIndex = tIndex + 2
			end
		until false
	end

	return self
end

function Application:onEvent(...)
	self.gui:onEvent(...)
end

function Application:addControls(...)
	self.gui:addControls(...)
end

function Application:removeControls(...)
	self.gui:removeControls(...)
end

function Application:getControlByName(...)
	self.gui:getControlByName(...)
end

function Application:setAccelerator(...)
	self.gui:setAccelerator(...)
end

function Application:getAccelerator(...)
	self.gui:getAccelerator(...)
end

function Application:processEvents()

	local window = self.window

	if not window:isOpen() then
		return
	end

	local osclock = os.clock
	
	for e in window:getEvents(self.frameTime > 0 and self.waitForInputs) do
		self.gui:processEvent(e)
		if e.Resized then
			local view = window:getView()
			local w, h = window:getSize()
			view:setSize(w, h)
			view:setCenter(w / 2, h / 2)
			window:setView(view)
			self.width, self.height = e.width, e.height
		elseif e.Closed then
			local onClose = self.onCloseOperation
			if onClose then
				if onClose() == true then
					self:close()
					self.onCloseOperation = nil
				end
			else
				self:close()
			end
		end
	end

	local timeNow = osclock()
	local timeDelta = timeNow - self.lastClock
	self:onEvent("onUpdate", timeNow, timeDelta)
	self.lastClock = osclock()

	window:clear(0xFFFFFFFF)
	self:onEvent("onDraw", window)
	window:display()
	
	self.frames = self.frames + 1
	
	if timeNow - self.fpsClock >= 1 then
		self.fpsClock = timeNow
		self.lastSecondFrames = self.frames
		self.frames = 0
	end

	self.frameTime = osclock() - self.frameTime
	if self.frameTime < 1/self.maxFps then
		System.sleep((1/self.maxFps - self.frameTime) * 1000)
	end
	self.frameTime = osclock()

end

function Application:run()

	self:step()
	
	for index, func in ipairs (self.onRunFunctions or {}) do
		func()
	end

	self.running = true

	repeat
		self:step()
	until not self.window:isOpen()
end

function Application:close()
	self.window:close()
end