#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <queue>

using namespace std;

struct Process {
    int id;
    int arrival_time;    // t1
    int burst_time;      // t2
    int start_time;      // t3
    int completion_time; // t4
    int waiting_time;    // t5
    int turnaround_time; // t6
    int remaining_time;
    bool is_started;
};

bool compareArrival(Process p1, Process p2) {
    return p1.arrival_time < p2.arrival_time;
}

bool compareID(Process p1, Process p2) {
    return p1.id < p2.id;
}

int main() {
    int n, quantum;
    cout << "--- RR (Round Robin) Scheduling Algorithm ---\n";
    cout << "Enter the number of processes: ";
    cin >> n;
    cout << "Enter the time quantum: ";
    cin >> quantum;

    vector<Process> p(n);
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        cout << "Process " << p[i].id << " - Arrival time (t1): ";
        cin >> p[i].arrival_time;
        cout << "Process " << p[i].id << " - Burst time (t2): ";
        cin >> p[i].burst_time;
        p[i].remaining_time = p[i].burst_time;
        p[i].is_started = false;
    }

    sort(p.begin(), p.end(), compareArrival);

    queue<int> ready_queue;                  // черга готових процесів
    vector<bool> in_queue(n, false);        // чи знаходиться процес у черзі
    vector<pair<int, int>> timeline;        // дані для діаграми Ганта

    int current_time = 0;                  // поточний час системи
    int completed = 0;                     // кількість завершених процесів
    int idx = 0;                           // індекс наступного процесу

    // Додаємо процеси, що надійшли на момент часу 0
    while (idx < n && p[idx].arrival_time <= current_time) {
        ready_queue.push(idx);
        in_queue[idx] = true;
        idx++;
    }

    // Головний цикл: поки не завершені всі процеси
    while (completed != n) {

        // Якщо черга порожня — процесор простоює
        if (ready_queue.empty()) {
            int next_arrival = p[idx].arrival_time; // час надходження наступного процесу

            timeline.push_back({ 0, next_arrival - current_time }); // простій CPU
            current_time = next_arrival;

            // Додаємо процеси, які вже надійшли
            while (idx < n && p[idx].arrival_time <= current_time) {
                ready_queue.push(idx);
                in_queue[idx] = true;
                idx++;
            }
        }

        // Беремо перший процес із черги
        int current_process = ready_queue.front();
        ready_queue.pop();
        in_queue[current_process] = false;

        // Якщо процес запускається вперше
        if (!p[current_process].is_started) {
            p[current_process].start_time = current_time;
            p[current_process].is_started = true;
        }

        // Виконуємо процес квант часу або залишок
        int execute_time = min(p[current_process].remaining_time, quantum);

        p[current_process].remaining_time -= execute_time; // зменшуємо залишок часу
        current_time += execute_time;                      // збільшуємо поточний час

        timeline.push_back({ p[current_process].id, execute_time }); // запис у діаграму

        // Додаємо нові процеси, що надійшли за цей час
        while (idx < n && p[idx].arrival_time <= current_time) {
            ready_queue.push(idx);
            in_queue[idx] = true;
            idx++;
        }

        // Якщо процес не завершений — повертаємо в кінець черги
        if (p[current_process].remaining_time > 0) {
            ready_queue.push(current_process);
            in_queue[current_process] = true;
        }
        else {
            // Якщо завершений — обчислюємо показники
            p[current_process].completion_time = current_time;

            p[current_process].turnaround_time =
                p[current_process].completion_time - p[current_process].arrival_time;

            p[current_process].waiting_time =
                p[current_process].turnaround_time - p[current_process].burst_time;

            completed++;
        }
    }

    // Обчислення сумарних значень
    float total_waiting_time = 0;
    float total_turnaround_time = 0;

    for (int i = 0; i < n; i++) {
        total_waiting_time += p[i].waiting_time;
        total_turnaround_time += p[i].turnaround_time;
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
    int time_counter = 0;
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