Class "Label" : extends "UIControl"

LABEL_IMAGE_ORIENTATION_TOP = 1
LABEL_IMAGE_ORIENTATION_BOTTOM = 2
LABEL_IMAGE_ORIENTATION_CENTER = 3
LABEL_IMAGE_ORIENTATION_LEFT = 4
LABEL_IMAGE_ORIENTATION_RIGHT = 5

	function Label:constructor(text, size, font)
		self.super()
		self.text = text and Text(text or "", size or 14)
		self.margin = 8
		self.orientation = LABEL_IMAGE_ORIENTATION_LEFT
		self.characterSize = size or 14
		self.font = font
		
		if self.text then
			local shx, shy = self.text:getBounds()
			self.text:setOrigin(shx, shy)
			self.text:setColor(0, 0, 0)
			if font then
				self.text:setFont(font)
			end
		end
		
		self:update()
	end

	----------------------------------------------------------------
	------ User Functions ------------------------------------------
	----------------------------------------------------------------

	function Label:setText(text)
		if text and not self.text then
			self.text = Text(text, self.characterSize)
			if self.font then
				self.text:setFont(self.font)
			end
		elseif not text then
			self.text = nil
		else
			self.text:setString(text)
		end
		self:update()
	end
	
	function Label:setTextColor(color)
		if self.text then
			self.text:setColor(color)
		end
	end
	
	function Label:getSize()
		self:updateSize()
		return self.width, self.height
	end
	
	function Label:getWidth()
		self:updateSize()
		return self.width
	end
	
	function Label:getHeight()
		self:updateSize()
		return self.height
	end
	
	function Label:setFontSize(size)
		self.characterSize = size
		self.text:setCharacterSize(size)
		self:update()
	end

	function Label:getText(text)
		return self.text and self.text:getString() or ""
	end

	function Label:setPosition(x, y, centerX, centerY)
		self.x = math.floor(x - (centerX and self.width / 2 or 0))
		self.y = math.floor(y - (centerY and self.height / 2 or 0))
		self:update()
	end
	
	function Label:setImage(sprite, orientation, w, h)
		self.sprite = sprite
		self.orientation = orientation or LABEL_IMAGE_ORIENTATION_CENTER
		self.imageWidth = w
		self.imageHeight = h
		self:update()
	end
	
	function Label:getImage()
		return self.sprite
	end

	----------------------------------------------------------------
	------ Internal Functions --------------------------------------
	----------------------------------------------------------------

	function Label:updateSize()
		local width, height = 0, 0
		if self.text then
			width, height = self.text:getSize()
		end
		if self.sprite then
			local orientation = self.orientation
			local w, h = self.sprite:getSize(true)
			w = self.imageWidth or w
			h = self.imageHeight or h
			if orientation == LABEL_IMAGE_ORIENTATION_BOTTOM or
			 orientation == LABEL_IMAGE_ORIENTATION_TOP then
				if height > 0 and h > 0 then
					height = height + self.margin
				end
				height = height + h
			elseif orientation == LABEL_IMAGE_ORIENTATION_LEFT or
			 orientation == LABEL_IMAGE_ORIENTATION_RIGHT then
				if width > 0 and w > 0 then
					width = width + self.margin
				end
				width = width + h
			end
			width = math.max(width, w)
			height = math.max(height, h)
		end
		self.width, self.height = math.floor(width), math.floor(height)
	end
	
	function Label:update()

		self:updateSize()
		
		local x, y, width, height = self.x, self.y, self.width, self.height
		local orientation = self.orientation
		local margin = self.margin
		local wTxt, hTxt = 0, 0 if self.text then wTxt, hTxt = self.text:getSize(true) end
		local wSpr, hSpr = 0, 0 if self.sprite then wSpr, hSpr = self.sprite:getSize(true) end
		
		if not self.sprite then
			if self.text then
				self.text:setPosition(x, y)
			end
		elseif not self.text then
			self.sprite:setPosition(x + math.floor((width - wSpr) / 2), y + math.floor((height - hSpr) / 2))
		elseif orientation == LABEL_IMAGE_ORIENTATION_CENTER then
			self.sprite:setPosition(x + math.floor((width - wSpr) / 2), y + (height - hSpr) / 2)
			self.text:setPosition(x + math.floor((width - wTxt) / 2), y + (height - hTxt) / 2)
		elseif orientation == LABEL_IMAGE_ORIENTATION_LEFT then
			self.sprite:setPosition(x, y + math.floor((height - hSpr) / 2))
			self.text:setPosition(x + wSpr + margin, y + math.floor((height - hTxt) / 2))
		elseif orientation == LABEL_IMAGE_ORIENTATION_RIGHT then
			self.sprite:setPosition(x + width - wSpr, y + math.floor((height - hSpr) / 2))
			self.text:setPosition(x, y + math.floor((height - hTxt) / 2))
		elseif orientation == LABEL_IMAGE_ORIENTATION_BOTTOM then
			self.sprite:setPosition(x + math.floor((width - wSpr) / 2), y + math.floor(height - hSpr))
			self.text:setPosition(x + math.floor((width - wTxt) / 2), y)
		elseif orientation == LABEL_IMAGE_ORIENTATION_TOP then
			self.sprite:setPosition(x + math.floor((width - wSpr) / 2), y)
			self.text:setPosition(x + math.floor((width - wTxt) / 2), y + hSpr + margin)
		end
	end
	
	function Label:onDraw(window)

		if self.sprite then
			self.sprite:draw(window)
		end
		
		if self.text then
			self.text:draw(window)
		end

	end