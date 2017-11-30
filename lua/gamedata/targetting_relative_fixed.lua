local target = function (context)
  local owner =     util.enforce(context.owner, 4, "owner")
  context.targets = util.default(context.targets, {})
  
  
  local ent = safe.get_entity(owner)
  local transform = safe.get_transform(ent)
  local char_data = safe.get_component(ent, "CharacterDataComponent")
  local unit = safe.get_component(ent, "GridUnitComponent")
  
  local targets = {}
  for _,vec in pairs(char_data.attack_area) do
    local x = unit.x + vec.x
    local y = unit.y + vec.y
    if transform.scale.x < 0 then x = unit.x - vec.x end
    
    context.targets[#context.targets+1] = {x=x,y=y}
  end
end

gamedata.targetting["relative_fixed"] = target

