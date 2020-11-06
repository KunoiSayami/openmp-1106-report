#include <cstdio>
#include <cstring>
#include <parallel/algorithm>

constexpr size_t MAX_SIZE = 9000000;
int data[MAX_SIZE];

int main()
{
	int n;
	double start, stop;
	scanf("%d", &n);
	for (int i=0; i<n; i++)
		scanf("%d", &data[i]);
	start = omp_get_wtime();
	__gnu_parallel::stable_sort(data, data+n);
	stop = omp_get_wtime();
#ifdef PYTHONTEST
	print_list(data, n);
#else
	printf("Num count: %d\nTime: %g\n", n, stop-start);
#endif
}