/*
 * Question 3 — Deadlock Prevention via Resource Ordering (FIXED Scenario: Strict Global Order)
 * Operating System Lab Assignment 6
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    const char *name;
    bool locked;
    int held_by;
} Lock;

Lock lock1 = {"Lock1 (Printer)", false, -1};
Lock lock2 = {"Lock2 (Scanner)", false, -1};

void acquire_lock(Lock *l, int pid, const char *process_name) {
    printf("[%s] ATTEMPTING to acquire %s...\n", process_name, l->name);
    if (l->locked) {
        printf("[%s] WAITING for %s (Currently held by Process %d)\n", process_name, l->name, l->held_by);
    } else {
        l->locked = true;
        l->held_by = pid;
        printf("[%s] ACQUIRED %s successfully.\n", process_name, l->name);
    }
}

void release_lock(Lock *l, const char *process_name) {
    if (l->locked) {
        l->locked = false;
        l->held_by = -1;
        printf("[%s] RELEASED %s.\n", process_name, l->name);
    }
}

int main() {
    printf("========================================================\n");
    printf(" QUESTION 3: DEADLOCK PREVENTION - FIXED RUN (STRICT ORDERING)\n");
    printf("========================================================\n");

    acquire_lock(&lock1, 1, "Process A");
    acquire_lock(&lock1, 2, "Process B");
    acquire_lock(&lock2, 1, "Process A");
    release_lock(&lock2, "Process A");
    release_lock(&lock1, "Process A");

    acquire_lock(&lock1, 2, "Process B");
    acquire_lock(&lock2, 2, "Process B");
    release_lock(&lock2, "Process B");
    release_lock(&lock1, "Process B");

    printf("\nSYSTEM STATUS: SUCCESS! WORKLOAD COMPLETED WITHOUT HANGING.\n");
    return 0;
}
