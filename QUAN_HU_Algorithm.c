#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define true 1
#define false 0

typedef struct _job {
    int index; //Index of the job (which also indicates the priority)
    int O; //Release time
    int C; //Computation time
    int D; //Absolute Deadline
    struct _job * next; 
} Job;

typedef struct _set {
    Job * head; //head of the list
    int size; //number of jobs
} Set;

typedef struct _interval {
    int ts;
    int tf;
} Interval;

typedef struct _interval_and_speed {
    Interval interval;
    float S;
} Interval_and_speed;

typedef struct _intervalSet {
    Interval interval;
    struct _intervalSet * next;
} IntervalSet;

typedef struct _speedSet {
    float S;
    struct _speedSet * next;
} SpeedSet;

typedef struct _schedulingPoints {
    int val;
    struct _schedulingPoints * next;
} SchedulingPoints;


/* Computes the earliest scheduling point of a job Jn based on a set of jobs */
int earliest_scheduling_point (Job j, Set s) {
    
    Job * current = s.head; //pointer on the current job
    Job * tmp = NULL; 
    int TE = j.O; //We initialise the earliest scheduling point to the current job release
    
    while (current != NULL){
       
        if (current.O >= 0 && current.O <= j.O) { //First, we check if the considered scheduling point is in [0, Rn]
            
            int found = true;
            
            tmp  = s.head;
            while (tmp != NULL) {
                if (current.O > tmp.O && current.O < tmp.D) {
                    found = false;
                    break;
                }
                tmp = tmp->next;
            }
            
            if (found) {
                if (current.O < TE) TE = current.O;
            }
        }
        current = current->next;
    }

    return TE;
}

/* Returns the latest scheduling point of a job Jn */
int latest_scheduling_point (Job j) {  
    return j.D;
}

float minimum_constant_speed (Set s, int ta, int tb) {
    
    Job * current = s.head; //pointer on the current job
    int sum = 0; //sum of the computation times in [ta, tb]

    for (int i = 0; i < s.size; i++) {
        if (current->O >= ta && current->O < tb) { //We check if the current job is computed in [ta, tb]
            sum += current.C;
        }
    }
    
    return (1.0)sum/(tb-ta);
} 

SchedulingPoints * create_scheduling_point(SchedulingPoints * sp, int val) {
    SchedulingPoints * new = (SchedulingPoints *) malloc (sizeof(SchedulingPoints));
    new->val = val;
    new->next = sp;
    return new;
}

Interval_and_speed essential_interval_Jn (int TE, int TL, Set s, Job * j) {
    int ta_prim , tb;
    int ta = TE;
    int tb_prim = TL;

    Job * current = s.head;
    //We create a set of j-scheduling points
    SchedulingPoints * sp = create_scheduling_point(NULL, j->D);
    while (current != NULL) {
        sp = create_scheduling_point(sp, current.O);
        current = current->next;
    }

    while (ta != ta_prim || tb_prim != tb) {
        ta = ta_prim;
        tb_prim = tb;

        int min = -1;
        SchedulingPoints * curr = sp;
        while (curr != NULL) {
            if (curr->val >= ta && curr->val <= TL) {
                if (min == -1){
                    min = curr->val;
                }
                else {
                    int tmp = minimum_constant_speed (s, ta, curr->val);
                    if (tmp < min) {
                        min = curr->val;
                    }
                }
            }
        }
        tb = min;

        int max = -1;
        SchedulingPoints * curr = sp;
        while (curr != NULL) {
            if (curr->val >= TE && curr->val <= ta) {
                if (max == -1){
                    max = curr->val;
                }
                else {
                    int tmp = minimum_constant_speed (s, curr->val; tb);
                    if (tmp < max) {
                        max = curr->val;
                    }
                }
            }
        }
        ta_prim = max;
    }

    Interval_and_speed res;
    res.interval.ts = ta;
    res.interval.tf = tb;
    res.S = minimum_constant_speed(s, ta, tb);
    
    return res;
}

void critical_intervals (Set s) {

    Job * current = s.head; //pointer on the current job

    while (current != NULL) {
        int TE, TL;
        TE = earliest_scheduling_point(*current, s);
        TL = latest_scheduling_point(*current);
        Interval_and_speed tmp = essential_interval_Jn(TE, TL, s);

        current = current->next;
    }    

}

int main (int argc, char * argv[]) {
    
    //We create 3 jobs
    Job j3 = {3, 3, 5, 10, NULL};
    Job j2 = {2, 2, 4, 8, &j3};
    Job j1 = {1, 0, 1, 9, &j2};

    Set s = {&j1, 3};

    return 0;
}