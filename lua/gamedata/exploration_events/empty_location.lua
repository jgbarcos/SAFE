local ExplorationEvent = require "game.exploration_event"

local EmptyLocation = class("EmptyLocation", ExplorationEvent)

function EmptyLocation:initialize()
  -- Do nothing
end

function EmptyLocation:perform(context) 
  print("The location is empty.")
end

gamedata.exploration_events["empty_location"] = EmptyLocation