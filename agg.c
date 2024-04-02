#include "utils.h"
#include "kvec.h"

typedef struct {
    char *title;
    int   target;
    int   head_line;
} opt_t;

void agg_opt_init(opt_t *opt);

static kvec_t( const char* ) vs;
static khash_t(int) *h;
static double *matrix;

void agg_print( int n );

int agg_main(int argc, char *argv[]){
    
    opt_t opt;
    agg_opt_init(&opt);
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
        fprintf(stderr, "\nUsage: quant-tk agg [options] [label:text ...]\n\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  -t STR  the title key, default: [catalog];\n");
        fprintf(stderr, "  -c INT  the target column default: [2];\n");
        fprintf(stderr, "  -i      ignore the head line;\n\n");
        return 1;
    }

    kv_init(vs);
    h = kh_init(int);
    
    khint_t k;
    kstring_t   kt    = {0, 0, 0};
    kstring_t   kswap = {0, 0, 0};
    kstring_t   head  = {0, 0, 0};

    gzFile fp;

    int *fields, i, n;
    int label = 0;
    int n_val = argc - optind;

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
                k = kh_get(int, h, kt.s);
                if( k  == kh_end(h) ){

                    k = kh_put(int, h, kt.s, &ret);
                    kh_key(h, k)     = strdup( kt.s );
                    kh_val(h, k)     = label;
                    ++label;
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

    matrix    = (double *) calloc(n_val * label, sizeof(double));

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
  
                k = kh_get(int, h, kt.s);
                if( k != kh_end(h) ) matrix[ kh_val(h, k) * n_val +  i - optind ] = atof(kt.s + fields[ opt.target ] );
             
             }
             
             ks_destroy(ks);
             gzclose(fp);

        }else{
            fprintf(stderr, "[ERR]: can't open file %s\n", aux.p + 1);
            exit(1);
        }
    }

    printf("#%s%s\n", opt.title, head.s);
    agg_print(n_val);

    free(kt.s);
    free(kswap.s);
    free(head.s);
    kfree(matrix);

    kh_destroy(int, h);
    kv_destroy(vs);
    return 0;
}

void agg_opt_init(opt_t *opt){
    
    memset(opt, 0, sizeof(opt_t));
    opt->title     = NULL;
    opt->target    = 1;
    opt->head_line = 0;

}

void agg_print ( int n ){

    int i, j, loct;
    loct = 0;
    for (i = 0; i < kv_size(vs); ++i) {
        printf("%s", kv_A(vs, i));
        for (j = 0; j < n; ++j) printf("\t%.6lf", matrix[loct++]);
        printf("\n");
    }

}
