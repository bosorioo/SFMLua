Class "OptionsBar" : extends "UIControl"

local FONT = R.getFont(1)
local CLICKED_OUTER_COLOR = RGBA(0, 0, 0, 175)
local CLICKED_INNER_COLOR = RGBA(0, 0, 0, 33)
local SELECTED_OUTER_COLOR = RGBA(0, 0, 0, 75)
local SELECTED_INNER_COLOR = RGBA(0, 0, 0, 4)

	function OptionsBar:constructor()
		self.super(5, 5, 0, 22)
		self.selection = Body()
		self.selection:setInnerColor(0, 0, 0, 4)
		self.selection:setOuterColor(0, 0, 0, 75)
		self.selection:setBorderWidth(1)
		self.option_spacing = 10
		self.items = {}
		self.body = Body()
		self.body:setRoundRect(self.width, self.height, 4, 4, 20)
		self.body:setBorderWidth(1)
		self.body:setInnerColor(235, 235, 242)
		self.body:setOuterColor(120, 120, 129)
		self.body:setOrigin(-self.width/2, -10)
		self.body:setPosition(4, 4)
	end

	----------------------------------------------------------------
	------ User Functions ------------------------------------------
	----------------------------------------------------------------

	function OptionsBar:addOption(label, action)

		local T = Text(label or " ", 15)
		T:setFont(FONT)
		T:setColor(0, 0, 0, 255)
		local w, h = T:getSize()
		local x, y = self.x + self:get_next_position(), self.y + (self.height - h) / 2
		T:setOrigin(T:getBounds())
		T:setPosition(math.floor(x), math.floor(y))

		local item = {label = T, x = x, y = y, width = w, height = h}

		if type(action) == "table" then
			item.menu = Menu(action, item.x - 5, item.y + 20)
			item.menu.parent = self
		else
			item.action = action
		end

		table.insert(self.items, item)
		self:increase_width(item.width + self.option_spacing * 2)

		return item.menu or item
	end

	function OptionsBar:getOptionIndex(name)
		local current, index
		for entry in name:gmatch('[^%.]+') do
			if current and current.getItemByName then
				index = current:getItemIndexByName(entry)
				current = current:getItemByName(entry)
			else
				for index, option in pairs (self.items) do
					if option.label:getString() == entry then
						current = option.menu or option
						break
					end
				end
				if not current then return end
			end
		end
		return index
	end

	function OptionsBar:getOption(index)
		if tonumber(index) then
			return self.items[index] and self.items[index].menu or self.items[index]
		else
			local current
			for entry in index:gmatch('[^%.]+') do
				if current and current.getItemByName then
					current = current:getItemByName(entry)
				else
					for index, option in pairs (self.items) do
						if option.label:getString() == entry then
							current = option.menu or option
						end
					end
					if not current then return end
				end
			end
			return current
		end
	end
	
	function OptionsBar:getWidth()
		return self.width
	end
	
	function OptionsBar:getHeight()
		return self.height
	end

	function OptionsBar:getPosition()
		return self.x, self.y
	end
	
	----------------------------------------------------------------
	------ Internal Functions --------------------------------------
	----------------------------------------------------------------

	function OptionsBar:increase_width(value)
		self.width = self.width + value
		self.body:setRoundRect(self.width, self.height, 4, 4, 20)
		self.body:setOrigin(-self.width/2, -self.height/2)
	end

	function OptionsBar:get_next_position()
		local pos = 8
		local item = self.items[#self.items]
		if item then
			pos = pos + item.x + item.width + self.option_spacing
		end
		return pos
	end

	function OptionsBar:select(index)
		local item = self.items[index]
		if not item then self.selected = nil return end
		local body = self.selection
		body:setRoundRect(item.width + 8, 16, 3, 3, 20)
		body:setPosition(item.x + math.ceil(item.width/2), item.y + 6)
		body:setInnerColor(self.clicked and CLICKED_INNER_COLOR or SELECTED_INNER_COLOR)
		body:setOuterColor(self.clicked and CLICKED_OUTER_COLOR or SELECTED_OUTER_COLOR)
		self.selected = index
	end

	function OptionsBar:onTextEntered()
		return self.menu and EVENT_STOP
	end
	
	function OptionsBar:onMouseMove(x, y)

		local menu = self.items[self.selected] and self.items[self.selected].menu
		local selection = self.selected

		if x >= self.x and x < self.x + self.width and y >= self.y and y < self.y + self.height then
		
			for i = 1, #self.items do
			
				local item = self.items[i]
				
				if item.x <= x and x <= item.x + item.width and
				   item.y <= y and y <= item.y + item.height then
				   
					if i == selection then
						return EVENT_STOP
					end
					
					if item.menu or not self.clicked then
					
						self:select(i)
					   
						if menu and menu:isOpen() then
							menu:close()
						end
						
						self:select(i)
						
						if self.clicked and item.menu then
							self.menu = item.menu
							self.menu:open()
						end
						
					end
					
					break
				end
			end
			return EVENT_STOP
		end

		if menu and self.clicked then
			return menu:onMouseMove(x, y)
		end
		
		self:select()
		return EVENT_PROPAGATE
	end

	function OptionsBar:onMouseDown(button, x, y)
		
		if self.clicked then
		
			local item = self.items[self.selected]
			
			if item.menu then
			
				local ret = item.menu:onMouseDown(button, x, y)
				
				if ret == EVENT_PROPAGATE then
					self.clicked = nil
					self.selected = nil
					self:onMouseMove(x, y)
					return ret
				end

			end
			
			return EVENT_FOCUS
		end
		
		if self.x > x or self.x + self.width < x or
		   self.y > y or self.y + self.height < y then
			return EVENT_PROPAGATE
		end
		
		if button ~= Mouse.Left then
			return EVENT_STOP
		end
		
		if not self.selected then
			self.clicked = nil
			self.selection:setInnerColor(SELECTED_INNER_COLOR)
			self.selection:setOuterColor(SELECTED_OUTER_COLOR)
			return EVENT_PROPAGATE
		end
		
		self.clicked = self.selected
		
		if self.items[self.clicked].menu then
			self.menu = self.items[self.clicked].menu
			self.menu:open()
		end
		
		self.just_clicked = true
		self.selection:setInnerColor(CLICKED_INNER_COLOR)
		self.selection:setOuterColor(CLICKED_OUTER_COLOR)
		return EVENT_FOCUS
	end
	
	function OptionsBar:onKeyDown(keycode, alt, shift, control)
	
		if self.menu and self.menu:isOpen() then
		
			if keycode == Keys.Escape then
				self.menu:close()
				self.menu = nil
				self.clicked = nil
				self:select(self.selected)
			end
			
			return EVENT_STOP
		end

		return EVENT_PROPAGATE
	end
	
	function OptionsBar:onKeyUp(keycode)
		return EVENT_PROPAGATE
	end

	function OptionsBar:onMouseUp(button, x, y, drag_distance, click_duration)
	
		if button ~= Mouse.Left then
			return EVENT_STOP
		end
		
		if self.just_clicked then
		
			self.just_clicked = nil
			
			local item = self.items[self.selected]
			
			if item and item.action and self.selected == self.clicked then
				self.selection:setInnerColor(0, 0, 0, 4)
				self.selection:setOuterColor(0, 0, 0, 75)
				self.clicked = nil
				self.menu = nil
				item.action()
			elseif not self.selected then
				self.selection:setInnerColor(0, 0, 0, 4)
				self.selection:setOuterColor(0, 0, 0, 75)
				self.clicked = nil
				self.menu = nil
			else
				self.clicked = self.selected
				if self.menu and self.menu.selected then
					self.menu:onMouseUp(button, x, y,
					  drag_distance, click_duration)
					if not self._open then
						self.menu = nil
						self.clicked = nil
						self.selection:setInnerColor(0, 0, 0, 4)
						self.selection:setOuterColor(0, 0, 0, 75)
					end
				end
			end
			
			return self:onMouseMove(x, y)
		end
		
		if self.menu then
			self.menu:onMouseUp(button, x, y, drag_distance, click_duration)
			if self.menu:isOpen() then
				return
			end
		end
		self.clicked = nil
		self.menu = nil
		self.selection:setInnerColor(0, 0, 0, 4)
		self.selection:setOuterColor(0, 0, 0, 75)
		self:onMouseMove(x, y)
	end

	function OptionsBar:onUpdate(now, tick)
		if self.menu then
			self.menu:onUpdate(now, tick)
		end
		return EVENT_PROPAGATE
	end
	
	function OptionsBar:onMouseLeft()
		if not self.clicked then
			self.selected = nil
		elseif self.menu then
			self.menu:onMouseLeft()
		end
	end

	function OptionsBar:onFocusOff()
		if self.menu then
			self.menu:onFocusOff()
		end
		self.selected = nil
		self.clicked = nil
		self.menu = nil
		return EVENT_PROPAGATE
	end
	
	function OptionsBar:onMouseWheel(delta, x, y)
	
		if self.menu and self.menu:isOpen() then
			return EVENT_STOP
		end
		
		return EVENT_PROPAGATE
	end

	function OptionsBar:onDraw(window)

		self.body:draw(window)

		if self.selected or self.clicked then
			self.selection:draw(window)
			if self.menu then
				self.menu:onDraw(window)
			end
		end

		for index, item in ipairs (self.items) do
			item.label:draw(window)
		end
	end