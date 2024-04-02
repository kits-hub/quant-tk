/*****************************************************************************
  quant-tk.c

  quant-tk command line interface.

  (c) 2022-2024 - LEI ZHANG
  Logic Informatics Co.,Ltd.
  zhanglei@logicinformatics.com
  
  Licenced under The MIT License.
******************************************************************************/

#include <stdio.h>
#include <string.h>

int idxstats_main(int argc, char *argv[]);
int abund_main(int argc, char *argv[]);
int fpkm_main(int argc, char *argv[]);
int tpm_main(int argc, char *argv[]);
int colsum_main(int argc, char *argv[]);
int agg_main(int argc, char *argv[]);
int strip_main(int argc, char *argv[]);
int joint_main(int argc, char *argv[]);

static int usage(){
    
    fprintf(stderr, "\n");
    fprintf(stderr, "Usage:   quant-tk <command> <arguments>\n");
    fprintf(stderr, "Version: 0.0.1\n\n");
    fprintf(stderr, "Command:\n");
    fprintf(stderr, "     idxstats   gene length and counts stats from sam.\n");
    fprintf(stderr, "     abund      generate gene catalog abundance.\n");
    fprintf(stderr, "     fpkm       Fragments Per Kilobase Million\n");
    fprintf(stderr, "     tpm        Transcripts Per Million.\n");
    fprintf(stderr, "     colsum     sum for each column.\n");
    fprintf(stderr, "     agg        combinate multi-file numeric column.\n");
    fprintf(stderr, "     joint      combinate multi-file numeric column.\n");
    fprintf(stderr, "     strip      strip taling zeros.\n");

    fprintf(stderr, "\n");

    fprintf(stderr, "\nLicenced:\n");
    fprintf(stderr, "(c) 2022-2024 - LEI ZHANG\n");
    fprintf(stderr, "Logic Informatics Co.,Ltd.\n");
    fprintf(stderr, "zhanglei@logicinformatics.com\n");
    fprintf(stderr, "\n");

    return 1;

}

int main(int argc, char *argv[]){

    if (argc < 2) return usage();
   
    if (strcmp(argv[1], "idxstats") == 0)    idxstats_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "abund") == 0)  abund_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "fpkm") == 0)   fpkm_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "tpm") == 0)    tpm_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "colsum") == 0) colsum_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "agg") == 0)    agg_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "joint") == 0)  joint_main(argc - 1, argv + 1);
    else if (strcmp(argv[1], "strip") == 0)  strip_main(argc - 1, argv + 1);
    else {
        fprintf(stderr, "[main]: unrecognized command '%s'. Abort!\n", argv[1]);
        return 1;
    }
    return 0;

}