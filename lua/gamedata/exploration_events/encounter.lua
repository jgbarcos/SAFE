local ExplorationEvent = require "game.exploration_event"

local Encounter = class("Encounter", ExplorationEvent)

function Encounter:initialize()
  -- Do nothing
end

function Encounter:perform(context) 
  print("This is a encounter.")
end

gamedata.exploration_events["encounter"] = Encounter