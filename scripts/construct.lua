
function s_construct()
 ClearButtons()
 AddButton(ico_TROLLNEW,"Najmout", "c_najmout")
 AddButton(ico_TROLLFREE,"Propustit", "c_propustit")
end

--Idiot pro staveni budov
--b
--wood_rem			mnozstvi dreva zbyvajici pro dokonceni stavby
--stone_rem			mnozstvi kamene zbyvajici pro dokonceni stavby
--wrk_count			pocet stavejicich tupounu
--wrk_count_wood	pocet tupounu nosicich drevo
--wrk_stone_stone	pocet tupounu nosicich kamen
--wood				mnozstvi dreva v budove pro stavbu
--stone				mnozstvi kamene v budove pro stavbu
function i_construct(b)

 --uz nic nechybi, konec
 if b.rem_wood == 0 and b.rem_stone == 0 then
  return nil
 end
 
 --chybi jenom jedna surovina a nikdo ji nenosi
 if b.wood_rem == 0 and b.wrk_count_stone == 0 then
  return { type = 0, sur = EBS_Stone, num=10, percent = 90 }
 end
 if b.stone_rem == 0 and b.wrk_count_wood == 0 then
  return { type = 0, sur = EBS_Wood, num=10, percent = 90 }
 end
 
 w_coef = b.wrk_count_wood / b.wood_rem
 s_coef = b.wrk_count_stone / b.stone_rem
 b_coef = b.wrk_count / (b.wood+b.stone)
 min = math.min(w_coef, s_coef, b_coef)
 
 --rozlozeni podle poctu zbyvajicich surovin
 if min == w_coef then
  return { type = 0, sur = EBS_Wood, num = 10, percent = 90 }
 end
 if min == s_coef then
  return { type = 0, sur = EBS_Stone, num = 10, percent = 90 }
 end
 if min == b_coef then
  return { type = 1, percent = 100 }
 end
 
 return nil
end
