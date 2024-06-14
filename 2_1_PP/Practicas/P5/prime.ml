let is_prime n =
  let rec check_from i =
  i >= n ||
  (n mod i <> 0 && check_from (i+1))
  in check_from 2;;

is_prime 4;;

let next_prime n =
  let rec check_from i =
    if(is_prime(i)) then i else check_from (i+1)
  in check_from (n+1);;

next_prime 11;;
next_prime 12;;

let last_prime_to n =
  let rec check_from i =
    if(is_prime(i)) then i else check_from (i-1)
  in check_from n;;

last_prime_to 11;;
last_prime_to 12;;

let sqrtPRO n = int_of_float (sqrt(float_of_int n));;
let is_prime2 n =
  let rec check_from i =
  i >= (sqrtPRO(n)+1) ||
  (n mod i <> 0 && check_from (i+1))
  in check_from 2;;

is_prime2 6;;

is_prime 1_000_000_007;;
is_prime2 1_000_000_007;;