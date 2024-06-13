let b = function a -> a * 2;;
let v = b 2;;

let w = 1. *. 2. +. 3. -. 2. /. 3.;;

let x = if (1>2) then 'M' else 'm';;

let d = function c -> c + 2 > 5;;
let f = function e -> e - 2 <= 5;;
let y = d 4 && f 7;;


let g = function h -> h mod 2;;
let z = string_of_int(g(2));;
