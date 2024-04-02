#include "utils.h"
#include "kvec.h"

int idxstats_main(int argc, char *argv[]){

    if ( optind == argc || argc != optind + 1 ) {
        
        fprintf(stderr, "\nUsage: quant-tk idxstats <sam>\n\n");
        return 1;
    
    }

    kstring_t kt = {0, 0, 0};

    int *fields, n, i, num, ret;

    kvec_t(const char *) vs;
    kv_init(vs);
    
    kvec_t(int) vl;
    kv_init(vl);

    kvec_t(int) vr;
    kv_init(vr);

    kvec_t(int) vc;
    kv_init(vc);

    khash_t(int) *h;
    h = kh_init(int);
    khint_t k,r;

    khash_t(set) *reads;
    reads = kh_init(set);

    kstream_t *ks;
    gzFile     fp;
    fp = strcmp(argv[ optind ], "-")? gzopen(argv[ optind ], "r") : gzdopen(fileno(stdin), "r");
    if (fp) {

        ks  = ks_init(fp);
        num = 0;
        
        while(ks_getuntil(ks, '\n', &kt, 0) >=  0){

            fields = ksplit(&kt, '\t', &n);

            if( kt.s[0] == '@' ){
                
                if( kt.s[1] != 'S') continue;
                k = kh_put(int, h, kt.s + fields[1] + 3, &ret);

                kh_key(h, k) = strdup(kt.s + fields[1] + 3);
                kh_val(h, k) = num;

                kv_push(const char*, vs, kh_key(h, k) );
                kv_push(int, vl, atoi(kt.s + fields[2] + 3) );
                kv_push(int, vr, 0 );
                kv_push(int, vc, 0 );
                ++num;
            
            }else{
                

                k =  kh_get(int, h, kt.s + fields[2]);
                if( k == kh_end(h) ){
                    fprintf(stderr, "[ERR]: can't find id %s in header.\n", kt.s + fields[2]);
                    exit(1);
                }
                
                ++kv_A(vc, kh_val(h, k));

                if( (kt.s + fields[6])[0] != '=') continue;
                r = kh_get(set, reads, kt.s);
                if( r  != kh_end(reads) ) continue;
                ++kv_A(vr, kh_val(h, k));

                r = kh_put(set, reads, kt.s, &ret);
                kh_key(reads, r) = strdup(kt.s);

            }    
        }
        
        ks_destroy(ks);
        gzclose(fp);

    }else{
        fprintf(stderr, "[ERR]: can't open file %s\n", argv[ optind ]);
        exit(1);
    }

    for(i = 0; i < num; ++i)
        printf("%s\t%d\t%d\t%d\n", kv_A(vs, i), kv_A(vl, i), kv_A(vr, i), kv_A(vc, i) );

    kh_int_destroy(h);
    kh_set_destroy(reads);
    
    kv_destroy(vs);
    kv_destroy(vl);
    kv_destroy(vr);
    kv_destroy(vc);

    free(kt.s);

    return 0;
}