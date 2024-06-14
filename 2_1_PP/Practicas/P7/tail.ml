let rec to0from n =
  if n < 0 then []
  else n :: to0from (n-1)
  
(*Su version recursiva-terminal*)
let to0from n =
  let rec aux num l =
    if num < 0 then l
    else aux (num-1) (num::l)
in aux n [];;

to0from 10;;

let rec fromto m n =
  if m > n then []
  else m :: fromto (m+1) n

(*Su version recursiva-terminal*)
let fromto m n =
  let rec aux num l =
    if num > n then List.rev l
    else aux (num+1) (num::l)
  in aux m [];;
  
fromto 5 10;;

let rec remove x = function
[] -> []
| h::t -> if x = h then t
else h :: remove x t

(*Su version recursiva-terminal*)
let remove x =
  let rec aux laux = function
    [] -> List.rev laux
    | h::t -> if x = h then List.rev_append laux t
    else aux (h::laux) t
  in aux [];;

remove 1 (1::2::3::1::2::[]);;

let rec compress = function
  h1::h2::t -> if h1 = h2 then compress (h2::t)
    else h1 :: compress (h2::t)
  | l -> l;;

(*Su version recursiva-terminal*)
let compress l =
  let rec aux laux = function
    h1::h2::t -> if h1 = h2 then aux laux (h2::t)
      else aux (h1::laux) (h2::t)
    | l -> List.rev_append laux l
  in aux [] l;;
    
compress (1::2::2::3::2::4::5::2::2::5::5::[]);;

(*Estas funciones ya son recursivas terminales*)
let append' = List.append;;
let map' = List.map;;
let fold_right' = List.fold_right;;

let incseg l =
  List.fold_right (fun x t -> x::List.map ((+) x) t) l [];;

(*Su version recursiva-terminal*)
let incseg l =
  let rec aux laux suma = function
    [] -> List.rev laux
    | h::t -> aux ((h + suma)::laux) (h + suma) t
  in aux [] 0 l;;
  
incseg (1::3::2::4::5::[]);;