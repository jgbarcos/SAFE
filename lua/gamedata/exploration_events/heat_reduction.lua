local ExplorationEvent = require "game.exploration_event"

local HeatReduction = class("HeatReduction", ExplorationEvent)

function HeatReduction:initialize()
  -- Do nothing
end

function HeatReduction:perform(context) 
  local amount = 100
  game.heat = game.heat - amount
  print("Heat reduced by "..amount)
end

gamedata.exploration_events["heat_reduction"] = HeatReduction