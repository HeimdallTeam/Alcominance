-- funkce pro tovarnu

function c_stavtovarna()
 SetBuilding(EBO_Factory,100,100,100)
end

function s_tovarna()
 ClearButtons()
 AddButton(model_NEWTROLL,2, "c_najmout")
 AddButton(model_PROPUST,"You want save some salary?", "c_release")
 AddButton(model_AGROINZENYR,"Even Stupids can earn some degree", "c_agroeng")
 AddButton(model_KOLEJOVYDOPRAVNIK,"Railway speeds up production", "c_railway")
 AddButton(model_DESTILATOR,"Doubles efficiency of destilation", "c_destil")
 AddButton(model_PREDPOVED,"Want some weather cast?", "c_weathercast")
 AddButton(model_KOTEL,"Fix the holes for better efficiency", "c_firefix")
 AddButton(model_STOPPRACI,"Stop the hard work", "c_workstop")
 PlaySound(model_TESTSOUND+1)
end

function i_factory(b)
 if b.sugar_avail > 0 and b.sugar < 20 then
     job = { type = 0, percent = 90, sur = EBS_Sugar, num = 10 }
     return job 
 end
 if b.herbe_avail > 0 and b.herbe < 10 then
     job = { type = 0, percent = 90, sur = EBS_Herbe, num = 10 }
     return job 
 end
 if b.alco_avail > 0 and b.alco < 20 then
     job = { type = 0, percent = 90, sur = EBS_Alco, num = 10 }
     return job 
 end
 if b.water_avail > 0 and b.water < 30 then
     job = { type = 0, percent = 90, sur = EBS_Water, num = 10 }
     return job 
 end

 if b.sugar >= 10 and b.alco >= 10 and b.water >= 20 and b.herbe >= 3 then
     job = { type = 1, percent = 100 }
     return job    
 end 

 return nil
end

