#include <stdio.h>

void srtf(int n, int arrival_time[], int burst_time[]) {
    int temp[10], completion_time[10], wait_time[10], turnaround_time[10];
    int i, smallest, count = 0, time;
    double total_wait_time = 0, total_turnaround_time = 0;
    float average_waiting_time, average_turnaround_time;

    for(i = 0; i < n; i++) {
        temp[i] = burst_time[i];
    }
    burst_time[9] = 9999;

    for(time = 0; count != n; time++) {
        smallest = 9;
        for(i = 0; i < n; i++) {
            if(arrival_time[i] <= time && burst_time[i] < burst_time[smallest] && burst_time[i] > 0) {
                smallest = i;
            }
        }
        burst_time[smallest]--;
        
        if(burst_time[smallest] == 0) {
            count++;
            completion_time[smallest] = time + 1;
            turnaround_time[smallest] = completion_time[smallest] - arrival_time[smallest];
            wait_time[smallest] = turnaround_time[smallest] - temp[smallest];

            total_turnaround_time += turnaround_time[smallest];
            total_wait_time += wait_time[smallest];
        }
    }

    average_waiting_time = total_wait_time / n;
    average_turnaround_time = total_turnaround_time / n;

    printf("\nSRTF Scheduling:\n");
    printf("Process | AT  | BT  | CT  | TAT | WT\n");
    printf("--------|-----|-----|-----|-----|-----\n");
    for(i = 0; i < n; i++) {
        printf("P%-7d| %-3d | %-3d | %-3d | %-3d | %-3d\n", i + 1, arrival_time[i], temp[i], completion_time[i], turnaround_time[i], wait_time[i]);
    }

    printf("\nAverage Waiting Time = %lf\n", average_waiting_time);
    printf("Average Turnaround Time = %lf\n", average_turnaround_time);
}

void round_robin(int n, int arrival_time[], int burst_time[]) {
    int cnt, t, remain, flag = 0, tq;
    int wt = 0, tat = 0, temp[10];
    int completion_time[10], turnaround_time[10], waiting_time[10];
    int remaining_burst_time[10]; // Array to keep track of remaining burst time for each process

    for (cnt = 0; cnt < n; cnt++) {
        temp[cnt] = burst_time[cnt];  // Store original burst times
        remaining_burst_time[cnt] = burst_time[cnt]; // Initialize remaining burst times
    }

    printf("Enter Time Quantum: ");
    scanf("%d", &tq);
    
    remain = n; // Set number of remaining processes
    t = 0; // Start time

    while (remain > 0) {
        int process_executed = 0;

        for (cnt = 0; cnt < n; cnt++) {
            // Check if the process has arrived and has remaining burst time
            if (arrival_time[cnt] <= t && remaining_burst_time[cnt] > 0) {  
                process_executed = 1;  // Mark that a process is being executed
                if (remaining_burst_time[cnt] <= tq) {  // Process will finish within this time quantum
                    t += remaining_burst_time[cnt];
                    remaining_burst_time[cnt] = 0; // Process completed
                } else {  // Process will be partially executed
                    remaining_burst_time[cnt] -= tq;
                    t += tq;
                }

                if (remaining_burst_time[cnt] == 0) {  // Process just finished
                    completion_time[cnt] = t;
                    turnaround_time[cnt] = completion_time[cnt] - arrival_time[cnt];
                    waiting_time[cnt] = turnaround_time[cnt] - temp[cnt];
                    wt += waiting_time[cnt];
                    tat += turnaround_time[cnt];
                    remain--;  // Decrement remaining processes
                }
            }
        }

        // If no process was executed in this cycle, move the time forward to the next available process
        if (!process_executed) {
            t++;
        }
    }

    int i;
    printf("\nRound Robin Scheduling:\n");
    printf("Process | AT  | BT  | CT  | TAT | WT\n");
    printf("--------|-----|-----|-----|-----|-----\n");
    for(i = 0; i < n; i++) {
        printf("P%-7d| %-3d | %-3d | %-3d | %-3d | %-3d\n", i + 1, arrival_time[i], temp[i], completion_time[i], turnaround_time[i], waiting_time[i]);
    }

    printf("\nAverage Waiting Time = %f\n", wt * 1.0 / n);
    printf("Average Turnaround Time = %f\n", tat * 1.0 / n);
}

int main() {
    int n, choice;
    
    printf("Enter Total Number of Processes: ");
    scanf("%d", &n);

    int arrival_time[n], burst_time[n];
    
    for(int i = 0; i < n; i++) {
        printf("Enter Arrival Time and Burst Time for Process %d: ", i + 1);
        scanf("%d %d", &arrival_time[i], &burst_time[i]);
    }
    
    printf("\nSelect Scheduling Algorithm:\n");
    printf("1. Shortest Remaining Time First (SRTF)\n");
    printf("2. Round Robin (RR)\n");
    printf("Enter your choice (1 or 2): ");
    scanf("%d", &choice);
    
    switch(choice) {
        case 1:
            srtf(n, arrival_time, burst_time);
            break;
        case 2:
            round_robin(n, arrival_time, burst_time);
            break;
        default:
            printf("Invalid choice!\n");
            break;
    }

    return 0;
}
