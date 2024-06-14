(*
   HECHO POR JAIME CAMIÑA CANEDA
   *)

(* VALID PATH*)
let rec existe e1 l = (*Verifica si existe en la lista un elemento*)
  match l with
  | [] -> false
  | h :: t -> if fst e1 = fst (List.hd l) && snd e1 = snd (List.hd l) then true else existe e1 t;;

let rec camino_valido e1 e2 = (* El cambio de posición es válido si (x1,_) = (x2,_) y el valor absoluto de (_,y1) y (_,y2) es igual a 1 y viceversa*)
  (fst e1 = fst e2 && abs (snd e1 - snd e2) = 1) || (snd e1 = snd e2 && abs (fst e1 - fst e2) = 1)

let valid_path m n ini fin path = 
  let rec aux l =
    match l with
    [] -> true
    | [h] -> true
    | h::t -> if existe h t then false (*Implementado manualmente de forma recursivo-terminal*)
      else if camino_valido h (List.hd t) then aux t else false
  in if fst ini > m || snd ini > n || fst fin > m || snd fin > n 
    || List.hd path <> ini || List.hd (List.rev path) <> fin
    then false
      else aux path;;

(* PATH WEIGHT*)
let rec path_weight v path =
  match path with
  | [] -> 0
  | (x, y) :: t -> v.(x).(y) + path_weight v t

(*NOTA DEL AUTOR: 
  Para la función estrella del programa, min_weight_path, creé dos versiones, una donde se comprueba si el nodo actual ya fue recorrido usando
  List.mem y otra donde usa una matriz auxiliar de booleanos. Esto es así para asegurar el que funcione una de las dos versiones en caso de
  fallar una de ellas en el compilado (Es más eficiente la que usa una matriz auxiliar ya que List.mem tiene una complejidad O(n) comparada con 
  O(1) del acceso al vector)
  
  EXPLICACIÓN ALGORITMO:
  Este algoritmo, llamado dijkstra (Adaptado a ocaml), empieza a mirar desde un punto del vector, al que llamaré nodo para la explicación,
  todos los caminos más cortos hasta el punto final. El objetivo es buscar el camino más corto de menor peso. Para ello se emplea recursividad
  hacia los 4 nodos adyacentes (Siempre comprobando que sean válidos) hasta llegar al caso base, que es llegar al nodo destino. En caso de
  introducir algún nodo no válido como argumento a la función este devuelve Invalid_argument, evitando accesos que pongan en peligro el programa.
  
  Se usan 3 funciones auxiliares y una ayuda:
  -errorinicial: Comprueba que no se salga de los límites del array la búsqueda del camino
  -empate (AUXILIAR DE min4): En caso de haber dos caminos con el mismo peso, mira qué camino es más corto
  -min4: Recibe 4 nodos y verifica cuál de ellos tiene un peso menor

  +max_int: Se usa para al llegar a un nodo no válido, devolver un peso alto para comparar correctamente el resto de caminos
*)


(*PESO MINIMO CON List.mem*)
let max_int = 100000;;

let errorinicial v ini fin =
  fst ini > Array.length v || snd ini > Array.length v.(0) || fst fin > Array.length v || snd fin > Array.length v.(0) ;;

let empate a b = (* Buscar el que tenga el camino más corto*)
  let lenl1 = List.length (snd a) in
  let lenl2 = List.length (snd b) in
  if lenl1 < lenl2 then b else a;;

let min4 (a, b, c, d) = (*Le pasas un tipo (int, list). Aqui lo que hace es comparar ese int, el tamño del camino*)
  let min_ab = if fst a = fst b then empate a b else (if fst a < fst b then a else b) in
  let min_cd = if fst c = fst d then empate c d else (if fst c < fst d then c else d) in
  if fst min_ab = fst min_cd then empate min_ab min_cd else (if fst min_ab < fst min_cd then min_ab else min_cd);;

let min_weight_path v ini fin =
  let rec aux i j mini path =
    if (i,j) = fin then (mini, (i,j) :: path) else (*Caso base: Encontrar el nodo final*)
      if List.mem (i, j) path && mini > 0 then (max_int, path) else (*Si ya se visitó el nodo no vuelve a comprobarlo*)
        let nuevo_path = (i, j) :: path in
        let nuevo_min = mini + v.(i).(j) in
        let caminoMIN = min4 (
          (if i - 1 >= 0 then aux (i - 1) j nuevo_min nuevo_path else (max_int, path)),
          (if i + 1 < Array.length v then aux (i + 1) j nuevo_min nuevo_path else (max_int, path)),
          (if j - 1 >= 0 then aux i (j - 1) nuevo_min nuevo_path else (max_int, path)),
          (if j + 1 < Array.length v.(0) then aux i (j + 1) nuevo_min nuevo_path else (max_int, path)) )
        in caminoMIN
