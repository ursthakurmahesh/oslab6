/*
 * Question 1 — Banker's Algorithm Simulation
 * Operating System Lab Assignment 6
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define N 5 // Number of processes (P0 to P4)
#define M 3 // Number of resource types (A, B, C)

int allocation[N][M] = {
    {0, 1, 0}, {2, 0, 0}, {3, 0, 2}, {2, 1, 1}, {0, 0, 2}
};

int max_claim[N][M] = {
    {7, 5, 3}, {3, 2, 2}, {9, 0, 2}, {2, 2, 2}, {4, 3, 3}
};

int available[M] = {3, 3, 2};
int need[N][M];

void calculate_need() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            need[i][j] = max_claim[i][j] - allocation[i][j];
        }
    }
}

void print_state() {
    printf("\n================ CURRENT SYSTEM STATE ================\n");
    printf("Process | Allocation |   Max Claim  |     Need     |\n");
    printf("        |  A   B   C |  A   B   C   |  A   B   C   |\n");
    printf("--------+------------+--------------+--------------+\n");
    for (int i = 0; i < N; i++) {
        printf("   P%d   |  %d   %d   %d |  %d   %d   %d   |  %d   %d   %d   |\n",
               i,
               allocation[i][0], allocation[i][1], allocation[i][2],
               max_claim[i][0], max_claim[i][1], max_claim[i][2],
               need[i][0], need[i][1], need[i][2]);
    }
    printf("----------------------------------------------------\n");
    printf("Available Resources: A = %d, B = %d, C = %d\n", available[0], available[1], available[2]);
    printf("====================================================\n\n");
}

bool is_safe(int safe_sequence[]) {
    int work[M];
    bool finish[N];
    for (int j = 0; j < M; j++) work[j] = available[j];
    for (int i = 0; i < N; i++) finish[i] = false;

    int count = 0;
    while (count < N) {
        bool found = false;
        for (int i = 0; i < N; i++) {
            if (!finish[i]) {
                bool can_allocate = true;
                for (int j = 0; j < M; j++) {
                    if (need[i][j] > work[j]) { can_allocate = false; break; }
                }
                if (can_allocate) {
                    for (int j = 0; j < M; j++) work[j] += allocation[i][j];
                    safe_sequence[count++] = i;
                    finish[i] = true;
                    found = true;
                }
            }
        }
        if (!found) return false;
    }
    return true;
}

bool request_resources(int process_id, int request[]) {
    printf(">>> Process P%d requests resources: [ %d, %d, %d ]\n", process_id, request[0], request[1], request[2]);
    for (int j = 0; j < M; j++) {
        if (request[j] > need[process_id][j]) {
            printf("Error: Process P%d exceeded maximum claim!\n", process_id);
            return false;
        }
        if (request[j] > available[j]) {
            printf("Process P%d must wait: Resources not available!\n", process_id);
            return false;
        }
    }
    for (int j = 0; j < M; j++) {
        available[j] -= request[j];
        allocation[process_id][j] += request[j];
        need[process_id][j] -= request[j];
    }

    int safe_seq[N];
    if (is_safe(safe_seq)) {
        printf("SUCCESS: Request GRANTED safely to P%d.\nNew Safe Sequence: < ", process_id);
        for (int i = 0; i < N; i++) printf("P%d%s", safe_seq[i], (i == N - 1) ? " " : ", ");
        printf(">\n\n");
        return true;
    } else {
        printf("Request DENIED — would lead to unsafe state!\nRolling back...\n\n");
        for (int j = 0; j < M; j++) {
            available[j] += request[j];
            allocation[process_id][j] -= request[j];
            need[process_id][j] += request[j];
        }
        return false;
    }
}

int main() {
    printf("========================================================\n");
    printf("       BANKER'S ALGORITHM SIMULATION (XV6 / POSIX)     \n");
    printf("========================================================\n");

    calculate_need();
    print_state();

    int safe_sequence[N];
    if (is_safe(safe_sequence)) {
        printf("Initial state is SAFE.\nSafe Sequence: < ");
        for (int i = 0; i < N; i++) printf("P%d%s", safe_sequence[i], (i == N - 1) ? " " : ", ");
        printf(">\n");
    } else {
        printf("Initial state is UNSAFE!\n");
        return 1;
    }

    printf("\n--------------------------------------------------------\n");
    printf("SCENARIO 1: Safe Request (P1 requests [1, 0, 2])\n");
    printf("--------------------------------------------------------\n");
    int req1[M] = {1, 0, 2};
    request_resources(1, req1);
    print_state();

    printf("--------------------------------------------------------\n");
    printf("SCENARIO 2: Unsafe Request (P0 requests [0, 2, 0])\n");
    printf("--------------------------------------------------------\n");
    int req2[M] = {0, 2, 0};
    request_resources(0, req2);
    print_state();

    return 0;
}
