Class "List" : extends "UIControl"

local VERTICAL_MARGIN = 12
local HORIZONTAL_MARGIN = 10
local ITEM_GAP_HEIGHT = 20
local MOVEMENT_DELTA_MULTPLIER = 1
local PAGE_UP_DOWN_MOVE_PERCENT = .1
local SELECTION_MOVE_TOP = -9901
local SELECTION_MOVE_BOTTOM = -9902
local HOVER_BORDER_COLOR = 0xFF121212
local HOVER_BACKGROUND_COLOR = 0x14000000
local SELECTION_BORDER_COLOR = 0xFF5765E6
local SELECTION_BACKGROUND_COLOR = 0x555765E6

local function floor(n)
	if not tonumber(n) then return n end
	return math.floor(n)
end

function List:constructor(width, height, x, y)
    self.super(x, y, width or 80, height or 160)
    self.verticalPosition = 1
    self.visibleItemsList = {}
    self.itemsList = {}
    self.nextSelection = nil
    self.mouseSelectionIndex = nil
    self.mouseSelectionIndexOld = nil
    self.mouseHoverPosition = nil
    self.mouseHoverIndex = nil
    self.mouseSelectionDrag = nil
    self.selectedFontColor = R.color.WHITE
    self.unselectedFontColor = R.color.BLACK

    if self.width > 0 and self.height > 0 then
        self.renderTexture = RenderTexture(self.width, self.height)
        self.renderTexture:clear(0xFFFFFFFF)
    end
    self.displayUpdate = false

    self.visibleItems = 0
    self.visibleItemsUpdate = true

    self.maxVerticalPosition = 1
    self.maxVerticalPositionUpdate = false

    self.onSelect = nil
    self.onHover = nil

    self.innerColor = RGBA(255, 255, 255)
    self.outerColor = RGBA(0, 0, 120)

    self:move(x or 0, y or 0)

    self.onSelectCallback = nil
    self.onHoverCallback = nil

    self.movementDeltaMultiplier = nil
end

function List:destructor()

end

--[[ User Functions ]]--

function List:getSize()
    return #self.itemsList
end

function List:setItemVisible(index, bool)

    local item = self:getItem(index)

    if item then
        if item.visible == bool then
            return
        end
        item:setVisible(bool)
    end

    self.maxVerticalPositionUpdate = true
    self.visibleItemsUpdate = true
    self:repaint()

    local visList = self.visibleItemsList
    local visibleListIndex
    local mouseSelectionIndex = self.nextSelection or self.mouseSelectionIndex

    if bool then

        visibleListIndex = select(2, table.binarysearch(visList, index))
        table.insert(visList, visibleListIndex, index)

        if mouseSelectionIndex and visibleListIndex <= mouseSelectionIndex then
            self.nextSelection = mouseSelectionIndex + 1
        end

    else
        visibleListIndex = table.binarysearch(visList, index)
        if visibleListIndex then

            table.remove(visList, visibleListIndex)

            if visibleListIndex < self.verticalPosition then
                self.verticalPosition = self.verticalPosition - 1
            end

            if not mouseSelectionIndex then
                self:correctVerticalPosition()
            elseif mouseSelectionIndex == visibleListIndex then
                local v = visList[mouseSelectionIndex]
                if not v then
                    self:onSelect(nil)
                else
                    local top = math.abs((visList[mouseSelectionIndex - 1] or math.huge) - v)
                    local bot = math.abs((visList[mouseSelectionIndex + 1] or math.huge) - v)
                    if top >= bot then
                        self.nextSelection = mouseSelectionIndex - 1
                    else
                        self.nextSelection = mouseSelectionIndex + 1
                    end
                end
            elseif mouseSelectionIndex > visibleListIndex then
                self.nextSelection = mouseSelectionIndex - 1
            end
        end
    end
end

function List:getItem(index)
    return self.itemsList[index]
end

function List:getSelectedItem()
    return self.visibleItemsList[self.nextSelection or self.mouseSelectionIndex]
end

function List:getSelectedIndex()
    return self.visibleItemsList[self.nextSelection or self.mouseSelectionIndex]
end

function List:getVisibleIndexByRealIndex(index)
    return table.binarysearch(self.visibleItemsList, index)
end

function List:repaint()
    self.displayUpdate = true
end

