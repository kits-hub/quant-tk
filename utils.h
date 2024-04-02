#include <zlib.h>
#include <stdio.h>
#include <stdlib.h>

#include "kstring.h"
#include "khash.h"
#include "kseq.h"

typedef struct{
    FILE *fh;
    FILE *rh;
    char *fn;
    char *rn;
} handle_t;

typedef struct{
    kstring_t val;
    int       label;
} joint_t;

KSEQ_INIT(gzFile, gzread)
void print_kseq(kseq_t *s, FILE *file);

KHASH_MAP_INIT_STR(reg, char *)
void kh_reg_destroy(khash_t(reg) *h);

KHASH_SET_INIT_STR(set)
void kh_set_destroy(khash_t(set) *h);

KHASH_MAP_INIT_STR(double, double)
void kh_double_destroy(khash_t(double) *h);

KHASH_MAP_INIT_STR(joint, joint_t *)
void kh_joint_destroy(khash_t(joint) *h);

KHASH_MAP_INIT_STR(int, int)
void kh_int_destroy(khash_t(int) *h);

KHASH_MAP_INIT_STR(handle, handle_t *)
void kh_handle_destroy(khash_t(handle) *h);