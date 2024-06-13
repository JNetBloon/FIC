let rec sumto = function 0 -> 0 | x -> x + sumto(x-1);;
let rec exp10 = function 0 -> 1 | n -> 10 * exp10(n-1);;
let rec num_cifras x = if (x<10 && x>=0) then 1 else if (x<0) then num_cifras(-x) else 1 + num_cifras(x/10);;
let rec sum_cifras x = if (x<10 && x>=0) then x else if (x<0) then num_cifras(-x) else x mod 10 + sum_cifras(x/10);;

let rec factorial = function 0 -> 1 | n -> n * factorial (n-1);;
(*Inicializa la funcion recursva factorial*)
(*val factorial : int - int <fun>*)
factorial 0 + factorial 1 + factorial 2;;
(*Suma el resultado del factorial de 0, 1 y 2 (1+1+2)*)
(*- : int = 4*)
factorial 10;;
(*Imprime el resultado del factorial de 10*)
(*- : int = 3628800*)
factorial 100;;
(*Debería dar error al ser un número tan grande para calcularlo*)
(*- : int = 0*)
factorial (-1);;
(*Daría un error en su ejecución ya que entra en un bucle infinito*)
(*Stack overflow during evaluation (looping recursion?).*)
