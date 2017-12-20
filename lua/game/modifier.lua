local Modifier = class("Modifier")

function Modifier:initialize( params )
  self.name =       util.enforce( params.name, 4,  "name" )
  self.stats =      util.enforce( params.stats, 4, "stats" )
  self.can_stack =  util.default( params.can_stack,   false )
  -- Removes modifier when event count is achieved
  self.event_count = util.default( params.event_count, 1 )
  self.event =      util.default( params.event, nil )
  
  -- Set default values to multiplicative and additive values
  for _,v in pairs(self.stats) do
    v.mult = util.default ( v.mult, 1.0 )
    v.add  = util.default ( v.add,  0 )
  end
end

function Modifier:stack( other )
  for att,v in pairs (other.stats) do
    self.stats[att].mult = self.stats[att].mult * v.mult
    self.stats[att].add = self.stats[att].add + v.add
  end
end

return Modifier
