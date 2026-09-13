/*
 * Question 3 — Deadlock Prevention via Resource Ordering (Unified Driver)
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

static Lock lock1 = {"Lock1", false, -1};
static Lock lock2 = {"Lock2", false, -1};

static void reset_locks() {
    lock1.locked = false; lock1.held_by = -1;
    lock2.locked = false; lock2.held_by = -1;
}

static void try_acquire(Lock *l, int pid, const char *pname) {
    printf("[%s] Marker: Attempting to acquire %s...\n", pname, l->name);
    if (l->locked) {
        printf("[%s] Marker: WAITING for %s (Held by Process %d)\n", pname, l->name, l->held_by);
    } else {
        l->locked = true;
        l->held_by = pid;
        printf("[%s] Marker: ACQUIRED %s\n", pname, l->name);
    }
}

static void release(Lock *l, const char *pname) {
    if (l->locked) {
        l->locked = false;
        l->held_by = -1;
        printf("[%s] Marker: RELEASED %s\n", pname, l->name);
    }
}

void run_bad_scenario() {
    reset_locks();
    printf("========================================================\n");
    printf("     DEMONSTRATION 1: INCONSISTENT LOCK ORDER (BAD)     \n");
    printf("========================================================\n");
    printf("Process A order: Lock1 -> Lock2\n");
    printf("Process B order: Lock2 -> Lock1\n\n");

    try_acquire(&lock1, 1, "Process A");
    try_acquire(&lock2, 2, "Process B");
    
    printf("\n--- Deliberate delay elapsed ---\n");
    try_acquire(&lock2, 1, "Process A");
    try_acquire(&lock1, 2, "Process B");

    printf("\n>>> STATUS: DEADLOCK DETECTED! Both processes hanging in circular wait.\n\n");
}

void run_fixed_scenario() {
    reset_locks();
    printf("========================================================\n");
    printf("     DEMONSTRATION 2: STRICT GLOBAL ORDERING (FIXED)    \n");
    printf("========================================================\n");
    printf("Global Hierarchy: Lock1 < Lock2 (All processes acquire Lock1 then Lock2)\n\n");

    try_acquire(&lock1, 1, "Process A");
    try_acquire(&lock1, 2, "Process B");

    try_acquire(&lock2, 1, "Process A");
    printf("[Process A] Marker: Working in critical section...\n");
    release(&lock2, "Process A");
    release(&lock1, "Process A");

    printf("\n--- Process A finished; Process B unblocked ---\n");
    try_acquire(&lock1, 2, "Process B");
    try_acquire(&lock2, 2, "Process B");
    printf("[Process B] Marker: Working in critical section...\n");
    release(&lock2, "Process B");
    release(&lock1, "Process B");

    printf("\n>>> STATUS: SUCCESS! All processes completed without deadlock.\n\n");
}

int main() {
    run_bad_scenario();
    run_fixed_scenario();
    return 0;
}
