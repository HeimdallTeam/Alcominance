-- funkce pro tovarnu

function c_stavtovarna()
 SetBuilding(EBO_Factory,100,100,100)
end

function s_tovarna()
 ClearButtons()
 AddButton(ID_NEWTROLL,2, "c_addtroll")
 AddButton(ID_PROPUST,"You want save some salary?", "c_release")
 AddButton(ID_AGROINZENYR,"Even Stupids can earn some degree", "c_agroeng")
 AddButton(ID_KOLEJOVYDOPRAVNIK,"Railway speeds up production", "c_railway")
 AddButton(ID_DESTILATOR,"Doubles efficiency of destilation", "c_destil")
 AddButton(ID_PREDPOVED,"Want some weather cast?", "c_weathercast")
 AddButton(ID_KOTEL,"Fix the holes for better efficiency", "c_firefix")
 AddButton(ID_STOPPRACI,"Stop the hard work", "c_workstop")
 PlaySound(ID_TESTSOUND+1)
end


