let f n = if n mod 2 = 0 then n / 2 else 3 * n + 1;;

let rec orbit n =
  if n = 1 then "1"
  else string_of_int n ^ ", " ^ orbit (f n);;

orbit;;
orbit 13;;
orbit 1;;

let length num =
  let rec aux i n=
    if n = 1 then i
    else
      aux (i + 1) (f n)
  in aux 0 num;;

length;;
length 13;;
length 27;;

let top num =
  let rec aux top n=
    if n = 1 then top
    else
      aux (if f n > top then f n else top) (f n)
  in aux 1 num;;

top;;
top 13;;
top 27;;

let length'n'top num =
  let rec aux (i, top) n =
    if n = 1 then (i,top)
    else
      aux(i+1,if f n > top then f n else top) (f n)
    in aux (0, 1) num;;

length'n'top;;
length'n'top 13;;
length'n'top 27;;

let longest_in num1 num2 =
  let rec aux i nmax max =
    if i > num2 then (nmax,max) else if length i > max then aux (i+1) i (length i) else aux (i + 1) nmax max
  in aux num1 1 1;;

longest_in;;
longest_in 1 1000;;

let highest_in num1 num2 =
  let rec aux i nmax max =
    if i > num2 then (nmax,max) else if top i > max then aux (i+1) i (top i) else aux (i + 1) nmax max
  in aux num1 1 1;;

highest_in;;
highest_in 1 1000;;