/*
 * Question 2 — Deadlock Detection using Resource Allocation Graph (Wait-For Graph)
 * Operating System Lab Assignment 6
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_P 10
#define MAX_R 10

typedef struct {
    int num_processes;
    int num_resources;
    int allocation[MAX_P][MAX_R];
    int request[MAX_P][MAX_R];
    int wfg[MAX_P][MAX_P];
} RAG;

#define UNVISITED 0
#define VISITING 1
#define VISITED 2

int state[MAX_P];
int parent_node[MAX_P];
int cycle_start = -1;
int cycle_end = -1;

void build_wait_for_graph(RAG *rag) {
    int n = rag->num_processes;
    int m = rag->num_resources;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            rag->wfg[i][j] = 0;

    for (int i = 0; i < n; i++) {
        for (int k = 0; k < m; k++) {
            if (rag->request[i][k] > 0) {
                for (int j = 0; j < n; j++) {
                    if (i != j && rag->allocation[j][k] > 0) {
                        rag->wfg[i][j] = 1;
                    }
                }
            }
        }
    }
}

void print_rag(RAG *rag, const char *scenario_name) {
    printf("\n========================================================\n");
    printf(" SCENARIO: %s\n", scenario_name);
    printf("========================================================\n");

    printf("\n--- ALLOCATION MATRIX ---\nProcess | ");
    for (int k = 0; k < rag->num_resources; k++) printf("R%d ", k);
    printf("\n--------+-------\n");
    for (int i = 0; i < rag->num_processes; i++) {
        printf("   P%d   | ", i);
        for (int k = 0; k < rag->num_resources; k++) printf(" %d ", rag->allocation[i][k]);
        printf("\n");
    }

    printf("\n--- REQUEST MATRIX ---\nProcess | ");
    for (int k = 0; k < rag->num_resources; k++) printf("R%d ", k);
    printf("\n--------+-------\n");
    for (int i = 0; i < rag->num_processes; i++) {
        printf("   P%d   | ", i);
        for (int k = 0; k < rag->num_resources; k++) printf(" %d ", rag->request[i][k]);
        printf("\n");
    }

    printf("\n--- CONSTRUCTED WAIT-FOR GRAPH (WFG ADJACENCY MATRIX) ---\n        | ");
    for (int j = 0; j < rag->num_processes; j++) printf("P%d ", j);
    printf("\n--------+-------\n");
    for (int i = 0; i < rag->num_processes; i++) {
        printf("   P%d   | ", i);
        for (int j = 0; j < rag->num_processes; j++) printf(" %d ", rag->wfg[i][j]);
        printf("\n");
    }

    printf("\n--- WFG EDGES ---\n");
    bool has_edges = false;
    for (int i = 0; i < rag->num_processes; i++) {
        for (int j = 0; j < rag->num_processes; j++) {
            if (rag->wfg[i][j]) {
                printf("  P%d  -->  P%d (P%d waiting for resource held by P%d)\n", i, j, i, j);
                has_edges = true;
            }
        }
    }
    if (!has_edges) printf("  (No wait edges in graph)\n");
    printf("--------------------------------------------------------\n");
}

bool dfs_detect_cycle(RAG *rag, int u) {
    state[u] = VISITING;

    for (int v = 0; v < rag->num_processes; v++) {
        if (rag->wfg[u][v]) {
            if (state[v] == VISITING) {
                cycle_start = v;
                cycle_end = u;
                return true;
            }
            if (state[v] == UNVISITED) {
                parent_node[v] = u;
                if (dfs_detect_cycle(rag, v)) return true;
            }
        }
    }

    state[u] = VISITED;
    return false;
}

void detect_deadlock(RAG *rag) {
    int n = rag->num_processes;
    for (int i = 0; i < n; i++) {
        state[i] = UNVISITED;
        parent_node[i] = -1;
    }
    cycle_start = -1;
    cycle_end = -1;

    bool deadlock_found = false;
    for (int i = 0; i < n; i++) {
        if (state[i] == UNVISITED) {
            if (dfs_detect_cycle(rag, i)) {
                deadlock_found = true;
                break;
            }
        }
    }

    if (deadlock_found) {
        printf("\n[RESULT] DEADLOCK DETECTED!\n");
        printf("Cycle of processes involved in deadlock: ");

        int cycle_path[MAX_P];
        int path_len = 0;
        cycle_path[path_len++] = cycle_start;

        for (int curr = cycle_end; curr != cycle_start && curr != -1; curr = parent_node[curr]) {
            cycle_path[path_len++] = curr;
        }
        cycle_path[path_len++] = cycle_start;

        for (int k = path_len - 1; k >= 0; k--) {
            printf("P%d%s", cycle_path[k], (k == 0) ? "" : " -> ");
        }
        printf("\n");
    } else {
        printf("\n[RESULT] NO DEADLOCK DETECTED. Wait-for graph is acyclic.\n");
    }
    printf("========================================================\n\n");
}

int main() {
    printf("========================================================\n");
    printf("   DEADLOCK DETECTION USING RESOURCE ALLOCATION GRAPH   \n");
    printf("========================================================\n");

    RAG scenario1 = {
        .num_processes = 4,
        .num_resources = 3,
        .allocation = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {0, 0, 0} },
        .request = { {0, 1, 0}, {0, 0, 1}, {0, 0, 0}, {1, 0, 0} }
    };
    build_wait_for_graph(&scenario1);
    print_rag(&scenario1, "Scenario 1 - Acyclic Wait-For Graph (No Deadlock)");
    detect_deadlock(&scenario1);

    RAG scenario2 = {
        .num_processes = 4,
        .num_resources = 4,
        .allocation = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} },
        .request = { {0, 1, 0, 0}, {0, 0, 1, 0}, {1, 0, 0, 0}, {0, 0, 1, 0} }
    };
    build_wait_for_graph(&scenario2);
    print_rag(&scenario2, "Scenario 2 - 3-Process Circular Wait Deadlock (P0 -> P1 -> P2 -> P0)");
    detect_deadlock(&scenario2);

    return 0;
}
