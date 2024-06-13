#define N 32768

int i;
int a[N], b[N], t;

int main() {

  for(i=0; i<N; i++) {
    b[i] = rand() % (N-1);
    a[i] = i;
    
    if(i<(N/4)){
      t += a[b[i]];
    }
  }
  
}

// valgrind --tool=cachegrind --cachegrind-out-file=output --D1=1024,2,32
// cg_annotate --auto=yes cachegrind.out.<pid>
