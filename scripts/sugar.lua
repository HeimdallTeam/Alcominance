-- funkce pro cukrovar

function c_stavcukr()
  info("stav cukr")
  AddCash(456)
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
 
 if b.sugar > 0 and b.sugar_out > 0 then
     job = { type = 2, sur = EBS_Sugar, num=b.sugar,  percent = 100 }
     if job.num > b.sugar_out then
      job.num = b.sugar_out
     end
     return job 
 end
 
 if b.cane > 1 and b.works == 0 then
     job = { type = 1, percent = 90 }
     return job 
 end
 
 if b.cane > 25 and b.works_max > b.works then
      job = { type = 1, percent = 90 }
     return job 
 end
 
 --if b.cane_avail > 0 then
 if b.cane_avail > 0 then
     job = { type = 0, sur = EBS_Cane, num=b.cane_avail,  percent = 100 }
     return job    
 end 

 if b.coal_avail > 0 then
     job = { type = 0, sur = EBS_Coal, num=b.coal_avail,  percent = 100 }
     return job    
 end 
 
 -- jestli je nejaka trtina tak zpracovat
 if b.cane > 0 then 
     job = { type = 1, percent = 90 }
     return job    
 end 
 
 return nil
end
