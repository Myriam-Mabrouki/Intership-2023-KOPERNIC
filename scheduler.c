#define _GNU_SOURCE  
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include "/home/mmabrouk/Documents/tacle-bench/bench/librairies/adpcm_dec/adpcm_dec.h"
#include "/home/mmabrouk/Documents/tacle-bench/bench/librairies/adpcm_enc/adpcm_enc.h"
#include "/home/mmabrouk/Documents/tacle-bench/bench/librairies/ammunition/ammunition.h"
#include "/home/mmabrouk/Documents/tacle-bench/bench/librairies/anagram/anagram.h"
#include "/home/mmabrouk/Documents/tacle-bench/bench/librairies/audiobeam/audiobeam.h"
#include "/home/mmabrouk/Documents/tacle-bench/bench/librairies/cjpeg_transupp/cjpeg_transupp.h"
#include "/home/mmabrouk/Documents/tacle-bench/bench/librairies/cjpeg_wrbmp/cjpeg_wrbmp.h"
#include "/home/mmabrouk/Documents/tacle-bench/bench/librairies/dijkstra/dijkstra.h"
#include "/home/mmabrouk/Documents/tacle-bench/bench/librairies/epic/epic.h"
#include "/home/mmabrouk/Documents/tacle-bench/bench/librairies/fmref/fmref.h"
#include "/home/mmabrouk/Documents/tacle-bench/bench/librairies/g723_enc/g723_enc.h"
#include "/home/mmabrouk/Documents/tacle-bench/bench/librairies/gsm_dec/gsm_dec.h"
#include "/home/mmabrouk/Documents/tacle-bench/bench/librairies/gsm_enc/gsm_enc.h"
#include "/home/mmabrouk/Documents/tacle-bench/bench/librairies/h264_dec/h264_dec.h"
#include "/home/mmabrouk/Documents/tacle-bench/bench/librairies/huff_dec/huff_dec.h"
#include "/home/mmabrouk/Documents/tacle-bench/bench/librairies/huff_enc/huff_enc.h"
#include "/home/mmabrouk/Documents/tacle-bench/bench/librairies/mpeg2/mpeg2.h"
#include "/home/mmabrouk/Documents/tacle-bench/bench/librairies/ndes/ndes.h"
#include "/home/mmabrouk/Documents/tacle-bench/bench/librairies/petrinet/petrinet.h"
#include "/home/mmabrouk/Documents/tacle-bench/bench/librairies/rijndael_dec/rijndael_dec.h"
#include "/home/mmabrouk/Documents/tacle-bench/bench/librairies/rijndael_enc/rijndael_enc.h"
#include "/home/mmabrouk/Documents/tacle-bench/bench/librairies/statemate/statemate.h"
#include "/home/mmabrouk/Documents/tacle-bench/bench/librairies/susan/susan.h"

/* #include "/home/myriam/Videos/stage/tacle-bench/bench/librairies/adpcm_dec/adpcm_dec.h"
#include "/home/myriam/Videos/stage/tacle-bench/bench/librairies/adpcm_enc/adpcm_enc.h"
#include "/home/myriam/Videos/stage/tacle-bench/bench/librairies/ammunition/ammunition.h"
#include "/home/myriam/Videos/stage/tacle-bench/bench/librairies/anagram/anagram.h"
#include "/home/myriam/Videos/stage/tacle-bench/bench/librairies/audiobeam/audiobeam.h"
#include "/home/myriam/Videos/stage/tacle-bench/bench/librairies/cjpeg_transupp/cjpeg_transupp.h"
#include "/home/myriam/Videos/stage/tacle-bench/bench/librairies/cjpeg_wrbmp/cjpeg_wrbmp.h"
#include "/home/myriam/Videos/stage/tacle-bench/bench/librairies/dijkstra/dijkstra.h"
#include "/home/myriam/Videos/stage/tacle-bench/bench/librairies/epic/epic.h"
#include "/home/myriam/Videos/stage/tacle-bench/bench/librairies/fmref/fmref.h"
#include "/home/myriam/Videos/stage/tacle-bench/bench/librairies/g723_enc/g723_enc.h"
#include "/home/myriam/Videos/stage/tacle-bench/bench/librairies/gsm_dec/gsm_dec.h"
#include "/home/myriam/Videos/stage/tacle-bench/bench/librairies/gsm_enc/gsm_enc.h"
#include "/home/myriam/Videos/stage/tacle-bench/bench/librairies/h264_dec/h264_dec.h"
#include "/home/myriam/Videos/stage/tacle-bench/bench/librairies/huff_dec/huff_dec.h"
#include "/home/myriam/Videos/stage/tacle-bench/bench/librairies/huff_enc/huff_enc.h"
#include "/home/myriam/Videos/stage/tacle-bench/bench/librairies/mpeg2/mpeg2.h"
#include "/home/myriam/Videos/stage/tacle-bench/bench/librairies/ndes/ndes.h"
#include "/home/myriam/Videos/stage/tacle-bench/bench/librairies/petrinet/petrinet.h"
#include "/home/myriam/Videos/stage/tacle-bench/bench/librairies/rijndael_dec/rijndael_dec.h"
#include "/home/myriam/Videos/stage/tacle-bench/bench/librairies/rijndael_enc/rijndael_enc.h"
#include "/home/myriam/Videos/stage/tacle-bench/bench/librairies/statemate/statemate.h"
#include "/home/myriam/Videos/stage/tacle-bench/bench/librairies/susan/susan.h" */

int main () {
    /* main_adpcm_dec();
    main_ammunition();
    main_audiobeam();
    main_cjpeg_transupp();
    main_cjpeg_wrbmp();
    main_dijkstra();
    main_epic();
    main_fmref();
    main_g723_enc();
    main_gsm_dec();
    main_gsm_enc();
    main_h264_dec();
    main_huff_dec();
    main_huff_enc();
    main_mpeg2();
    main_ndes();
    main_petrinet();
    main_rijndael_dec();
    main_rijndael_enc();
    main_statemate();
    main_susan(); */
    int i = 0;
    pthread_t thA, thB;
    int err = 0;
    while (i < 100) {
        printf("%d\n", i);
        if (i > 0) err = pthread_tryjoin_np(thA, NULL);
        if (err != 0) {
            perror("Deadline miss\n");
            return 1;
        }
        pthread_create(&thA, NULL, (void *) main_ndes, NULL);
        pthread_setschedprio(thA, -20);
        printf("a %d\n", i);
        sleep(1);
        if (i > 0) err = pthread_tryjoin_np(thB, NULL);
        if (err != 0) {
            perror("Deadline miss\n");
            return 1;
        }
        pthread_create(&thB, NULL, (void *) main_dijkstra, NULL);
        pthread_setschedprio(thB, -19);
        sleep(1);
        i++;
    } 
    
    return 0;
}