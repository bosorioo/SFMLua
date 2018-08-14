Class "Shape"

	function Shape:constructor()
		self.points = {}
		self.x = 0
		self.y = 0
		self.width, self.height = 0, 0
		self.rotation = 0
		self.filled = false
		self.cycled = nil
		self.color = 0xFFFFFFFF
		self.visible = true
	end

	function Shape:setPosition(x, y, center)
		if center then
			x, y = x - self.width / 2, y - self.height / 2
		end
		self.x, self.y = math.floor(x), math.floor(y)
		return self
	end

	function Shape:setCenter(ox, oy)
		self:setPosition(ox, oy, true)
		return self
	end

	function Shape:setVisible(bool)
		if bool then
			self.visible = true
		else
			self.visible = nil
		end
	end
	
	function Shape:setGradient(gradient, c1, c2)
		if not GradientFill then return end
		if not gradient then
			self.gradient = nil
			return
		end
		if type(gradient) == "table" then
			self.gradient = gradient
		else
			self.gradient = GradientFill(self.box[3], self.box[4])
			if gradient == "vertical" then
				self.gradient:setVertical(c1, c2)
			elseif gradient == "horizontal" then
				self.gradient:setHorizontal(c1, c2)
			elseif gradient == "radial" then
				self.gradient:setRadial(c1, c2)
			end
		end
		self.gradient:setCenter(self.x, self.y)
		return self
	end

	function Shape:clear()
		self.points = {}
		self.filled = false
		return self
	end

	function Shape:cycle(bool)

		if (bool and self.cycled) or not (bool or self.cycled) then
			return
		end

		self.cycled = bool
		local index = #self.points

		if bool then
			self.points[index + 1] = self.points[1]
			self.points[index + 2] = self.points[2]
		else
			self.points[index + 1] = nil
			self.points[index + 2] = nil
		end

		return self
	end

	function Shape:setEllipse(a, b, points)
		points = points or 30
		self:clear()
		self.points, self.box = getEllipsePoints(0, 0, a, b, points)
		return self
	end

	function Shape:setCircle(radius, points)
		self:clear()
		self:setEllipse(0, 0, radius, radius, points)
		return self
	end

	function Shape:setRectangle(w, h)
		self:clear()
		self.points, self.box = getRectangularPoints(w/2, h/2, w, h)
		self:cycle()
		return self
	end

	function Shape:setSquare(l)
		self:clear()
		self:setRectangle(l, l)
		return self
	end

	function Shape:setRoundRect(w, h, a, b, points, corners)
		self:clear()
		corners = corners or {}
		a, b = math.min(a, w/2), math.min(b, h/2)
		table.tinsert(self.points, not corners.a and
			getEllipsePoints(-w/2 + a, -h/2 + b,
			a, b, points/4, math.pi, math.pi/2) or {-w/2, -h/2})
		table.tinsert(self.points, not corners.b and
			getEllipsePoints(w/2 - a, -h/2 + b,
			a, b, points/4, math.pi/2, 0) or {w/2, -h/2})
		table.tinsert(self.points, not corners.c and
			getEllipsePoints(w/2 - a, h/2 - b,
			a, b, points/4, 0, -math.pi/2) or {w/2, h/2})
		table.tinsert(self.points, not corners.d and
			getEllipsePoints(-w/2 + a, h/2 - b,
			a, b, points/4, math.pi*3/2, math.pi) or {-w/2, h/2})
		self.box = {w/2, -h/2, w, h}
		return self
	end

	function Shape:setSuperEllipse(a, b, points, m, n, from, to)
		self:clear()
		self.points, self.box = getEllipsePoints(0, 0, a, b, points,
			from, to, m, n)
		return self
	end

	function Shape:setSuperShape(a,b,m,n1,n2,n3,points,f,t)
		self:clear()
		self.points, self.box = getSuperPoints(0,0,a,b,m,n1,n2,n3,
			points or 30,f,t)
		return self
	end

	function Shape:rotate(angle)
		--[[local angle = math.radians(angle)
		local p = self.points
		local sin, cos = math.sin(angle), math.cos(angle)
		local x, y = self.x, self.y
		for i = 1, #p, 2 do
			p[i], p[i + 1] = (p[i] - x) * cos - (p[i + 1] - y) * sin + x,
				(p[i] - x) * sin + (p[i + 1] - y) * cos + y
		end]]
		self.rotation = self.rotation + angle
		return self
	end

	function Shape:translate (x, y)
		--[[for i = 1, #self.points, 2 do
			self.points[i] = self.points[i] + x
			self.points[i+1] = self.points[i+1] + y
		end
		self.x, self.y = self.x + x, self.y + y
		if self.box then
			self.box[1], self.box[2] = self.box[1] + x, self.box[2] + y
		end]]
		self:setPosition(self.x + x, self.y + y)
		return self
	end

	function Shape:pointAlignment(point_index, x, y)
		local p = self.points
		local a, b, c, d = p[point_index], p[point_index + 1], p[point_index + 2],
			p[point_index + 3]
		if point_index == #p - 1 then
			c, d = p[1], p[2]
		end
		return a * d + c * y + x * b - b * c - d * x - y * a
	end

	function Shape:isPointInside(x, y)
		x, y = x - self.x, y - self.y
		if self.box then
			return self.box[1] <= x and self.box[3] >= x and
				self.box[2] <= y and self.box[4] >= y
		end
	end

	function Shape:_isColliding(shape)
		local x, y = shape.x, shape.y
		local distance, d = math.huge
		local index, p = 0, self.points
		for i = self.filled and 3 or 1, #self.points - 1, 2 do
			d = (x - p[i])^2 + (y - p[i + 1])^2
			if d < distance then
				distance = d
				index = i
			end 
		end
		self.close = index
		return index > 0 and shape:isPointInside(p[index], p[index + 1])
	end

	function Shape:isBoxColliding(shape)
		if not self.box or not shape.box then return true end
		if self.box[1] + self.box[3] < shape.box[1] or
		   self.box[1] > shape.box[1] + shape.box[3] or
		   self.box[2] + self.box[4] < shape.box[2] or
		   self.box[2] > shape.box[2] + shape.box[4] then
			return false
		end
	return true
	end

	function Shape:isInsideBox(x, y)
		x, y = x - self.x, y - self.y
		return x >= self.box[1] and x <= self.box[1] + self.box[3] and
		y >= self.box[2] and y <= self.box[2] + self.box[4]
	end

	function Shape:isColliding(shape)
		if not self:isBoxColliding(shape) then return false end
		return self:_isColliding(shape) or shape:_isColliding(self) or
		self:isPointInside(shape.x, shape.y) or
		shape:isPointInside(self.x, self.y)
	end

	function Shape:setFilled(bool)
		self:setFill(bool)
		return self
	end

	function Shape:setFill(bool)
		if bool and not self.filled then

			if not self.cycled then
				self:cycle()
			end

			table.insert(self.points, 1, 0)
			table.insert(self.points, 1, 0)
		elseif not bool and self.filled then
			table.remove(self.points, 1)
			table.remove(self.points, 1)
		end
		self.filled = bool	
		return self
	end

	function Shape:setColor(color)
		self.color = color
		return self
	end

	function Shape:onDraw(window)
		self:draw(window)
	end

	function Shape:getSize()
		if self.box then
			self.width, self.height = math.floor(self.box[3] - self.box[1]),
				math.floor(self.box[4] - self.box[2])
			self.box = nil
		end
		return self.width, self.height
	end

	function Shape:draw(window, lineMode, mode)

		if not self.visible then
			return
		end

		window:translateView(-self.x, -self.y)

		if self.gradient then
			self.gradient:start()
		else
			Graphics.setColor(self.color)
		end

		local p, np = self.points, #self.points

		if lineMode and self.filled then

			if window then
				for i = 3, np, 2 do
					window:points({p[1], p[2], p[i], p[i + 1]}, Vertex.Lines)
				end
			else
				Graphics.setColor(self.color)
				for i = 3, np, 2 do
					Graphics.line(p[1], p[2], p[i], p[i + 1])
				end
			end
		else
			local mode = mode or self.filled and Vertex.TrianglesFan or
				Vertex.LinesStrip
			if window then
				window:points(p, mode)
				--if not self.cycled and self.filled then
				--	window:points({p[1], p[2], p[3], p[4], p[np-1], p[np]},
				--		mode)
				--elseif not self.cycled then
				--	window:points({p[1],p[2],p[np-1],p[np]}, mode)
				--end
			else
				Graphics.points(self.points, mode)
				if not self.cycled and self.filled then
					Graphics.points({p[1],p[2],p[3],p[4],p[np-1],p[np]}, mode)
				elseif not self.cycled then
					Graphics.points({p[1],p[2],p[np-1],p[np]}, mode)
				end
			end
		end

		if self.gradient then
			self.gradient:finish(window)
		end

		window:translateView(self.x, self.y)
	end		
		