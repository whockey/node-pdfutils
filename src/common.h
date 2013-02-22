#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define ERROR(x, a...) fprintf(stderr, x "\n", a); exit(EXIT_FAILURE);

#define LOCK_2_MUTEXES(a, b) if(&a > &b) { uv_mutex_lock(&a); uv_mutex_lock(&b); } else { uv_mutex_lock(&b); uv_mutex_lock(&a); }
