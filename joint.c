#include "utils.h"
#include "kvec.h"
KHASH_MAP_INIT_STR(map, double *)

typedef struct {
    char *title;
    int   target;
    int   head_line;
} opt_t;

void joint_opt_init(opt_t *opt);

static kvec_t( const char* ) vs;
static khash_t(joint) *h;

void joint_refresh ( );
void joint_print ( );

int joint_main(int argc, char *argv[]){
    
    opt_t opt;
    joint_opt_init(&opt);
    int c;
    while ((c = getopt(argc, argv, "t:c:i")) >= 0) {

        if (c == 't') {
             opt.title = optarg;
        }else if (c == 'c') {
             opt.target = atoi(optarg) - 1;
        }else if (c == 'i') {
             opt.head_line = 1;
        }

    }
    if ( optind == argc || argc < optind + 1) {
        fprintf(stderr, "\nUsage: quant-tk joint [options] [label:text ...]\n\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  -t STR  the title key, default: [catalog];\n");
        fprintf(stderr, "  -c INT  the target column default: [2];\n");
        fprintf(stderr, "  -i      ignore the head line;\n\n");
        return 1;
    }

    kv_init(vs);

    
    h = kh_init(joint);

    khint_t k;

    kstring_t   kt    = {0, 0, 0};
    kstring_t   kswap = {0, 0, 0};
    kstring_t   head  = {0, 0, 0};

    int *fields, i, n;
    gzFile fp;

    for (i = optind; i < argc; ++i){

        kswap.l = 0;
        kputs(argv[i], &kswap);

        ks_tokaux_t aux;  char *p;
        p = kstrtok(kswap.s, ":", &aux);
        kswap.s[ aux.p - p] = '\0';

        kputc('\t',    &head);
        kputs(kswap.s, &head);

        fp = strcmp(aux.p + 1, "-")? gzopen(aux.p + 1, "r") : gzdopen(fileno(stdin), "r");
        if (fp) {
    
            kstream_t *ks;
            ks = ks_init(fp);
            int ret;

            if(opt.head_line) ks_getuntil( ks, '\n', &kt, 0);
            
            while( ks_getuntil( ks, '\n', &kt, 0) >=  0){
                
                if(kt.s[0] == '#') continue;
                fields  = ksplit(&kt, '\t', &n); 
                k = kh_get(joint, h, kt.s);
                if( k  == kh_end(h) ){
                    k = kh_put(joint, h, kt.s, &ret);
                    
                    joint_t * entity = (joint_t *) malloc(sizeof( joint_t ));
                    kstring_t kval   = {0, 0, 0}; 
                    entity->label    = 0;
                    entity->val      = kval;

                    kh_key(h, k)     = strdup( kt.s );
                    kh_val(h, k)     = entity;
                    kv_push( const char *, vs, kh_key(h, k));

                }

             }
             
             ks_destroy(ks);
             gzclose(fp);

        }else{
            fprintf(stderr, "[ERR]: can't open file %s\n", aux.p + 1);
            exit(1);
        }
    }

    for (i = optind; i < argc; ++i){

        kswap.l = 0;
        kputs(argv[i], &kswap);

        ks_tokaux_t aux;  char *p;
        p = kstrtok(kswap.s, ":", &aux);
        kswap.s[ aux.p - p] = '\0';

        fp = strcmp(aux.p + 1, "-")? gzopen(aux.p + 1, "r") : gzdopen(fileno(stdin), "r");
        if (fp) {
    
            kstream_t *ks;
            ks = ks_init(fp);

            if(opt.head_line) ks_getuntil( ks, '\n', &kt, 0);
            
            while( ks_getuntil( ks, '\n', &kt, 0) >=  0){
                
                if(kt.s[0] == '#') continue; 
                
                fields  = ksplit(&kt, '\t', &n); 
  
                k = kh_get(joint, h, kt.s);
                if( k != kh_end(h) ){
                    kputc('\t', &kh_val(h, k)->val);
                    kputs( kt.s + fields[ opt.target ], &kh_val(h, k)->val);
                    kh_val(h, k)->label = 1;
                }

             }
             
             joint_refresh();
             ks_destroy(ks);
             gzclose(fp);

        }else{
            fprintf(stderr, "[ERR]: can't open file %s\n", aux.p + 1);
            exit(1);
        }
    }
  
    printf("#%s%s\n", opt.title, head.s);

    joint_print();
  
    free(kt.s);
    free(kswap.s);
    free(head.s);

    kv_destroy(vs);
    kh_joint_destroy(h);
    return 0;
}

void joint_opt_init(opt_t *opt){
    
    memset(opt, 0, sizeof(opt_t));
    opt->title      = NULL;
    opt->target    = 1;
    opt->head_line = 0;

}

void joint_refresh ( ){

    int i;
    khint_t k;
    for (i = 0; i < kv_size(vs); ++i) {

        k = kh_get(joint, h, kv_A(vs, i));
        if( k != kh_end(h) ){
            if(kh_val(h, k)->label == 0) ksprintf(&kh_val(h, k)->val, "\t0");
        }else{
            fprintf(stderr, "[ERR]: can't locate %s in DICT.\n", kv_A(vs, i));
            exit(-1);
        }

        kh_val(h, k)->label = 0;
    
    }

}

void joint_print ( ){

    int i;
    khint_t k;
    for (i = 0; i < kv_size(vs); ++i) {
        k = kh_get(joint, h, kv_A(vs, i));
        if( k != kh_end(h) ){
            printf("%s%s\n", kh_key(h, k), kh_val(h, k)->val.s);
        }else{
            fprintf(stderr, "[ERR]: can't locate %s in DICT.\n", kv_A(vs, i));
            exit(-1);
        }
    }

}