in if errorinicial v ini fin then invalid_arg "min_weight_path" else 
  if ini = fin then [ini] else List.rev (snd (aux (fst ini) (snd ini) 0 []));; (*Comprobación inicial*)

(*PESO MINIMO CON ARRAY AUXILIAR*)
let max_int = 100000;;

let errorinicial v ini fin =
  fst ini > Array.length v || snd ini > Array.length v.(0) || fst fin > Array.length v || snd fin > Array.length v.(0) ;;

let empate a b = (* Buscar el que tenga el camino más corto*)
  let lenl1 = List.length (snd a) in
  let lenl2 = List.length (snd b) in
  if lenl1 < lenl2 then b else a;;

let min4 (a, b, c, d) = (*Le pasas un tipo (int, list). Aqui lo que hace es comparar ese int, el tamño del camino*)
  let min_ab = if fst a = fst b then empate a b else (if fst a < fst b then a else b) in
  let min_cd = if fst c = fst d then empate c d else (if fst c < fst d then c else d) in
  if fst min_ab = fst min_cd then empate min_ab min_cd else (if fst min_ab < fst min_cd then min_ab else min_cd);;

let min_weight_path v ini fin =
  let visitado = Array.make_matrix (Array.length v) (Array.length v.(0)) false in
  let rec aux i j mini path =
    if i = fst fin && j = snd fin then (mini, (i, j) :: path) (*Caso base: Encontrar el nodo final*)
    else if visitado.(i).(j) then (max_int, path) (*Si ya se visitó el nodo no vuelve a comprobarlo*)
      else
        let () = visitado.(i).(j) <- true in
        let nuevo_path = (i, j) :: path in
        let nuevo_min = mini + v.(i).(j) in
        let caminoMIN = min4 (
          (if i - 1 >= 0 then aux (i - 1) j nuevo_min nuevo_path else (max_int, path)),
          (if i + 1 < Array.length v then aux (i + 1) j nuevo_min nuevo_path else (max_int, path)),
          (if j - 1 >= 0 then aux i (j - 1) nuevo_min nuevo_path else (max_int, path)),
          (if j + 1 < Array.length v.(0) then aux i (j + 1) nuevo_min nuevo_path else (max_int, path))
        ) in
        let () = visitado.(i).(j) <- false in (* Al acabar la recursion en cada rama hay que declararlo como no visitado para que peuda ser visitado por otros nodos*)
        caminoMIN
  in if errorinicial v ini fin then invalid_arg "min_weight_path" else 
    if ini = fin then [ini] else List.rev (snd (aux (fst ini) (snd ini) 0 []));; (*Comprobación inicial*)

(*TEST*)
let v = [|[|6; 2; 1; 1; 6|]; [|6; 5; 1; 2; 1|]; [|1; 2; 3; 1; 1|];[|1; 8; 6; 5; 1|]|];;
min_weight_path v (2, 3) (2, 3);;
min_weight_path v (2, 3) (3, 3);;
min_weight_path v (2, 3) (3, 2);;
let p = min_weight_path v (0, 0) (3, 4);;
valid_path 4 5 (0, 0) (3, 4) p;;
path_weight v p;;

let v = [|[| 1; 1; 1; 1; 10|]; [| 1; 4; 6; 8; 10|]; [| 1; 1; 1; 1; 1|]; [|10; 8; 6; 4; 1|]; [| 1; 1; 1; 1; 1|]|];;
let p = min_weight_path v (0, 4) (4,0);;
path_weight v p;;

let v = [|[|1; 1; 1; 10|]; [|1; 3; 5; 7|]; [|1; 1; 1; 10|]|];;
let p = min_weight_path v (0,3) (2,0);;
path_weight v p;;

(*TEST ERRORES*)
let v = [|[|6; 2; 1; 1; 6|]; [|6; 5; 1; 2; 1|]; [|1; 2; 3; 1; 1|];[|1; 8; 6; 5; 1|]|];;
Array.length v;;
Array.length v.(0);;
min_weight_path v (1, 3) (4, 6);;
min_weight_path v (5, 1) (2, 3);;
