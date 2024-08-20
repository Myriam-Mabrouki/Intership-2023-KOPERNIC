#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


long f_mem(int nb) {
    int* a[nb];
    for (int i = 0; i < nb; i++)
        a[i] = (int*)malloc(nb * sizeof(int));

    for (int j = 0; j < nb; j++) {
        for (int i = 0; i < nb; i++){
            a[i][j] = 5;
        }
    }
    return a[0][0];
}

long f_CPU(int nb) {
    long a = 0;
    for (int i = 0; i < nb * nb ; i++){
    	a++;
    }
    return a;
}


int main (int argv, char ** argc) {
    if (argv != 3) {
        perror("Wrong number of arguments");
        return EXIT_FAILURE;
    }

    int nb = atoi(argc[1]);

    if (!atoi(argc[2]))
        f_CPU(nb);
    else
        f_mem(nb);

    return 0;
}
