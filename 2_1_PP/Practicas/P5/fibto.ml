let rec fib n =
  if n <= 1 then n
  else fib (n-1) + fib (n-2)

let fibto n =
  let rec fibtoAux i =
    if fib(i) <= n then (
        print_int (fib(i));
        print_newline ();
        fibtoAux (i + 1)
      )
  in fibtoAux 0;;

if Array.length Sys.argv < 2 then
  print_string "fibto: Invalid number of arguments\n"
else
  let rec cont i =
    if i < Array.length Sys.argv then (
      let arr = Sys.argv.(i) in
      fibto (int_of_string arr);
      cont (i + 1)
    )
  in cont 1;;
