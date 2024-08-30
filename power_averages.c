#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main () {

    FILE *f_power, *f_uart;

    /* Open the files in read mode */
    f_power = fopen("Main power - Ace.csv", "r");
    f_uart = fopen("UART log - Ace.csv", "r");

    if (!f_power || !f_uart) {
        perror("Error while opening the files");
        return EXIT_FAILURE;
    }

    /* Store the content of the file */
    char line_power[64];
    char line_uart[64];

    long double begin, end;

    long double power_sum = 0;
    int nb_measures = 0;

    /* Read the content and */
    int i = 0;
    while(nb_measures < 100 && fgets(line_uart, 64, f_uart)) {

        /* Skip the two first lines */
        if (i == 0 || i == 1) {
            i++;
            continue;
        }

        const char * separators = " ,";
        char * tmp_uart = strtok (line_uart, separators);  
        if (i % 2 == 0){
            begin = atol(tmp_uart);
            end = -1;
        }
        else {
            end = atol(tmp_uart);
            if (begin == end) {
                end += 0.0005;
            }
        }

        /*  */
        if (end != -1) {
            
            long double power_frames_sum = 0;
            long long nb_frames = 0;
            char * tmp_power = "";
            while(fgets(line_power, 64, f_power) && begin > atof(tmp_power)) {
                tmp_power = strtok (line_power, separators);
                printf("%s\n", line_power);
            }

            while (fgets(line_power, 64, f_power) && end > atof(tmp_power)) {
                tmp_power = strtok (NULL, separators);
                power_frames_sum += atof(tmp_power);
                tmp_power = strtok (line_power, separators);
                nb_frames++;
            }

            printf("test %Lf %lld %s\n", power_frames_sum, nb_frames, tmp_power);
            power_sum += (power_frames_sum / nb_frames);
            nb_measures++;
            printf("Average power %Lf %d\n", power_sum, nb_measures);
        }

        i++;
    }

    /* Close the file */
    fclose(f_power);
    fclose(f_uart); 

    printf("Average power %Lf\n", power_sum/nb_measures);
    return 0;
}
