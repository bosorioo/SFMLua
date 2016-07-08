local files = {
	"resources",
	{"utils",
		"functions",
		"table compiler",
	},
	{"classes",
		"gui manager",
		"body",
		"shape",
		"accelerator map"
	},
	{"controls",
		"consolewindow",
		"frame",
		"optionsbar",
		"menu",
		"textinput",
		"list",
		"dynamic sprite",
		"gridlist",
		"pane",
		"label",
		"button"
	},
		"application"
}

for _, file in ipairs (files) do
	if type (file) == "table" then
		for index = 2, #file do
			--print ("requiring "..file[1] ..'.'..file[index].."\n")
			require ("libs." .. file[1] .. "." .. file[index])
		end
	else
		--print("requiring "..file..".\n")
		require ("libs." .. file)
	end
end

math.randomseed(os.time())
math.random();math.random();math.random();