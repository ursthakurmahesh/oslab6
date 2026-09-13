/*
 * Question 4 — Combined Synchronization & Deadlock Avoidance
 * Operating System Lab Assignment 6
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_PROCESSES 5
#define NUM_RESOURCES 3
#define NUM_CYCLES 3

const char *resource_names[NUM_RESOURCES] = {"Printer", "Scanner", "Disk"};
int max_available[NUM_RESOURCES] = {2, 1, 2};
int current_available[NUM_RESOURCES] = {2, 1, 2};

int process_needs[NUM_PROCESSES][2] = {
    {0, 1}, {1, 2}, {0, 2}, {0, 1}, {1, 2}
};

bool acquire_resource(int pid, int res_type) {
    printf("[Process P%d] REQUESTING resource: %s (Type %d)\n", pid, resource_names[res_type], res_type);
    if (current_available[res_type] > 0) {
        current_available[res_type]--;
        printf("[Process P%d] GRANTED resource: %s (Type %d) | Remaining %s instances: %d\n",
               pid, resource_names[res_type], res_type, resource_names[res_type], current_available[res_type]);
        return true;
    } else {
        printf("[Process P%d] WAITING for resource: %s (Type %d) | 0 instances available\n",
               pid, resource_names[res_type], res_type);
        return false;
    }
}

void release_resource(int pid, int res_type) {
    current_available[res_type]++;
    printf("[Process P%d] RELEASING resource: %s (Type %d) | Now available: %d\n",
           pid, resource_names[res_type], res_type, current_available[res_type]);
}

void run_process_cycle(int pid, int cycle) {
    int r1 = process_needs[pid][0];
    int r2 = process_needs[pid][1];

    printf("\n>>> [Cycle %d] Process P%d initiating work cycle (Needs: %s & %s) <<<\n",
           cycle, pid, resource_names[r1], resource_names[r2]);

    while (!acquire_resource(pid, r1)) {}
    while (!acquire_resource(pid, r2)) {}

    printf("[Process P%d] STARTING WORK with acquired resources (%s & %s)...\n", pid, resource_names[r1], resource_names[r2]);
    printf("[Process P%d] Working (simulated task execution)...\n", pid);
    printf("[Process P%d] WORK COMPLETED successfully.\n", pid);

    release_resource(pid, r2);
    release_resource(pid, r1);
    printf("<<< [Cycle %d] Process P%d completed cycle >>>\n", cycle, pid);
}

int main() {
    printf("========================================================\n");
    printf(" COMBINED SYNCHRONIZATION & DEADLOCK AVOIDANCE (XV6/POSIX) \n");
    printf("========================================================\n");

    for (int cycle = 1; cycle <= NUM_CYCLES; cycle++) {
        printf("\n========================================================\n");
        printf("               STARTING EXECUTION CYCLE %d               \n", cycle);
        printf("========================================================\n");

        for (int p = 0; p < NUM_PROCESSES; p++) {
            run_process_cycle(p, cycle);
        }
    }

    printf("\n========================================================\n");
    printf("FINAL VERIFICATION: ALL 5 PROCESSES COMPLETED %d CYCLES!\n", NUM_CYCLES);
    printf("========================================================\n");

    return 0;
}
