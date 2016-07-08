Class "TextInput" : extends "UIControl"

local HINT_TEXT_COLOR = 0xFF99a8c4

	function TextInput:constructor(width, height, text, size, font)
		self.super(0, 0, tonumber(width) or 120, tonumber(height) or 24)
		self.position = 0
		self.track = Text("|", size or 14)
		self.track:setColor(0, 0, 0)
		local shx, shy, w, h = self.track:getBounds()
		self.track:setOrigin(shx, shy)
		self.text = Text(text or '', size or 14)
		self.text:setPosition(4, math.ceil((self.height - shy - h) / 2))
		self.track:setPosition(4, math.ceil((self.height - h) / 2))
		if font then
			self.track:setFont(font)
			self.text:setFont(font)
		end
		self.text:setColor(0, 0, 0)
		self.fillColor = 0xFFFFFFFF
		self.borderColor = 0xFF191919
		self.keySpecificCallback = nil
		self.textChangedCallback = nil
		self.hint = nil
	end

	----------------------------------------------------------------
	------ User Functions ------------------------------------------
	----------------------------------------------------------------

	function TextInput:setHint(text)
		self.hint = text
		self:checkText()
	end
	
	function TextInput:setText(text, blockOnTextChange)
	
		if not text or #text == 0 then
			self:advanceTrack(-self.position)
			if self.hint then
				self:setHintOn(true)
			else
				self.text:setString('')
			end
		else
			if self.hintOn then
				self:setHintOn(false)
			end
			self.text:setString(text)
		end
		
		self:checkText()
		
		if not blockOnTextChange and self.textChangedCallback then
			self.textChangedCallback(self, text)
		end

	end

	function TextInput:getText(text)
		return self.hintOn and "" or self.text:getString()
	end

	function TextInput:setLengthLimit(limit)
		self.lengthLimit = limit
	end

	function TextInput:setOnlyNumbers(bool)
		self.onlynumbers = bool
		self:checkText()
	end

	function TextInput:setPosition(x, y)
		x, y = math.floor(x), math.floor(y)
		local dx, dy = x - self.x, y - self.y
		self.x, self.y = x, y
		self.text:move(dx, dy)
		self.track:move(dx, dy)
	end

	function TextInput:setKeyCallback(key, func)
		if type(key) == "string" then
			key = string.byte(key)
		end
		if not self.callbacks then self.callbacks = {} end
		self.callbacks[key] = func
	end
	
	function TextInput:setOnTextChanged(callback)
		self.textChangedCallback = callback
	end

	----------------------------------------------------------------
	------ Internal Functions --------------------------------------
	----------------------------------------------------------------

	function TextInput:checkText()
	
		local str = self:getText()
		local size = #str
	
		if self.onlynumbers and not tonumber(str) then
			self:setText("")
			return
		end
		
		if self.hintOn then
			return
		end
		
		if size == 0 then
			self:setHintOn(true)
		elseif self.lengthLimit and size > self.lengthLimit then
			self:setText((str:sub(1, self.lengthLimit)))
		end
	
	end
	
	function TextInput:setHintOn(bool)
		
		if self.hintOn and bool or not self.hint then
			return
		end
		
		if bool then
			self.hintOn = true
			self.text:setString(self.hint)
			self.text:setColor(HINT_TEXT_COLOR)
		else
			self.hintOn = nil
			self.text:setColor(0xFF000000)
		end
	end

	function TextInput:addChar(c, p)
		
		if self.onlynumbers and not tonumber(c) then
			return
		end
		
		local text = self:getText()
		
		if self.lengthLimit and self.lengthLimit <= #text then
			return
		end
		
		if p then
			text = text:sub(1, p) .. c .. text:sub(p + 1)
		else
			text = text .. c
		end
		
		self:setText(text)
		self:advanceTrack(1)
	end

	function TextInput:removeChar(p)
		
		if self.hintOn then
			return
		end
		
		local text = self:getText()

		if p then
			text = text:sub(1, self.position - 1) .. text:sub(self.position + 1)
		else
			text = text:sub(1, #text - 1)
		end

		self:setText(text)
		self:advanceTrack(-1)
	end

	function TextInput:advanceTrack(p)
		self.position = self.position + p
		self.position = math.max(0, self.position)
		self.position = math.min(#self.text:getString(), self.position)
		local x, _ = self.text:getCharPosition(self.position)
		local _, y = self.track:getPosition()
		self.track:setPosition(x, y)
	end

	function TextInput:onMouseDown(button, x, y)
	
		if button ~= Mouse.Left then return end
		
		if x < self.x or x > self.x + self.width or
		   y < self.y or y > self.y + self.height then
			return EVENT_PROPAGATE
		end
		
		if not self.trackTick then
			self.trackTick = os.clock()
		end
		
		return EVENT_FOCUS
	end

	function TextInput:onTextEntered(code)

		self.trackTick = os.clock()
		
		if self.callbacks and self.callbacks[code] then
			local ret = self.callbacks[code](self)
			if ret then return EVENT_FOCUS end
		end
		
		if code == 13 then
			-- enter
		elseif code == 27 then
			-- esc
		elseif code == 8 then
			self:removeChar(true)
		elseif code < 32 then
			-- non printable
		else
			self:addChar(string.char(code) , self.position)
		end

		return EVENT_FOCUS
	end
	
	function TextInput:onKeyDown(keycode, alt, shift, control)
		return EVENT_PROPAGATE
	end

	function TextInput:onLoseFocus()
		self.trackTick = nil
	end
	
	function TextInput:onGainFocus()
		self.trackTick = os.clock()
	end

	function TextInput:onDraw(window)
	
		Graphics.setColor(self.fillColor)
		Graphics.rectangle(true, self.x, self.y, self.width, self.height)
		Graphics.setColor(self.borderColor)
		Graphics.rectangle(false, self.x, self.y, self.width, self.height)
		setScissor(self.x, self.y, self.width, self.height)
		self.text:draw(window)
		if self.trackTick and (os.clock() - self.trackTick) % 1.2 < .6 then
			self.track:draw(window)
		end
		setScissor()
	end