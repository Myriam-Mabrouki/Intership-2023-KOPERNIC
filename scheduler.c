#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
//#include "/home/mmabrouk/Documents/tacle-bench/bench/librairies/adpcm_dec/adpcm_dec.h"
//#include "/home/mmabrouk/Documents/tacle-bench/bench/librairies/adpcm_enc/adpcm_enc.h"

#include "/home/myriam/Videos/stage/tacle-bench/bench/librairies/adpcm_dec/adpcm_dec.h"
#include "/home/myriam/Videos/stage/tacle-bench/bench/librairies/adpcm_enc/adpcm_enc.h"
#include "/home/myriam/Videos/stage/tacle-bench/bench/librairies/anagram/anagram.h"

int main () {
    
    pthread_t thA, thB;
    pthread_create(&thA, NULL, (void *) main_adpcm_dec, NULL);
    pthread_create(&thB, NULL, (void *) main_anagram, NULL);
    pthread_setschedprio(thA, -20);
    pthread_setschedprio(thB, -19);
    pthread_join(thA, NULL);
    pthread_join(thB, NULL);
    return 0;
}