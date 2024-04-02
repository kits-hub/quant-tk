#include "utils.h"
#include "kvec.h"

int colsum_main(int argc, char *argv[]){
    
    char* label = "label";
    int c;
    while ((c = getopt(argc, argv, "l:")) >= 0) {
        if (c == 'l') label = optarg;
    }

    if ( optind == argc || argc != optind + 1 ) {
        fprintf(stderr, "\nUsage: quant-tk colsum [options] <tsv>\n\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  -l STR label for colsum. default: ['label'];\n\n");
        return 1;
    }
    
    kstring_t kt = {0, 0, 0};
    int *fields, n, i;

    kvec_t(int) sum;
    kv_init(sum);

    gzFile fp;
    fp = strcmp(argv[ optind ], "-")? gzopen(argv[ optind ], "r") : gzdopen(fileno(stdin), "r");
    if (fp) {

        kstream_t *ks;
        ks = ks_init(fp);

        while( ks_getuntil( ks, '\n', &kt, 0) >=  0){

            if(kt.s[0] == '#'){
               puts(kt.s); 
               fields = ksplit(&kt,'\t', &n);   
               for (i = 1; i < n; ++i) kv_push(int, sum, 0 );
               continue;
            }

            fields = ksplit(&kt,'\t', &n);
            for (i = 1; i < n; ++i){
                if( atoi(kt.s + fields[i]) > 0 ) kv_a(int, sum, i - 1 ) += 1;
            }
            
        }

        ks_destroy(ks);
        gzclose(fp);

    }else{
        fprintf(stderr, "[ERR]: can't open file %s\n", argv[1]);
        exit(1);
    }
    
    printf("%s", label);
    for (i = 0; i < kv_size(sum); ++i) printf("\t%d", kv_A(sum, i));
    printf("\n");
   
    kv_destroy(sum);
    free(kt.s);

    return 0;
}