(*Definiciones del enunciado de isort*)
let rec insert x = function
  [] -> [x]
  | h::t -> if x <= h then x :: h :: t
    else h :: insert x t;;

let rec isort = function
  [] -> []
  | h::t -> insert h (isort t);;

(*Test para isort y isort_t*)
let bigl = List.init 250000 (fun x -> x+1);;
isort bigl;;

(* Recursivo terminal *)
let insert_t x l =
  let rec aux laux = function
    | [] -> List.rev (x :: laux)
    | h :: t ->
        if x <= h then List.rev_append laux (x :: h :: t)
        else aux (h :: laux) t
  in aux [] l;;

let rec isort_t l =
  let rec aux laux = function
    | [] -> laux
    | h :: t -> aux (insert_t h laux) t
  in aux [] l;;

isort_t bigl;;
isort_t (5::3::4::2::1::[]);;
isort_t (1::2::4::3::5::[]);;

(*
  NOTA: No estaba muy seguro de qué forma implementar la condición del enunciado "no contener muchos valores repetidos" por lo 
  que cree dos versiones, una que evita que haya numeros repetidos consecutivos y otra con valores únicos (Con el tamaño de la lista)   
*)
(*Lista al azar que no repite numeros consecutivos (MÁS RAPIDA)*)
let rec notsamenum n prevnum =
  let num = Random.int n in
  if num = prevnum then
    notsamenum n prevnum
  else num;;

let rlist n =
  let rec aux l i prevNum =
    let prevNum = (notsamenum n prevNum) in
    if (i<n) then aux (prevNum :: l) (i+1) prevNum
    else l
  in aux [] 0 0;;

let l = rlist 10;;

(*Lista al azar que no repite numeros en la misma lista (MÁS LENTA)*)
let rec existNum n = function
  [] -> false
  | h::t -> if h = n then true else existNum n t;;

let rec rNum n l =
  let num = Random.int n in
  if existNum num l then rNum n l else num;; 

let rlist n =
  let rec aux l i =
    if (i<n) then 
      let rn = rNum n l in aux (rn :: l) (i+1)
    else l
  in aux [] 0;;

let l = rlist 10;;

(* Comparaciones de funciones isort *)

let lc1 = List.init 10000 (fun x -> x+1);;
let lc2 = List.init 20000 (fun x -> x+1);;
let ld1 = List.init 10000 (fun x -> 10000 - x);;
let ld2 = List.init 20000 (fun x -> 20000 - x);;
let lr1 = rlist 10000;;
let lr2 = rlist 20000;;

let crono f x =
  let t = Sys.time () in
  let _ = f x in
  Sys.time () -. t;;

(*Caso optimo. En este caso duplicar el tamaño no afecta mucho al tiempo. isort_t tiene peor tiempo*)
isort lc1;;
isort_t lc1;;
isort lc2;;
isort_t lc2;;

(*Peor caso. El duplicar el tamaño empeora mucho el tiempo para isort. isort_t obtiene mejores tiempos*)
isort ld1;;
isort_t ld1;;
isort ld2;;
isort_t ld2;;

(*Caso medio. Cuanto más desordenado mejor opción es isort_t*)
crono isort lr1;;
crono isort_t lr1;;
crono isort lr2;;
crono isort_t lr2;;

(*isort G*)
let insert_g funcion x l =
  let rec aux laux = function
    | [] -> List.rev (x :: laux)
    | h :: t ->
        if funcion x h then List.rev_append laux (x :: h :: t)
        else aux (h :: laux) t
  in aux [] l;;

let isort_g funcion l =
  let rec aux acc = function
    | [] -> acc
    | h :: t -> aux (insert_g funcion h acc) t
  in aux [] l;;

let listaASCEN x y = x <= y;;
isort_g listaASCEN [5; 3; 4; 2; 1];;
let listaDESCEN x y = x > y;;
isort_g listaDESCEN [5; 3; 4; 2; 1];;

