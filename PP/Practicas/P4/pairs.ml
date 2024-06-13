let next (x,y) = if (x == 1 && y == 1) 
  then (x,y+1) 
  else if (x==1 && y mod 2 ==0) 
    then (x+1,y-1) 
  else if (x==1 && y mod 2 ==1) 
    then (x,y+1) 
  else if (y==1 && x mod 2 == 0) 
    then (x+1, y) 
  else if y mod 2 == 0 
    then if x mod 2 == 0 
      then (x-1,y+1) 
      else (x+1,y-1) 
  else if x mod 2 == 0 
    then (x+1,y-1) 
    else (x-1,y+1);;
next (2, 2);;
next (1, 3);;
next (1, 4);;
next (2, 3);;
next (3, 2);;
next (4, 1);;
next (5, 1);;

let rec steps_from (x,y) n = if n<1 then (x,y) else steps_from (next(x,y)) (n-1);;
steps_from(2,3) 0;;
steps_from(2,3) 10;;

let pair n = if n<1 then (1,1) else steps_from(1,1) (n-1);;
pair 1;;
pair 9;;
pair 20;;
pair 1001;;
pair 99999;;