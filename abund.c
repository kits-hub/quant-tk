#include "utils.h"

int abund_main(int argc, char *argv[]){

    double factor = 1.0;
    int c;
    while ((c = getopt(argc, argv, "f:")) >= 0) {
        if (c == 'f') factor = atof(optarg);
    }

    if ( optind == argc || argc != optind + 1 ) {
        fprintf(stderr, "\nUsage: quant-tk abund <idxstats>\n\n");
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  -f INT norm by a factor. (TPM:1,000,000). default: [1];\n\n");
        return 1;
    }

    int *fields, n, num;
    double norm = 0.;
 
    kstring_t kt = {0, 0, 0};   
    kstream_t *ks;
    gzFile     fp;
    fp =  gzopen(argv[ optind ], "r");

    if (fp) {
        
        ks = ks_init(fp);
        while( ks_getuntil( ks, '\n', &kt, 0) >=  0){

            if( kt.s[0] == '*' ) continue;
            fields = ksplit(&kt, '\t', &n);
            num    = atoi(kt.s + fields[3]);
            if( num <= 2 ) continue;
            norm  += 1.0 * num/atof(kt.s + fields[1]);
        
        }

    }else{
        fprintf(stderr, "[ERR]: can't open file %s\n", argv[ optind + 1]);
        exit(1);
    }

    gzrewind(fp);
    ks = ks_init(fp);

    int len = 0;
    printf("#gene\tlength\tcounts\ttpm\n");

    while( ks_getuntil( ks, '\n', &kt, 0) >=  0){
        
        if( kt.s[0] == '*' ) continue;
        fields = ksplit(&kt, '\t', &n);

        len    = atof(kt.s + fields[1]);
        num    = atof(kt.s + fields[3]);
        
        if( num <= 2 )
            printf("%s\t%s\t0\t0\n", kt.s, kt.s + fields[1]);
        else
            printf("%s\t%s\t%s\t%lf\n", kt.s,
                                        kt.s + fields[1],
                                        kt.s + fields[3],
                                        factor * num/(len * norm));
    
    }

    ks_destroy(ks);
    gzclose(fp);

    free(kt.s);
    return 0;
}


