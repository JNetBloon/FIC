#define N 32768

int i;
int a[N], b[N], t;

int main() {

  for(i=0; i<N; i++) {
    b[i] = rand() % (N-1);
    a[i] = i;
  }
    
    // bucle principal
    
  for(i=0; i<(N/4); i++) {
    t += a[b[i]];
  }
  
}

