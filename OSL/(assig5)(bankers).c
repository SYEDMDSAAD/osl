#include <stdio.h>
#include <stdbool.h>
#define P 5
#define R 3

void calculateNeed(int need[P][R], int max[P][R], int allot[P][R]) {
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < R; j++) {
            need[i][j] = max[i][j] - allot[i][j];
        }
    }
}

bool isSafe(int processes[], int avail[], int max[][R], int allot[][R]) {
    int need[P][R];
    calculateNeed(need, max, allot);
    bool finish[P] = {0};
    int safeSeq[P];
    int work[R];

    for (int i = 0; i < R; i++) {
        work[i] = avail[i];
    }

    int count = 0;
    while (count < P) {
        bool found = false;
        for (int p = 0; p < P; p++) {
            if (!finish[p]) {
                int j;
                for (j = 0; j < R; j++) {
                    if (need[p][j] > work[j]) {
                    break;
                    }
                }
                if (j == R) {
                for (int k = 0; k < R; k++) {
                    work[k] += allot[p][k];
                }
                safeSeq[count++] = processes[p];
                finish[p] = true;
                found = true;
                }
            }   
        }
        if (!found) {
                printf("System is not in a safe state.\n");
                return false;
        }
    }
    printf("System is in a safe state.\nSafe sequence is: ");
    for (int i = 0; i < P; i++) {
        printf("%d ", safeSeq[i]);
    }
    printf("\n");
    return true;
}

int main() {

int processes[P];
int avail[R];
int max[P][R];
int allot[P][R];

printf("Enter the processes IDs:\n");

for (int i = 0; i < P; i++) {
    printf("Process %d ID: ", i + 1);
    scanf("%d", &processes[i]);
}

printf("\nEnter the available resources:\n");

for (int i = 0; i < R; i++) {
    printf("Resource %d: ", i + 1);
    scanf("%d", &avail[i]);
}

printf("\nEnter the maximum resources:\n");

for (int i = 0; i < P; i++) {
    printf("Process %d: ", processes[i]);
    for (int j = 0; j < R; j++) {
        scanf("%d", &max[i][j]);
    }
}

printf("\nEnter the allocated resources:\n");

for (int i = 0; i < P; i++) {
    printf("Process %d: ", processes[i]);
        for (int j = 0; j < R; j++) {
        scanf("%d", &allot[i][j]);
    }
}

isSafe(processes, avail, max, allot);

return 0;
}
