val valid_path : int -> int -> int * int -> int * int -> (int * int) list -> bool

val path_weight : int array array -> (int * int) list -> int

val min_weight_path :
  int array array -> int * int -> int * int -> (int * int) list