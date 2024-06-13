let p = (1 +1, asin 1.), true;;
(*val p : (int * float) * bool = ((2, 1.57079632679489656), true)*)
(*Crea una variable p con dos elementos*)
let (x,y), z = p;;
(*
   val x : int = 2
   val y : float = 1.57079632679489656
   val z : bool = true
   *)
(*Asigna tres valores a los resultados de p*)
let p1, p2 = p in let p11, _ = p1 in (p2, 2 * p11);;
(*- : bool * int = (true, 4)*)
(*Hace que p1 y p2 tengan los elementos correspondientes de p
   Luego asigna p1 a p11, ignorando su segundo elemento con "_"
   finalmente asigna al valor "true" de p2 y el resultado de 2 * p11 (Vale 1+1, lo que le pasa p)*)
let f(x, y) = 2 * x + y;;
(*val f : int * int -> int = <fun>*)
(*Definicion de funcion de dos variables*)
let f2 x y z =  x + 2 * y + 3 * z;;
(*val f2 : int -> int -> int -> int = <fun>*)
(*Consigue tres argumentos int y devuelve un int resultado*)
let g x y z = x (y, z);;
(*val g : ('a * 'b -> 'c) -> 'a -> 'b -> 'c = <fun>*)
(*g toma los argumentos y devuelve una funcion de dos argumentos*)
g fst 1 "hola";;
(*- : int = 1*)
(*fst toma el primer elemento de la tupla (x, y), es como hacer fast(1, "hola")*)
g snd fst true;;
(*- : bool = true*)
(*snd sirve para tomar el segundo elemento de la tupla. Aqui, g toma x como snd, y como fst y z como true*)
g f 2 3;;
(* - : int = 7*)
g (function (f, x) -> f (f x)) (function x -> x * x) 3;;
(*- : int = 81*)
(*Define la funcion g con dos argumentos tipo funcion y un numero entero f se convierte en la funcion x * x por su siguiente argumento, 
   y ese adquiere el valor 3 por el tercer argumento. De esa forma acabas haciendo 3 * 9 * 3 = 81*)
let x, y, z = 1, 2, 3;;
(*val x : int = 1
  val y : int = 2
  val z : int = 3
*)
f2 x y z;;
(*- : int = 14*)
let x, y, z = y, z, x in f2 x y z;;
(*- : int = 11*)
(*Cambiamos los valores, por lo que "x" es "y", "y" es "z", y "z" es "x". Por eso devuelve otro valor*)
f2 x, y, z;;
(*- : (int -> int -> int) * int * int = (<fun>, 2, 3)*)
(*Si pones comas lo interpreta como una funcion que define el usuario*)
let swap (x, y) = y, x;;
(*val swap : 'a * 'b -> 'b * 'a = <fun>*)
let p = 1, 2;;
(*val p : int * int = (1, 2)*)
(*Defines dos argumentos en p de tipo entero int * int*)
f p;;
(*- : int = 4*)
let p = swap p in f p;;
(*- : int = 5*)
f p;;
(*- : int = 4*)