function List:addItem(obj)

    local objIndex = #self.itemsList + 1
    self.itemsList[objIndex] = obj
    self.visibleItemsList[#self.visibleItemsList + 1] = objIndex
    obj.parent = self

    self.maxVerticalPositionUpdate = true
    self.visibleItemsUpdate = true
    self:repaint()
end

function List:removeItem(obj)

    if not tonumber(obj) then
        for i, item in pairs (self.itemsList) do
            if item == obj then
                return self:removeItem(i)
            end
        end
        return
    end

    local mouseSelectionIndex = self.nextSelection or self.mouseSelectionIndex

    table.remove(self.itemsList, tonumber(obj))
    index = tonumber(obj)
    local visibleListIndex = table.binarysearch(self.visibleItemsList, obj)

    if visibleListIndex then
        table.remove(self.visibleItemsList, visibleListIndex)
    end

    if mouseSelectionIndex == index then
        self:onSelect(nil)
    elseif self.mouseSelectionIndex > index then
        self.nextSelection = mouseSelectionIndex - 1
    end

    self.maxVerticalPositionUpdate = true
    self.visibleItemsUpdate = true
    self:repaint()
    self:correctVerticalPosition()

end

function List:clear()
    self.itemsList = {}
    self.visibleItemsList = {}
    self.verticalPosition = 1
    self:onSelect(nil)
    self:onHover(nil)
    self.visibleItems = 0
    self.maxVerticalPosition = 1
    if self.renderTexture then
        self.renderTexture:clear(0xFFFFFFFF)
    end
end

function List:getItemHeight(index, excludeGaps)
    local visList = self.visibleItemsList
    local item = self.itemsList[visList[index]]
    if item and item.visible then
        local height = item.getHeight and item:getHeight() or item.height or 0
        local gapHeight = excludeGaps and 0 or (self.itemGapHeight or ITEM_GAP_HEIGHT)
        if index == 1 or index == #visList then
            return height + gapHeight / 2
        else
            return height + gapHeight
        end
    end
    return 0
end

function List:addStringItem(str, size, font)
    local item = Label(str, size, font)
    self:addItem(item)
end

function List:move(x, y)
    self:setPosition(self.x + x, self.y + y)
end

function List:setPosition(x, y)
    self.x = math.floor(x)
    self.y = math.floor(y)
end

function List:setSize(width, height)

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

    self.visibleItemsUpdate = true
    self.maxVerticalPositionUpdate = true
    self:repaint()
    self:correctVerticalPosition()
    self:viewSelectedItem()
end

function List:getVisibleItems()

    if not self.visibleItemsUpdate then
        return self.visibleItems
    end

    local visibleItems = 0
    local verticalPosition = self.verticalPosition
    local index = math.floor(verticalPosition)
    local heightLeft = self.height - 2
    local lastIndex = #self.visibleItemsList
    local visList = self.visibleItemsList
    local gapHeight = self.itemGapHeight or ITEM_GAP_HEIGHT

    repeat
        local itemHeight = self:getItemHeight(index)
        if itemHeight > 0 then
            if index == 1 or index == lastIndex then
                itemHeight = itemHeight + VERTICAL_MARGIN
            end
            if index == math.floor(verticalPosition) then
                local totalHeight = itemHeight
                itemHeight = math.floor(itemHeight * (1 - (verticalPosition - index)))
                visibleItems = visibleItems + itemHeight / totalHeight
            else
                visibleItems = visibleItems + 1
            end
            heightLeft = heightLeft - itemHeight
            if heightLeft < 0 then
                visibleItems = visibleItems + (heightLeft/itemHeight)
            end
        elseif index > lastIndex then
            visibleItems = lastIndex
            break
        end
        index = index + 1
    until heightLeft <= 0

    self.visibleItemsUpdate = nil
    self.visibleItems = visibleItems
    return visibleItems
end

function List:setVerticalPosition(position)
    if self.verticalPosition ~= position then
        self.visibleItemsUpdate = true
        self:repaint()
        self.verticalPosition = position
        self:correctVerticalPosition()
    end
end

function List:moveVerticalPosition(delta)
    if math.abs(delta) > 0 then
        self.verticalPosition = self.verticalPosition + delta
        self.visibleItemsUpdate = true
        self:repaint()
        self:correctVerticalPosition()
    end
end

function List:setOnSelect(callback)
    self.onSelectCallback = callback
end

function List:setOnHover(callback)
    self.onHoverCallback = callback
end

function List:setHoverColor(background, border)
    self.HOVER_BACKGROUND_COLOR = RGBA(background)
    self.HOVER_BORDER_COLOR = RGBA(border)
end

function List:setSelectionColor(background, border)
    self.SELECTION_BACKGROUND_COLOR = RGBA(background)
    self.SELECTION_BORDER_COLOR = RGBA(border)
end

function List:setWheelMultiplier(m)
    self.movementDeltaMultiplier = m
end

--[[ Internal Functions ]]--

function List:correctVerticalPosition(t)

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

function List:getMaxVerticalPosition(specificIndex)

    if specificIndex and specificIndex > #self.itemsList then
        specificIndex = nil
    end

    if not self.maxVerticalPositionUpdate and not specificIndex then
        return self.maxVerticalPosition
    end

    local lastIndex = #self.visibleItemsList
    local index = specificIndex and math.floor(specificIndex) or lastIndex
    local heightLeft = self.height - 2
    local itemHeight
    local visList = self.visibleItemsList
    local gapHeight = self.itemGapHeight or ITEM_GAP_HEIGHT

    repeat
        itemHeight = self:getItemHeight(index)
        if itemHeight > 0 then
            if index == 1 or index == lastIndex then
                itemHeight = itemHeight + VERTICAL_MARGIN
            end
            heightLeft = heightLeft - itemHeight
            if heightLeft <= 0 then
                index = index - heightLeft/itemHeight
                break
            end
        end
        index = index - 1
        if index <= 0 then
            index = 1
            break
        end
    until false

    if specificIndex then
        return index
    end

    self.maxVerticalPosition = index
    self.maxVerticalPositionUpdate = nil
    return index
end

function List:onSelect(index)

    local visList = self.visibleItemsList

    index = index == 0 and 1 or
        index == #self.visibleItemsList + 1 and
        #self.visibleItemsList or tonumber(index) and
        math.floor(index)

    local realIndex = visList[index]

    if not realIndex then
        self.mouseSelectionIndex = nil
    else
        self.mouseSelectionIndex = index
    end

    if realIndex ~= self.mouseSelectionIndexOld then
        if self.onSelectCallback then
            self.onSelectCallback(realIndex, self.mouseSelectionIndexOld)
        end
        self:repaint()
        self.mouseSelectionIndexOld = realIndex
    end

    self:viewSelectedItem()
    self:correctVerticalPosition()
    self.nextSelection = nil
end

function List:onHover(position)

    self.mouseHoverPosition = position

    local visList = self.visibleItemsList
    local index = index == 0 and 1 or
        index == #visList + 1 and
        #visList or tonumber(position) and
        math.floor(position)

    local realIndex = visList[index]

    if not realIndex then
        self.mouseHoverIndex = nil
    else
        self.mouseHoverIndex = realIndex
    end

    if realIndex ~= self.mouseHoverIndexOld then
        self:repaint()
        if self.onHoverCallback then
            self.onHoverCallback(realIndex, self.mouseHoverIndexOld)
        end
        self.mouseHoverIndexOld = realIndex
    end
end

function List:getItemIndexFromY(y)

    if y < 0 or y > self.height then
        return
    end

    local indexFrom = math.floor(self.verticalPosition)
    local indexTo = indexFrom + math.ceil(self:getVisibleItems())
    local lastIndex = #self.visibleItemsList
    local visList = self.visibleItemsList
    local getItemHeight = self.getItemHeight
    local gapHeight = self.itemGapHeight or ITEM_GAP_HEIGHT

    for index = indexFrom, indexTo do
        local itemHeight = getItemHeight(self, index)
        if index == 1 or index == lastIndex then
            itemHeight = itemHeight + VERTICAL_MARGIN
        end
        if itemHeight > 0 then
            if index == indexFrom then
                itemHeight = itemHeight * (1 - (self.verticalPosition - indexFrom))
            end
            y = y - itemHeight
            if y <= 0 then
                return (index >= 1 and index <= lastIndex and (1 + index + y / itemHeight))
            end
        end
    end
end

function List:isPointWithin(x, y)
    return x >= self.x and x <= self.x + self.width and y >= self.y and y <= self.y + self.height
end

function List:isSelectedItemVisible()

    local mouseSelectionIndex = self.nextSelection or self.mouseSelectionIndex

    if not mouseSelectionIndex then
        return false
    end

    local visibleFrom = self.verticalPosition
    local visibleTo = visibleFrom + self:getVisibleItems() - 1
    local gapHeight = self.itemGapHeight or ITEM_GAP_HEIGHT
    local extraSpace = gapHeight / 4 / self:getItemHeight(mouseSelectionIndex)

    return mouseSelectionIndex >= extraSpace + visibleFrom and
        mouseSelectionIndex <= visibleTo - extraSpace
end

function List:viewSelectedItem()

    local mouseSelectionIndex = self.nextSelection or self.mouseSelectionIndex

    if not mouseSelectionIndex or self:isSelectedItemVisible() then
        return
    end

    local currentVerticalPosition = self.verticalPosition
    local newVerticalPosition = currentVerticalPosition
    local gapHeight = self.itemGapHeight or ITEM_GAP_HEIGHT
    local extraSpace = gapHeight/ 4 / self:getItemHeight(mouseSelectionIndex)

    if mouseSelectionIndex <= currentVerticalPosition then
        newVerticalPosition = mouseSelectionIndex - extraSpace
    else
        newVerticalPosition = self:getMaxVerticalPosition(mouseSelectionIndex) + extraSpace
    end

    self:moveVerticalPosition(newVerticalPosition - currentVerticalPosition)

    if self.mouseHoverIndex then
        self:onHover(self.mouseHoverPosition + self.verticalPosition - currentVerticalPosition)
    end

end

function List:moveSelection(delta)

    local mouseSelectionIndex = self.nextSelection or self.mouseSelectionIndex
    local minIndex = 1
    local maxIndex = #self.visibleItemsList

    if delta == SELECTION_MOVE_BOTTOM then
        mouseSelectionIndex = maxIndex
        delta = 0
    elseif delta == SELECTION_MOVE_TOP then
        mouseSelectionIndex = minIndex
        delta = 0
        directionUp = false
    elseif not delta then
        self:onSelect(nil)
        return
    end

    if not mouseSelectionIndex then
        return
    end

    mouseSelectionIndex = math.floor(mouseSelectionIndex + delta)

    if mouseSelectionIndex < minIndex then
        mouseSelectionIndex = minIndex
    elseif mouseSelectionIndex > maxIndex then
        mouseSelectionIndex = maxIndex
    end

    self:onSelect(mouseSelectionIndex)
end

function List:onKeyDown(keycode)

    if keycode == Keys.Home then
        self:moveSelection(SELECTION_MOVE_TOP)
    elseif keycode == Keys.End then
        self:moveSelection(SELECTION_MOVE_BOTTOM)
    elseif keycode == Keys.Up then
        self:moveSelection(-1)
    elseif keycode == Keys.Down then
        self:moveSelection(1)
    elseif keycode == Keys.PageUp then
        self:moveSelection(math.floor(#self.itemsList * -PAGE_UP_DOWN_MOVE_PERCENT))
    elseif keycode == Keys.PageDown then
        self:moveSelection(math.ceil(#self.itemsList * PAGE_UP_DOWN_MOVE_PERCENT))
    elseif keycode == Keys.Escape then
        self:moveSelection()
    else
        return EVENT_PROPAGATE
    end

    return EVENT_STOP
end

function List:onMouseMove(x, y)


    if not self:isPointWithin(x, y) and not self.mouseSelectionDrag then
        self:onHover(nil)
        return EVENT_PROPAGATE
    end

    local newHoverPosition = self:getItemIndexFromY(y - self.y)
    self:onHover(newHoverPosition)

    if self.mouseSelectionDrag then
        if newHoverPosition then
            self:onSelect(newHoverPosition)
        else
            local mouseSelection = self.nextSelection or self.mouseSelectionIndex
            if mouseSelection then
                newHoverPosition = y >= self.y + self.height and mouseSelection + 1 or
                    y <= 0 and mouseSelection - 1
                if newHoverPosition then
                    self:onSelect(newHoverPosition)
                end
            end
        end
    end

    return EVENT_STOP
end

function List:onMouseDown(button, x, y)

    if not self:isPointWithin(x, y) then
        return EVENT_PROPAGATE
    end

    if button == Mouse.Left then
        local mouseSelectionIndex = self:getItemIndexFromY(y - self.y)
        self.mouseSelectionDrag = true
        self:onSelect(mouseSelectionIndex)
    end

    return EVENT_FOCUS
end

function List:onMouseUp(button, x, y)

    if button == Mouse.Left then
        self.mouseSelectionDrag = nil
    end

    if not self:isPointWithin(x, y) then
        return EVENT_PROPAGATE
    end

end

function List:onMouseWheel(delta, x, y)

    if not self:isPointWithin(x, y) then
        return EVENT_PROPAGATE
    end

    local currentVerticalPosition = self.verticalPosition

    self:moveVerticalPosition(-delta * (self.movementDeltaMultiplier or MOVEMENT_DELTA_MULTPLIER))

    local oldHoverIndex = self.mouseHoverIndex
    local mouseHoverIndex = self.mouseHoverIndex

    if self.mouseSelectionDrag then
        self:moveSelection(self.verticalPosition - currentVerticalPosition)
        mouseHoverIndex = self.nextSelection or self.mouseSelectionIndex
    elseif mouseHoverIndex then
        mouseHoverIndex = math.floor(self:getItemIndexFromY(y - self.y))
    end

    if mouseHoverIndex ~= oldHoverIndex then
        self:onHover(mouseHoverIndex)
    end

end

function List:onMouseLeft()
    self:onHover(nil)
end

function List:onMouseOut()
    self:onHover(nil)
end

function List:onUpdate(time, delta)
    if self.nextSelection then
        self:onSelect(self.nextSelection)
    end
end

function List:onDraw(window)

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

    local floor = math.floor
    local verticalPosition = self.verticalPosition
    local indexFrom = floor(verticalPosition)
    local indexTo = indexFrom + math.ceil(self:getVisibleItems())
    local itemY = 0
    local visList = self.visibleItemsList
    local itemsList = self.itemsList
    local lastIndex = #visList
    local self_x = 0
    local self_width = self.width
    local mouseSelectionIndex = floor(self.nextSelection or self.mouseSelectionIndex or 0)
    local hoverIndex = floor(self.mouseHoverIndex or 0)
    local setColor = Graphics.setColor
    local rectangle = Graphics.rectangle
    local gapHeight = self.itemGapHeight or ITEM_GAP_HEIGHT
    local boxMarginVertical = math.ceil(gapHeight / 2) - 2
    local boxMarginHorizontal = 4

    local selectionBackgroundColor = self.SELECTION_BACKGROUND_COLOR or SELECTION_BACKGROUND_COLOR
    local hoverBackgroundColor = self.HOVER_BACKGROUND_COLOR or HOVER_BACKGROUND_COLOR
    local selectionBorderColor = self.SELECTION_BORDER_COLOR or SELECTION_BORDER_COLOR
    local hoverBorderColor = self.HOVER_BORDER_COLOR or HOVER_BORDER_COLOR

    for index = indexFrom, indexTo do
        local realIndex = visList[index]
        local item = itemsList[realIndex]
        local itemHeight = self:getItemHeight(index)
        if index == 1 then
            itemY = VERTICAL_MARGIN
        end
        if index == indexFrom then
            itemY = itemY - (verticalPosition - indexFrom) * (itemHeight + (index == 1 and VERTICAL_MARGIN or 0))
        end
        if index > 0 and index <= #visList then
            local x, y, w, h = self_x + HORIZONTAL_MARGIN - boxMarginHorizontal + 1,
                math.floor(itemY) - boxMarginVertical + ((index > 1 and index <= lastIndex) and gapHeight / 2 or 0),
                self_width - 2 * (HORIZONTAL_MARGIN - boxMarginHorizontal + 1),
                self:getItemHeight(index, true) + 2 * boxMarginVertical
            if mouseSelectionIndex == index or hoverIndex == realIndex then
                setColor(mouseSelectionIndex == index and selectionBackgroundColor or hoverBackgroundColor)
                rectangle(true, x, y, w, h)
                setColor(hoverIndex == realIndex and hoverBorderColor or selectionBorderColor)
                rectangle(false, x, y, w, h)
            end
        end

        if item then
            if index == 1 then
                item:setPosition(self_x + HORIZONTAL_MARGIN, math.floor(itemY))
            else
                item:setPosition(self_x + HORIZONTAL_MARGIN, math.floor(itemY + gapHeight / 2))
            end
            item:onDraw(self.renderTexture)
        end

        itemY = itemY + itemHeight
    end

    Graphics.setColor(self.outerColor)
    Graphics.rectangle(false, 1, 1, self.width - 2, self.height - 2)
    self.renderTexture:display()

    Graphics.setTarget(window)
    window:setActive(true)
    self:onDraw(window)

end
