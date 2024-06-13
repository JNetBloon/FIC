let l1 = [1; 2; 3;];;
let l2 = [];;
let l3 = 4::5::[];;
List.hd(l1);;
List.hd(l2);;

let hd l =
  match l with
   [] -> failwith "hd"
  | t :: _ -> t;;

hd;;
hd l1;;
hd l2;;

let tl l =
  match l with
  | [] -> failwith "tl"
  | _ :: t -> t;;

tl;;
tl l1;;
tl l2;;

let length l =
  let rec aux i= function
    [] -> i
    | _::laux -> aux (i+1) laux
  in aux 0 l;;

length;;
length l1;;
length l2;;

let rec compare_lengths l1 l2 = 
  match (l1, l2) with
  ([], []) -> 0
| ([], _) -> -1
| (_, []) -> 1
| (_::t1, _::t2) -> compare_lengths t1 t2;;

let rec compare_length_with l num =
  match (l, num) with
  | ([], 0) -> 0
  | ([], _) -> -1
  | (_, 0) -> 1
  | (_::t, n) -> compare_length_with t (n - 1);;

let init len f =
  let rec aux i =
    if i = len then []
    else f (i) :: aux (i+1)
  in if len < 0 then failwith "init" else aux 0;;

let f x = x;;
let l3 = init 5 f;;

let nth l num =
  let rec aux i = function
    [] -> failwith "nth"
    | h::t -> if i = num then h else aux (i + 1) t
  in if num < 0 
    then failwith "nth" else aux 0 l;;

nth l1 2;;
nth l1 3;;
nth l2 0;;

let rec append l1 l2 =
	match l1 with
	[] -> l2
	| h::t ->  h :: append t l2;;

append l1 l3;;

let rec rev_append l1 l2 = 
  match l1 with
	[] -> l2
	| h::t -> rev_append t (h::l2);;

rev_append l1 l3;;
l1;;
l3;;

(*Dos formas de hacer la misma funcion*)
let rev l =
  let rec aux m = function
    [] -> m
  | h::t -> aux (h::m) t
  in aux [] l;;

let rev l = rev_append l [];;

rev l1;;
l1;;
(*NOTA: Tambien se puede usar para crear append con rev_append (rev l1) l2 
   [Se invierte l1 para que lo vuelva a invertir y juntar con l2]*)

let rec concat l =
  match l with
  | [] -> []
  | hd :: tl -> append hd (concat tl);;

let ll1 = [[1; 2; 3]; [4; 5]; [6; 7; 8]];;
concat ll1;;

let flatten l = concat l;;
let ll1 = [[1; 2; 3]; [4; 5]; [6; 7; 8]];;
flatten ll1;;

let split l =
  let rec aux maux naux = function
    | [] -> (rev maux, rev naux)
    | (x, y) :: t -> aux (x :: maux) (y :: naux) t
  in aux [] [] l;;

let ll1 = [(1, "uno"); (2, "dos"); (3, "tres")];;
split ll1;;

let combine l1 l2 =
  let rec aux maux l1 l2 =
    match (l1,l2) with
    | ([],[]) -> rev maux
    | (h1::t1, h2::t2) -> aux ((h1, h2) :: maux) t1 t2
    | _ -> failwith "combine" (*Si no se cumple ningun caso anterior*)
  in aux [] l1 l2;;

combine l1 l3;;
let charlist = "a"::"b"::"c"::[];;
let l4 = 1::2::3::[];;
combine l1 l4;;
combine l1 charlist;;

let rec map f l =
  match l with
  [] -> []
  | h::t -> (f h) :: map f t;;

map;;
let f n = n * 2;;
map f l1;;

let map2 f l1 l2 =
  let rec aux f maux l1 l2 =
    match (l1,l2) with
    | ([],[]) -> rev maux
    | (h1::t1, h2::t2) -> aux f ((f h1 h2) :: maux) t1 t2
    | _ -> failwith "map2" (*Si no se cumple ningun caso anterior*)
  in aux f [] l1 l2;;

  let f n1 n2 = n1 + n2;;
  let l1 = 1::2::3::[];;
  let l2 = 3::2::1::[];;
  map2 f l1 l2;;

(*La primera solucion no es recursiva terminal, la segunda si*)
let rev_map f l = rev (map f l);;

let rev_map f l =
  let rec aux laux = function
    [] -> laux
    | (h::t) -> aux (f h :: laux) t
  in aux [] l;; 

let f n = n * 2;;
map f l1;;
rev_map f l1;;

let rec for_all f l =
  match l with
  [] -> true
  | (h::t) -> if f h then for_all f t else false;;

let f n = n mod 2 = 0;;
let l1 = 2::4::6::8::10::[];;
for_all f l1;;
let l1 = 2::4::6::8::11::[];;
for_all f l1;;

let rec exists f l =
  match l with
  [] -> false
  | (h::t) -> if f h then true else exists f t;;

let f n = n mod 2 = 0;;
let l1 = 1::3::5::7::10::[];;
exists f l1;;
let l1 = 1::3::5::7::9::[];;
exists f l1;;

let rec mem n l =
  match l with 
  [] -> false
  | (h::t) -> if h = n then true else mem n t;;

let l1 = 1::2::3::[];;
mem 3 l1;;
mem 4 l1;;

let rec find f l =
  match l with 
  [] -> failwith "find"
  | (h::t) -> if f h then h else find f t;;

let f n = n mod 2 = 0;;
let l1 = 1::3::6::7::10::[];;
find f l1;;
let l1 = 1::3::5::7::9::[];;
find f l1;;

let filter f l =
  let rec aux f laux l =
    match l with
     [] -> rev laux
     | (h::t) -> if f h then aux f (h::laux) t else aux f laux t
    in aux f [] l;;

let f n = n mod 2 = 0;;
let l1 = 2::3::6::7::10::[];;
filter f l1;;
    
let find_all f l = filter f l;;
find_all f l1;;

let partition f l =
  let rec aux f good bad l =
    match l with
     [] -> (rev good, rev bad)
     | (h::t) -> if f h then aux f (h::good) bad t else aux f good (h::bad) t
    in aux f [] [] l;;

let f n = n mod 2 = 0;;
let l1 = 2::3::6::7::10::[];;
partition f l1;;

let rec fold_left f init l =
  match l with
  [] -> init
  | (h::t) -> fold_left f (f init h) t;;

let f x y = x + y;;
let l1 = 1::2::3::4::[];;
fold_left f 0 l1;;

let rec fold_right f l init =
  match l with
  [] -> init
  | (h::t) -> f h (fold_right f t init);;

let f x y = x + y;;
let l1 = 1::2::3::4::[];;
fold_right f l1 0;;

let rec assoc a l =
  match l with
  [] -> failwith "assoc"
  | (x,y)::t -> if x = a then y else assoc a t;;

let l1 = [("a", 1); ("b", 2); ("c", 3)];;
assoc "b" l1;;
assoc "d" l1;;

let rec mem_assoc a l =
  match l with
  [] -> false
  | (x,y) :: t -> if x = a then true else mem_assoc a t;;

  let l1 = [("a", 1); ("b", 2); ("c", 3)];;

mem_assoc "b" l1;;
mem_assoc "d" l1;;

let remove_assoc a l =
  let rec aux laux l =
    match l with
    [] -> rev laux
    | (x,y) :: t -> if x = a then append laux t else aux ((x,y)::laux) t
  in aux [] l;;

let l1 = [("a", 1); ("b", 2); ("c", 3); ("b", 4)];;

remove_assoc "d" l1;;