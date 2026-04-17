#include <stdio.h>

enum {
  N = 3
};

void swap(int *a, int *b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

void transpose(int m[N][N]) {
  for (int i = 0; i < N; ++i) {
    for (int j = i; j < N; ++j) {     // начинаем не с 0, а с i
        swap(&m[i][j], &m[j][i]);
    }
  }
}

int main() {
  int m[N][N] = {{1, 2, 3},
                 {4, 5, 6},
                 {7, 8, 9}};

  transpose(m);

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      printf("%d ", m[i][j]);
    }
    printf("\n");
  }
}
