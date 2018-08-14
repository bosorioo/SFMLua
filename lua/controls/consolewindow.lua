Class "ConsoleWindow"

	function ConsoleWindow:constructor(title, width, height, fontsize)
		self.maxLines = 200
		self.width = width or 662
		self.height = height or 306
		self.fontsize = fontsize or 16
		self.title = title or "Console - Untitled"
		self.lines = {{"", extra_lines = 0}}
		self.window = Window({width = self.width, height = self.height,
			style = Styles.Titlebar + Styles.Close, title = self.title})
		self.window:setVerticalSync(true)
		self.buffer = ""
		self.buffer_multi_line = 0
		self.topline = 1
		self.multiline_topline = 1
		self.currentline = 1
		self.multi_lines_count = 0
		self.lastInputClock = 0
		self.trackPosDelay = 0.8
		self.trackPosition = 0
		self.userPrefix = nil
		self.userMessage = nil
		self.systemCommand = false
		self.commandsCallbacks = {}
		local temp = Text("A", self.fontsize)
		local l, t, w, h = temp:getBounds(true)
		self.charheight = math.ceil((h - t) * 3)
		self.visible = true
	end

	function ConsoleWindow:destructor()
		self.lines = {{""}}
		if self.window:isOpen() then
			self.window:close()
		end
	end

	function ConsoleWindow:toggleInput(b)
		if b ~= nil then
			self.inputBlocked = not b
		else
			self.inputBlocked = not self.inputBlocked
		end
	end

	function ConsoleWindow:toggleSystemCommand(b)
		if b ~= nil then
			self.systemCommand = b
		else
			self.systemCommand = not self.systemCommand
		end
	end

	function ConsoleWindow:sleep(ms, sleepfunc)

		local total = 0
		local miniStep = 100
		local lastDraw = 0
		ms = tonumber(ms)
		if not ms then return end

		while total < tonumber(ms) do

			if total + miniStep > ms then
				miniStep = ms - total
			end

			(sleepfunc or System.sleep)(miniStep)

			total = total + miniStep
			lastDraw = lastDraw + miniStep

			if lastDraw >= 1000/60 then
				lastDraw = 0
				self:onDraw()
			end
		end
	end

	function ConsoleWindow:setUserMessage(msg)
		self.userMessage = msg
	end

	function ConsoleWindow:getUserMessage()
		return self.userMessage
	end

	function ConsoleWindow:setUserPrefix(pref)
		self.userPrefix = pref
	end

	function ConsoleWindow:getUserPrefix()
		return self.userPrefix
	end
	
	function ConsoleWindow:setInputPrefix(pref)
		self.inputPrefix = pref
	end

	function ConsoleWindow:getInputPrefix()
		return self.inputPrefix
	end

	function ConsoleWindow:getInput()
		self:clearbuffer()
		local blockStatus = self.inputBlocked
		self:toggleInput(true)
		local answer, input
		while answer ~= "BUFFERSEND" and not self:isClosed() do
			answer, input = self:update()
			self:onDraw()
		end
		self:toggleInput(not blockStatus)
		return input
	end

	function ConsoleWindow:getch(printChar)
		self:clearbuffer()
		local blockStatus = self.inputBlocked
		self:toggleInput(true)
		self.getching = true
		local answer, input
		while answer ~= "GETCH" and not self:isClosed() do
			answer, input = self:update()
			self:onDraw()
		end
		self:toggleInput(not blockStatus)
		self.getching = false
		if printChar then
			self:print (string.char(input))
		end
		return string.char(input or 0)
	end
	
	function ConsoleWindow:kbhit()
		self:clearbuffer()
		local blockStatus = self.inputBlocked
		self:toggleInput(true)
		self.kbhitting = true
		local answer, input
		while answer ~= "KBHIT" and not self:isClosed() do
			answer, input = self:update()
			self:onDraw()
		end
		self:toggleInput(not blockStatus)
		self.kbhitting = false
		return input
	end
	
	function ConsoleWindow:getchar()
		return self:getch(true)
	end

	function ConsoleWindow:addCommand(command, callback)
		self.commandsCallbacks[command] = callback
	end

	function ConsoleWindow:runCommand(command)
		local com, arg = command:match("([^%s]+)%s*(.+)")
		local func = self.commandsCallbacks[com] or self.commandsCallbacks[command]
		if type(func) == "function" then
			func(self, arg)
		elseif self.systemCommand then
			os.execute(command)
		end
	end

	function ConsoleWindow:getLine(line)
		return self.lines and self.lines[line or self.currentline]
	end

	function ConsoleWindow:getLineText(line)
		local line = self:getLine(line)
		if not line then return ""
		else return table.concat(line, "")
		end
	end

	function ConsoleWindow:setLineText(text, line)
		line = line or self.currentline
		if self.lines[line] then
			self.lines[line] = {text}
		end
	end

	function ConsoleWindow:newLine(count)
		count = count or 1
		while count >= 1 do
			self.currentline = self.currentline + 1
			self.lines[self.currentline] = {extra_lines = 0}
			count = count - 1
		end
		while #self.lines > self.maxLines do
			self.currentline = self.currentline - 1
			table.remove(self.lines, 1)
		end
		if self.currentline - self.topline >= self:getVisibleLinesCount() then
			self.topline = self.currentline - self:getVisibleLinesCount() + 1
		end
	end

	function ConsoleWindow:decodeValue(v)
		local class = type(v)
		if class == "string" then
			return v
		elseif class == "number" then
			return tostring(v)
		elseif class == "boolean" then
			return v and "true" or "false"
		else
			return class
		end
		return "?"
	end

	function ConsoleWindow:print(text)
		if not text or self:isClosed() then return end
		text = self:decodeValue(text)
		local lines = text:explode("\n")
		if not self:getLine() then
			self:newLine()
		end
		for i = 1, #lines do
			if i > 1 then
				self:newLine()
			end
			table.insert(self:getLine(), lines[i])
			self:updateMultiLinesCount(self.currentline)
		end
		self:tracePosition()
	end

	function ConsoleWindow:getVisibleLinesCount()
		return math.ceil(self.height / self.charheight)
	end

	function ConsoleWindow:printf(...)
		self:print(string.format(...))
	end

	function ConsoleWindow:clear()
		self.lines = {{""}}
		self.topline = 1
		self.multiline_topline = 1
		self.currentline = 0
		if self.userMessage then
			self:print(self.userMessage)
			self:newLine(2)
		end
		self:updateMultiLinesCount()
	end

	function ConsoleWindow:clearbuffer()
		self.buffer = ""
		self.trackPosition = 0
		self.buffer_multi_line = 0
	end

	function ConsoleWindow:tracePosition(p)
		self:traceLine()
		self.trackPosition = math.max(0, math.min(#self.buffer, self.trackPosition + (p or 0)))
		return self.trackPosition
	end

	function ConsoleWindow:traceLine(l)

		if l == 0 then self:correctTraces() return end -- move vertically by 0, so do nothing

		if l == nil then -- move to the cursor
			if self.currentline < self.topline or
			   self.currentline >= self:getMaxTopline() then
				self.topline, self.multiline_topline = self:getMaxTopline()
				self:correctTraces()
			end
		return
		end

		local extraLines = self.lines[self.topline] and self.lines[self.topline].extra_lines or 0

		if extraLines and extraLines > 0 then
			if self.multiline_topline + l > extraLines + 1 then
				local linesSkip = extraLines + 1 - self.multiline_topline + 1
				self.multiline_topline = 1
				self.topline = self.topline + 1
				self:traceLine(l - linesSkip)
			elseif self.multiline_topline + l < 1 then
				local linesSkip = self.multiline_topline - 1 + 1
				self.topline = self.topline - 1
				self.multiline_topline = (self.lines[self.topline] and  self.lines[self.topline].extra_lines + 1) or 1
				self:traceLine(l + linesSkip)
			else
				self.multiline_topline = self.multiline_topline + l
			end
		else
			if l > 0 then
				self.topline = self.topline + 1
				self.multiline_topline = 1
				self:traceLine(l - 1)
			else
				self.topline = self.topline - 1
				self.multiline_topline = self.lines[self.topline] and self.lines[self.topline].extra_lines + 1 or 1
				self:traceLine(l + 1)
			end
		end

		self:correctTraces()
	end

	function ConsoleWindow:correctTraces()
		local maxTopLine, maxMultiTopLine = self:getMaxTopline()

		if self.topline > maxTopLine then
			self.topline = maxTopLine
			self.multiline_topline = maxMultiTopLine
		elseif self.topline == maxTopLine and self.multiline_topline > maxMultiTopLine then
			self.multiline_topline = maxMultiTopLine
		end

		self.topline = math.max(1, self.topline)
		self.multiline_topline = math.max(1, self.multiline_topline)
	end

	function ConsoleWindow:getMaxTopline()
		local visibleLines = self:getVisibleLinesCount() - 1
		local maxTopLine, maxMultiTopLine = #self.lines, self.lines[#self.lines].extra_lines + 1
		while visibleLines > 0 and maxTopLine >= 1 and maxTopLine <= #self.lines do
			maxMultiTopLine = maxMultiTopLine - math.min(visibleLines, self.lines[maxTopLine].extra_lines + 1)
			visibleLines = visibleLines - self.lines[maxTopLine].extra_lines - 1
			if maxMultiTopLine < 1 then
				maxTopLine = maxTopLine - 1
				if maxTopLine >= 1 and maxTopLine <= #self.lines then
					maxMultiTopLine = self.lines[maxTopLine].extra_lines + 1
				end
			end	
		end
		return maxTopLine, maxMultiTopLine
	end

	function ConsoleWindow:getMultiLinesCount()
		return self.multi_lines_count or 0
	end

	function ConsoleWindow:updateMultiLinesCount(specificLine)
		if specificLine then
			self.multi_lines_count = self.multi_lines_count - (self.lines[specificLine].extra_lines or 0)
		else
			self.multi_lines_count = 0
		end
		local multiLines = 0
		local lineText = Text("", self.fontsize)
		lineText:setStyle(Styles.Bold)
		for index = specificLine or 1, specificLine or #self.lines do
			lineText:setString(self:getLineText(index))
			local l, t, w, h = lineText:getBounds(true)
			self.multi_lines_count = self.multi_lines_count + math.floor(w / (self.width - 4))
			self.lines[index].extra_lines = math.floor(w / (self.width - 4))
			multiLines = multiLines + math.floor(w / (self.width - 4))
		end
		return multiLines
	end

	function ConsoleWindow:writebuffer(str)
		self.buffer = self.buffer .. str
		self.trackPosition = self.trackPosition + #str
		self.buffer_multi_line = math.floor(self:getStringWidth(self.buffer) / self.width)
	end

	function ConsoleWindow:removebuffer(n)
		n = n or -1
		self.buffer = self.buffer:sub(0, self.trackPosition + n) .. self.buffer:sub(self.trackPosition + n + 2)
		self:tracePosition(n)
	end

	function ConsoleWindow:isClosed()
		return not self.window:isOpen()
	end

	function ConsoleWindow:close()
		self:destructor()
	end
	
	function ConsoleWindow:setEventCallback(func)
		self.eventCallbackFunction = func
	end

	function ConsoleWindow:setMessageCallback(func)
		self.messageCallbackFunction = func
	end

	function ConsoleWindow:update(now, past)

		if not self.window:isOpen() then return end

		for _, e in pairs (self.window:getEvents()) do

			if self.eventCallbackFunction then
				self.eventCallbackFunction(e)
			end
			
			if self.kbhitting and e.type == Events.KeyPressed then
				return "KBHIT", e.keycode
			end
		
			if e.type == Events.Closed then
			
				self:destructor()
				return
				
			elseif e.type == Events.TextEntered and not self.inputBlocked then

				self.lastInputClock = os.clock()

				if self.getching and (e.unicode == 9 or e.unicode == 13 or
				   (e.unicode < 256 and e.unicode > 31)) then
					return "GETCH", e.unicode
				end

				if e.unicode == 9 then -- TAB
					self:writebuffer('\t')
					self:traceLine()
					return "BUFFERINPUT", 't'
				elseif e.unicode == 13 then -- ENTER

					self:traceLine()
					local buffer = self.buffer
					self:clearbuffer()

					if self.messageCallbackFunction and
					   self.messageCallbackFunction(buffer) == false then
						return "BUFFERSEND", buffer
					end
				
					self:print((self.inputPrefix or "") .. buffer)
					self:newLine()
					self:runCommand(buffer)
					return "BUFFERSEND", buffer

				elseif e.unicode == 8 then -- BACKSPACE
					self:removebuffer(-1)
					self:traceLine()
				elseif e.unicode < 256 then
					if e.unicode > 31 then
						self:traceLine()
						self:writebuffer(string.char(e.unicode))
						return "BUFFERINPUT", string.char(e.unicode)
					end
				else
					print ("Entered unicode " .. e.unicode..".\n")
				end

			elseif e.type == Events.KeyPressed and not self.inputBlocked then
			
				if e.keycode == 71 then -- Left Arrow
					self:tracePosition(-1)
				elseif e.keycode == 72 then -- Right Arrow
					self:tracePosition(1)
				elseif e.keycode == 73 then -- Up Arrow
					self:traceLine(-1)
				elseif e.keycode == 74 then -- Down Arrow
					self:traceLine(1)
				elseif e.keycode == 66 then -- Delete
					self:removebuffer(0)
				end
				
			elseif e.type == Events.MouseWheel then
			
				self:traceLine(-e.delta)
				
			end
		end
	end

	function ConsoleWindow:run(forever)
		if forever then
			while self.window:isOpen() do
				self:update()
				self:onDraw()
			end
		else
			self:update()
			self:onDraw()
		end
	end

	function ConsoleWindow:breakMultiLine(text)
		if not text then return {} end
		local lines = {}
		local lastIndex = 1
		local label = Text("", self.fontsize)
		label:setStyle(Styles.Bold)
		for i = 1, #text do
			label:setString(text:sub(lastIndex, i))
			local l, t, w, h = label:getBounds(true)
			if w >= self.width then
				table.insert(lines, text:sub(lastIndex, i - 1))
				lastIndex = i
			end
		end
		table.insert(lines, text:sub(lastIndex))
		return lines
	end

	function ConsoleWindow:getStringWidth(str)
		local label = Text(str, self.fontsize)
		label:setStyle(Styles.Bold)
		local l, t, w, h = label:getBounds(true)
		return w
	end

	function ConsoleWindow:setVisible(bool)
		if bool == self.visible then return end
		self.visible = bool
		self.window:setVisible(bool)
	end

	function ConsoleWindow:toggleVisibility()
		self:setVisible(not self.visible)
	end

	function ConsoleWindow:onDraw()

		if not self.window:isOpen() or not self.visible then return end

		self.window:clear(0, 0, 0)

		local index = self.topline
		local multiline_index = 0
		local limit = self.topline + math.min(self:getVisibleLinesCount(), #self.lines) - 1
		local clock = os.clock() - self.lastInputClock
		local visibleLines = self:getVisibleLinesCount()

		while visibleLines > 0 do

			local text = self:getLineText(index)

			if index == self.currentline then
				text = text..self.buffer
				
				if self.userPrefix then
					text = self.userPrefix .. text
				end
				
				if not self.inputBlocked and 
				   (clock < self.trackPosDelay or math.floor(clock - self.trackPosDelay) % 2 == 1) then
					text = text .. "_"
				end
			end

			for tIndex, brokenText in ipairs(self:breakMultiLine(text)) do
				if index ~= self.topline or tIndex >= self.multiline_topline then
					if (index == self.topline and tIndex > self.multiline_topline)
					   or (index ~= self.topline and tIndex > 1) then
						multiline_index = multiline_index + 1
					end
					self.window:print(brokenText, 2, self.charheight * (index - self.topline + multiline_index),
					  {size = self.fontsize, color = {192, 192, 192}, style = Styles.Bold})
					visibleLines = visibleLines - 1
					if visibleLines <= 0 then break end
				end
			end

			index = index + 1

			if index > limit then
				break
			end

		end

		self.window:display()
	end