local space = safe.get_space("combat")

print("entity", "unit.x", "unit.y")
for _,ent in pairs(space:get_entities()) do  
    comp = ent:get_component("GridUnitComponent")
    if comp then
        print(ent:get_name(), comp.x, comp.y)
    end
end
