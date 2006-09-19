-- funkce pro lihovar

function c_stavlihovar()
 SetBuilding(EBO_Destilate,10,10,0)
end

function s_lihovar()
 ClearButtons()
 AddButton(ID_STOPPRACI,2, "c_najmout")
 AddButton(ID_LAHVARNA,"People pay more for nice bottles", "c_bottles")

end

function i_alco(b)
 -- tady pujde hlavne o to vychytat kdy nosit a kdy pracovat
 -- jako vstup je struktura se informacema o budove
 
 info("Lihovar ma ",b.sugar," cukru a ",b.alco," lihu")
 -- pokud je nejaky cukr tak odnest
 -- jestlize se trtina vejde tak trtina
 if b.sugar > 1 and b.works == 0 then
     job = { type = 1, percent = 90 }
     return job 
 end
 
 if b.sugar_avail > 0 then
     job = { type = 0, sur = EBS_Sugar, num=b.sugar_avail,  percent = 100 }
      if job.num > 10 then
       job.num = 10
      end
     return job    
 end 
 
 -- jestli je nejaka trtina tak zpracovat
 if b.sugar > 0 then 
     job = { type = 1, percent = 90 }
     return job    
 end 
 
 return nil
end

