#include "utils.h"

int fpkm_main(int argc, char *argv[]){

    if ( optind == argc || argc != optind + 1 ) {
        fprintf(stderr, "\nUsage: quant-tk fpkm <idxstats>\n\n");
        return 1;
    }

    int *fields, n, num, tot;
    tot = 0;
    
    kstring_t kt = {0, 0, 0};   
    kstream_t *ks;
    gzFile     fp;
    fp =  gzopen(argv[ optind ], "r");

    if (fp) {
        
        ks = ks_init(fp);
        while( ks_getuntil( ks, '\n', &kt, 0) >=  0){

            if( kt.s[0] == '*' ) continue;
            fields = ksplit(&kt, '\t', &n);
            tot  +=  atoi(kt.s + fields[2]);
        }

    }else{
        fprintf(stderr, "[ERR]: can't open file %s\n", argv[ optind + 1]);
        exit(1);
    }

    gzrewind(fp);
    ks = ks_init(fp);

    int len = 1.0;

    printf("#gene\tlength\tcounts\tfpkm\n");

    while( ks_getuntil( ks, '\n', &kt, 0) >=  0){
        
        if( kt.s[0] == '*' ) continue;
        fields = ksplit(&kt, '\t', &n);

        len    = atof(kt.s + fields[1]);
        num    = atof(kt.s + fields[2]);

        printf("%s\t%s\t%s\t%lf\n", kt.s,
                                    kt.s + fields[1],
                                    kt.s + fields[2],
                                    1000000000.0 * num/(len * tot));
    
    }

    ks_destroy(ks);
    gzclose(fp);

    free(kt.s);
    return 0;
}


