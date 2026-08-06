#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Process {
    int id;
    int arrival_time;    // t1
    int burst_time;      // t2
    int start_time;      // t3
    int completion_time; // t4
    int waiting_time;    // t5
    int turnaround_time; // t6
};

bool compareArrival(Process p1, Process p2) {
    return p1.arrival_time < p2.arrival_time;
}

bool compareID(Process p1, Process p2) {
    return p1.id < p2.id;
}

int main() {
    int n;
    cout << "--- FCFS Scheduling Algorithm ---\n";
    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> p(n);
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        cout << "Process " << p[i].id << " - Arrival time (t1): ";
        cin >> p[i].arrival_time;
        cout << "Process " << p[i].id << " - Burst time (t2): ";
        cin >> p[i].burst_time;
    }

    sort(p.begin(), p.end(), compareArrival);

    int current_time = 0;
    float total_waiting_time = 0;
    float total_turnaround_time = 0;

    vector<pair<int, int>> timeline;

    for (int i = 0; i < n; i++) {
        if (current_time < p[i].arrival_time) {
            timeline.push_back({ 0, p[i].arrival_time - current_time });
            current_time = p[i].arrival_time;
        }

        p[i].start_time = current_time;
        p[i].completion_time = current_time + p[i].burst_time;
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;

        total_waiting_time += p[i].waiting_time;
        total_turnaround_time += p[i].turnaround_time;

        timeline.push_back({ p[i].id, p[i].burst_time });
        current_time = p[i].completion_time;
    }

    sort(p.begin(), p.end(), compareID);

    cout << "\nResults table:\n";
    cout << "--------------------------------------------------------------------------------------------------\n";
    cout << "| No | Arrival (t1)   | Burst (t2)     | Start (t3)   | Completion (t4) | Waiting (t5)    | Turnaround (t6) |\n";
    cout << "--------------------------------------------------------------------------------------------------\n";
    for (int i = 0; i < n; i++) {
        cout << "| " << setw(2) << p[i].id
            << " | " << setw(14) << p[i].arrival_time
            << " | " << setw(14) << p[i].burst_time
            << " | " << setw(12) << p[i].start_time
            << " | " << setw(15) << p[i].completion_time
            << " | " << setw(15) << p[i].waiting_time
            << " | " << setw(15) << p[i].turnaround_time << " |\n";
    }
    cout << "--------------------------------------------------------------------------------------------------\n";

    cout << "\nAverage waiting time: " << total_waiting_time / n;
    cout << "\nAverage turnaround time: " << total_turnaround_time / n << "\n";

    cout << "\nGantt Chart:\n";
    int time_counter = timeline[0].first == 0 ? 0 : p[0].start_time;
    cout << time_counter;
    for (size_t i = 0; i < timeline.size(); i++) {
        if (timeline[i].first == 0) cout << " [Idle] ";
        else cout << " [ P" << timeline[i].first << " ] ";
        time_counter += timeline[i].second;
        cout << time_counter;
    }
    cout << "\n";

    return 0;
}