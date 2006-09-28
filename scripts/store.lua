-- funkce pro sklad

function c_stavsklad()
 
 cost = GetVar("store_cost")
 wood = GetVar("store_cost_wood")
 stone = GetVar("store_cost_stone")
 
 if cost < GetCash() then
  info("Nedostatek penez pro stavbu")
 end
 
 AddCash(-cost)
 SetBuilding(EBO_Store,cost,wood,stone)
end

function camerka()
 Camera(300, 300)
end

function camerkazpet()
 Camera(0, 0)
end

function s_sklad()
 info("Vybranej sklad")
 ClearButtons()
 AddButton(model_NEWTROLL,"Najmout delnika", "c_najmout")
 AddButton(model_NEWTROLL,"", "camerka")
 AddButton(model_NEWTROLL,"", "camerkazpet")
end