(*Implementacion de ordenacion por fusión*)
let rec split l = match l with
  h1::h2::t -> let t1, t2 = split t
  in h1::t1, h2::t2
  | _ -> l, [];;

let rec merge (l1,l2) = match l1, l2 with
  [], l | l, [] -> l
  | h1::t1, h2::t2 -> if h1 <= h2 then h1 :: merge (t1, l2)
  else h2 :: merge (l1, t2);;

let rec msort l = match l with
  [] | [_] -> l
  | _ -> let l1, l2 = split l
  in merge (msort l1, msort l2);;

msort (5::3::4::2::1::[]);;
msort (1::2::4::3::5::[]);;

(*bigl2 para probar msort y msorrt'*)
let bigl2 = List.init 300000 (fun x -> 300000-x);;
msort bigl2;;

(*Implementacion de ordenacion por fusión RECURSIVA TERMINAL*)
let split_t l = 
  let rec aux l1 l2 = function
    | h1::h2::t -> aux (h1::l1) (h2::l2) t
    | [] -> List.rev l1, List.rev l2
    | h::[] -> List.rev (h::l1), List.rev l2
  in aux [] [] l;;

let merge_t (l1,l2) = 
  let rec aux laux l1 l2 = 
    match l1, l2 with
    | [], l | l, [] -> List.rev_append laux l
    | h1::t1, h2::t2 -> if h1 <= h2 then aux (h1 :: laux) t1 l2
    else aux (h2 :: laux) l1 t2
  in aux [] l1 l2;;
  
let rec msort' l = match l with
  [] | [_] -> l
  | _ -> let l1, l2 = split_t l
  in merge_t (msort' l1, msort' l2);;

msort (5::3::4::2::1::[]);;
msort (1::2::4::3::5::[]);;
msort' (5::3::4::2::1::[]);;
msort' (1::2::4::3::5::[]);;


(*
  Aun que msort' no es recursiva terminal, no da error al probarlo porque al usar divide y vencerás con la recursión terminal,
  las llamadas a la función no se acumulan en la pila. Cada vez que termina un paso devuelve el resultado y lo libera de la pila, evitando
  cualquier tipo de desbordamiento.

  bigl3 no inicia ninguna recursión por lo que tampoco da problemas.
*)

msort bigl2;;
msort' bigl2;;

let bigl3 = [];;
msort' bigl3;;
msort bigl3;;

(*Comparación de tiempos en el peor y el mejor caso *)
crono msort lc1;;
crono msort lc2;;
crono msort ld1;;
crono msort ld2;; 
crono msort' lc1;;
crono msort' lc2;;
crono msort' ld1;;
crono msort' ld2;; 

(* msort' obtiene mejores tiempos en ambos casos *)

(*Tiempos con msort *)
crono msort lr1;;
crono msort lr2;;
crono msort' lr1;;
crono msort' lr2;;

(* msort' obtiene mejores tiempos *)


let split_g l = 
  let rec aux l1 l2 = function
    | h1::h2::t -> aux (h1::l1) (h2::l2) t
    | [] -> List.rev l1, List.rev l2
    | h::[] -> List.rev (h::l1), List.rev l2
  in aux [] [] l;;

let merge_g funcion (l1,l2) = 
  let rec aux laux l1 l2 = 
    match l1, l2 with
    | [], l | l, [] -> List.rev_append laux l
    | h1::t1, h2::t2 -> if funcion h1 h2 then aux (h1 :: laux) t1 l2
    else aux (h2 :: laux) l1 t2
  in aux [] l1 l2;;
  
let rec msort_g funcion l = match l with
  [] | [_] -> l
  | _ -> let l1, l2 = split_g l
  in merge_g funcion (msort_g funcion l1, msort_g funcion l2);;

msort_g;;
let listaASCEN x y = x <= y;;
msort_g listaASCEN [5; 3; 4; 2; 1];;
let listaDESCEN x y = x > y;;
msort_g listaDESCEN [5; 3; 4; 2; 1];;