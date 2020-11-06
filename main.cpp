#include <cstdio>
#include <cstring>
#include "omp.h"

constexpr size_t MAX_SIZE = 9000000;
int data[MAX_SIZE], xtmp[MAX_SIZE];

void print_list(int * x, int n) {
	int i;
	for (i = 0; i < n; i++) {
		printf("%d ",x[i]);
	}
}

void merge(int * X, int n, int * tmp) {
	int i = 0;
	int j = n/2;
	int ti = 0;

	while (i<n/2 && j<n) {
		if (X[i] < X[j]) {
			tmp[ti] = X[i];
			ti++; i++;
		} else {
			tmp[ti] = X[j];
			ti++; j++;
		}
	}
	while (i<n/2) { /* finish up lower half */
		tmp[ti] = X[i];
		ti++; i++;
	}
	while (j<n) { /* finish up upper half */
		tmp[ti] = X[j];
		ti++; j++;
	}
	memcpy(X, tmp, n*sizeof(int));

} // end of merge()

void mergesort(int * X, int n, int * tmp)
{
	if (n < 2) return;

#pragma omp task default(none) firstprivate (X, n, tmp)
	mergesort(X, n/2, tmp);

#pragma omp task default(none) firstprivate (X, n, tmp)
	mergesort(X+(n/2), n-(n/2), tmp);

#pragma omp taskwait

	/* merge sorted halves into sorted list */
#pragma omp critical
	merge(X, n, tmp);
}


int main(int argc, char const * argv[])
{
	//printf("argc: %d\n", argc);
	if (argc > 1 && !strcmp(argv[1], "single")) {
		omp_set_num_threads(2);
		puts("single thread mode");
	}
	int n;
	double start, stop;
	scanf("%d", &n);
	for (int i=0; i<n; i++)
		scanf("%d", &data[i]);
	start = omp_get_wtime();
#pragma omp parallel default(shared)
	{
#pragma omp single
		mergesort(data, n, xtmp);
	}
	stop = omp_get_wtime();
#ifdef PYTHONTEST
	print_list(data, n);
#endif
#ifndef PYTHONTEST
	printf("Num count: %d\nTime: %g\n", n, stop-start);
#endif
}