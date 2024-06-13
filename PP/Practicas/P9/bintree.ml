type 'a bintree = Empty | Node of 'a * 'a bintree * 'a bintree;;

let rec in_order = function
  Empty -> []
| Node (r, i, d) -> in_order i @ r :: in_order d;;

let t41 = Node (1, Empty, Empty);;
let t42 = Node (3, Empty, Empty);;
let t31 = Node (2, t41, t42);;
let t32 = Node (6, Empty, Empty);;
let t33 = Node (9, Empty, Empty);;
let t21 = Node (5, t31, t32);;
let t22 = Node (8, Empty, t33);;
let t1 = Node (7, t21, t22);;

in_order t1;;

(*
            7
        5/       \8
    2/    \6   9/
  1/  \3
*)

let rec insert ord tree x = 
  match tree with
    Empty -> Node (x, Empty, Empty)
    | Node (r, i, d) -> if ord x r then Node (r, insert ord i x, d)
      else Node (r, i, insert ord d x);;

let comp x y = x <= y;; (*Si el elemento es menor o igual al nodo, va a la izquierda*)
insert comp t1 3;;
insert comp t1 10;;
in_order (insert comp t1 6);;

let bst ord l =
  let rec aux tree = function
    [] -> tree
    | h :: t -> aux (insert ord tree h) t
  in aux Empty l;;

bst (<=) ['c'; 'a'; 's'; 'u'; 'a'; 'l'];;


let qsort ord l =
  in_order (bst ord l);;

qsort (>=) [1; 0; 2; -1; 3; 10; 100];;
qsort (<=) [1; 0; 2; -1; 3; 10; 100];;