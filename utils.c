#include "utils.h"

void kh_reg_destroy(khash_t(reg) *h){
    
    khint_t k;
    if  (h == 0) return;
    for (k = 0; k < kh_end(h); ++k) {
        if (kh_exist(h, k)) {
            free((char*)kh_key(h, k));
            free((char*)kh_val(h, k));
        }
    }
    kh_destroy(reg, h);

}

void kh_joint_destroy(khash_t(joint) *h){
    
    khint_t k;
    if  (h == 0) return;
    for (k = 0; k < kh_end(h); ++k) {
        if (kh_exist(h, k)) {
            free((char*)kh_key(h, k));
            joint_t *joint = (joint_t *)kh_val(h, k);
            free( joint->val.s);
            free( joint );

        }
    }
    kh_destroy(joint, h);

}

void kh_set_destroy(khash_t(set) *h){

    khint_t k;
    if (h == 0) return;
    for (k = 0; k < kh_end(h); ++k) {
        if (kh_exist(h, k)) free((char*)kh_key(h, k));
    }
    kh_destroy(set, h);
}

void kh_double_destroy(khash_t(double) *h){
    
    khint_t k;
    if (h == 0) return;
    for (k = 0; k < kh_end(h); ++k) {
        if (kh_exist(h, k)){
            free((char*)kh_key(h, k));
        }
    }
    kh_destroy(double, h);

}

void kh_int_destroy(khash_t(int) *h){
    
    khint_t k;
    if (h == 0) return;
    for (k = 0; k < kh_end(h); ++k) {
        if (kh_exist(h, k)){
            free((char*)kh_key(h, k));
        }
    }
    kh_destroy(int, h);

}

void print_kseq(kseq_t *s, FILE *file){

    if( s->qual.l ){
        fprintf(file, "@%s", s->name.s);
        if (s->comment.l) fprintf(file, " %s", s->comment.s);
        fprintf(file, "\n%s\n+\n%s\n",s->seq.s, s->qual.s); 
    }else{
        fprintf(file, ">%s", s->name.s);
        if (s->comment.l) fprintf(file, " %s", s->comment.s);
        fprintf(file, "\n%s\n", s->seq.s); 
    }

}

void kh_handle_destroy(khash_t(handle) *h){

    khint_t k;
    if  (h == 0) return;
    for (k = 0; k < kh_end(h); ++k) {
        if (kh_exist(h, k)){
            free( (char *)kh_key(h, k) );
            free( kh_val(h, k)->fn );
            free( kh_val(h, k)->rn );
            fclose( kh_val(h, k)->fh );
            fclose( kh_val(h, k)->rh );
            free( (handle_t *)kh_val(h, k));
        }
    }
    kh_destroy(handle, h);

}