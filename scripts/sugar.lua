-- funkce pro cukrovar

function c_stavcukr()
  SetBuilding(EBO_Sugar,1000,10,0)
end

function s_cukr()
 ClearButtons()
 AddButton(ID_STOPPRACI,2, "c_najmout")
 PlaySound(ID_TESTSOUND)
end

--max_store kolik se vejde do skladu
--cane_avail kolik je trtiny k dispozici
--cane trtiny v miniskladu
--sugar cukro v miniskladu
--works pocet pracujicich
--works_max max. pocet pracujicich
function i_sugar(b)
 -- tady pujde hlavne o to vychytat kdy nosit a kdy pracovat
 -- jako vstup je struktura se informacema o budove
 
 info("Cukrovar ma ",b.cane," trtiny a ",b.sugar," cukru")
 -- pokud je nejaky cukr tak odnest
 -- jestlize se trtina vejde tak trtina
 if b.cane > 1 and b.works == 0 then
     job = { type = 1, percent = 90 }
     return job 
 end
 
 if b.cane_avail > 0 then
     job = { type = 0, sur = EBS_Cane, num=10,  percent = 100 }
     return job    
 end 
 
 -- jestli je nejaka trtina tak zpracovat
 if b.cane > 0 then 
     job = { type = 1, percent = 90 }
     return job    
 end 
 
 return nil
end
