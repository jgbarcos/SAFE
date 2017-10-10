proc = require "procedural"

local module = {}

--
-- UNIT ARCHETYPES DATA
-- 
module.archetypes = {
  ["Melee"] = {
    base_health =   {min=5, max=10},
    base_movement = {min=4, max=6},
    base_attack =   {min=4, max=7},
    
    attack_area = {
      forward_length = {min=1, max=2},
      side_length = 1,
      back_length = {min=1, max=2}
    }
  },
  ["Ranged"] = {
    base_health =   {min=3, max=6},
    base_movement = {min=3, max=5},
    base_attack =   {min=3, max=6},
    
    attack_area = {
      forward_init = 3,
      forward_length = {min=2, max=4}
    }  
  },
  ["Assault"] = {
    base_health =   {min=4, max=8},
    base_movement = {min=2, max=5},
    base_attack =   {min=4, max=7},
    
    attack_area = {
      forward_length = {min=2, max=4}
    }  
  },
  ["Support"] = {
    base_health =   {min=8, max=11},
    base_movement = {min=3, max=4},
    base_attack =   {min=3, max=5},
    
    attack_area = {
      forward_init = {min=1, max=2},
      forward_length = {min=3, max=4}
    } 
  },
  ["Heavy"] = {
    base_health =   {min=10, max=15},
    base_movement = {min=2, max=3},
    base_attack =   {min=4, max=5},
    
    attack_area = {
      forward_length = {min=2, max=3},
      side_length = {min=2, max=3}
    }  
  },
}

--
-- PUBLIC FUNCTIONS
--
function module:gen_archetype(key)
  local data = {}

  if( self.archetypes[key] == nil ) then
    error("Archetype "..key.." does not exist")
    return data
  end

  for k,v in pairs( self.archetypes[key] ) do
    if (k == "attack_area") then
      data["attack_area"] = self:gen_attack_area(v)
    else
      data[k] = self:gen_value(v)
    end
  end
  return data
end

--
-- AUXILIARY FUNCTIONS
--
function module:append(a, b)
  for k,v in pairs( b ) do
    a[#a+1] = v
  end
end

function module:gen_attack_area(t)
  local vec = {}
  
  --  forward
  local length = self:gen_value( t.forward_length )
  if( length == nil ) then length = 0 end
  
  local init = self:gen_value( t.forward_init )
  if( init == nil ) then init = 1 end
  
  if( length > 0 ) then
    local fa = proc:hor_line_length( init, 0, length)
    self:append(vec, fa)
  end 
  
  -- sides
  length = self:gen_value( t.side_length )
  if( length == nil ) then length = 0 end
  
  init = self:gen_value( t.side_init )
  if( init == nil ) then init = 0 end
  
  if( length > 0) then
    local fs = proc:hor_line_length( init, 1, length)
    self:append(vec, fs)
    fs = proc:hor_line_length( init, -1, length)
    self:append(vec, fs)
  end
  
  -- back
  length = self:gen_value( t.back_length )
  if( length == nil ) then length = 0 end
  
  init = self:gen_value( t.back_init )
  if( init == nil ) then init = -1 end
  
  if( length > 0) then
    local fb = proc:hor_line_length(init, 0, -length)
    self:append(vec, fb)
  end

  return vec
end

function module:gen_value(t)
  if type(t) == "table" then
    return math.random(t.min, t.max)
  else
    return t
  end
end

return module
