Class "Menu" : extends "UIControl"

local FONT_COLOR = R.color.BLACK
local FONT_COLOR_DISABLED = R.color.GRAY
local ITEM_HEIGHT_DISTANCE = 25
local SEPARATION_DISTANCE_DECREASE = 11
local INNER_COLOR = RGB(248, 248, 255)
local OUTER_COLOR = RGB(111, 111, 111)

Menu.Separator = {"-", nil}

	function Menu:constructor(options, x, y)
		self.super(x, y)
		if options then
			self:create_menu(options)
		end
	end

	----------------------------------------------------------------
	------ User Functions ------------------------------------------
	----------------------------------------------------------------

	function Menu:setPosition(x, y)
		local dx, dy = x - self.x, y - self.y
		self:move(math.floor(dx), math.floor(dy))		
	end

	function Menu:open(x, y)
		if x and y then
			self:setPosition(x, y)
		end
		self._open = true
	end
	
	function Menu:isOpen()
		return self._open
	end

	function Menu:close()

		self._open = false

		if self.submenu then
			self.submenu:close()
		end

		self.submenu = nil
		self.submenu_i = nil
		self.selected = nil
	end

	function Menu:setItemDisabled(itemIndex, bool)
		local item = self.menu[itemIndex]
		if item then
			item.disabled = bool
			if item.label then
				item.label:setColor(bool and FONT_COLOR_DISABLED or FONT_COLOR)	  
			end
			if item.accelLabel then
				item.accelLabel:setColor(bool and FONT_COLOR_DISABLED or FONT_COLOR)
			end
		end
	end

	function Menu:setItemText(itemIndex, text)
		local item = self.menu[itemIndex]
		if item then
			item.label:setString(text)
			self:updateWidth()
		end
	end

	function Menu:getItemText(itemIndex)
		local item = self.menu[itemIndex]
		if item then
			return item.label:getString()
		end
	end

	function Menu:getItemIndexByName(name)
		if not self.menu then
			return
		end
		for i = 1, #self.menu do
			if self.menu[i].label and self.menu[i].label:getString() == name then
				return i
			end
		end
	end

	function Menu:getItemByName(name)
		for i = 1, #self.menu do
			if self.menu[i].label and self.menu[i].label:getString() == name then
				return self.menu[i].menu or self.menu[i]
			end
		end
	end

	----------------------------------------------------------------
	------ Internal Functions --------------------------------------
	----------------------------------------------------------------

	function Menu:move(x, y)
		if self.body then
			self.body:translate(x, y)
			self.selectionShape:translate(x, y)
		end
		for index, it in ipairs (self.menu) do
			if it.label then it.label:move(x, y) end
			if it.accelLabel then it.accelLabel:move(x, y) end
			if it.menu then it.menu:move(x, y) end
			if it.bool_x then it.bool_x = it.bool_x + x end
			if it.bool_y then it.bool_y = it.bool_y + y end
			if it.arrow_y then it.arrow_y = it.arrow_y + y end
		end
		self.x, self.y = self.x + x, self.y + y
	end

	function Menu:create_menu(options)
		self.menu = {}
		local option_y = 0
		local biggest_label = 0
		local biggestAccelLabel = 0

		for i, it in ipairs (options) do

			local item = {}

			if it[1] == '-' and it[2] == nil then
				item.isSeparation = true
				item.x = self.x + 12
				item.y = self.y + option_y + 6
				option_y = option_y - SEPARATION_DISTANCE_DECREASE
			else

				if type(it[2]) == "function" then
					item.action = it[2]
				elseif type(it[2]) == "table" then
					item.menu = Menu(it[2], 0, self.y + option_y)
					item.arrow_y = self.y + option_y
				elseif type(it[2]) == "boolean" then
					item.bool = it[2]
					item.bool_y = self.y + option_y
					if type(it[3]) == "function" then
						item.boolaction = it[3]
					end
				end

				local accelIndex = item.bool ~= nil and 4 or 3

				if type(it[accelIndex]) == "string" then
					if AcceleratorMap.setAccelerator(it[accelIndex], function ()
						if item then
							if item.boolaction then item.bool = not item.bool item.boolaction(item.bool) 
							elseif item.action then item.action() end
						end end) then
						item.accelerator = it[accelIndex]
					end
				end

				item.label = Text(it[1], 14)
				item.label:setFont(FONT)
				item.label:setColor(0, 0, 0, 255)
				item.label:setOrigin(item.label:getBounds())
				
				local w, h = item.label:getSize()
				
				item.label:setPosition(self.x + 12, math.floor(3 + self.y + option_y + h / 2))

				if item.accelerator then
					item.accelLabel = Text(item.accelerator, 14)
					item.accelLabel:setFont(FONT)
					item.accelLabel:setColor(0, 0, 0, 255)
					item.accelLabel:setOrigin(item.accelLabel:getBounds())
					item.accelLabel:setPosition(math.floor(self.x - item.accelLabel:getWidth()) -
						7, math.floor(self.y + option_y + 3 + h / 2))
				end
			end

			table.insert(self.menu, item)
			option_y = option_y + ITEM_HEIGHT_DISTANCE
		end


		self:updateWidth()
		self.height = option_y + 1
	end

	function Menu:updateWidth()

		local biggestAccelLabel = 0
		local biggestLabel = 0
		local oldWidth = self.width or 0

		for index, item in pairs (self.menu) do

			if item.label then
				local labelWidth = item.label:getWidth()
				local accelWidth = item.accelLabel and item.accelLabel:getWidth() or 0

				if biggestAccelLabel < accelWidth then
					biggestAccelLabel = accelWidth
				end

				if biggestLabel < labelWidth then
					biggestLabel = labelWidth
				end
			end

		end

		self.width = 42 + math.ceil(biggestLabel + biggestAccelLabel)
		
		for index, item in ipairs (self.menu) do

			if item.accelLabel then
				item.accelLabel:move(math.floor(self.width - oldWidth), 0)
			end

			if item.menu then
				item.menu:move(self.x + self.width + 3, 0)
			elseif item.bool ~= nil then
				item.bool_x = math.floor(self.x + self.width - 11 - biggestAccelLabel)
			end
		end
		
		local shape = Body()
		shape:setInnerColor(83, 175, 236, 20)
		shape:setOuterColor(83, 175, 236, 255)
		shape:setBorderWidth(1)
		shape:setRoundRect(self.width - 5, 20, 4, 4, 20)
		shape:setOrigin(math.floor(-self.width/2) + 7, -10)
		self.selectionShape = shape

	end
	
	function Menu:isPointWithin(x, y)
		return x >= self.x and x <= self.x + self.width and
		   y >= self.y and y <= self.y + self.height
	end

	function Menu:getMenuFromPoint(x, y)

		if self:isPointWithin(x, y) then
			return self
		end

		if self.submenu then
			return self.submenu:getMenuFromPoint(x, y)
		end

	end

	function Menu:onKeyDown(keycode, alt, shift, ctrl)

		if self._open and keycode == Keys.Escape then
			self:close()
			return EVENT_STOP
		end
		
		return EVENT_PROPAGATE
	end

	function Menu:onMouseMove(x, y)
	
		if not self._open or
		   self.x >= x or self.x + self.width <= x or
		   self.y >= y or self.y + self.height <= y then
			if self.selected == self.submenu_i and
			   self.submenu then
				return self.submenu:onMouseMove(x, y)
			end
			self.selected = nil
			if self.submenu then
				self.submenu:close()
				self.submenu = nil
			end
			return EVENT_PROPAGATE
		end

		local old = self.selected
		y = y - self.y

		local ySum = 0
		local sepSum = 0

		if y < 0 then
			self.selected = 1
		else
			self.selected = nil
			for i = 1, #self.menu do

				ySum = ySum + ITEM_HEIGHT_DISTANCE

				if self.menu[i].isSeparation then
					sepSum = sepSum + SEPARATION_DISTANCE_DECREASE
				end

				if y <= ySum - sepSum then
					if self.menu[i].isSeparation or self.menu[i].disabled then
						self.selected = nil
					else
						self.selected = i
					end
					break
				end
			end
		end

		if self.selected and self.menu[self.selected].isSeparation then
			self.selected = nil
		elseif self.selected and old ~= self.selected then
			self.selectionShape:setPosition(self.x + 7,
				self.y + self.selected * ITEM_HEIGHT_DISTANCE - sepSum - 22)
			self.selection_time = os.clock()
			if self.submenu and self.selected ~= self.submenu_i then
				self.submenu:close()
				self.submenu = nil
			end				
		end

		return EVENT_FOCUS
	end

	function Menu:onUpdate(now, tick)
		if not self._open then return end
		
		if self.submenu then
			self.submenu:onUpdate(now, tick)
			return EVENT_PROPAGATE
		end
		
		if not self.selection_time then
			return EVENT_PROPAGATE
		end
		
		if now - self.selection_time > .65 and
		   self.selected and self.menu[self.selected].menu then
			self.submenu = self.menu[self.selected].menu
			self.submenu:open()
			self.submenu_i = self.selected
			self.selection_time = nil
		end
		
		return EVENT_PROPAGATE
	end

	function Menu:onMouseDown(button, x, y, lastclick)

		if not self._open then
			return EVENT_PROPAGATE
		end

		local menu = self:getMenuFromPoint(x, y)

		if not menu then
			self:close()
			return EVENT_PROPAGATE
		end

		if menu ~= self then
			return menu:onMouseDown(button, x, y, lastclick)
		end

		local item = self.menu[self.selected]

		if not item then
			return EVENT_FOCUS
		end

		if item.menu then
		
			self.selection_time = nil
		
			if self.submenu == item.menu then
				self.submenu:close()
				self.submenu = nil
			else
				if self.submenu then
					self.submenu:close()
				end
				self.submenu = item.menu
				self.submenu_i = self.selected
				self.submenu:open()
			end
		end

		return EVENT_FOCUS
	end

	function Menu:onMouseUp(button, x, y, drag, duration)
	
		local menu = self:getMenuFromPoint(x, y)
		
		if not menu then
			self:close()
			return EVENT_PROPAGATE
		end
		
		if menu ~= self then
			local ret = menu:onMouseUp(button, x, y, drag, duration)
			if not menu:isOpen() then
				self:close()
			end
			return ret
		end
		
		local item = self.menu[self.selected]

		if item then
			if item.action then
				item.action()
			elseif item.bool ~= nil then
				item.bool = not item.bool
				if item.boolaction then
					item.boolaction(item.bool)
				end
				return
			elseif item.menu then
				return
			end
			
			self:close()

		end
		
	end

	function Menu:onFocusOff()
		self:close()
		return EVENT_PROPAGATE
	end

	function Menu:onMouseLeft()
		self.selected = nil
		if self.submenu then
			self.submenu:close()
			self.submenu_i = nil
		end
		return EVENT_PROPAGATE
	end

	function Menu:onDraw(window)
	
		if not self._open then return end
		
		Graphics.setColor(INNER_COLOR)
		Graphics.rectangle(true, self.x, self.y, self.width, self.height)
		Graphics.setColor(OUTER_COLOR)
		Graphics.rectangle(false, self.x, self.y, self.width, self.height)
		
		if self.selected and self.selectionShape then
			self.selectionShape:draw(window)
		end
		
		for index, item in ipairs(self.menu) do

			if item.isSeparation then

				Graphics.setColor(140, 140, 149)
				Graphics.rectangle(true, item.x, item.y, self.width - 2 * (item.x - self.x), 2)

			elseif item.label then

				item.label:draw(window)

				if item.accelLabel then
					item.accelLabel:draw(window)
				end

				if item.menu then
					window:points({
					  self.x + self.width - 16, item.arrow_y + 9,
					  self.x + self.width - 16, item.arrow_y + 17,
					  self.x + self.width - 11, item.arrow_y + 13},
					  Vertex.Triangles, item.disabled and R.color.GRAY or R.color.BLACK)
				elseif item.bool ~= nil then
					if item.disabled then
						Graphics.setColor(20, 20, 20)
					else
						Graphics.setColor(0, 0, 0)
					end
					Graphics.rectangle(false, item.bool_x - 11, item.bool_y + 7, 10, 10)
					if item.bool then
						if item.disabled then
							Graphics.setColor(150, 150, 150)
							Graphics.circle(true, item.bool_x - 6, item.bool_y + 12, 4, 8)
							Graphics.setColor(50, 50, 50)
							Graphics.circle(false, item.bool_x - 6, item.bool_y + 12, 4, 8)
						else
							Graphics.setColor(0, 215, 40)
							Graphics.circle(true, item.bool_x - 6, item.bool_y + 12, 4, 8)
							Graphics.setColor(0, 0, 0)
							Graphics.circle(false, item.bool_x - 6, item.bool_y + 12, 4, 8)
						end
					end
				end

			end
		end

		if self.submenu then
			self.submenu:onDraw(window)
		end
	end