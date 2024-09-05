#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

void power_averages(FILE *f_power, FILE *f_uart){
    
    // Store lines of the files */
    char line_power[64];
    char line_uart[64];

    long double begin, end;

    long double power_sum = 0;
    long double time_sum = 0;
    int nb_measures = 0;

    // Read the content and 
    int i = 0;
    while(nb_measures < 100 && fgets(line_uart, 64, f_uart)) {

        // Skip the two first lines
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
        }

        // One execution has been measured entirely
        // Check the power consumption of this measure
        if (end != -1) {
            
            long double power_frames_sum = 0;
            long long nb_frames = 0;
            char * tmp_power = "";
            while(fgets(line_power, 64, f_power) && begin > atof(tmp_power)) {
                tmp_power = strtok (line_power, separators);
            }

            while (fgets(line_power, 64, f_power) && end > atof(tmp_power)) {
                tmp_power = strtok (NULL, separators);
                power_frames_sum += atof(tmp_power);
                tmp_power = strtok (line_power, separators);
                nb_frames++;
            }

            time_sum += nb_frames * 20; // Each frame represent a unit of 20 microseconds
            power_sum += (power_frames_sum / nb_frames);
            nb_measures++;
        }

        i++;
    }
    
    printf("Average execution time %Lf ms \n", (time_sum/nb_measures) / 1000); // Time is initially in microseconds and then converted in milliseconds
    printf("Average power %Lf, nb measures %d\n", power_sum/nb_measures, nb_measures);
}

int main () {

    DIR* dir;
    struct dirent* ent;

    // Open the directory
    dir = opendir("./");
    if (dir == NULL) {
        perror("Failed to open directory");
        return EXIT_FAILURE;
    }

    // Iterate through all entries in the directory
    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;  // Skip current and parent directories
        }
        
        // Construct full path of the entry
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", "./", ent->d_name);

        // Construct name of the power file
        char power_file_name[2048];
        snprintf(power_file_name, sizeof(power_file_name), "%s/%s", full_path, "Main power - Ace.csv");
        
        // Construct name of the uart file
        char uart_file_name[2048];
        snprintf(uart_file_name, sizeof(uart_file_name), "%s/%s", full_path, "UART log - Ace.csv");

        printf("%s\n", ent->d_name);

        // Open the files    
        FILE *f_power = fopen(power_file_name, "r");
        FILE *f_uart = fopen(uart_file_name, "r");

        // Check errors
        if (!f_power || !f_uart) {
            perror("Error while opening the files");
            return EXIT_FAILURE;
        }

        // Compute averages
        power_averages(f_power, f_uart);
        
        // CLose the files   
        fclose(f_power);
        fclose(f_uart);

    }

    // Close the directory
    closedir(dir);

    return 0;
}
