local Action = require "safe.action"

local PrintDamage = class("PrintDamage", Action)
function PrintDamage:initialize(params)
    Action.initialize(self, params)

    self.att_team = util.default( params.att_team, "" )
    self.def_team = util.default( params.def_team, "" )
    self.att =      util.default( params.att, "" )
    self.def =      util.default( params.def, "" )
    self.damage =   util.default( params.damage, 0 )
end

function PrintDamage:on_enter()
    print(string.format(
        "> [%s]%s attacks [%s]%s for %s damage.", 
        self.att_team, self.att, self.def_team, self.def, self.damage
    ))
end

gamedata.actions["print_damage"] = PrintDamage