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

typedef struct _job_TE_TL {
    int TE;
    int TL;
    Job job;
    struct _job_TE_TL * next;
} Job_TE_TL;


/* Computes the earliest scheduling point of a job Jn based on a set of jobs */
int earliest_scheduling_point (Job j, Set s) {
    
    Job * current = s.head; //pointer on the current job
    Job * tmp = NULL; 
    int TE = j.O; //We initialise the earliest scheduling point to the current job release

    while (current != NULL){
       
        if (current->O >= 0 && current->O <= j.O) { //First, we check if the considered scheduling point is in [0, Rn]
            
            int found = true;
            
            tmp  = s.head;
            while (tmp != NULL) {
                if (current->O > tmp->O && current->O < tmp->D) {
                    found = false;
                    break;
                }
                tmp = tmp->next;
            }
            
            if (found) {
                if (current->O < TE) TE = current->O;
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

    while (current) {
        if (current->O >= ta && current->O < tb) { //We check if the current job is computed in [ta, tb]
            sum += current->C;
        }
        current = current->next;
    }
    
    return ((float) sum)/(tb-ta);
} 

SchedulingPoints * create_scheduling_point (SchedulingPoints * sp, int val) {
    SchedulingPoints * new = (SchedulingPoints *) malloc (sizeof(SchedulingPoints));
    new->val = val;
    new->next = sp;
    return new;
}

Interval_and_speed essential_interval_Jn (int TE, int TL, Set s, Job * j) {
    int ta_prim = j->O, tb = j->O;
    int ta = TE;
    int tb_prim = TL;

    Job * current = s.head;

    //We create a set of j-scheduling points
    SchedulingPoints * sp = create_scheduling_point(NULL, j->D);
    while (current != NULL) {
        sp = create_scheduling_point(sp, current->O);
        current = current->next;
    }

    while (ta != ta_prim || tb_prim != tb) {
        ta = ta_prim;
        tb_prim = tb;

        int min_speed = -1;
        SchedulingPoints * curr = sp;
        while (curr != NULL) {
            if (curr->val > ta && curr->val <= TL) {
                if (min_speed == -1){
                    min_speed = minimum_constant_speed (s, ta, curr->val);
                    tb = curr->val;
                }
                else {
                    int speed = minimum_constant_speed (s, ta, curr->val);
                    if (speed < min_speed) {
                        min_speed = speed;
                        tb = curr->val;
                    }
                }
            }
            curr = curr->next;
        }

        int max_speed = -1;
        curr = sp;
        while (curr != NULL) {
            if (curr->val >= TE && curr->val <= ta) {
                int speed = minimum_constant_speed (s, curr->val, tb);
                if (speed > max_speed) {
                    max_speed = speed;
                    ta_prim = curr->val;
                }
            }
            curr = curr->next;
        }
    }

    Interval_and_speed res;
    res.interval.ts = ta;
    res.interval.tf = tb;
    res.S = minimum_constant_speed(s, ta, tb);
    
    return res;
}

IntervalSet * create_interval_set (IntervalSet * is, Interval val) {
    IntervalSet * new = (IntervalSet *) malloc (sizeof(IntervalSet));
    new->interval = val;
    new->next = is;
    return new;
}

SpeedSet * create_speed_set (SpeedSet * ss, float val) {
    SpeedSet * new = (SpeedSet *) malloc (sizeof(SpeedSet));
    new->S = val;
    new->next = ss;
    return new;
}

Job_TE_TL * create_job_TE_TL (Job_TE_TL * j, int te, int tl, Job job) {
    Job_TE_TL * new = (Job_TE_TL *) malloc (sizeof(Job_TE_TL));
    new->TE = te;
    new->TL = tl;
    new->job = job;
    new->next = j;
    return new;
}

void critical_intervals (Set s) {

    Job * current_job = s.head; //pointer on the current job

    IntervalSet * A = NULL;
    SpeedSet * S_prim = NULL;
    Job_TE_TL * job_TE_TL = NULL;

    while (current_job != NULL) {
        int TE, TL;
        TE = earliest_scheduling_point(*current_job, s);
        TL = latest_scheduling_point(*current_job);

        job_TE_TL = create_job_TE_TL(job_TE_TL, TE, TL, *current_job);
        
        Interval_and_speed tmp = essential_interval_Jn(TE, TL, s, current_job);
        A = create_interval_set(A, tmp.interval);
        S_prim = create_speed_set(S_prim, tmp.S);

        current_job = current_job->next;
    }

    int k = 1;
    IntervalSet * T = NULL;
    SpeedSet * S = NULL;

    while (A != NULL) {
        

        float S_j = S_prim->S;
        Interval A_j = A->interval;

        T = create_interval_set(T, A_j);
        S = create_speed_set(S, S_j);

        SpeedSet * tmp_speed = S_prim;
        IntervalSet * tmp_interval = A;
        A = A->next;
        S_prim = S->next;
        free(tmp_speed);
        free(tmp_interval);

        int m = 1;
        current_job = s.head;
        Job_TE_TL * curr_job_TE_TL = job_TE_TL;
        while (current_job != NULL) {
            if (m <= k && current_job->O >= A_j.ts && current_job->O < A_j.tf) {

                //Remove Jm from J, the set of jobs
                Job * tmp = current_job;
                current_job = current_job->next;
                free(tmp);

                //Remove Sm from S and corresponding [ts, tf] from A
                


                //We also remove the job from job_TE_Tl because its release time, TE or TL won't be changed
                Job_TE_TL * tmp2 = curr_job_TE_TL;
                curr_job_TE_TL = curr_job_TE_TL->next;
                free(tmp2);
            }
            else {
                if (current_job->O > A_j.ts && current_job->O <= A_j.tf) {
                    current_job->O = A_j.ts;
                    curr_job_TE_TL->job.O = A_j.ts;
                }
                if (curr_job_TE_TL->TE > A_j.ts && curr_job_TE_TL->TE <= A_j.tf) {
                    curr_job_TE_TL->TE = A_j.ts;
                }
                if (curr_job_TE_TL->TL > A_j.ts && curr_job_TE_TL->TL <= A_j.tf) {
                    curr_job_TE_TL->TL = A_j.ts;
                }

                curr_job_TE_TL = curr_job_TE_TL->next;
            }
            current_job = current_job->next;
        }

        while (1) {

        } 

        curr_job_TE_TL = job_TE_TL;
        while (curr_job_TE_TL != NULL) {

            curr_job_TE_TL = curr_job_TE_TL->next;
        }

        k++;
    }    

}

int main (int argc, char * argv[]) {
    
    //We create 3 jobs
    Job j3 = {3, 3, 5, 10, NULL};
    Job j2 = {2, 2, 4, 8, &j3};
    Job j1 = {1, 0, 1, 9, &j2};

    Set s = {&j1, 3};

    Interval_and_speed tmp = essential_interval_Jn(earliest_scheduling_point(j3, s), latest_scheduling_point(j3), s, &j3);
    printf("ts : %d, tf : %d, S : %.2f\n", tmp.interval.ts, tmp.interval.tf, tmp.S);

    return 0;
}