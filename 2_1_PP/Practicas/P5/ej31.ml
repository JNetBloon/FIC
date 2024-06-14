let g n = (n >= 0 && n mod 2 = 0) || n mod 2 = -1;;
let g1 n = if n >= 0 then if n mod 2 = 0 then true else if n mod 2 = -1 then true else false else if n mod 2 = -1 then true else false;;
let g2 n = (function
  | true -> (function | true -> true | false -> (function | true -> true | false -> false) (n mod 2 = -1)) (n mod 2 = 0)
  | false -> (function | true -> true | false -> false) (n mod 2 = -1)
) (n >= 0);;
g 0;;
g 1;;
g 2;;
g (-2);;
g (-3);;
g (-4);;

g1 0;;
g1 1;;
g1 2;;
g1 (-2);;
g1 (-3);;
g1 (-4);;

g2 0;;
g2 1;;
g2 2;;
g2 (-2);;
g2 (-3);;
g2 (-4);;

(*Todos deberian dar true false true false true false*)