(function x -> x) 1;;
(function y -> y) 2;;
(function x -> (function y -> x - y));;
(*- : int = -1*)
(function x -> (function y -> y - y));;
(*- : int = 0*)
(function z -> (function x->x) 1 + (function y->y) 2);;
(*inicializar z - val z : int = 3*)
(function x -> x) 5;;
(*Cambiar valor de x - val x : int = 5*)
(function x -> (function y -> x + y));;
(*- : int = 7*)
(function x -> (function y -> x + (function y->y) 5));;
(*- : int = 10*)
(function x -> (function y -> x + y));;
(*- : int = 7*)
(function y -> (function x -> (function z -> z + (x+y) + y * (x + y))));;
(*- : int = 28*)
(function x -> (function y -> (function z -> x + y + z)));;
(*- : int = 14*)
function x -> 2 * x;;
(*Defines la funcion - : int -> int = <fun>*)
(function x -> 2 * x) (2 + 1);;
(*- : int = 6*)
(function x -> 2 * x) 2 + 1;;
(*- : int = 5*)
(function f -> (function x -> 2 * x));;
(*Definicion de una nueva funcion, f - val f : int -> int = <fun>*)
(function f -> (function x -> 2 * x)) (2+1);;
(*- : int = 6*)
(function f -> (function x -> 2 * x)) 2;;
(*- : int = 5*)
(function f -> (function x -> 2 * x)) 5;;
(function x -> x) 100;;
(*Ahora x valdra 100 - val x : int = 100*)
(function f -> (function x -> 2 * x)) (function x -> x) 100;;;;
(*- : int = 200*)
(function m -> m) 1000;;
(*val m : int = 1000*)
(function g -> (function x -> (function m -> x+m)));;
(*Definicion de una nueva funcion, g - val g : int -> int = <fun>*)

(function g -> (function x -> (function m -> x+m))) 3;;
(*- : int = 1003*)
(function m -> m) 7;;
(*Cambiar el valor de m a 7 - val m : int = 7*)
(function g -> (function x -> (function m -> x+m))) 3;;
(*- : int = 1003*)
(function istrue -> (function true -> true));;
(*Devuelve un warning avisando que hay ejemplos donde debería dar false*)
(function istrue -> (function true -> true)) (1<2);;
(*- : bool = true*)
(function istrue -> (function true -> true)) (2<1);;
(*Imprime un error de ejecución: Exception: Match_failure ("//toplevel//", 1, 13). Esto se debe a que no es capaz de imprimir false, es un error de ejecucion*)

(function iscero_v1 -> (function 0 -> true));;
(*val iscero_v1 : int -> bool = <fun>*)
(function iscero_v1 -> (function 0 -> true)) 0;;
(*- : bool = true*)
(function iscero_v1 -> (function 0 -> true)) 1;;
(*Imprime: Exception: Match_failure ("//toplevel//", 1, 13). Esto se debe a que no es capaz de imprimir false, es un error de ejecucion*)

(function iscero_v2 -> (function 0 -> true | _ -> false));;
(*Esta funcion devuelve val iscero_v2 : int -> bool = <fun>*)
(function iscero_v2 -> (function 0 -> true | _ -> false)) 0;;
(*- : bool = true*)
(function iscero_v2 -> (function 0 -> true | _ -> false)) 1;;
(*- : bool = false*)
(function all_to_true -> (function true -> true | false -> true));;
(*Esta funcion devuelve val all_to_true : bool -> bool = <fun>*)
(function all_to_true -> (function true -> true | false -> true)) (1<2);;
(*- : bool = true*)
(function all_to_true -> (function true -> true | false -> true)) (2<1);;
(*- : bool = true*)
(function first_all_to_true -> (function all_to_true -> (function true -> true | false -> true)));;
(*Esta funcion devuelve val first_all_to_true : bool -> bool = <fun>*)
(function all_to_true -> (function x -> true));;
(*Esta funcion devuelve val all_to_true : 'a -> bool = <fun>*)
(function all_to_true -> (function x -> true)) (1<2);;
(*- : bool = true*)
(function all_to_true -> (function x -> true)) (2<1);;
(*- : bool = true*)
(function all_to_true -> (function x -> true)) 0;;
(*- : bool = true*)