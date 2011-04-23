#include <stdlib.h>
void
qsort_withuserdata(void *a, size_t n, size_t es, int (*cmp)(const void *, const void *, void* userdata), void* userdata);
