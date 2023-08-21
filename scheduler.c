#define _GNU_SOURCE  
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <fcntl.h> 
#include <string.h>
#include <termios.h>


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

int set_interface_attribs (int fd, int speed, int parity)
{
        struct termios tty;
        if (tcgetattr (fd, &tty) != 0)
        {
                //perror ("error %d from tcgetattr", errno);
                return -1;
        }

        cfsetospeed (&tty, speed);
        cfsetispeed (&tty, speed);

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // disable break processing
        tty.c_lflag = 0;                // no signaling chars, no echo,
                                        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= parity;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
        {
                //perror ("error %d from tcsetattr", errno);
                return -1;
        }
        return 0;
}

void
set_blocking (int fd, int should_block)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0) {
            //perror ("error %d from tggetattr", errno);
            return;
        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        /* if (tcsetattr (fd, TCSANOW, &tty) != 0)
            perror ("error %d setting term attributes", errno); */
}

int main () {

    char *portname = "/dev/ttyS0";

    int fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0){
        //perror ("error %d opening %s: %s", errno, portname, strerror (errno));
        return -1;
    }
    
    set_interface_attribs (fd, 115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
    set_blocking (fd, 0);                // set no blocking

    main_adpcm_dec(); //ok
    main_adpcm_enc(); //ok
    main_anagram(); //pas ok
    main_ammunition(); //ok (ne pas utiliser car assign CPU ne marche pas)
    main_audiobeam(); //pas ok
    main_cjpeg_transupp(); //ok
    main_cjpeg_wrbmp(); //ok
    main_dijkstra(); //ok
    main_epic(); //ok
    main_fmref(); //ok
    main_g723_enc(); //ok
    main_gsm_dec(); //ok
    main_gsm_enc(); //ok
    main_h264_dec(); //ok
    main_huff_dec(); //pas ok
    main_huff_enc(); //ok
    main_mpeg2(); //ok
    main_ndes(); //ok
    main_petrinet(); //ok
    main_rijndael_dec(); //ok
    main_rijndael_enc(); //ok
    main_statemate(); //ok
    main_susan(); //ok (ne pas utiliser car assign CPU ne marche pas)
    int i = 0;
    pthread_t thA, thB, thC;
    int err = 0;
    while (i < 100) {
        write (fd, "begin\n", 7);
        printf("%d\n", i);
        if (i > 0) err = pthread_tryjoin_np(thA, NULL);
        if (err != 0) {
            perror("Deadline miss");
            exit(EXIT_FAILURE);
        }
        pthread_create(&thA, NULL, (void *) main_petrinet, NULL);
        pthread_setschedprio(thA, -20);
        sleep(1);
        if (i > 0) err = pthread_tryjoin_np(thB, NULL);
        if (err != 0) {
            perror("Deadline miss\n");
            exit(EXIT_FAILURE);
        }
        pthread_create(&thB, NULL, (void *) main_ammunition, NULL);
        pthread_setschedprio(thB, -19);
        sleep(1);
        if (i > 0) err = pthread_tryjoin_np(thC, NULL);
        if (err != 0) {
            perror("Deadline miss\n");
            exit(EXIT_FAILURE);
        }
        pthread_create(&thC, NULL, (void *) main_susan, NULL);
        pthread_setschedprio(thC, -18);
        sleep(1);
        i++;
        write (fd, "end\n", 5);
    } 

    char buf [100];
    int n = read (fd, buf, sizeof buf);  // read up to 100 characters if ready to read  
    
    return 0;
}