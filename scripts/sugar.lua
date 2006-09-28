-- funkce pro cukrovar

function c_sugar()

  cost = 
  info("stav cukr")
  AddCash(456)
  SetBuilding(EBO_Sugar,1000,10,0)
end

function s_cukr()
 ClearButtons()
 AddButton(model_STOPPRACI,2, "c_najmout")
 --PlaySound(model_TESTSOUND)
end

--b
--max_store		kolik se vejde do skladu

--cane_avail	pocet lokalit zdroje trtiny
--cane_wrkcount	pocet zamestnancu nosici trtinu
--cane			kolik je trtiny v miniskladu

--coal_avail	pocet lokalit zdroje uhli
--coal_wrkcount	pocet zamestnancu nosici uhli
--coal			kolik je uhli pro kotel
--coal_max		kolik se vejde uhli

--sugar			kolik je cukru v miniskladu
--sugar_out		pocet budov akceptujici cukr

--works_count	pocet pracujicich (vyrabejicich)
--works_max		max. pocet pracujicich
function i_sugar(b)
 
 info("Cukrovar ma ",b.cane," trtiny a ",b.sugar," cukru")
 
 --zjisti se recept (kolik je treba trtiny pro jednotku/jendotky cukr/u)
 recept =  GetVar("sugar_recept")
 count, units=getReceptCount(recept, "C")
 info("C",count," ",units)
 
 -- kdyz je trtina aspon na jednu jednotku a nikdo nezpracovava, zpracuj
 if b.cane >= count/units and b.works_count == 0 then 
     job = { type = 1, percent = 90 }
     info("trtina aspon na jednu jednotku, zpracuji")
     return job
 end
 
 -- kdyz je trtina aspon na dvojnasobek a jeste je misto, zpracuj
 if b.cane >= count*2 and b.works_count < b.works_max then
     job = { type = 1, percent = 100 }
     info("trtina aspon na dvojnasobek a jeste je misto, zpracuji")
     return job 
 end
 
 -- kdyz je dostupna trtina a je dostatecne mnozstvi uhli nebo pro nej aspon nekdo chodi, 
 -- nebo pokud neni zadne uhli dostupne, nos trtinu
 info("dostupna trtina a je dostatecne mnozstvi uhli nebo pro nej aspon nekdo chodi, nebo pokud neni zadne uhli dostupne - ")
 if b.cane_avail > 0 and (b.coal_avail == 0 or b.coal > 10 or b.coal_wrkcount > 0) then
     job = { type = 0, sur = EBS_Cane, num=10,  percent = 100 }
     info("ano, budu nosit trtinu")
     return job     
 end 
 info("ne, nebudu nosit trtinu")

 -- kdyz je dostupne uhli, nos uhli
 if b.coal_avail > 0 and b.coal < b.coal_max then
     job = { type = 0, sur = EBS_Coal, num=10,  percent = 100 }
     info("dostupne uhli, prinesu")
     return job    
 end 
 info("ne, nebudu nosit uhli")
 
 return nil
end
