local target = function (context)
  local owner =     util.enforce(context.owner, 4, "owner")
  context.targets = util.default( context.targets, {} )
  context.targets.tiles = util.default( context.targets.tiles, {} )
  context.targets.units = util.default( context.targets.units, {} )
  
  local space = safe.get_space(context.space)
  local ent = space:get_entity(owner)
  local transform = safe.get_transform(ent)
  local char_data = safe.get_component(ent, "CharacterDataComponent")
  local unit = safe.get_component(ent, "GridUnitComponent")
  
  for _,vec in pairs(char_data.attack_area) do
    local x = unit.x + vec.x
    local y = unit.y + vec.y
    if transform.scale.x < 0 then x = unit.x - vec.x end
    
    context.targets.tiles[#context.targets.tiles+1] = {x=x,y=y}
  end
  
  for _,v in pairs(context.targets.tiles) do
    util.concat( context.targets.units, safe.get_entities_at(v.x, 0) )
  end
end

gamedata.targetting["relative_tiles"] = target

