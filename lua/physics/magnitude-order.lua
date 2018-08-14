local low_orders = {'m', 'u', 'n', 'p', 'f', 'a'}
local high_orders = {'k', 'M', 'G', 'T', 'P', 'E'}

local MagnitudeOrder = {

	normalize = function (number)
		local order = 0
		local abs = math.abs(number)
		if abs < 1 then
			while abs < 1 and order < #low_orders do
				abs = abs * 1000
				order = order + 1
			end
			order = low_orders[order]
		elseif abs >= 1000 then
			while abs >= 1000 and order < #high_orders do
				abs = abs * .001
				order = order + 1
			end
			order = high_orders[order]
		else
			return tostring(number), ''
		end
		return abs * (number < 0 and -1 or 1), order or ''
	end

}

return MagnitudeOrder
