local Action = require "safe.action"

local PrintConsole = class("PrintConsole", Action)
function PrintConsole:initialize(params)
    Action.initialize(self, params)

    self.text = util.default( params.text, "" )
end

function PrintConsole:on_enter()
    print(self.text)
end

gamedata.actions["print_console"] = PrintConsole