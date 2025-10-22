#include <stdio.h>
#include <stdlib.h>

struct Process {
    int pid;             
    int arrival_time;    
    int burst_time;      
    int turnaround_time;
    int waiting_time;   
    int response_time;  
};

void fcfs(struct Process p[], int n);
void sjf(struct Process p[], int n);
void calculate_and_print(struct Process p[], int n, const char *algo_name);

void swap(struct Process *a, struct Process *b) {
    struct Process temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process p[n];
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Enter the arrival time and burst time for process %d: ", i + 1);
        scanf("%d %d", &p[i].arrival_time, &p[i].burst_time);
    }

    fcfs(p, n);
    sjf(p, n);

    return 0;
}

void fcfs(struct Process p[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].arrival_time > p[j + 1].arrival_time)
                swap(&p[j], &p[j + 1]);
        }
    }

    int current_time = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < p[i].arrival_time)
            current_time = p[i].arrival_time;
        p[i].response_time = p[i].waiting_time = current_time - p[i].arrival_time;
        current_time += p[i].burst_time;
        p[i].turnaround_time = current_time - p[i].arrival_time;
    }

    calculate_and_print(p, n, "First Come First Served (FCFS)");
}

void sjf(struct Process p[], int n) {
    struct Process temp[n];
    for (int i = 0; i < n; i++) temp[i] = p[i];

    int completed = 0, current_time = 0;
    int visited[n];
    for (int i = 0; i < n; i++) visited[i] = 0;

    struct Process result[n];

    while (completed < n) {
        int idx = -1, min_burst = 1e9;
        for (int i = 0; i < n; i++) {
            if (temp[i].arrival_time <= current_time && !visited[i]) {
                if (temp[i].burst_time < min_burst) {
                    min_burst = temp[i].burst_time;
                    idx = i;
                }
                else if (temp[i].burst_time == min_burst) {
                    if (temp[i].arrival_time < temp[idx].arrival_time)
                        idx = i;
                }
            }
        }

        if (idx != -1) {
            visited[idx] = 1;
            result[completed] = temp[idx];
            if (current_time < temp[idx].arrival_time)
                current_time = temp[idx].arrival_time;
            result[completed].response_time = result[completed].waiting_time =
                current_time - temp[idx].arrival_time;
            current_time += temp[idx].burst_time;
            result[completed].turnaround_time = current_time - temp[idx].arrival_time;
            completed++;
        } else {
            current_time++;
        }
    }

    calculate_and_print(result, n, "Shortest Job First (SJF)");
}

void calculate_and_print(struct Process p[], int n, const char *algo_name) {
    float avg_wt = 0, avg_tat = 0, avg_rt = 0;

    printf("\n=== %s ===\n", algo_name);
    printf("Gantt Chart: ");
    for (int i = 0; i < n; i++)
        printf("| P%d ", p[i].pid);
    printf("|\n");

    printf("PID\tAT\tBT\tWT\tTAT\tRT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].arrival_time, p[i].burst_time,
               p[i].waiting_time, p[i].turnaround_time, p[i].response_time);
        avg_wt += p[i].waiting_time;
        avg_tat += p[i].turnaround_time;
        avg_rt += p[i].response_time;
    }

    printf("\nAverage Waiting Time: %.2f", avg_wt / n);
    printf("\nAverage Turnaround Time: %.2f", avg_tat / n);
    printf("\nAverage Response Time: %.2f\n", avg_rt / n);
}

