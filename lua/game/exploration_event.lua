local ExplorationEvent = class("ExplorationEvent")

function ExplorationEvent:initialize(params) 
  self.name =             util.enforce( params.name,     4, "name" )
  self.category =         util.enforce( params.category, 4, "category" )
    
end

return ExplorationEvent

