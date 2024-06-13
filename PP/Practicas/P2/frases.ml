(*x - y;;
Unbound value of x and y. No fueron inicializados*)
let x = 1;;
(*inicializar x - val x : int = 1*)
(*x - y;;
Unbound value of y. No fue inicializado*)
let y = 2;;
(*inicializar y - val y : int = 2*)
x - y;;
(*- : int = -1*)
let x = y in x - y;;
(*- : int = 0*)
x - y;;
(*- : int = -1*)
(*z;;
Unbound value of z. No fue inicializado*)
let z = x + y;;
(*inicializar z - val z : int = 3*)
z;;
(*- : int = 3*)
let x = 5;;
(*Cambiar valor de x - val x : int = 5*)
x + y;;
(*- : int = 7*)
z;;
(*- : int = 7*)
let y = 5 in x + y;;
(*- : int = 10*)
x + y;;
(*- : int = 7*)
let x = x + y in let y = x * y in x + y + z;;
(*- : int = 28*)
x + y + z;;
(*- : int = 14*)
function x -> 2 * x;;
(*Defines la funcion - : int -> int = <fun>*)
(function x -> 2 * x) (2 + 1);;
(*- : int = 6*)
(function x -> 2 * x) 2 + 1;;
(*- : int = 5*)
let f = function x -> 2 * x;;
(*Definicion de una nueva funcion, f - val f : int -> int = <fun>*)
f;;
(*Devuelve los parametros de su ejecucion - int -> int = <fun>*)
f(2 + 1);;
(*- : int = 6*)
f 2 + 1;;
(*- : int = 5*)
f x;;
(*- : int = 10*)
let x = 100;;
(*Ahora x valdra 100 - val x : int = 100*)
f x;;
(*- : int = 200*)
let m = 1000;;
(*val m : int = 1000*)
let g = function x -> x + m;;
(*Definicion de una nueva funcion, g - val g : int -> int = <fun>*)

g;;
(*Devuelve los parametros de su ejecucion int -> int = <fun>*)
g 3;;
(*- : int = 1003*)
(*g 3.0;;
Solo admite int, da error de tipo*)
let m = 7;;
(*Cambiar el valor de m a 7 - val m : int = 7*)
g 3;;
(*- : int = 1003*)
let istrue = function true -> true;;
(*Devuelve un warning avisando que hay ejemplos donde debería dar false*)
istrue;;
(*Devuelve los parametros de su ejecucion - : bool -> bool = <fun>*)
istrue (1<2);;
(*- : bool = true*)
istrue (2<1);;
(*Imprime un error de ejecución: Exception: Match_failure ("//toplevel//", 1, 13). Esto se debe a que no es capaz de imprimir false, es un error de ejecucion*)
(*istrue 0;;*)
(*Error de tipo. No admite enteros, solo booleanos*)


let iscero_v1 = function 0 -> true;;
(*val iscero_v1 : int -> bool = <fun>*)
iscero_v1 0;;
(*- : bool = true*)
(*iscero_v1 0.;;*)
(*Error de tipo. No admite tipo flotante*)
iscero_v1 1;;
(*Imprime: Exception: Match_failure ("//toplevel//", 1, 13). Esto se debe a que no es capaz de imprimir false, es un error de ejecucion*)


let iscero_v2 = function 0 -> true | _ -> false;;
(*Esta funcion devuelve val iscero_v2 : int -> bool = <fun>*)
iscero_v2 0;;
(*- : bool = true*)
iscero_v2 1;;
(*- : bool = false*)
(*iscero_v1 0.;;*)
(*No admite tipo flotante, error de tipo*)
let all_to_true = function true -> true | false -> true;;
(*Esta funcion devuelve val all_to_true : bool -> bool = <fun>*)
all_to_true (1<2);;
(*- : bool = true*)
all_to_true (2<1);;
(*- : bool = true*)
(*all_to_true 0;;*)
(*Error de tipo, solo admite booleanos*)
let first_all_to_true = all_to_true;;
(*Esta funcion devuelve val first_all_to_true : bool -> bool = <fun>*)
let all_to_true = function x -> true;;
(*Esta funcion devuelve val all_to_true : 'a -> bool = <fun>*)
all_to_true (1<2);;
(*- : bool = true*)
all_to_true (2<1);;
(*- : bool = true*)
all_to_true 0;;
(*- : bool = true*)
(*first_all_to_true 0;;*)
(*Error de tipo, solo admite booleanos*)
