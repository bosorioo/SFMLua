local require_name, full_path = ...

if not require_name then
	require_name = ""
	full_path = ""
end

local dir = full_path:match("(.+[\\/]).-$") or full_path

local fonts = {
	Font(dir .. "sansation.ttf"),
	Font(dir .. "opensans.ttf"),
	Font(dir .. "magnus.ttf")
}

Graphics.setFont(fonts[2])
Resources = {}
R = Resources

R.getFont = function(fontId)
	return fonts[fontId]
end

R.setDefaultFont = function(fontId)
	if fontId == 1 or not fonts[fontId] then
		return
	end
	fonts[1], fonts[fontId] = fonts[fontId], fonts[1]
end

R.color = {
	BLACK = {0, 0, 0},
	WHITE = {255, 255, 255},
	GRAY = {128, 128, 128},
	TRANSPARENT = {0, 0, 0, 0}
}

COLOR_BLACK = R.color.BLACK

R.setDefaultFont(DEFAULT_FONT or 1)