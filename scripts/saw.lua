function c_saw()

 cost=GetVar("saw_cost")
 wood = GetVar("factory_cost_wood")
 stone = GetVar("factory_cost_stone")
 
 if cost > GetCash() then
   info("Nedostatek penez pro stavbu")
 end
 
 AddCash(-cost)
 SetBuilding(EBO_Saw,cost,wood,stone)
end

