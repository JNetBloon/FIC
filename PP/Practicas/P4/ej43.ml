let rec num_cifras n = if n<10 then 0 else 1 + num_cifras (n/10);;
let rec exp10 = function 0 -> 1 | n -> 10 * exp10(n-1);;
let rec reverse x = if x<10 then x else ((x mod 10) * exp10(num_cifras(x))) + reverse(x/10);;
reverse 3;;
reverse 12;;
reverse 20;;
reverse 10247;;
reverse 0b101;;
reverse 0b100000;;

let rec palindromo a = if (a = "") || (String.length a = 1) then true else if String.get a 0 = String.get a (String.length a -1) then palindromo(String.sub a 1 (String.length a -2)) else false;;
palindromo "";;
palindromo "a";;
palindromo "elle";;
palindromo "reconocer";;
palindromo "recoger";;
palindromo "Ana";;

let rec mcd (x,y) = if y>0 then mcd (y, x mod y) else x;;
mcd (17, 23);;
mcd (10,12);;
mcd (48, 96);;