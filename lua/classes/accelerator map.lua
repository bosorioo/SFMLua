local accelerators = {}
local isEnabled = true
local ALT = 1
local CTRL = 2
local SHIFT = 4

local function void() end

AcceleratorMap = {

	getVoidFunction = function()
		return void
	end,
	
	setAccelerator = function (accelString, obj)

		local modifier = 0
		local accel
		accelString = accelString:gsub("%s", "")	

		for key in accelString:gmatch("[^%+]+") do
			local lkey = key:lower()
			if lkey == "alt" then
				modifier = (modifier | ALT)
			elseif lkey == "ctrl" then
				modifier = (modifier | CTRL)
			elseif lkey == "shift" then
				modifier = (modifier | SHIFT)
			elseif tonumber(key) then
				accel = key
			elseif Keys[key] then
				accel = Keys[key]
			end
		end
	
		if not accel then
			return
		end
	
		if not accelerators[modifier] then
			accelerators[modifier] = {}
		end
	
		accelerators[modifier][accel] = obj
		return true
	end,

	getAcceleratorObject = function(key, alt, shift, ctrl, isAscii)
	
		if not isEnabled then
			return
		end

		local modifier = 0
		if alt then modifier = (modifier | ALT) end
		if ctrl then modifier = (modifier | CTRL) end
		if shift then modifier = (modifier | SHIFT) end
		
		if not isAscii then
			if key >= Keys.Num0 and key <= Keys.Num9 then
				key = tostring(key - Keys.Num0)
			elseif key >= Keys.Numpad0 and key <= Keys.Numpad9 then
				key = tostring(key - Keys.Numpad0)
			end
		end

		local accel = accelerators[modifier]
		return accel and accel[key]
	end,

	setEnabled = function (bool)
		isEnabled = bool
	end
}