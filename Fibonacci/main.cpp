#include <cstdio>
#include <future>
#include <omp.h>

unsigned fib_seq(unsigned n){
	if (n < 2)
		return n;
	return fib_seq(n - 1) + fib_seq(n - 2);
}

unsigned fib_omp(unsigned n){
	if (n < 2)
		return n;
	unsigned r1, r2;
	#pragma omp task shared(r1)
	{
		r1 = fib_seq(n - 1);
	}
	#pragma omp task shared(r2)
	{
		r2 = fib_seq(n - 2);
	}
	#pragma omp taskwait
	return r1 + r2;
}

unsigned fib_async(unsigned n, std::launch policy) {
    if (n < 2) return n;
    unsigned r1 = std::async(policy,fib_async, n - 1, policy).get();
    unsigned r2 = std::async(policy,fib_async, n - 2, policy).get();
    return r1 + r2;
}

int main(int argc, char** argv){
	unsigned n = 20;

	double t0 = omp_get_wtime();
	std::printf("SEQ\n");
	for (unsigned i = 0; i < n; ++i)
		std::printf("%u ", fib_seq(i));
	std::printf("\ntime: %f", omp_get_wtime()-t0);
	
	t0 = omp_get_wtime();
	std::printf("\n\nOMP\n");
	for (unsigned i = 0; i < n; ++i)
		std::printf("%u ", fib_omp(i));
	std::printf("\ntime: %f", omp_get_wtime()-t0);

	t0 = omp_get_wtime();
	std::printf("\n\nAsync(async)\n");
	for (unsigned i = 0; i < n; ++i)
		std::printf("%u ", fib_async(i, std::launch::async));
	std::printf("\ntime: %f", omp_get_wtime()-t0);

	t0 = omp_get_wtime();
	std::printf("\n\nAsync(deferred)\n");
	for (unsigned i = 0; i < n; ++i)
		std::printf("%u ", fib_async(i, std::launch::deferred));
	std::printf("\ntime: %f", omp_get_wtime()-t0);
	return 0;
}