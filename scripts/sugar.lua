-- funkce pro cukrovar

function c_sugar()
  cost = GetVar("sugar_cost")  
  wood = GetVar("sugar_cost_wood")
  stone = GetVar("sugar_cost_stone")
  SetBuilding(EBO_Sugar,cost,wood,stone)
end

function s_cukr()
 ClearButtons()
 AddButton(ico_TROLLNEW,"Najmout", "c_najmout")
 AddButton(ico_TROLLFREE,"Propustit", "c_propustit")
 --PlaySound(model_TESTSOUND)
end

--b: handle na cukrovar
--i:
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

-- zjisti pripadne zmeny a odesle budove zpravy
-- napr. neni trtina a je spousta pracujicich

function i_sugarbuild(h,i)
 --id = GetMem(h, "test", 0)
 --info("Id je ",id)
 --id = id + 1
 --SetMem(h, "test", id)
 -- spocitat pomery
 a,b,c,d = GetInfo(h, BINFO_ReqStone, BINFO_ReqWood, BINFO_CanStone, BINFO_CanWood)
 if i.stone > 0 and i.wood > 0 then
    SendMsg(h, BMSG_Chief, "F>W")
 elseif c > 0 and a>0 and (a/i.stone) > (b/i.wood) then
    if SendMsg(h, BMSG_Chief, "F>IK") == 0 then
       SendMsg(h, BMSG_Chief, "W>IK")
    end
 elseif d > 0 and b>0 then
    if SendMsg(h, BMSG_Chief, "F>ID") == 0 then
        SendMsg(h, BMSG_Chief, "W>ID")
    end
 end
  
 return nil
end

function i_sugar(h, i)
  print("Idiot sugar")

 s = GetInfo(h, BINFO_CanCane)
 if i.cane > 0 then
    SendMsg(h, BMSG_Chief, "F>W")
 elseif s > 0 and SendMsg(h, BMSG_Chief, "F>IC") == 0 then
       SendMsg(h, BMSG_Chief, "W>IC")
 end
end

function i_fake()
 --zjisti se recept (kolik je treba trtiny pro jednotku/jendotky cukr/u)
 recept =  GetVar("sugar_recept")
 count, units=getReceptCount(recept, "C")
 print("C",count," ",units)
 
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
