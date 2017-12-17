local Effect = require "game.effect"

local EditContext = class("EditContextEffect", Effect)
function EditContext:initialize(params)
  Effect.initialize(self, params)
  self.edit_context = util.enforce(params.edit_context, 4, "edit_context")
end

function EditContext:perform(context)  
  for k,v in pairs(self.edit_context) do
    context[k] = v
  end
end

gamedata.effects["edit_context"] = EditContext