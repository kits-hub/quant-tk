#include "utils.h"

void strip(char * s){

    int l = strlen(s);
    char *p = strchr (s, '.');
    
    if (p != NULL) {
        
        p = s + l - 1;
        while (*p == '0') *p-- = '\0';
        if (*p == '.') *p = '\0';
    
    }    
    
    strcmp(s, "-0")  ?  printf("\t%s", s) : printf("\t0");

}

int strip_main(int argc, char *argv[]){
    

    if ( optind == argc || argc != optind + 1 ) {
        
        fprintf(stderr, "\nUsage: ogu-tk strip <table>\n\n");
        return 1;
    
    }

    kstring_t kt    = {0, 0, 0};
    int *fields, i, n;

    gzFile fp;
    fp = strcmp(argv[ optind ],  "-")? gzopen(argv[ optind ], "r") : gzdopen(fileno(stdin), "r"); 
    if (fp) {
        

        kstream_t *ks;
        ks  = ks_init(fp);

        while( ks_getuntil( ks, '\n', &kt, 0) >=  0){
            
            if(kt.s[0] == '#'){
                puts(kt.s);
                continue;
            }

            fields = ksplit(&kt, '\t', &n);
            
            printf("%s", kt.s);
            for (i = 1; i < n; ++i) strip(kt.s + fields[i]);
            printf("\n");
        
        }


        free(kt.s);
        ks_destroy(ks);
        gzclose(fp);

    }else{
        fprintf(stderr, "[ERR]: can't open file %s\n", argv[ optind ]);
        exit(1);
    }

    return 0;
}