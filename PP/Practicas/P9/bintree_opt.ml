open Bintree;;

let rec is_bst ord tree =  
  match tree with
    Empty -> true (*Caso base: El nodo está vacío*)
    | Node (r, i, d) ->  
      (match i with (*Primero se comprueba el orden en el nodo izquierdo*)
        Empty -> true
        | Node (ri, ii, di) -> ord ri r) 
      &&
      (match d with (*Despues en el nodo derecho*)
        Empty -> true
        | Node (rd, id, dd) -> ord r rd)
      && is_bst ord i && is_bst ord d;; (*Y se llama de forma rescursiva hasta el caso base*)

let t31 = Node (8, Empty, Empty);;
let t32 = Node (9, Empty, Empty);;
let t21 = Node (5, Empty, Empty);;
let t22 = Node (8, t31, t32);;
let t1 = Node (7, t21, t22);;

(*
      7
  5/    \8  
      8/    \9    

*)

is_bst (<=) t1;;
is_bst (<) t1;; (*Da true pero si el nodo más a la izquierda fuera 7 daría false*)
is_bst (>) t1;;


(* No fui capaz de hacer la función de forma recursiva no terminal, por lo que solo devuelve el arbol en una lista *)
let bfs tree = 
  match tree with
  Empty -> []
  | Node (r, i, d) -> [r];;

let bfs' tree =
  let rec aux nuevotre l =
    match nuevotre with
    [] -> List.rev l
    | Empty :: t -> aux t l(*Si esta vacío el nodo se avanza al siguiente de los pendientes*)
    | Node (r, i, d) :: t -> aux (t @ i::d::[]) (r :: l) (*Si hay un nodo se añade la raiz a la lista y los hijos quedan para procesar en la siguiente llamada*)
  in aux [tree] [];; (*Para esta funcion hice que el arbol estuviera en una lista para poder procesar nodo a nodo*)

bfs t1;;
bfs' t1;;

(* FUNCIÓN altura SACADA DE CLASES DE TEORÍA *)
let rec altura = function
	Empty -> 0
	| Node (r, i, d) -> 1 + max (altura i) (altura d);;

let rec perfecto = function
  Empty -> true
  | Node (r, i, d) -> altura i = altura d && perfecto i && perfecto d;; 
  (*Si por un camino y por otro hay la misma altura, se avanza a los siguientes nodos*)

let t33 = Node (4, Empty, Empty);;
let t34 = Node (6, Empty, Empty);;
let t31 = Node (8, Empty, Empty);;
let t32 = Node (9, Empty, Empty);;
let t21 = Node (5, t33, t34);;
let t22 = Node (8, t31, t32);;
let t1 = Node (7, t21, t22);;

(*
      7
  5/      \8
4/ \6   8/    \9

*)

bfs' t1;;
perfecto t1;;

let rec casi_completo = function
  Empty -> true
  | Node (r, i, d) -> if abs (altura i - altura d) <= 1 && (altura i >= altura d) then casi_completo i && casi_completo d else false;;
  (*Si el valor absoluto (Para cuando da negativo) es menor o igual a 1 significa que tienen un nivel de diferencia máximo*)
  (*En la siguiente condición se comprueba que la altura del nodo izquierdo es mayor al derecho, ya que deben estar pegados a la izquierda*)

let t41 = Node (10, Empty, Empty);;
let t33 = Node (4, t41, Empty);;
let t34 = Node (6, Empty, Empty);;
let t31 = Node (8, Empty, Empty);;
let t32 = Node (9, Empty, Empty);;
let t21 = Node (5, t33, t34);;
let t22 = Node (8, t31, t32);;
let t1 = Node (7, t21, t22);;

(*
        7
    5/      \8
  4/  \6  8/  \9
10/                

*)

bfs' t1;;
perfecto t1;;
casi_completo t1;;

let t41 = Node (10, Empty, Empty);;
let t33 = Node (4, Empty, t41);;
let t34 = Node (6, Empty, Empty);;
let t31 = Node (8, Empty, Empty);;
let t32 = Node (9, Empty, Empty);;
let t21 = Node (5, t33, t34);;
let t22 = Node (8, t31, t32);;
let t1 = Node (7, t21, t22);;

(*
        7
    5/      \8
  4/  \6  8/  \9
   \10               

*)

bfs' t1;;
perfecto t1;;
casi_completo t1;;