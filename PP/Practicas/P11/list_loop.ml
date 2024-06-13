(* Sacadas del enunciado para evitar problemas al compilar en otros dispositivos *)
let length = List.length;;
let rec last = function
[] -> failwith "last"
| h ::[] -> h
| _ :: t -> last t;;
let nth = List.nth;;
let rev = List.rev;;
let append = List.append;;
let concat = List.concat;;
let for_all = List.for_all;;
let exists = List.exists;;
let find_opt = List.find_opt;;
let iter = List.iter;;
let fold_left = List.fold_left;;

let length l =
  let i = ref 0 in
  let listaV = ref l in
  while !listaV <> [] do
    i := !i + 1;
    listaV := List.tl !listaV;
  done;
  !i;;

(*
let l1 = [1;2;4;5];;
let l2 = ['a';'b';'z'];;
let l3 = [];;
length l1;;
length l2;;
length l3;;
*)
let last l =
  let listaV = ref l in
  if length !listaV < 1 then failwith "Lista vacía" else 
  while length !listaV <> 1 do
    listaV := List.tl !listaV
  done;
  List.hd !listaV;;
(*
last l1;;
last l2;;
last l3;;
*)
let nth l n =
  if n < 0 then invalid_arg "Posición solicitada inválida" else
  let i = ref 0 in
  let listaV = ref l in
  while !i < n && !listaV <> [] do
    i := !i + 1;
    listaV := List.tl !listaV;
  done;
  match !listaV with
  [] -> failwith "No se encontró la posición especificada"
  | hd :: _ -> hd;;
(*
nth l1 3;;
nth l2 3;;
nth l3 (-2);;
*)
let rev l =
  let listaV = ref l in
  let listaOUT = ref [] in
  while !listaV <> [] do
    listaOUT := List.hd !listaV :: !listaOUT;
    listaV := List.tl !listaV;
  done;
  !listaOUT;;
(*
rev l1;;
rev l2;;
rev l3;;
*)
let meterlista lV listaOUT =
  while !lV <> [] do
    listaOUT := List.hd !lV :: !listaOUT;
    lV := List.tl !lV;
  done;;

let append l1 l2 =
  let listaV = ref l1 in
  let listaOUT = ref [] in
  let listaV2 = ref l2 in
  meterlista listaV listaOUT;
  meterlista listaV2 listaOUT;
  rev !listaOUT;;
(*
let l4 = [7;8];;
append l1 l4;;
*)
let concat l =
  let listaV = ref l in
  let listaOUT = ref [] in
  while !listaV <> [] do
    let lTMP = List.hd !listaV in
    listaOUT := append !listaOUT lTMP;
    listaV := List.tl !listaV;
  done;
  !listaOUT;;

(*
let juntar = [[1;2;]; [3;4;5]; [6]];;
concat juntar;;
*)
let for_all cond l =
  let listaV = ref l in
  let booleano = ref true in
  while !listaV <> [] && !booleano do
    if not (cond (List.hd !listaV)) then booleano := false 
      else listaV := List.tl !listaV;
  done;
  !booleano;;
(*
for_all (fun x -> x > 0) l1;;
(*for_all (fun x -> x > 0) l2;;*)
for_all (fun x -> x < 0) l1;;
*)

(* Basicamente lo mismo cambiando el booleano *)
let exists cond l =
  let listaV = ref l in
  let booleano = ref false in
  while !listaV <> [] && not !booleano do
    if cond (List.hd !listaV) then booleano := true
      else listaV := List.tl !listaV;
  done;
  !booleano;;

(*
for_all (fun x -> x > 3) l1;;
(*for_all (fun x -> x > 3) l2;;*)
for_all (fun x -> x < 6) l1;;
*)

let find_opt cond l =
  let listaV = ref l in
  let encontrado = ref None in
  while !listaV <> [] && !encontrado = None do
    if cond (List.hd !listaV) then encontrado := Some (List.hd !listaV)
      else listaV := List.tl !listaV;
  done;
  !encontrado;;

(*
find_opt (fun x -> x > 0) l1;;
find_opt (fun x -> x > 3) l1;;
(*find_opt (fun x -> x > 2) l2;;*)
find_opt (fun x -> x > 5) l1;;
*)
let iter f l =
  let listaV = ref l in
  while !listaV <> [] do
    f (List.hd !listaV);
    listaV := List.tl !listaV;
  done;;
(*
iter (fun x -> print_int x; print_string " ") l1;;
*)
let fold_left f ini l =
  let listaV = ref l in
  let listaOUT = ref ini in
  while !listaV <> [] do
    listaOUT := f !listaOUT (List.hd !listaV);
    listaV := List.tl !listaV;
  done;
  !listaOUT;;
(*
fold_left (fun ini x -> ini + x) 0 l1;; (*12*)
fold_left (fun ini x -> ini + x) 1 l1;; (*12 + 1 = 13*)
*)