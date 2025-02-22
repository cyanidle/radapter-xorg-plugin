assert(args[1], "Please specify plugins dir")

load_plugin(args[1].."/radapter_xorg_plugin")


local xorg = XorgPlugin{}


local function press(key)
    xorg {
        kb = {
            key = key,
            press = true
        }
    }
    xorg {
        kb = {
            key = key,
            press = false
        }
    }
end

each(1000, function()
--    xorg {
--        mouse = {
--            x = 100,
--            y = 100,
--            scroll = 0
--        }
--    }
    press('lmb')
end)
