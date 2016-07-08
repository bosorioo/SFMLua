Class "DynamicSprite" : extends "UIControl"

	function DynamicSprite:constructor(...)
		self.super()
		self.labels = {}
	end
	
	----------------------------------------------------------------
	------ User Functions ------------------------------------------
	----------------------------------------------------------------

	function DynamicSprite:setPosition(x, y, centerX, centerY)
	
		x = math.floor(x - (centerX and self.width / 2 or 0))
		y = math.floor(y - (centerY and self.height / 2 or 0))

		self.x, self.y = x, y
		
		for index = 1, #self.labels, 3 do
			self.labels[index]:setPosition(self.x + self.labels[index + 1], self.y + self.labels[index + 2])
		end
	end
	
	function DynamicSprite:getHeight()		
		return self.height
	end
	
	function DynamicSprite:getWidth()
		return self.width
	end
	
	function DynamicSprite:clear()
		self.labels = {}
	end
	
	function DynamicSprite:addImage(sprite, x, y)
		local label = Label(nil)
		x, y = math.floor(x or 0), math.floor(y or 0)
		label:setImage(sprite, LABEL_IMAGE_ORIENTATION_CENTER)
		self.labels[#self.labels + 1] = label
		self.labels[#self.labels + 1] = x
		self.labels[#self.labels + 1] = y
		local width, height = 0, 0
		if sprite then
			width, height = sprite:getSize(true)
		end
		self.width = math.floor(math.max(self.width, width + x))
		self.height = math.floor(math.max(self.height, height + y))
		label:setPosition(self.x + x, self.y + y)
	end
	
	function DynamicSprite:addText(text, x, y, size, font)
		local label = Label(text, size or 10, font)
		x, y = math.floor(x or 0), math.floor(y or 0)
		self.labels[#self.labels + 1] = label
		self.labels[#self.labels + 1] = x
		self.labels[#self.labels + 1] = y
		local width, height = label:getSize()
		self.width = math.floor(math.max(self.width, width + x))
		self.height = math.floor(math.max(self.height, height + y))
		label:setPosition(self.x + x, self.y + y)
	end
	
	function DynamicSprite:getBounds()
		return self.x, self.y, self.width, self.height
	end
	
	function DynamicSprite:getSize()
		return self.width, self.height
	end

	----------------------------------------------------------------
	------ Internal Functions --------------------------------------
	----------------------------------------------------------------

	function DynamicSprite:onDraw(window)
		self:draw(window)
	end
	
	function DynamicSprite:draw(window)
		for index = 1, #self.labels, 3 do
			self.labels[index]:onDraw(window)
		end
	end