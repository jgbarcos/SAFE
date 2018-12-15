local Effect = require "game.effect"

local DialogEffect = class("DialogEffect", Effect)
function DialogEffect:initialize(params)
  self.text = util.enforce( params.text, 4, "text")
end

function DialogEffect:perform(context)
  Effect.perform(self, context)
  print(self.text)
end

gamedata.effects["dialog"] = DialogEffect