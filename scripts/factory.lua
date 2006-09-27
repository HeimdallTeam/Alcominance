-- funkce pro tovarnu

function c_stavtovarna()
 cost = GetVar("factory_cost")
 if cost < GetCash() then return 0;
 
 AddCash(-cost)
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
function i_factory(b)

--zjisti se recept (kolik je treba cukru a lihu pro jednotku/jendotky alkoholu)
 countSugar, units = getReceptCount(GetVar("factory_recept"), "S")
 countAlco = getReceptCount(GetVar("factory_recept"), "A")
 countHerbe = getReceptCount(GetVar("factory_recept"), "H")
 countWater = getReceptCount(GetVar("factory_recept"), "W")

 -- pokud je dostatek surovin na vyrobu jedne jednotky a nikdo nezpracovava, zpracuj
 if b.sugar >= countSugar/units and b.alco >= countAlco/units
	and b.herbe >= countHerbe/units and b.water >= countWater/units
	and b.works == 0 then
	
     job = { type = 1, percent = 90 }
     return job
 end
 
 -- kdyz jsou vstupni suroviny aspon na dvojnasobku a jeste je misto, zpracuj
 if b.sugar >= countSugar*2 and b.alco >= countAlco*2 
	and b.herbe >= countHerbe*2 and b.water >= countWater*2
	and b.works_count < b.works_max then
 
     job = { type = 1, percent = 100 }
     return job
 end
 
 -- kdyz jsou dostupne vstupni suroviny a je dostatecne mnozstvi uhli nebo pro nej aspon nekdo chodi, 
 -- nebo pokud neni zadne uhli dostupne, nos to, ceho je vic dostupno
 info("dostupne vstupni suroviny a je dostatecne mnozstvi uhli nebo pro nej aspon nekdo chodi, nebo pokud neni zadne uhli dostupne - ")
 if b.coal_avail == 0 or b.coal > 10 or b.coal_wrkcount > 0 then
	maxValue = math.max(
		b.sugar_avail*countSugar, b.alco*countAlco, b.herbe_avail*countHerbe, b.water_avail*countWater
	)
	-- poradi case urcuje take prednost pri stejne hodnote	
	if maxValue == b.water_avail*countWater then
		job = { type = 0, sur = EBS_Water, num=10, percent = 90 }
	end	
	if maxValue == b.herbe_avail*countHerbe then
		job = { type = 0, sur = EBS_Herbe, num=10, percent = 90 }
	end
	if maxValue == b.sugar_avail*countSugar then
		job = { type = 0, sur = EBS_Sugar, num=10, percent = 90 }
	end
	if maxValue == b.alco_avail*countAlco then
		job = { type = 0, sur = EBS_Alco, num=10, percent = 90 }
	end
	
	return job
 end
 
 return nil
end

