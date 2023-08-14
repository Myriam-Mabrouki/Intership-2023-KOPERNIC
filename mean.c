#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

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
		fprintf (stderr, "Mauvais nombre d'arguments\n");
		exit(EXIT_FAILURE);
	}
	
	FILE * file_res = fopen("results.txt", "a"); 

	char dirname[18] = "perf_measures";
	strcat(dirname, argv[1]);

	DIR* dir = opendir(dirname);
	struct dirent *cur;

	while ((cur = readdir(dir))) {
		if (cur->d_name[0] != '.') {  //ne pas afficher les fichiers cachés
			
			char str[MAX] = "";
			float sum = 0;
			int i = 0;

			char * filename = (char *) malloc(sizeof(char) * (strlen(dirname)+strlen(cur->d_name)+2));
			strcpy(filename, dirname);
			strcat(filename, "/");
			strcat(filename, cur->d_name);

			FILE * file = fopen(filename, "r");

			if (file == NULL) {
				perror("FILE ERROR");
				exit(EXIT_FAILURE);
			}
	
			while (fgets(str, MAX, file) != NULL){
				
				sum += atol((const char *) str);
				i++;
			
			}
			
			fclose(file);
			
			free(filename);

			fprintf(file_res, "%s %d  %f\n", substr(cur->d_name, 3, strlen(cur->d_name)-4), atoi(argv[1]), (sum/i));
		}
	}

	closedir(dir);
	
	fclose(file_res);
	 	
	return 0;
}

