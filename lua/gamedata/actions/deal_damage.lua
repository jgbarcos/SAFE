local Action = require "safe.action"

local DealDamage = class("DealDamage", Action)
function DealDamage:initialize(params)
    Action.initialize(self, params)
    
    self.target = util.enforce( params.target, 4, "target entity")
    self.damage = util.enforce( params.damage, 4, "damage")
end

function DealDamage:on_enter()
    local ptr = safe.get_entity(self.target)
    local char_data = safe.get_component(ptr, "CharacterDataComponent")

    char_data.current.health = char_data.current.health - self.damage
end

gamedata.actions["deal_damage"] = DealDamage