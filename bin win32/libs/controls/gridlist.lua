Class "GridList" : extends "UIControl"

local VERTICAL_MARGIN = 12
local HORIZONTAL_MARGIN = 10
local ITEM_GAP_HEIGHT = 10
local ITEM_GAP_WIDTH = 12
local PAGE_UP_DOWN_MOVE_PERCENT = .1
local MOVEMENT_DELTA_MULTPLIER = 0.45
local SELECTION_MOVE_UP = -9900
local SELECTION_MOVE_TOP = -9901
local SELECTION_MOVE_BOTTOM = -9902
local SELECTION_MOVE_DOWN = -9903
local SELECTION_MOVE_PAGE_UP = -9904
local SELECTION_MOVE_PAGE_DOWN = -9905
local HOVER_BORDER_COLOR = 0xFF121212
local HOVER_BACKGROUND_COLOR = 0x14000000
local SELECTION_BORDER_COLOR = 0xFF5765E6
local SELECTION_BACKGROUND_COLOR = 0x555765E6

local function floor(n)
	if not tonumber(n) then return n end
	return math.floor(n)
end

	function GridList:constructor(width, height, x, y)
		self.super(x, y, width, height)
		self.verticalPosition = 1
		self.itemsList = {}
		self.rows = {}
		self.nextSelection = nil
		self.selectedIndex = nil
		self.selectedIndexOld = nil
		self.mouseHoverIndex = nil
		self.hoverIndexOld = nil
		self.mouseHoverRow = nil
		self.mouseSelectionDrag = nil
		self.selectedFontColor = R.color.WHITE
		self.unselectedFontColor = R.color.BLACK
		
		if self.width > 0 and self.height > 0 then
			self.renderTexture = RenderTexture(self.width, self.height)
			self.renderTexture:clear(0xFFFFFFFF)
		end
		self.displayUpdate = false

		self.visibleRows = 0
		self.visibleRowsUpdate = false

		self.maxVerticalPosition = 0
		self.maxVerticalPositionUpdate = false

		self.innerColor = RGBA(255, 255, 255)
		self.outerColor = RGBA(0, 0, 120)

		self:move(x or 0, y or 0)
		
		self.onSelectCallback = nil
		self.onHoverCallback = nil
		self.onDoubleClickCallback = nil
		self.onKeyCallback = nil
		
		self.lastClickIndex = nil
		self.lastClickTick = 0
		
		self.movementDeltaMultiplier = nil
	end

	function GridList:destructor()

	end

	--[[ User Functions ]]--
	
	function GridList:setWheelMultiplier(m)
		self.movementDeltaMultiplier = m
	end
	
	function GridList:getItem(index)
		return self.itemsList[index]
	end
	
	function GridList:getSize()
		return #self.itemsList
	end
	
	function GridList:setGapSize(horizontal, vertical)
		if self.itemGapWidth ~= horizontal or
		   self.itemGapHeight ~= vertical then
			self:repaint()
		end
		
		self.itemGapWidth = horizontal
		self.itemGapHeight = vertical
	end
	
	function GridList:clear()
		self.itemsList = {}
		self.rows = {}
		self.selectedIndex = nil
		self:onHover(nil)
		self:onSelect(nil)
		self.mouseHoverIndex = nil
		self.mouseHoverRow = nil
		self.mouseSelectionDrag = nil
		self.verticalPosition = 1
		self.visibleRows = 0
		self.visibleRowsUpdate = false
		self.maxVerticalPosition = 0
		self.maxVerticalPositionUpdate = false
		
		if self.renderTexture then
			self.renderTexture:clear(0xFFFFFFFF)
		end
	end

	function GridList:addItem(obj, index)
		local itemsList = self.itemsList
		local size = #itemsList
		local selectedIndex = self.nextSelection or self.selectedIndex
		index = math.min(index or size + 1, size + 1)
		if selectedIndex and index and selectedIndex >= index then
			self.nextSelection = selectedIndex + 1
		end
		table.insert(itemsList, index, obj)
		obj.parent = self
		self:computeRows(index <= size)
		self.maxVerticalPositionUpdate = true
		self:repaint()
	end
	
	function GridList:removeItem(obj)
	
		local index = tonumber(obj)
		
		if not index then
			for i = 1, #self.itemsList do
				if self.itemsList[i] == obj then
					index = i
					break
				end
			end
		end
		
		if not index then
			return
		end
		
		local itemsList = self.itemsList
		local size = #itemsList
		local selectedIndex = self.nextSelection or self.selectedIndex
		
		if selectedIndex then
			if selectedIndex == index then
				if index == size then
					if size == 1 then
						self:onSelect(nil)
					elseif index > 1 then
						self.nextSelection = selectedIndex - 1
					else
						self.nextSelection = selectedIndex
					end
				else
					self.nextSelection = selectedIndex
				end
			elseif selectedIndex > index then
				self.nextSelection = selectedIndex - 1
			else
				self.nextSelection = selectedIndex
			end
		end

		table.remove(itemsList, index)
		self:computeRows(true)
		self:correctVerticalPosition()
	end
	
	local binary_search_func
	
	function GridList:getItemRow(itemIndex)
		if not binary_search_func then
			binary_search_func = function(row, index, itemIndex)
				if row.indexFrom > itemIndex then
					return true
				elseif row.indexTo < itemIndex then
					return false
				end
			end
		end
		return itemIndex and table.binarysearch(self.rows, binary_search_func, itemIndex)
	end
	
	function GridList:getItemHeight(index)
		local item = self.itemsList[index]
		return item and (item.getHeight and item:getHeight() or item.height)
	end
	
	function GridList:getItemWidth(index)
		local item = self.itemsList[index]
		return item and (item.getWidth and item:getWidth() or item.width)
	end
	
	function GridList:getRowHeight(index)
		local row = self.rows[index]
		return row and row.height
	end

	function GridList:addStringItem(str, size, font)
		local item = Label(str, size, font)
		item:setPosition(self.width - 2 * HORIZONTAL_MARGIN, 26)
		self:addItem(item)
	end

	function GridList:move(x, y)
		self:setPosition(self.x + x, self.y + y)
	end

	function GridList:setPosition(x, y)
		self.x = math.floor(x)
		self.y = math.floor(y)
	end

	function GridList:setSize(width, height)
	
		width, height = math.floor(width), math.floor(height)
		width, height = math.max(0, width), math.max(0, height)
		
		if self.width == width and self.height == height then
			return
		end

		self.width, self.height = width, height
		
		if width > 0 and height > 0 then
			self.renderTexture = RenderTexture(width, height)
			self.renderTexture:clear(0xFFFFFFFF)
		else
			self.renderTexture = nil
		end
		
		self:computeRows(true)
		self.visibleRowsUpdate = true
		self.maxVerticalPositionUpdate = true
		self:correctVerticalPosition()
		self:repaint()
	end
	
	function GridList:getVisibleRows()

		if #self.rows == 0 then
			return 0
		end
		
		local visibleRows = 0
		local verticalPosition = self.verticalPosition
		local index = math.floor(verticalPosition) - 1
		local heightLeft = self.height - (verticalPosition == 1 and VERTICAL_MARGIN or 0)
		local lastItem = #self.itemsList
		local rows = self.rows
		local rowHeight
		
		repeat
			index = index + 1
			rowHeight = self:getRowHeight(index)
			if not rowHeight then
				break
			end
			if rowHeight > 0 then
				if index == math.floor(verticalPosition) then
					local visibility = 1 - (verticalPosition - index)
					rowHeight = rowHeight * visibility
					visibleRows = visibleRows + visibility
				else
					visibleRows = visibleRows + 1
				end
				heightLeft = heightLeft - rowHeight
				if heightLeft < 0 then
					visibleRows = visibleRows + (heightLeft/rowHeight)
				end
			end
		until heightLeft <= 0
		return visibleRows
	end
	
	function GridList:setVerticalPosition(position)
		if self.verticalPosition ~= position then
			self.visibleRowsUpdate = true
			self:repaint()
			self.verticalPosition = position
			self:correctVerticalPosition()
		end
	end
	
	function GridList:moveVerticalPosition(delta)
		if math.abs(delta) > 0 then
			self.visibleRowsUpdate = true
			self:repaint()
			self.verticalPosition = self.verticalPosition + delta
			self:correctVerticalPosition()
		end
	end
	
	function GridList:getSelectedItem()
		local selectedIndex = self.nextSelection or self.selectedIndex
		return selectedIndex and self.itemsList[selectedIndex]
	end
	
	function GridList:getSelectedIndex()
		return self.nextSelection or self.selectedIndex
	end

	function GridList:setOnSelect(callback)
		self.onSelectCallback = callback
	end
	
	function GridList:setOnHover(callback)
		self.onHoverCallback = callback
	end
	
	function GridList:setOnDoubleClick(callback)
		self.onDoubleClickCallback = callback
	end
	
	function GridList:setOnKeyDown(callback)
		self.onKeyCallback = callback
	end
	
	function GridList:setHoverColor(background, border)
		self.HOVER_BACKGROUND_COLOR = RGBA(background)
		self.HOVER_BORDER_COLOR = RGBA(border)
	end
	
	function GridList:setSelectionColor(background, border)
		self.SELECTION_BACKGROUND_COLOR = RGBA(background)
		self.SELECTION_BORDER_COLOR = RGBA(border)
	end
	
	function GridList:repaint()
		self.displayUpdate = true
	end
	
	--[[ Internal Functions ]]--
	
	function GridList:computeRows(allOver)
	
		local rows = allOver and {} or self.rows
		local rowsCount = #rows
		local itemsList = self.itemsList
		local size = #itemsList
		local gapWidth = self.itemGapWidth or ITEM_GAP_WIDTH
		local gapHeight = self.itemGapHeight or ITEM_GAP_HEIGHT
		local rowIndex, row = rowsCount, rows[rowsCount]
		local getItemWidth, getItemHeight = self.getItemWidth, self.getItemHeight
		local width = self.width
		local max = math.max
		self.rows = rows
		
		if size == 0 then
			self.visibleRows = 0
			return
		end
		
		self:repaint()
		
		for itemIndex = allOver and 1 or size, size do
			local itemWidth = getItemWidth(self, itemIndex)
			local itemHeight = getItemHeight(self, itemIndex)
			if not row or row.width < itemWidth then
				if row and rowIndex > 1 then
					row.height = row.height + gapHeight / 2
				end
				row = {height = itemHeight + gapHeight / 2,
						width = width - itemWidth - 2 * HORIZONTAL_MARGIN -
						gapWidth / 2, indexFrom = itemIndex, indexTo = itemIndex}
				rowIndex = rowIndex + 1
				rows[rowIndex] = row
				self.maxVerticalPositionUpdate = true
				self.visibleRowsUpdate = true
			else
				row.height = max(row.height, itemHeight + gapHeight / 2)
				row.width = row.width - itemWidth - gapWidth / 2
				if row.width < 0 then
					row.width = row.width + gapWidth / 2
				end
				row.indexTo = itemIndex
			end	
		end
	end
	
	function GridList:correctVerticalPosition()

		local minPosition = 1
		
		if self.verticalPosition < minPosition then
			self.verticalPosition = minPosition
			self:repaint()
			return
		end

		local maxPosition = self:getMaxVerticalPosition()

		if self.verticalPosition > maxPosition then
			self.verticalPosition = maxPosition
			self:repaint()
		end
	
	end

	function GridList:getMaxVerticalPosition(specificIndex)

		if specificIndex and specificIndex > #self.rows then
			specificIndex = nil
		end
		
		local rows = self.rows
		local size = #rows
		local index = specificIndex and math.floor(specificIndex) or size
		local rowHeight
		local heightLeft = self.height - (index == size and VERTICAL_MARGIN or 0)
		
		repeat
			rowHeight = self:getRowHeight(index)
			if not rowHeight then
				index = index + 1
				break
			end
			if rowHeight > 0 then
				heightLeft = heightLeft - rowHeight
				if heightLeft <= 0 then
					index = index - (heightLeft/rowHeight)
					break
				end
			end
			index = index - 1
		until false

		return index
	end
	
	function GridList:onSelect(index, triggerCallback)
	
		local visList = self.itemsList
	
		index = index == 0 and 1 or
			index == #visList + 1 and
			#visList or tonumber(index) and
			math.floor(index) or index
			
		self.selectedIndex = index
		
		if index ~= self.selectedIndexOld or triggerCallback then
			if self.onSelectCallback then
				self.onSelectCallback(index, self.selectedIndexOld)
			end
			self.selectedIndexOld = index
			self:repaint()
		end
		
		self:viewSelectedItem()
		self.nextSelection = nil
	end

	function GridList:onHover(index)
	
		local visList = self.itemsList
	
		index = index == 0 and 1 or
			index == #visList + 1 and
			#visList or tonumber(index) and
			math.floor(index)

		local realIndex = visList[index]
		
		if not realIndex then
			self.mouseHoverIndex = nil
		else
			self.mouseHoverIndex = index
		end
		
		if realIndex ~= self.hoverIndexOld then
			if self.onHoverCallback then
				self.onHoverCallback(realIndex, self.hoverIndexOld)
			end
			self.hoverIndexOld = realIndex
			self:repaint()
		end
	end

	function GridList:getRowIndexFromY(y)

		local indexFrom = math.floor(self.verticalPosition)
		local indexTo = indexFrom + math.ceil(self:getVisibleRows()) - 1
		local lastIndex = #self.itemsList
		local rows = self.rows
		local lastRow = #rows

		if y >= self.height then
			return
		end
		
		if indexFrom == 1 or indexTo == lastIndex then
			y = y - (indexFrom == 1 and VERTICAL_MARGIN or -VERTICAL_MARGIN)
		end
		
		for index = indexFrom, indexTo do
			local rowHeight = self:getRowHeight(index)
			if not rowHeight then
				break
			end
			if rowHeight > 0 then
				if index == indexFrom then
					rowHeight = rowHeight * (1 - (self.verticalPosition - indexFrom))
				end
				y = y - rowHeight
				if y <= 0 then
					return (index >= 1 and index <= lastIndex and (1 + index + y / rowHeight))
				end
			end
		end
	end
	
	function GridList:getItemIndexFromX(x, rowIndex)
		local getItemWidth = self.getItemWidth
		local row = self.rows[rowIndex]
		x = x - HORIZONTAL_MARGIN
		if not row or x <= 0 or x > self.width then
			return
		end
		local gapWidth = self.itemGapWidth or ITEM_GAP_WIDTH
		local indexFrom, indexTo = row.indexFrom, row.indexTo
		for index = indexFrom, indexTo do
			local itemWidth = getItemWidth(self, index)
			if not itemWidth then
				break
			end
			x = x - gapWidth / 2
			if itemWidth > 0 then
				x = x - itemWidth
				if x <= 0 then
					return index
				end
			end
			x = x - gapWidth / 2
		end
	end
	
	function GridList:getItemXFromIndex(itemIndex, rowIndex)
		local getItemWidth = self.getItemWidth
		local row = self.rows[rowIndex]
		if not row then
			return
		end
		local indexFrom, indexTo = row.indexFrom, row.indexTo
		local gapWidth = self.itemGapWidth or ITEM_GAP_WIDTH
		local x = HORIZONTAL_MARGIN - gapWidth / 2
		for index = indexFrom, indexTo do
			local itemWidth = getItemWidth(self, index)
			if not itemWidth then break end
			x = x + gapWidth / 2
			if index == itemIndex then
				return x + itemWidth / 2
			else
				x = x + itemWidth + gapWidth / 2
			end
		end
	end
	
	function GridList:isPointWithin(x, y)
		return x >= self.x and x <= self.x + self.width and y >= self.y and y <= self.y + self.height
	end
	
	function GridList:isSelectedItemVisible()
	
		local selectedIndex = self.nextSelection or self.selectedIndex
		local selectedRow = self:getItemRow(selectedIndex)
	
		if not selectedRow then
			return false
		end
		
		local verticalPosition = self.verticalPosition
		
		local ret = verticalPosition <= selectedRow and
			math.floor(verticalPosition + self:getVisibleRows() - 1) >= selectedRow
			
		return ret
	end
	
	function GridList:viewSelectedItem()
		if not self:isSelectedItemVisible() then
			self:viewRow(self:getItemRow(self.nextSelection or self.selectedIndex))
		end
	end
	
	function GridList:viewRow(rowIndex)
		if rowIndex then
			local currentVerticalPosition = self.verticalPosition
			local newVerticalPosition = currentVerticalPosition
			if rowIndex < currentVerticalPosition then
				newVerticalPosition = rowIndex
			else
				newVerticalPosition = self:getMaxVerticalPosition(rowIndex)
			end
			self:moveVerticalPosition(newVerticalPosition - currentVerticalPosition)
		end
	end
	
	function GridList:moveSelection(delta)
	
		local selectedIndex = self.selectedIndex
		local selectedRow = self:getItemRow(selectedIndex)
		local X = self:getItemXFromIndex(selectedIndex, selectedRow)
		
		if delta == SELECTION_MOVE_BOTTOM then
			selectedRow = #self.rows
		elseif delta == SELECTION_MOVE_TOP then
			selectedRow = 1
		elseif delta == SELECTION_MOVE_UP then
			selectedRow = selectedRow - 1
		elseif delta == SELECTION_MOVE_DOWN then
			selectedRow = selectedRow + 1
		elseif delta == SELECTION_MOVE_PAGE_DOWN then
			selectedRow = selectedRow + math.max(1, #self.rows * PAGE_UP_DOWN_MOVE_PERCENT)
		elseif delta == SELECTION_MOVE_PAGE_UP then
			selectedRow = selectedRow - math.max(1, #self.rows * PAGE_UP_DOWN_MOVE_PERCENT)
		elseif delta == SELECTION_MOVE_DOWN then
			selectedRow = selectedRow + 1
		elseif delta == 1 or delta == -1 then
		
			self:onSelect(selectedIndex + delta)
			
			if not self:isSelectedItemVisible() then
				self:viewSelectedItem()
			end
			
			if self.hoverLastX and self.hoverLastY then
				self:onMouseMove(self.hoverLastX, self.hoverLastY)
			end
			
			return
			
		elseif not delta then
			self:onSelect(nil)
			return
		end
		
		selectedRow = math.floor(math.max(1, math.min(#self.rows, selectedRow)))
		selectedIndex = self:getItemIndexFromX(X or 0, selectedRow) or self.rows[selectedRow].indexTo

		self:onSelect(selectedIndex)
		
		if not self:isSelectedItemVisible() then
			self:viewSelectedItem()
		end
		
		if self.hoverLastX and self.hoverLastY then
			self:onMouseMove(self.hoverLastX, self.hoverLastY)
		end

	end

	function GridList:onKeyDown(keycode)
	
		if self.onKeyCallback and self.onKeyCallback(keycode) == true then
			return EVENT_STOP
		end

		if keycode == Keys.Home then
			self:moveSelection(SELECTION_MOVE_TOP)
		elseif keycode == Keys.End then
			self:moveSelection(SELECTION_MOVE_BOTTOM)
		elseif keycode == Keys.Up then
			self:moveSelection(SELECTION_MOVE_UP)
		elseif keycode == Keys.Down then
			self:moveSelection(SELECTION_MOVE_DOWN)
		elseif keycode == Keys.Right then
			self:moveSelection(1)
		elseif keycode == Keys.Left then
			self:moveSelection(-1)
		elseif keycode == Keys.PageUp then
			self:moveSelection(SELECTION_MOVE_PAGE_UP)
		elseif keycode == Keys.PageDown then
			self:moveSelection(SELECTION_MOVE_PAGE_DOWN)
		elseif keycode == Keys.Escape then
			self:moveSelection()
		else
			return EVENT_PROPAGATE
		end

		return EVENT_STOP
	end
	
	function GridList:getItemIndexFromXY(x, y)
		local rowIndex = self:getRowIndexFromY(y - self.y)
		return self:getItemIndexFromX(x - self.x, rowIndex)
	end
	
	function GridList:onMouseMove(x, y)

		local oldIndex = self.mouseHoverIndex
		local oldSelection = self.selectedIndex
		local mouseHoverIndex = self.mouseHoverIndex
		local selectedIndex = self.selectedIndex
		
		if not self:isPointWithin(x, y) and not self.mouseSelectionDrag then
			self:onHover(nil)
			return EVENT_PROPAGATE
		end

		local rowIndex = self:getRowIndexFromY(y - self.y)
		self.mouseHoverRow = rowIndex
		mouseHoverIndex = self:getItemIndexFromX(x - self.x, math.floor(rowIndex or 0))
		
		if self.mouseSelectionDrag then
			selectedIndex = mouseHoverIndex
		end

		self:onHover(mouseHoverIndex)
	
		if self.mouseSelectionDrag  then
			self:onSelect(selectedIndex)
		end
		
		self.hoverLastX = x
		self.hoverLastY = y
		
		return EVENT_STOP
	end
	
	function GridList:onMouseDown(button, x, y)

		if not self:isPointWithin(x, y) then
			return EVENT_PROPAGATE
		end	
		
		if button == Mouse.Left then
		
			self.mouseSelectionDrag = true
			self:onSelect(self.mouseHoverIndex)
			
			local time = os.clock()
			
			if time - self.lastClickTick < 0.45 and
			self.lastClickIndex == self.mouseHoverIndex and
			self.onDoubleClickCallback then
				local hoverIndex = self.mouseHoverIndex
				self.onDoubleClickCallback(hoverIndex)
				self.lastClickTick = 0
			else
				self.lastClickTick = time
			end

			self.lastClickIndex = self.mouseHoverIndex
		end
	
		return EVENT_FOCUS
	end
	
	function GridList:onMouseUp(button, x, y)

		if button == Mouse.Left then
			self.mouseSelectionDrag = nil
		end
		
		if not self:isPointWithin(x, y) then
			return EVENT_PROPAGATE
		end
	
	end
	
	function GridList:onMouseWheel(delta, x, y)
		
		if not self:isPointWithin(x, y) then
			return EVENT_PROPAGATE
		end
		
		local currentVerticalPosition = self.verticalPosition
		
		self:moveVerticalPosition(-delta * (self.movementDeltaMultiplier or MOVEMENT_DELTA_MULTPLIER))
		self:onMouseMove(x, y)
		
		if self.mouseSelectionDrag then
			self:onSelect(self.selectedIndex)
		end
	end
	
	function GridList:onMouseLeft()
		self:onHover(nil)
	end
	
	function GridList:onMouseOut()
		self:onHover(nil)
	end

	function GridList:onUpdate(time, delta)
		if self.nextSelection then
			self:onSelect(self.nextSelection, true)
		end
	end
	
	function GridList:onDraw(window)
	
		if not self.renderTexture then
			return
		end
	
		if not self.displayUpdate then
			window:translateView(-self.x, -self.y)
			self.renderTexture:draw(window)
			window:translateView(self.x, self.y)
			return
		end

		self.displayUpdate = false
		
		self.renderTexture:setActive(true)
		Graphics.setTarget(self.renderTexture)
		
		self.renderTexture:clear(self.innerColor)
		Graphics.setColor(self.outerColor)
		Graphics.rectangle(false, 0, 0, self.width, self.height)
		
		local gapWidth = self.itemGapWidth or ITEM_GAP_WIDTH
		local gapHeight = self.itemGapHeight or ITEM_GAP_HEIGHT
		local verticalPosition = self.verticalPosition
		local indexFrom = math.floor(verticalPosition)
		local indexTo = math.ceil(verticalPosition + self:getVisibleRows()) - 1
		local itemY = verticalPosition == 1 and VERTICAL_MARGIN or 0
		local itemsList = self.itemsList
		local rows = self.rows
		local lastIndex = #rows
		local selectedIndex = math.floor(self.nextSelection or self.selectedIndex or 0)
		local hoverIndex = math.floor(self.mouseHoverIndex or 0)
		local setColor = Graphics.setColor
		local rectangle = Graphics.rectangle

		local selectionBackgroundColor = self.SELECTION_BACKGROUND_COLOR or SELECTION_BACKGROUND_COLOR
		local hoverBackgroundColor = self.HOVER_BACKGROUND_COLOR or HOVER_BACKGROUND_COLOR
		local selectionBorderColor = self.SELECTION_BORDER_COLOR or SELECTION_BORDER_COLOR
		local hoverBorderColor = self.HOVER_BORDER_COLOR or HOVER_BORDER_COLOR
		
		for index = indexFrom, indexTo do
			local row = rows[index]
			local itemX = HORIZONTAL_MARGIN
			local rowHeight = self:getRowHeight(index)
			if index == indexFrom then
				itemY = itemY - (verticalPosition - indexFrom) * rowHeight
			end
			itemY = itemY + (index > 1 and gapHeight/2 or 0)
			local itemIndexFrom = row.indexFrom
			local itemIndexTo = row.indexTo
			for itemIndex = itemIndexFrom, itemIndexTo do
				local item = itemsList[itemIndex]
				local itemWidth, itemHeight = self:getItemWidth(itemIndex), self:getItemHeight(itemIndex)
				local boxIncrease = 3
				local x, y, w, h = -boxIncrease + math.floor(itemX), -boxIncrease +
					math.floor(itemY), itemWidth + 2 * boxIncrease,
					itemHeight + 2 * boxIncrease
				if selectedIndex == itemIndex or hoverIndex == itemIndex then
					setColor(selectedIndex == itemIndex and selectionBackgroundColor or hoverBackgroundColor)
					rectangle(true, x, y, w, h)
					setColor(hoverIndex == itemIndex and hoverBorderColor or selectionBorderColor)
					rectangle(false, x, y, w, h)
				end

				if item then
					item:setPosition(itemX, itemY)
					item:onDraw(self.renderTexture)
				end

				itemX = itemX + itemWidth + gapWidth
			end
			
			itemY = itemY + rowHeight - (index > 1 and gapHeight/2 or 0)
		end

		self.renderTexture:display()
		
		Graphics.setTarget(window)
		window:setActive(true)
		self:onDraw(window)
	end