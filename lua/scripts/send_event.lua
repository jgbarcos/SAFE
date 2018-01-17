local event = {
  type = "end_turn",
  payload = "holamundo"  
}

safe.post_event(event)
print ("event posted")
pprint(event)