assert(args[1], "Please specify plugins dir")
load_plugin(args[1].."/radapter_xorg_plugin")

local ws = WebsocketServer{
    port = 11337
}
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

on(ws, "keyboard:key", press)
on(ws, "mouse", function (msg)
    xorg {
        mouse = {
           x = msg.x,
           y = msg.y,
           scroll = msg.scroll,
       }
    }
end)