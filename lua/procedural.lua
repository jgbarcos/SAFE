local module = {}

function module:hor_line(initx, inity, endx)
  local t = {}
  local incr = 1
  if( initx > endx )
  then
    incr = -1
  end 
  
  for ix=initx, endx-incr, incr
  do
    t[#t+1] = {x=ix, y=inity}
  end
  return t
end

function module:hor_line_length(initx, inity, length)
  if( length == 0 ) then
    return {}
  else
    local endx = initx+length
    return self:hor_line(initx, inity, endx)
  end
end

function module:random_element(array)
  if (type(array) == "table") then
    local index = math.random(1, #array)
    return array[index]
  else
    return array
  end
end

return module
