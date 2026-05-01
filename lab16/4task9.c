#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// global vars
int N;
int th_count;
int **A, **B, **C;

// memory
int** alloc_matrix(int n) {
    int **M = malloc(n*sizeof(int*));   // arr of pointers
    for (int i=0; i<n; i++) M[i] = calloc(n, sizeof(int));  // rows
    return M;
}
void free_matrix(int **M) {
    for (int i=0; i<N; i++) free(M[i]);
    free(M);
}

// multiply rows in matrix
void* mult_rows(void* arg) {
    int t = (int)(long)arg;

    int rows_per_th = N / th_count;
    int start = t * rows_per_th;
    int end = start + rows_per_th;

    for (int i=start; i<end; i++) {
        for (int j=0; j<N; j++) {
            C[i][j] = 0;
            for (int k=0; k<N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return NULL;
}

void print_matrix(char *title, int **M) {
    printf("Matrix %s: \n", title);
    for (int i=0; i<N; i++) {
        printf("    ");
        for (int j=0; j<N; j++) printf("%4d", M[i][j]);
        printf("\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc!=3) {
        fprintf(stderr, "Usage: %s <N> <threads>\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);
    th_count = atoi(argv[2]);

    if (N<=0 || th_count<=0) {
        fprintf(stderr, "N and threads must be positive.\n");
        return 0;
    }
    if (th_count>N) {
        fprintf(stderr, "thread > N, reduce to %d.\n", N);
        th_count = N;
    }

    A = alloc_matrix(N); B = alloc_matrix(N); C = alloc_matrix(N);
    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {
            A[i][j] = B[i][j] = 1;
        }
    }

    int rows_per_th = N / th_count;
    int rem = N % th_count;
    printf("N = %d, threads = %d, rows per thread = %d, rem = %d\n", N, th_count, rows_per_th, rem);
    
    pthread_t *tids = malloc(th_count*sizeof(pthread_t));

    // timer start
    struct timespec start_t, end_t;
    clock_gettime(CLOCK_MONOTONIC, &start_t);

    for (int t=0; t<th_count; t++) {
        printf("    Thread %d: stroke [%d, %d)\n", t, rows_per_th*t, rows_per_th*(t+1));
        pthread_create(&tids[t], NULL, mult_rows, (void*)(long)t);
    }

    for (int t=0; t<th_count; t++) {
        pthread_join(tids[t], NULL);
    }

    // remainder
    if (rem>0) {
        int rem_start = th_count * rows_per_th;
        printf("    Main thread: lines [%d, %d)\n", rem_start, rem_start+rem);
        for (int i=rem_start; i<(rem_start+rem); i++) {
            for (int j=0; j<N; j++) {
                C[i][j] = 0;
                for (int k=0; k<N; k++) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
    }

    // timer stop
    clock_gettime(CLOCK_MONOTONIC, &end_t);

    if (N<5) {
        print_matrix("A", A);
        print_matrix("B", B);
        print_matrix("C = A*B", C);
    }

    // check
    int ok = 1;
    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {
            if (C[i][j]!=N) ok = 0;
        }
    }
    printf("Check: expected %d in each cell - %s\n", N, ok ? "Correct" : "Error");

    // time
    double time = (end_t.tv_sec - start_t.tv_sec) + (end_t.tv_nsec - start_t.tv_nsec) / 1e9;
    printf("Execution time: %.6f seconds\n", time);
    
    free_matrix(A); free_matrix(B); free_matrix(C);
    free(tids);
    return 0;
}