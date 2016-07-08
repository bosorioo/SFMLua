Console.open()
Console.setPauseOnError()

require "framework"

local app = Application(800, 600, "Application Example")

local dragframe = Frame(200, 58, "Draggable Frame")
dragframe:setPosition(225, 40)
dragframe:setDraggable(true)
dragframe:addComponent(TextInput(), 4, 4)

local list = List(192, 506)
local staticframe = Frame(200, 540, "Static Frame")
staticframe:setPosition(10, 40)
staticframe:addComponent(list, 4, 4)

	for i = 1, 1000 do
		list:addStringItem("Item #"..i)
	end

local optBar = OptionsBar()

	optBar:addOption("OptionBar Menu 1", {
		{"Menu Button 1", function () print "Menu Button 1 Clicked\n" end},
		{"Menu Button 2", function () print "Menu Button 2 Clicked\n" end},
		{"-", nil},
		{"Menu Boolean 1", false, function (b) print ("Menu Boolean 1 toggled to " .. (b and "true" or "false") .."\n") end},
		{"Menu Boolean 2", true, function (b) print ("Menu Boolean 2 toggled to " .. (b and "true" or "false") .."\n") end},
		{"-", nil},
		{"Submenu 1", {
			{"Submenu Item 1", function () print "Submenu Item 1 Clicked\n" end},
			{"Submenu Item 2", function () print "Submenu Item 2 Clicked\n" end},
			{"Submenu Item 3", function () print "Submenu Item 3 Clicked\n" end}
			}
		},
		{"Submenu 2", {
			{"Submenu submenu 1", {
				{"Subsub Menu Item 1", function () print "SubSubmenu Item 1 Clicked\n" end},
				{"Subsub Menu Item 2", function () print "SubSubmenu Item 2 Clicked\n" end},
				}
				}
			}
		},
		{"-", nil},
		{"Exit", function () os.exit() end}
	})

	optBar:addOption("OptionBar Button 1", function() print "OptionBar Button 1 Clicked\n" end)
	
	optBar:addOption("OptionBar Hotkeys Menu 2", {
		{"Menu 2 Button 1", function () print "Menu 2 Button 1 Clicked\n" end, "Ctrl + 1"},
		{"Menu 2 Button 2", function () print "Menu 2 Button 2 Clicked\n" end, "Ctrl + 2"},
		{"-", nil},
		{"Menu 2 Boolean 1", false, function (b) print ("Menu 2 Boolean 1 toggled to " .. tostring(b) .."\n") end, "Ctrl + A"},
		{"Menu 2 Boolean 2", true, function (b) print ("Menu 2 Boolean 2 toggled to " .. tostring(b) .."\n") end, "Ctrl + B"},
		{"-", nil},
		{"Menu 2 Submenu 1", {
			{"Submenu Item 1", function () print "Menu 2 Submenu Item 1 Clicked\n" end, "Ctrl + 3"},
			{"Submenu Item 2", function () print "Menu 2 Submenu Item 2 Clicked\n" end, "Ctrl + 4"},
			{"Submenu Boolean 1", true, function (b) print ("Menu 2 Submenu Boolean 1 toggled to " ..tostring(b).."\n") end, "Ctrl + C"},
			}
		}
	})
	
app:addControls(dragframe, optBar, staticframe)
app:run()