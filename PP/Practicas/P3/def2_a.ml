let pi = 4.0 *. atan 1.0;;
let p r = if r >=0. then 2. *. pi *. r else 0.;;
let area r = if r >=0. then pi *. r**2. else 0.;;

let absf num = if num <0. then num *. -1. else num;;

let even num = if num mod 2 = 0 then true else false;; 

let next3 num = if num >= 0 then if num mod 3 = 0 then num else num + (3 - num mod 3) else if num mod 3 = 0 then num else num + (-num mod 3);;

let is_a_letter a = if (a >= 'a') && (a <= 'z') then true else if (a >= 'A') && (a <= 'Z') then true else false;;

let string_of_bool b = if b then "verdadero" else "falso";;