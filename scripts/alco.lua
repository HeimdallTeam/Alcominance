-- funkce pro lihovar

function c_stavlihovar()
 SetBuilding(EBO_Destilate,10,10,0)
end

function s_lihovar()
 ClearButtons()
 AddButton(model_STOPPRACI,2, "c_najmout")
 AddButton(model_LAHVARNA,"People pay more for nice bottles", "c_bottles")

end

--b
--max_store - kapacita skladu

--sugar_avail		pocet lokalit zdroje cukr
--sugar_wrkcount	pocet zamestnancu nosici cukr
--sugar				kolik je cukru v miniskladu

--alco_avail		pocet lokalit zdroje lih
--alco_wrkcount		pocet zamestnancu nosici lih
--alco				kolik je lihu v miniskladu

--coal_avail		pocet lokalit zdroje uhli
--coal_wrkcount		pocet zamestnancu nosici uhli
--coal				kolik je uhli pro kotel
--coal_max			kolik se vejde uhli

--works_count	pocet pracujicich (vyrabejicich)
--works_max		max. pocet pracujicich
function i_alco(b)
 
 info("Lihovar ma ",b.sugar," cukru a ",b.alco," lihu")
 
 --zjisti se recept (kolik je treba cukru a lihu pro jednotku/jendotky alkoholu)
 countSugar, units = getReceptCount(GetVar("dest_recept"), "S")
 countAlco = getReceptCount(GetVar("dest_recept"), "A")
 
 -- pokud je dostatek surovin na vyrobu jedne jednotky a nikdo nezpracovava, zpracuj
 if b.sugar >= countSugar/units and b.alco >= countAlco/units and b.works == 0 then
     job = { type = 1, percent = 90 }
     info("cukr a lih aspon na jednu jednotku, zpracuji")
     return job 
 end
 
 -- kdyz jsou vstupni suroviny aspon na dvojnasobku a jeste je misto, zpracuj 
 if b.sugar >= countSugar*2 and b.alco >= countAlco*2 and b.works_count < b.works_max then
     job = { type = 1, percent = 100 }
     info("cukr a lih aspon na dvojnasobek a jeste je misto - zpracuji")
     return job
 end 
 
 -- kdyz jsou dostupne vstupni suroviny a je dostatecne mnozstvi uhli nebo pro nej aspon nekdo chodi, 
 -- nebo pokud neni zadne uhli dostupne, nos to, ceho je vic dostupno
 info("dostupne vstupni suroviny a je dostatecne mnozstvi uhli nebo pro nej aspon nekdo chodi, nebo pokud neni zadne uhli dostupne - ")
 if b.coal_avail == 0 or b.coal > 10 or b.coal_wrkcount > 0 then
	if b.sugar_avail*countSugar > b.alco_avail*countAlco then
		job = { type = 0, sur = EBS_Sugar, num=10, percent = 90 }
		info("ano, budu nosit cukr (jeho vic a treba)")
	else
		job = { type = 0, sur = EBS_Alco, num=10, percent = 90 }
		info("ano, budu nosit lih (jeho vic a treba)")
	end
	
	return job
 end
 info("ne, nebudu nosit ani cukr ani lih")
 
 -- kdyz je dostupne uhli, nos uhli
 if b.coal_avail > 0 and b.coal < b.coal_max then
     job = { type = 0, sur = EBS_Coal, num=10,  percent = 100 }
     info("dostupne uhli - budu nosit uhli")
     return job    
 end
 
 return nil
end
