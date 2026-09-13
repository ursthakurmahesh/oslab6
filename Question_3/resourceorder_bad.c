/*
 * Question 3 — Deadlock Prevention via Resource Ordering (BAD Scenario: Inconsistent Lock Order)
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
        printf("[%s] WAITING for %s (Currently held by Process %d) -> DEADLOCK HANG!\n",
               process_name, l->name, l->held_by);
    } else {
        l->locked = true;
        l->held_by = pid;
        printf("[%s] ACQUIRED %s successfully.\n", process_name, l->name);
    }
}

int main() {
    printf("========================================================\n");
    printf("  QUESTION 3: DEADLOCK PREVENTION - BAD RUN (INCONSISTENT ORDER)\n");
    printf("========================================================\n");

    acquire_lock(&lock1, 1, "Process A");
    acquire_lock(&lock2, 2, "Process B");
    acquire_lock(&lock2, 1, "Process A");
    acquire_lock(&lock1, 2, "Process B");

    printf("\nSYSTEM STATUS: DEADLOCK CONFIRMED!\n");
    return 0;
}
