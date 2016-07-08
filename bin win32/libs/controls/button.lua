require 'libs.controls.label'

Class "Button" : extends "Label"

local BTN_BACKGROUND_COLOR = RGB(75, 155, 255)
local BTN_BACKGROUND_HOVER_COLOR = RGB(200, 220, 255)
local BTN_BACKGROUND_DOWN_COLOR = RGB(225, 225, 225)
local BTN_BORDER_COLOR = 0xFF00000A
local BTN_TEXT_ENABLED_COLOR = 0xFF000000
local BTN_TEXT_DISABLED_COLOR = 0xFF737373
local BTN_DISABLED_BG_COLOR = 0xFFE6E6E6

	function Button:constructor(label, fontSize, font)
		self.super(label, fontSize, font)
		self:setTextColor(BTN_TEXT_ENABLED_COLOR)
		self.marginX, self.marginY = 7, 6
		self.hover, self.click = false, false
		self.clickCallback = nil
		self.backgroundColor = nil
		self.disabled = nil
		self.standbyColor = BTN_BACKGROUND_COLOR
		self.hoverColor = BTN_BACKGROUND_HOVER_COLOR
		self.clickColor = BTN_BACKGROUND_DOWN_COLOR
		self.borderColor = BTN_BORDER_COLOR
		self:updateColor()
	end

	----------------------------------------------------------------
	------ User Functions ------------------------------------------
	----------------------------------------------------------------
	
	function Button:getWidth()
		return self.width + 2 * self.marginX
	end
	
	function Button:getHeight()
		return self.height + 2 * self.marginY
	end
	
	function Button:getSize()
		return self.width + 2 * self.marginX, self.height + 2 * self.marginY
	end
	
	function Button:setPosition(x, y)
		x = x + self.marginX
		y = y + self.marginY
		self.super:setPosition(x, y)
	end
	
	function Button:setOnClick(callback)
		self.clickCallback = callback
	end
	
	function Button:onClick()
		if not self.disabled and self.clickCallback then
			self.clickCallback(self)
		end
	end
	
	function Button:setEnabled(bool)
		if bool then
			self.disabled = nil
			self:setTextColor(BTN_TEXT_ENABLED_COLOR)
		else
			self.disabled = true
			self:setTextColor(BTN_TEXT_DISABLED_COLOR)
		end
	end
	
	function Button:setColors(normal, hover, click, border)
		self.standbyColor = normal or BTN_BACKGROUND_COLOR
		self.hoverColor = hover or BTN_BACKGROUND_HOVER_COLOR
		self.clickColor = click or BTN_BACKGROUND_DOWN_COLOR
		self.borderColor = border or BTN_BORDER_COLOR
		self:updateColor()
	end
	
	----------------------------------------------------------------
	------ Internal Functions --------------------------------------
	----------------------------------------------------------------
	
	function Button:isPointWithin(x, y)
		return x >= self.x - self.marginX and x <= self.x + self.width + self.marginX and
			y >= self.y - self.marginY and y <= self.y + self.height + self.marginY
	end

	function Button:updateColor()
		self.backgroundColor = self.click and self.clickColor or
			self.hover and self.hoverColor or self.standbyColor
	end
	
	function Button:setHover(bool)
		self.hover = bool
		self:updateColor()
	end
	
	function Button:onMouseIn()
		self:setHover(true)
	end
	
	function Button:onMouseOut()
		self:setHover(false)
	end
	
	function Button:onMouseLeft()
		self:setHover(false)
	end
	
	function Button:onMouseMove(x, y)
	
		if self:isPointWithin(x, y) then
			return EVENT_STOP
		end
		
		return EVENT_PROPAGATE
	end

	function Button:onMouseDown(button, x, y)

		if not self:isPointWithin(x, y) then
			return EVENT_PROPAGATE
		end
		
		if button == Mouse.Left then
			self.click = true
			self:updateColor()
		end

		return EVENT_FOCUS
	end
	
	function Button:onKeyDown(keycode, alt, shift, control)
	
		if keycode == Keys.Return then
			self:onClick()
			return EVENT_STOP
		end

		return EVENT_PROPAGATE
	end

	function Button:onMouseUp(button, x, y, drag_distance, click_duration)
	
		if button == Mouse.Left then
			if self.click then
				self.click = false
				self:updateColor()
				if not self:isPointWithin(x, y) then
					return EVENT_STOP
				end
				self:onClick()
				return EVENT_FOCUS
			end
		end
	
		if self:isPointWithin(x, y) then
			return EVENT_FOCUS
		end
		
		return EVENT_PROPAGATE
	end

	function Button:MouseLeft()
		self:setHover(false)
		return EVENT_PROPAGATE
	end

	function Button:onDraw(window)
		local setColor, rect = Graphics.setColor, Graphics.rectangle
		local disabled = self.disabled
		local x, y = self.x + 1, self.y + 1
		local w, h = self.width - 2, self.height - 2
		local mx, my = self.marginX, self.marginY
		setColor(disabled and BTN_DISABLED_BG_COLOR or self.backgroundColor)
		rect(true, x - mx, y - my, w + 2 * mx, h + 2 * my)
		setColor(disabled and BTN_DISABLED_TEXT_COLOR or self.borderColor)
		rect(false)
		self.super:onDraw(window)
	end