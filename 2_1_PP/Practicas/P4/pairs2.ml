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

let rec steps_from (x,y) n = if n<1 then (x,y) else steps_from (next(x,y)) (n-1);;

let pair n = if n<1 then (1,1) else steps_from(1,1) (n-1);;

let pair_i p =
  let rec find i paux =
    if paux = p then i
    else find (i + 1) (next paux)
  in find 1 (1,1);;

pair_i;;
pair_i (12, 130);; (*10000*)
pair_i (100, 101);; (*20000*)