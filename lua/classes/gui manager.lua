Class "GuiManager"
Class "UIControl"

require "libs.classes.accelerator map"

EVENT_FOCUS = 2
EVENT_PROPAGATE = 4
EVENT_STOP = 8

function GuiManager:constructor()
	self.alwaysOnTopControls = {}
	self.controls = {}
	self.controls_names = {}
	self.mouseMoveFocus = nil
	self.controlFocus = nil
	self.alwaysOnTopFocus = nil
	self.eventCallbacks = nil
	self.window = nil
end

function GuiManager:setWindow(window)
	self.window = window
end

function GuiManager:addControls(...)
	local index = #self.controls
	local controlName = nil
	for _, control in ipairs ({...}) do
		if type(control) == "string" then
			controlName = control
		else
			index = index + 1
			control.controlIndex = index
			control.parent = self
			table.insert(self.controls, control)

			if controlName then
				control.controlName = controlName
				self.controls_names[controlName] = index
				controlName = nil
			end
		end
	end
	return self
end

function GuiManager:removeControl(control)
	local count = #self.controls
	for i = 1, count do
		if self.controls[i] == control then
			table.remove(self.controls, i)
			if control.controlName and self.controls_names[control.controlName] then
				self.controls_names[control.controlName] = nil
			end
			return
		end
	end
	return self
end

function GuiManager:getControlByName(name)
	local index = self.controls_names[name]
	if not index or index == 0 or index > #self.controls then
		return
	end
	return self.controls[index]
end

