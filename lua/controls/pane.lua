Class "Pane" : extends "UIControl"

	function Pane:constructor(width, height)
		self.super(0, 0, width, height)
		self.borderThickness = 1
		self.borderColor = RGBA(75, 128, 255)
		self.callbacks = {}
		self.mouseInside = false
	end

	----------------------------------------------------------------
	------ User Functions ------------------------------------------
	----------------------------------------------------------------

	function Pane:setOnDraw(func)
		self.callbacks.onDraw = func
	end

	function Pane:setCallback(callback, func)
		self.callbacks[callback] = func
	end

	function Pane:setBorderThickness(thick)
		self.borderThickness = thick
	end

	function Pane:setBorderColor(color)
		self.borderColor = color
	end

	function Pane:setPosition(x, y, center)
		if center then
			x = x - self.width / 2
			y = y - self.height / 2
		end
		self.x, self.y = x, y
	end

	function Pane:getPosition()
		return self.x, self.y
	end
	
	function Pane:setSize(width, height)
		self.width, self.height = width, height
	end

	function Pane:getSize()
		return self.width, self.height
	end
	
	function Pane:getWidth()
		return self.width
	end

	function Pane:getHeight()
		return self.height
	end

	----------------------------------------------------------------
	------ Internal Functions --------------------------------------
	----------------------------------------------------------------
	
	function Pane:isPointWithin(x, y)
		return not (x < self.x or x > self.x + self.width or
		   y < self.y or y > self.y + self.height)
	end

	function Pane:onMouseDown(button, x, y, lastclick)

		if not self:isPointWithin(x, y) then
			return EVENT_PROPAGATE
		end
		
		if self.callbacks.onMouseDown then
			if self.callbacks.onMouseDown(button, x - self.x, y - self.y, lastclick) == EVENT_PROPAGATE then
				return EVENT_PROPAGATE
			end
		end

		return EVENT_FOCUS
	end

	function Pane:onMouseUp(button, x, y, drag, duration)
		if self.callbacks.onMouseUp then
			self.callbacks.onMouseUp(button, x - self.x, y - self.y, drag, duration)
		end
	end

	function Pane:onMouseLeft()
		if self.callbacks.onMouseLeft then
			self.callbacks.onMouseLeft()
		end
	end
	
	function Pane:onMouseOut()
		if self.callbacks.onMouseOut then
			self.callbacks.onMouseOut()
		end
	end
	
	function Pane:onMouseIn()
		if self.callbacks.onMouseIn then
			self.callbacks.onMouseIn()
		end
	end
	
	function Pane:onMouseMove(x, y)
		
		if not self:isPointWithin(x, y) then
			return EVENT_PROPAGATE
		end
		
		if self.callbacks.onMouseMove then
			self.callbacks.onMouseMove(x - self.x, y - self.y)
		end
		
		return EVENT_STOP
	end
	
	function Pane:onMouseWheel(delta, x, y)
	
		if not self:isPointWithin(x, y) then
			return EVENT_PROPAGATE
		end

		if self.callbacks.onMouseWheel then
			return self.callbacks.onMouseWheel(delta, x - self.x, y - self.y)
		end

		return EVENT_PROPAGATE

	end

	function Pane:onTextEntered(unicode)
		if self.callbacks.onTextEntered then
			return self.callbacks.onTextEntered(unicode)
		end
		return EVENT_PROPAGATE
	end
	
	function Pane:onKeyDown(keycode)
		if self.callbacks.onKeyDown then
			return self.callbacks.onKeyDown(keycode)
		end
		return EVENT_PROPAGATE
	end
	
	function Pane:onUpdate(now, delta)
		if self.callbacks.onUpdate then
			return self.callbacks.onUpdate(now, delta)
		end
	end

	function Pane:onDraw(window)

		Graphics.setColor(self.borderColor)
		Graphics.setOutlineWidth(self.borderThickness)
		Graphics.rectangle(false, self.x, self.y, self.width, self.height)
		
		if self.callbacks.onDraw then
			setScissor(self.x, self.y, self.width, self.height)
		
			local view = window:getView()
			local vw, vh = view:getSize()

			if view then
				view:move(-self.x, -self.y)
				--view:setSize(self.width, self.height)
				window:setView(view)
			end
		
			self.callbacks.onDraw(window, self.width, self.height)
		
			if view then
				view:move(self.x, self.y)
				--view:setSize(vw, vh)
				window:setView(view)
			end

			setScissor()
		end
	end




