# Intership-2023-KOPERNIC

Let's see how is organised the repository. 

## Time measures

This folder cointains bash scripts to make or delete the executables of our benchmark. The folder has as well a script to do the execution time measures thanks to the <perf> command.
To proceed, do as follows: 
- execute bash script to create executables
- put executables in folder named "executables"
- execute bash script to perform time measures

The results are in the folder "perf_measures". There is one file per program.

## Results

In this folder, there are time measures files.

## Statistics

There is a c file in this folder that computes statistical results (mean, confidence interval...) of our data. 

To use it, you have to:
- compile it : "gcc -o stat Statistics.c"
- execute it: ./stat <current_frequency> <name_of_the_folder_where_time_measures_are_done> 

## Speed policy

This folder has a c file containing an implementation of Quan and Hu's algorithm.

## Scheduler

This folder is for the scheduler. At first, there is a script to create libraries for the jobs of our real-time systems. Then there are a c file with a job scheduler and makefile.
To proceed, do as follows: 
- execute bash script
- type the following command line : ```export LD_LIBRARY_PATH=/usr/lib:$LD_LIBRARY_PATH```
- compile : ```gcc -L/usr/lib -Wall -o sched Job_Scheduler.c -lndes -lstatemate -lcjpeg_wrbmp``` 

## UART Communication

There are files in this folder that allows us to enable UART communication when doing energy consumption measures.

## Others

This folder is for other files that we do not use. For now, there is a file named <mouse_handling> which is useful to automate energy consumption measures in the software <Otii 3 App>. However, this automation is according to the size of our monitor.
