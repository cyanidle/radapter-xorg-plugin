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

pipe(ws, function (msg)
    log("From ws: {}", msg)
end)

on(ws, "keyboard:key", press)
on(ws, "mouse", function (msg)
    xorg {
        mouse = {
           dx = math.floor(msg.dx * 2.3),
           dy = math.floor(msg.dy * 2.3),
           scroll = math.floor(msg.scroll),
       }
    }
end)