-- funkce pro tovarnu

function c_stavtovarna()

 cost = GetVar("factory_cost")
 wood = GetVar("factory_cost_wood")
 stone = GetVar("factory_cost_stone")
 

 AddCash(-cost)
 info("tovarna set")
 SetBuilding(EBO_Factory,cost,wood,stone)
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
 --PlaySound(model_TESTSOUND+1)
end

--b
--max_store - kapacita skladu

--sugar_avail		pocet lokalit zdroje cukr
--sugar_wrkcount	pocet zamestnancu nosici cukr
--sugar				kolik je cukru v miniskladu

--alco_avail		pocet lokalit zdroje lih
--alco_wrkcount		pocet zamestnancu nosici lih
--alco				kolik je lihu v miniskladu

--herbe_avail		pocet lokalit zdroje koreni
--herbe_wrkcount	pocet zamestnancu nosici koreni
--herbe				kolik je koreni v miniskladu

--water_avail		pocet lokalit zdroje voda
--water_wrkcount	pocet zamestnancu nosici vodu
--water				kolik je vody v miniskladu

--coal_avail		pocet lokalit zdroje uhli
--coal_wrkcount		pocet zamestnancu nosici uhli
--coal				kolik je uhli pro kotel
--coal_max			kolik se vejde uhli

--works_count	pocet pracujicich (vyrabejicich)
--works_max		max. pocet pracujicich
function i_factory(h, i)
 a,b,c,d,e = GetInfo(h, BINFO_CanSur+EBS_Sugar, BINFO_CanSur+EBS_Alco, BINFO_CanSur+EBS_Water, BINFO_CanSur+EBS_Herbe, BINFO_CanSur+EBS_Coal )

 if b > 0 and i.alco < 10 and SendMsg(h, BMSG_Chief, 0,"F>IA") == 0 then
       SendMsg(h, BMSG_Chief, 0,"W>IA")
       return nil
 end
 if a > 0 and i.sugar < 10 and SendMsg(h, BMSG_Chief, 0,"F>IS") == 0 then
       SendMsg(h, BMSG_Chief, 0,"W>IS")
       return nil
 end
 if c > 0 and i.water < 30 and SendMsg(h, BMSG_Chief, 0,"F>IW") == 0 then
       SendMsg(h, BMSG_Chief, 0,"W>IW")
       return nil
 end
 if d > 0 and i.herbe < 10 and SendMsg(h, BMSG_Chief, 0,"F>IH") == 0 then
       SendMsg(h, BMSG_Chief, 0,"W>IH")
       return nil
 end
 
 if e > 0 and i.coal < 10 and SendMsg(h, BMSG_Chief, 0,"F>IU") == 0 then
       SendMsg(h, BMSG_Chief, 0,"W>IU")
       return
 end
 
 SendMsg(h, BMSG_Chief, 0,"F>W")
 
end

