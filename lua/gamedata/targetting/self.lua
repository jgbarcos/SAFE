local target = function (context)
  local owner =     util.enforce(context.owner, 4, "owner")
  context.targets = util.default( context.targets, {} )
  context.targets.tiles = util.default( context.targets.tiles, {} )
  context.targets.units = util.default( context.targets.units, {} )
  
  local ent = safe.get_entity(owner)
  local unit = safe.get_component(ent, "GridUnitComponent")
  
  context.targets.tiles[#context.targets.tiles+1] = { x=unit.x, y=unit.y }
  context.targets.units[#context.targets.units+1] = owner
end

gamedata.targetting["self"] = target

