Class "Frame" : extends "UIControl"

local TOPBAR_COLOR, BORDER_COLOR = RGBA(110, 163, 240), RGBA(100, 115, 140)
local TEXT_COLOR = 0xFFFFFFFF
local INNER_COLOR = RGBA(236, 236, 248)
local OUTER_COLOR = RGBA(125, 125, 125)

local TITLE_BAR_HEIGHT  = 26

	function Frame:constructor(width, height, title)
		self.super(0, 0, width, height)
		self.hasBorder = true
		self.mouseMoveFocus = nil
		self.componentFocus = nil
		self.oldComponentFocus = nil
		self.titleLabel = nil
		self.innerColor = INNER_COLOR
		self.outerColor = OUTER_COLOR
		self.borderWidth = 1
		self:setTitle(title)
		self.hideTopBar = nil
		self.alwaysOnTop = false
	end

	----------------------------------------------------------------
	------ User Functions ------------------------------------------
	----------------------------------------------------------------

	function Frame:setColor(...)
		self.innerColor = RGBA(...)
	end

	function Frame:setBorder(color, thickness)
		color = RGBA(color) or 0
		self.outerColor = color
		self.borderWidth = math.floor(thickness or 1)
		self.hasBorder = bit32.band(color, 0xFF000000) > 0 and self.borderWidth > 0
	end

	function Frame:setAlwaysOnTop(bool)
		if bool then
			self.alwaysOnTop = true
		else
			self.alwaysOnTop = nil
		end
	end
	
	function Frame:setTopBarVisible(bool)
		self.hideTopBar = not bool
	end

	function Frame:isTopBarVisible()
		return not self.hideTopBar
	end

	function Frame:setTitle(title, size)

		if not self.titleLabel and not title then
			return
		elseif not title then
			self.titleLabel = nil
		elseif not self.titleLabel then
			self.titleLabel = Text(title, size or 16)
			self.titleLabel:setColor(TEXT_COLOR)
			self.titleLabel:setPosition(self.x + 6, math.floor(self.y + TITLE_BAR_HEIGHT / 4) + 2)
			self.titleLabel:setStyle(Styles.Bold)
			local shx, shy = self.titleLabel:getBounds()
			self.titleLabel:setOrigin(shx, shy)
		else
			self.titleLabel:setString(title)
			if size then
				self.titleLabel:setCharacterSize(size)
			end
		end
	end

	function Frame:setPosition(x, y, center)
		if center then
			x = x - self.width / 2
			y = y - self.height / 2
		end
		x, y = math.floor(x), math.floor(y)
		local deltaX, deltaY = x - self.x, y - self.y
		self.x, self.y = x, y

		if self.titleLabel then
			self.titleLabel:move(deltaX, deltaY)
		end
	end

	function Frame:getPosition()
		return self.x, self.y
	end

	function Frame:setSize(width, height)
		self.width, self.height = math.floor(width), math.floor(height)
	end
	
	function Frame:getWidth()
		return self.width
	end

	function Frame:getHeight(excludeTitle)
		return self.height - (excludeTitle and self.titleLabel and TITLE_BAR_HEIGHT or 0) -
			self.borderWidth * 2
	end

	function Frame:setActive(bool)
		self.active = bool
	end

	function Frame:setDraggable(bool)
		self.draggable = bool
	end

	function Frame:addComponent(obj, x, y)

		if not self.components then
			self.components = {}
		end

		self.components[#self.components + 1] = obj
		obj.parent = self

		x, y = x or 0, y or 0

		if not self.hideTopBar then
			y = y + 0 * TITLE_BAR_HEIGHT
		end

		if x and y then
			obj:setPosition(x, y)
		end

	end
	
	----------------------------------------------------------------
	------ Internal Functions --------------------------------------
	----------------------------------------------------------------

	function Frame:isPointWithin(x, y)
		return x >= self.x and x <= self.x + self.width and
		   y >= self.y and y <= self.y + self.height
	end
	
	function Frame:isPointWithinFrameTitle(x, y) 
		return not self.hideTopBar and x >= self.x and x <= self.x + self.width and
		   y >= self.y + TITLE_BAR_HEIGHT and y <= self.y + self.height
	end
	
	function Frame:focusChange(newFocus, oldFocus)
		if newFocus == oldFocus then return end
		local event = oldFocus and oldFocus.onLoseFocus
		self.oldComponentFocus = oldFocus
		self.componentFocus = newFocus
		if event then event(oldFocus) end
		event = newFocus and newFocus.onGainFocus
		if event then event(newFocus) end
	end
	
	function Frame:mouseFocus(newFocus, oldFocus)
		if newFocus == oldFocus then return end
		local event = oldFocus and oldFocus.onMouseOut
		self.mouseMoveFocus = newFocus
		if event then event(oldFocus) end
		event = newFocus and newFocus.onMouseIn
		if event then event(newFocus) end
	end

	function Frame:onMouseDown(button, x, y, lastclick)

		local oldFocus = self.componentFocus
		local borderWidth = self.borderWidth
	
		for index, obj in ipairs (self.components or {}) do
			if obj.onMouseDown then 
				local ret = obj:onMouseDown(button, x - self.x - borderWidth,
					y - self.y - TITLE_BAR_HEIGHT - borderWidth, lastclick)
				if ret ~= nil and ret ~= EVENT_PROPAGATE then
					if ret == EVENT_FOCUS and oldFocus ~= obj then
						self:focusChange(obj, oldFocus)
					end
					return ret
				end
			end
		end
	
		if not self:isPointWithin(x, y) then
			return EVENT_PROPAGATE
		end
		
		if not self:isPointWithinFrameTitle(x, y) then
			if self.draggable and button == Mouse.Left and
			  y <= self.y + TITLE_BAR_HEIGHT then
				self.drag = {math.floor(x) - self.x, math.floor(y) - self.y}
			end
		end

		return EVENT_FOCUS
	end

	function Frame:onMouseUp(button, x, y, drag, duration)

		local oldFocus = self.componentFocus
		local borderWidth = self.borderWidth
	
		for index, obj in ipairs (self.components or {}) do
			if obj.onMouseUp then
				local ret = obj:onMouseUp(button, x - self.x - borderWidth,
					y - self.y - TITLE_BAR_HEIGHT - borderWidth, drag, duration)
				if ret ~= nil and ret ~= EVENT_PROPAGATE then
					if ret == EVENT_FOCUS and oldFocus ~= obj then
						self:focusChange(obj, oldFocus)
					end
					return ret
				end
			end
		end
	
		if self.drag then
			self.drag = nil
			return EVENT_NOEVENT
		end

		if not self:isPointWithin(x, y) then
			return EVENT_PROPAGATE
		end

	end

	function Frame:onMouseMove(x, y)

		if self.drag then

			local newX, newY = self.x, self.y

			if x < 0 then
				newX = 1
			elseif self.parent and self.parent.width and self.parent.width < x then
				newX = self.parent.width - 1
			else
				newX  = math.floor(x)
			end

			if y < 0 then
				newY = 1
			elseif self.parent and self.parent.height and self.parent.height < y then
				newY = self.parent.height - 1
			else
				newY  = math.floor(y)
			end

			self:setPosition(newX - self.drag[1], newY - self.drag[2])
			return EVENT_STOP
			
		end
		
		local borderWidth = self.borderWidth

		for index, obj in ipairs (self.components or {}) do
			if obj.onMouseMove then
				local ret = obj:onMouseMove(x - self.x - borderWidth,
					y - self.y - TITLE_BAR_HEIGHT - borderWidth)
				if ret ~= EVENT_PROPAGATE then
					self:mouseFocus(obj, self.mouseMoveFocus)
					return ret
				end
			end
		end
		
		self:mouseFocus(nil, self.mouseMoveFocus)
		
		if not self:isPointWithin(x, y) then
			return EVENT_PROPAGATE
		end

		return EVENT_STOP
	end
	
	function Frame:onMouseWheel(delta, x, y)
	
		if not self:isPointWithin(x, y) then
			return EVENT_PROPAGATE
		end
	
		local obj = self.mouseMoveFocus
		local borderWidth = self.borderWidth
		
		if obj and obj.onMouseWheel then
			obj:onMouseWheel(delta, x - self.x - borderWidth,
				y - self.y - TITLE_BAR_HEIGHT - borderWidth)
		end

		return EVENT_PROPAGATE
	end

	function Frame:onTextEntered(unicode)
	
		local focus = self.componentFocus
		local event = focus and focus.onTextEntered
		if event then
			event = event(focus, unicode)
			if event ~= EVENT_PROPAGATE then
				return event
			end
		end
		
		return EVENT_PROPAGATE
	end
	
	function Frame:onKeyDown(keycode, alt, shift, control)
	
		local focus = self.componentFocus
		local event = focus and focus.onKeyDown
		if event then
			event = event(focus, keycode, alt, shift, control)
			if event ~= EVENT_PROPAGATE then
				return event
			end
		end
	
		--[[for index, obj in ipairs (self.components or {}) do
			if obj.onKeyDown then
				local ret = obj:onKeyDown(keycode)
				if ret ~= nil and ret ~= EVENT_PROPAGATE then
					if ret == EVENT_FOCUS and oldFocus ~= obj then
						self:focusChange(obj, oldFocus)
					end
					return ret
				end
			end
		end
		return EVENT_FOCUS]]
		
		return EVENT_PROPAGATE
	end

	function Frame:onGainFocus()
		self:focusChange(self.oldComponentFocus, nil)
	end
	
	function Frame:onLoseFocus()
		self:focusChange(nil, self.componentFocus)
	end
	
	function Frame:onFocusOn()
		for index, obj in ipairs (self.components or {}) do
			if obj.onFocusOn then obj:onFocusOn() end
		end
	end
	
	function Frame:onFocusOff()
		for index, obj in ipairs (self.components or {}) do
			if obj.onFocusOff then obj:onFocusOff() end
		end
	end
	
	function Frame:onMouseLeft()
		for index, obj in ipairs (self.components or {}) do
			if obj.onMouseLeft then obj:onMouseLeft() end
		end
		self.mouseMoveFocus = nil
	end
	
	function Frame:onMouseIn()
	end
	
	function Frame:onMouseOut()
		self:mouseFocus(nil)
	end

	function Frame:onUpdate(time, delta)
	
		for index, obj in ipairs (self.components or {}) do
			if obj.onUpdate then obj:onUpdate(time, delta) end
		end
		
		return self.alwaysOnTop and EVENT_FOCUS
	end
	
	function Frame:onDraw(window)

		local setColor = Graphics.setColor
		local rectangle = Graphics.rectangle
		local line = Graphics.line
		local x, y, width, height = self.x, self.y, self.width, self.height
		local borderWidth = self.borderWidth
		local titleLabel = self.titleLabel
		local titleHeight = not self.hideTopBar and titleLabel and TITLE_BAR_HEIGHT or 0

		if borderWidth > 1 then
			setColor(self.outerColor)
			rectangle(true, x + 1, y + 1, width - 2, height - 2)
			x = x + borderWidth
			y = y + borderWidth
			width = width - 2 * borderWidth
			height = height - 2 * borderWidth
		end
		
		setColor(self.innerColor)
		rectangle(true, x + 1, y + 1, width - 2, height - 2)
		
		local components = self.components
		
		if components then

			setScissor(x + borderWidth, y + titleHeight + borderWidth,
				width - 2 * borderWidth, height - titleHeight - 2 * borderWidth)
			
			window:translateView(-x - borderWidth, - y - titleHeight - borderWidth)

			for index, obj in ipairs (components) do
				local event = obj.visible and obj.onDraw
				if event then event(obj, window) end
			end

			window:translateView(x + borderWidth, y + titleHeight + borderWidth)
				
			setScissor()
		end
		
		if titleHeight > 0 then
			setColor(TOPBAR_COLOR)
			rectangle(true, x, y, width, titleHeight)
			setColor(BORDER_COLOR)
			line(x, y + borderWidth + titleHeight, x + width, -1 + y + borderWidth + titleHeight)
		end
		
		if titleHeight > 0 then
			titleLabel:draw(window)
		end
		
		if borderWidth == 1 then
			setColor(self.outerColor)
			rectangle(false, x + 1, y + 1, width - 2, height - 2)
		end
		
	end




