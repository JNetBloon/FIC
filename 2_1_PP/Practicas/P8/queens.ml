(* Función queens y auxiliares sacadas (y modificadas para cumplir los requisitos) de clase de teoría de José María Molinelli*)

let valido (i1, j1) (i2, j2) =
  i1 = i2 || j1 = j2 || abs (i1 - i2) = abs (j1 - j2);;

let legal p l =
  not (List.exists (valido p) l);;

let queens n =
  let rec aux path i j =
    if i > n 
      then [path]
    else if j > n 
      then []
    else if legal (i, j) path 
      then
        match aux ((i, j) :: path) (i + 1) 1 
          with
            [] -> aux path i (j + 1)
          | laux -> laux @ aux path i (j + 1)
      else aux path i (j + 1)
  in aux [] 1 1;;

queens;;
queens 0;;
queens 1;;
queens 2;;
queens 4;;

let validpos e n =
  if fst e > n || fst e < 0
    || snd e > n || snd e < 0
  then false else true;;

let is_queens_sol n l =
  let rec aux laux =
    match laux with
    [] -> true
    | h :: t -> if (legal h t) && validpos h n then aux t else false
  in if n <> List.length l then false else aux l;;

(*Añadí más casos extra para probar su funcionamiento. Las que se usan en el pdf están separadas al final*)
is_queens_sol 1 [(1, 1)];;
is_queens_sol 1 [(-1, 1)];;
is_queens_sol 2 [(1, 1); (2, 2)];;
is_queens_sol 2 [(1, 1); (2, 3)];;
is_queens_sol 4 [(1, 2); (2, 4); (3, 1); (4, 3)];;
is_queens_sol 4 [(2, 4); (1, 2); (3, 1); (4, 3)];;
is_queens_sol 4 [(2, 3); (1, 2); (3, 1); (4, 3)];;


is_queens_sol 2 [(1, 1); (2, 2)];;
is_queens_sol 2 [(1, 1); (2, 3)];;
let check n = List.for_all (is_queens_sol n) (queens n);;
List.for_all check (List.init 13 abs);;
List.map (fun i -> i, List.length (queens i)) (List.init 13 abs);;