-- funkce pro doly

function c_stavkamen()
 SetBuilding(EBO_StoneMine,150,0,0)
end
function c_stavuhli()
 SetBuilding(EBO_CoalMine,150,0,0)
end

function s_coalmine()
 ClearButtons()
 -- jen update
 --AddButton(model_NEWTROLL,2, "c_addtroll")
 AddButton(model_PROPUST,"You want save some salary?", "c_release")
 AddButton(model_AGROINZENYR,"Even Stupids can earn some degree", "c_agroeng")
 AddButton(model_KOLEJOVYDOPRAVNIK,"Railway speeds up production", "c_railway")
 AddButton(model_DESTILATOR,"Doubles efficiency of destilation", "c_destil")
 AddButton(model_PREDPOVED,"Want some weather cast?", "c_weathercast")
 AddButton(model_KOTEL,"Fix the holes for better efficiency", "c_firefix")
 AddButton(model_STOPPRACI,"Stop the hard work", "c_workstop")
 --PlaySound(model_TESTSOUND+1)
end
