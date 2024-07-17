#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <math.h> 

#define MAX 256

char* substr(const char *src, int m, int n)
{
    int len = n - m;
 
    char *dest = (char*)malloc(sizeof(char) * (len + 1));

    for (int i = m; i < n && (*(src + i) != '\0'); i++)
    {
        *dest = *(src + i);
        dest++;
    }
 
    *dest = '\0';
 
    return dest - len;
}


int main(int argc, char **argv)
{
	if (argc != 2){
		perror("Wrong number of arguments\n");
		exit(EXIT_FAILURE);
	}
	
	FILE * file_res = fopen("results_mean.txt", "a"); 

	char dirname[18] = "perf_measures";
	strcat(dirname, argv[1]);

	DIR* dir = opendir(dirname);
	struct dirent *cur;

	while ((cur = readdir(dir))) {
		if (cur->d_name[0] != '.') {  //Don't display hidden files
			
			char str[MAX] = "";
			float mean, range, standard_deviation;
			float ci_lower, ci_upper; //Confidence interval lower and upper limits
			float sum = 0, max = -1, min = -1, variance = 0, tmp;
			int sample_size = 0;

			char * filename = (char *) malloc(sizeof(char) * (strlen(dirname)+strlen(cur->d_name)+2));
			strcpy(filename, dirname);
			strcat(filename, "/");
			strcat(filename, cur->d_name);

			FILE * file = fopen(filename, "r");

			if (file == NULL) {
				perror("Error while opening file");
				exit(EXIT_FAILURE);
			}
	
			while (fgets(str, MAX, file) != NULL){
				tmp = atol((const char *) str)
				if (max < tmp) max = tmp;
				if (min == -1 || min > tmp) min = tmp;
				sum += tmp;
				sample_size++;
			}
			mean = sum / i;
			range = max - min;

			while (fgets(str, MAX, file) != NULL){
				tmp = atol((const char *) str)
				variance += ((tmp - mean) << 1); //Square the difference of the sample mean and each individual result
			}
			variance /= sample_size;
			standard_deviation = sqrt(variance);
			ci_lower = mean - (2.58 * (standard_deviation / sqrt(sample_size)));
			ci_upper = mean + (2.58 * (standard_deviation / sqrt(sample_size)));
			
			fclose(file);
			
			free(filename);

			fprintf(file_res, "%s %d - mean : %f, range, %f, standard deviation : %f, confidence interval : [%f, %f]\n", substr(cur->d_name, 3, strlen(cur->d_name)-4), atoi(argv[1]), mean, range, standard_deviation, ci_lower, ci_upper);
		}
	}

	closedir(dir);
	
	fclose(file_res);
	return 0;
}

