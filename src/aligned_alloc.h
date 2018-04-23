#ifndef ALIGNED_ALLOC_H
#define ALIGNED_ALLOC_H

#include <stddef.h>

typedef struct {
	union {
		void *aligned;
		void *ptr;
	};
	void *raw;
} aligned_ptr;

aligned_ptr x_aligned_alloc(size_t extra, size_t s);
void x_aligned_free(aligned_ptr ptr);

#endif