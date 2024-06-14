/* programa traspuesta.c */

#define N 200
int a[N][N], b[N][N];

int main()
{
  int i, j;

  for(i = 0; i < N; i++)
    for(j = 0; j < N; j++)
    {
      a[i][j] = (i * j);
      b[j][i] = a[i][j];
    }
}