function GuiManager:addEventCallback(event, callback)
	if not self.eventCallbacks then self.eventCallbacks = {} end
	if not self.eventCallbacks[event] then self.eventCallbacks[event] = {} end
	local callbacks = self.eventCallbacks[event]
	callbacks[#callbacks + 1] = callback
end

function GuiManager:setAccelerator(accelString, func)
	AcceleratorMap.setAccelerator(accelString, func)
end

function GuiManager:getAccelerator(key, alt, shift, ctrl, isAscii)
	AcceleratorMap.getAcceleratorObject(key, alt, shift, ctrl, isAscii)
end

function GuiManager:draw(...)

	local controls = self.controls
	
	for index = #controls, 1, -1 do
		local control = controls[index]
		if control.visible ~= false and control.onDraw then
			control.onDraw(control, ...)
		end
	end
	
	controls = self.alwaysOnTopControls
	
	for index = #controls, 1, -1 do
		local control = controls[index]
		if control.visible ~= false and control.onDraw then
			control.onDraw(control, ...)
		end
	end
	
end

function GuiManager:processEvent(event)

	if event.Closed then
		self:onEvent("onClose")
	elseif event.KeyPressed then
		self:onEvent("onKeyDown", event.keycode, event.alt, event.shift, event.control)
	elseif event.KeyReleased then
		self:onEvent("onKeyUp", event.keycode, event.alt, event.shift, event.control)
	elseif event.MouseMoved then
		self:onEvent("onMouseMove", event.x, event.y)
	elseif event.MousePressed then
		self:onEvent("onMouseDown", event.button, event.x, event.y)
	elseif event.MouseReleased then
		self:onEvent("onMouseUp", event.button, event.x, event.y)
	elseif event.Resized then
		self:onEvent("onResize", event.width, event.height) 
	elseif event.MouseWheel then
		self:onEvent("onMouseWheel", event.delta, Mouse.getPosition(self.window))
	elseif event.TextEntered then
		self:onEvent("onTextEntered", event.unicode)
	elseif event.FocusOn then
		self:onEvent("onFocusOn")
	elseif event.FocusOff then
		self:onEvent("onFocusOff")
	elseif event.MouseLeft then
		self:onEvent("onMouseLeft")
	elseif event.MouseEntered then
		self:onEvent("onMouseEntered")
	end

end

local accelMapsetEnable = AcceleratorMap.setEnabled
function GuiManager:onEvent(event, ...)

	if self.eventCallbacks then
		local callbacks = self.eventCallbacks[event]
		if callbacks then
			for _, callback in pairs (callbacks) do
				callback(...)
			end
		end
	end
	
	if event == "onDraw" then
		return self:draw(...)
	end

	local index, control, answer = 1
	local isMouseMoveEvent = event == "onMouseMove"
	
	if event == "onMouseWheel" then
		if self.mouseMoveFocus and self.mouseMoveFocus.onMouseWheel then
		   self.mouseMoveFocus.onMouseWheel(self.mouseMoveFocus, ...)
		end
		return
	end
	
	accelMapsetEnable(true)
	
	local newMouseFocus
	
	for i = 1, 2 do
	local controls = i == 1 and self.alwaysOnTopControls or self.controls
	index = 1
	repeat
	
		answer = EVENT_PROPAGATE--self:getDefaultEventAnswer(event)
		control = controls[index]
		
		if not control then
			break
		end

		if control[event] then
			answer = tonumber(control.visible == false and EVENT_PROPAGATE or
				control[event](control, ...)) or answer
		end

		if isMouseMoveEvent and answer ~= EVENT_PROPAGATE then
			newMouseFocus = control
		end
			
		if not answer or answer == EVENT_STOP then
			break
		else
		
			local isUpdateEvent = event == "onUpdate"
		
			if answer == EVENT_FOCUS then 
			
				local oldFocus = self.controlFocus
				local newFocus = oldFocus
			
				if i == 1 then -- always on top control
					if not isUpdateEvent and oldFocus ~= control  then
						table.remove(controls, index)
						table.insert(controls, 1, index)
						newFocus = control
					end
				elseif isUpdateEvent then -- other control not alwaysOnTop, but is promoting to it
					table.remove(controls, index)
					index = index - 1
					table.insert(self.alwaysOnTopControls, control)
					if #self.alwaysOnTopControls == 1 then
						newFocus = control
					end
				elseif #self.alwaysOnTopControls == 0 and self.controlFocus ~= control then 
				-- other control not alwaysOnTop, but wants focus
				-- they cant gain focus if there is an alwaysOnTop
					table.remove(controls, index)
					table.insert(controls, 1, control)
					newFocus = control
				end
				if oldFocus ~= newFocus then
					if oldFocus and oldFocus.onLoseFocus then
						oldFocus:onLoseFocus()
					end
					self.controlFocus = newFocus
					if newFocus.onGainFocus then
						newFocus:onGainFocus()
					end
				end
			elseif isUpdateEvent and i == 1 then -- it is an alwaysOnTop control during an onUpdate that
			-- no longer returns propagation flag, so it wants demotion from alwaysOnTop
				table.remove(controls, index)
				index = index - 1
				table.insert(self.controls, control)
				if self.controlFocus == control then
					if control.onLoseFocus then
						control:onLoseFocus()
					end
					self.controlFocus = controls[index + 1] or self.controls[1]
					if self.controlFocus then
						if self.controlFocus.onGainFocus then
							self.controlFocus:onGainFocus()
						end
					end
				end
			end

			if answer == EVENT_PROPAGATE then
				index = index + 1
			else
				break
			end
		end
	until answer ~= EVENT_PROPAGATE end
	
	if isMouseMoveEvent and newMouseFocus ~= self.mouseMoveFocus then
	
		if self.mouseMoveFocus and self.mouseMoveFocus.onMouseOut then
			self.mouseMoveFocus:onMouseOut()
		end
		
		self.mouseMoveFocus = newMouseFocus
		
		if newMouseFocus and newMouseFocus.onMouseIn then
			newMouseFocus:onMouseIn()
		end
		
	elseif event == "onMouseLeft" then
	
		self.mouseMoveFocus = nil
		
	elseif event == "onKeyDown" then
	
		local accelObj = AcceleratorMap.getAcceleratorObject(...)
		
		if accelObj and type(accelObj) == "function" then
			accelObj()
		end	
		
	end
	
end

function UIControl:constructor(x, y, w, h)
	self.x, self.y, self.width, self.height = x or 0, y or 0, w or 0, h or 0
	self.visible = true
end

function UIControl:isVisible()
	return self.visible
end

function UIControl:setVisible(bool)
	if bool then
		self.visible = true
	else
		self.visible = nil
	end
end