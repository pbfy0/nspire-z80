#include "aligned_alloc.h"
#include "_syscalls.h"

/*int ctz(unsigned int a) {
    int c = __builtin_clz(a & -a);
    return a ? 31 - c : c;
}*/

aligned_ptr x_aligned_alloc(size_t extra, size_t s) {
	if(__builtin_clz(extra) + __builtin_ctz(extra) != 31) return (aligned_ptr){ NULL, NULL };
	//if(__builtin_popcount(extra) != 1) return (aligned_ptr){ NULL, NULL };
	void *a = _malloc(s+extra);
	void *al = (void *)((((intptr_t)a) | (extra-1)) + 1);
	return (aligned_ptr){ .aligned = al, .raw = a };
}

void x_aligned_free(aligned_ptr ptr) {
	_free(ptr.raw);
}