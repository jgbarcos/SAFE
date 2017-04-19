local module = {}


function module:hor_line(initx, inity, endx)
  t = {}
  incr = 1
  if( initx > endx )
  then
    incr = -1
  end 
  
  for ix=initx, endx, incr
  do
    t[#t+1] = {x=ix, y=inity}
  end
  return t
end

function module:hor_line_length(initx, inity, span)
  endx = initx+span
  return self:hor_line(initx, inity, endx)
end

return module
