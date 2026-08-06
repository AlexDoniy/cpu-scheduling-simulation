#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <queue>

using namespace std;

struct Process {
    int id;
    int arrival_time;  // t1
    int burst_time;    // t2
    int start_time;    // t3
    int completion_time; // t4
    int waiting_time;  // t5
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
    cout << "--- Алгоритм RR (Round Robin) ---\n";
    cout << "Введіть кількість процесів: ";
    cin >> n;
    cout << "Введіть квант часу: ";
    cin >> quantum;

    vector<Process> p(n);
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        cout << "Процес " << p[i].id << " - Час надходження (t1): ";
        cin >> p[i].arrival_time;
        cout << "Процес " << p[i].id << " - Очікуваний час виконання (t2): ";
        cin >> p[i].burst_time;
        p[i].remaining_time = p[i].burst_time;
        p[i].is_started = false;
    }

    sort(p.begin(), p.end(), compareArrival);

    queue<int> ready_queue;
    vector<bool> in_queue(n, false);
    vector<pair<int, int>> timeline; 
    
    int current_time = 0;
    int completed = 0;
    int idx = 0; // Індекс процесу для перевірки прибуття

    // Додаємо процеси, що прибули в момент часу 0
    while (idx < n && p[idx].arrival_time <= current_time) {
        ready_queue.push(idx);
        in_queue[idx] = true;
        idx++;
    }

    while (completed != n) {
        if (ready_queue.empty()) {
            // Якщо черга порожня, процесор простоює до наступного процесу
            int next_arrival = p[idx].arrival_time;
            timeline.push_back({0, next_arrival - current_time});
            current_time = next_arrival;
            
            while (idx < n && p[idx].arrival_time <= current_time) {
                ready_queue.push(idx);
                in_queue[idx] = true;
                idx++;
            }
        }

        int current_process = ready_queue.front();
        ready_queue.pop();
        in_queue[current_process] = false;

        // Фіксуємо час першого запуску
        if (!p[current_process].is_started) {
            p[current_process].start_time = current_time;
            p[current_process].is_started = true;
        }

        int execute_time = min(p[current_process].remaining_time, quantum);
        p[current_process].remaining_time -= execute_time;
        current_time += execute_time;
        
        timeline.push_back({p[current_process].id, execute_time});

        // Перевіряємо, чи прибули нові процеси під час виконання поточного
        while (idx < n && p[idx].arrival_time <= current_time) {
            ready_queue.push(idx);
            in_queue[idx] = true;
            idx++;
        }

        // Якщо процес ще не завершився, повертаємо його в кінець черги
        if (p[current_process].remaining_time > 0) {
            ready_queue.push(current_process);
            in_queue[current_process] = true;
        } else {
            // Процес завершено
            p[current_process].completion_time = current_time;
            p[current_process].turnaround_time = p[current_process].completion_time - p[current_process].arrival_time;
            p[current_process].waiting_time = p[current_process].turnaround_time - p[current_process].burst_time;
            completed++;
        }
    }

    float total_waiting_time = 0;
    float total_turnaround_time = 0;
    for (int i = 0; i < n; i++) {
        total_waiting_time += p[i].waiting_time;
        total_turnaround_time += p[i].turnaround_time;
    }

    sort(p.begin(), p.end(), compareID);

    cout << "\nТаблиця результатів:\n";
    cout << "--------------------------------------------------------------------------------------------------\n";
    cout << "| No | Час надх. (t1) | Очік. час (t2) | Початок (t3) | Завершення (t4) | Очікування (t5) | Повний (t6) |\n";
    cout << "--------------------------------------------------------------------------------------------------\n";
    for (int i = 0; i < n; i++) {
        cout << "| " << setw(2) << p[i].id 
             << " | " << setw(14) << p[i].arrival_time 
             << " | " << setw(14) << p[i].burst_time 
             << " | " << setw(12) << p[i].start_time 
             << " | " << setw(15) << p[i].completion_time 
             << " | " << setw(15) << p[i].waiting_time 
             << " | " << setw(11) << p[i].turnaround_time << " |\n";
    }
    cout << "--------------------------------------------------------------------------------------------------\n";

    cout << "\nСередній час очікування: " << total_waiting_time / n;
    cout << "\nСередній повний час виконання: " << total_turnaround_time / n << "\n";

    cout << "\nВізуалізація виконання (Діаграма Ганта):\n";
    int time_counter = 0;
    cout << time_counter;
    for (size_t i = 0; i < timeline.size(); i++) {
        if (timeline[i].first == 0) cout << " [Простій] ";
        else cout << " [ P" << timeline[i].first << " ] ";
        time_counter += timeline[i].second;
        cout << time_counter;
    }
    cout << "\n";

    return 0;
}