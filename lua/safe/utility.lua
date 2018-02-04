module = {}

-- Returns a default value when the original value was nil
module.default = function (value, def)
    if value == nil then 
        return def 
    else 
        return value
    end
end

-- Raises an error when the value is nil
module.enforce = function (value, depth, field)
    if value == nil then
        if(field == nil) then
            error("Some parameter is nil", depth)
        else
            error("Parameter \""..field.."\"".." must be valid", depth)
        end
    end
        
    return value
end  

module.concat = function (this, other)
    for _,v in ipairs(other) do
        this[#this+1] = v
    end
end

module.load = function (mod)
  package.loaded[mod] = nil
  require(mod)  
end

return module
