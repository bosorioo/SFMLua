Class "Body"

	function Body:constructor(type)
		self.body = ConvexShape(0)
	end

	function Body:destructor()
		self.body = nil
	end

	function Body:setInnerColor(color, ...)
		self.body:setFillColor(color, ...)
	end

	function Body:setOuterColor(color, ...)
		self.body:setOutlineColor(color, ...)
	end

	function Body:setBorderWidth(width)
		self.body:setOutlineThick(width or 1)
	end

	function Body:setEllipse(a, b, points)
		self.body:setPoints(getEllipsePoints(0, 0, a, b, points))
	end

	function Body:setCircle(radius, points)
		self:setEllipse(radius, radius, points)
	end

	function Body:setRectangle(w, h)
		self.body:setPoints(getRectangularPoints(0, 0, w, h))
	end

	function Body:setSquare(l)
		self:setRectangle(l, l)
	end

	function Body:setRoundRect(w, h, a, b, points, corners)
		local verts = {}
		corners = corners or {}
		points  = points or 60
		a, b = math.min(a, w/2), math.min(b, h/2)
		table.tinsert(verts, not corners.d and
			getEllipsePoints(-w/2 + a, h/2 - b,
			a, b, points/4, math.pi*3/2, math.pi) or {-w/2, h/2})
		table.tinsert(verts, not corners.a and
			getEllipsePoints(-w/2 + a, - h/2 + b,
			a, b, points/4, math.pi, math.pi/2) or {-w/2, -h/2})
		for i = #verts, 1, -2 do
			table.insert(verts, -verts[i - 1])
			table.insert(verts, verts[i])
		end
		--[[
		table.tinsert(verts, not corners.c and
			getEllipsePoints(w/2 - a, h/2 - b,
			a, b, points/4, 0, -math.pi/2) or {w/2, h/2})
		table.tinsert(verts, not corners.b and
			getEllipsePoints(w/2 - a,- h/2 + b,
			a, b, points/4, math.pi/2, 0) or {w/2, - h/2})
		table.tinsert(verts, not corners.d and
			getEllipsePoints(-w/2 + a, h/2 - b,
			a, b, points/4, math.pi*3/2, math.pi) or {-w/2, h/2})
		--]]
		self.body:setPoints(verts)
	end

	function Body:setSuperEllipse(a, b, points, m, n, from, to)
		self.body:setPoints(getEllipsePoints(a, b, points, from, to, m, n))
	end

	function Body:setSuperShape(a,b,m,n1,n2,n3,points,f,t)
		self.body:setPoints(getSuperPoints(a,b,m,n1,n2,n3,
			points or 30,f,t))
	end

	function Body:rotate(angle)
		self.body:rotate(angle)
	end

	function Body:translate (x, y)
		self.body:move(x, y)
	end

	function Body:setOrigin(x, y)
		self.body:setOrigin(x, y)
	end

	function Body:setScale(x, y)
		self.body:setScale(x, y)
	end

	function Body:setFilled(bool)
		if bool then
			self.color = {self.body:getInnerColor()}
			self.body:setFillColor(0, 0, 0, 0)
		elseif self.color then
			self.body:setFillColor(self.color)
			self.color = nil
		end
	end

	function Body:setFill(bool)
		self:setFilled(bool)	
	end

	function Body:onDraw(window)
		self:draw(window)
	end

	function Body:setPosition(x, y)
		self.body:setPosition(math.floor(x), math.floor(y))
	end

	function Body:getPosition()
		return self.body:getPosition()
	end

	function Body:outline(window)
		local r, g, b, a = self:getFillColor()
		if a > 0 then
			self:setFill(false)
		end
		self:draw(window)
		if a > 0 then
			self:setFill(true)
		end
	end

	function Body:draw(window)
		self.body:draw(window)
	end

	function Body:setScissor(window)
		if window then
			glEnable(GL_STENCIL_TEST)
			glClearStencil(0)
			glClear(GL_STENCIL_BUFFER_BIT)
			glStencilFunc(GL_NEVER, 1, 0)
			glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE)
			self.body:draw(window)
			glStencilFunc(GL_EQUAL, 1, 1)
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP)
		else
			glDisable(GL_STENCIL_TEST)
		end			
	end
		