let rec elevado x n = if n == 0 then 1 else x * elevado x (n-1);;
let rec power x y = if y==0 then x else x * elevado x (y-1);;
let rec power' x y = if y mod 2==0 then elevado(x*x) (y/2) else x * elevado (x*x) (y/2);;
power 2 10;;
power' 2 10;;
(*En termino de eficiencia merece mas la pena porque avanza mas rapido en la sucesion,
   ya que power va recorriendo y posiciones mientras que en power', al dividir entre 2,
   llegas a valores mas bajos con mucha mas rapidez*)

let rec elevadof x n = if n == 0 then 1. else x *. elevadof x (n-1);;
let rec powerf x y = if y mod 2==0 then elevadof(x*.x) (y/2) else x *. elevadof (x*.x) (y/2);;
powerf 2. 10;;