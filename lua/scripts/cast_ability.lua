local id = 0
local ability = "shoot"
local context = {owner=id}

local space = safe.get_space("combat")
local ent = space:get_entity(id)
local comp = safe.get_component(ent, "AbilitiesComponent")
comp.abilities[ability]:perform(context)