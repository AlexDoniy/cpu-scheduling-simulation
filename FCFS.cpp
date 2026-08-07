#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Process {
    int id;
    int arrival_time;  // t1
    int burst_time;    // t2
    int start_time;    // t3
    int completion_time; // t4
    int waiting_time;  // t5
    int turnaround_time; // t6
};

// Функція для сортування за часом надходження
bool compareArrival(Process p1, Process p2) {
    return p1.arrival_time < p2.arrival_time;
}

// Функція для сортування за ID (для фінального виводу)
bool compareID(Process p1, Process p2) {
    return p1.id < p2.id;
}

int main() {
    int n;
    cout << "--- Алгоритм FCFS ---\n";
    cout << "Введіть кількість процесів: ";
    cin >> n;

    vector<Process> p(n);
    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;
        cout << "Процес " << p[i].id << " - Час надходження (t1): ";
        cin >> p[i].arrival_time;
        cout << "Процес " << p[i].id << " - Очікуваний час виконання (t2): ";
        cin >> p[i].burst_time;
    }

    // Сортуємо процеси за часом надходження
    sort(p.begin(), p.end(), compareArrival);

    int current_time = 0;
    float total_waiting_time = 0;
    float total_turnaround_time = 0;
    
    vector<pair<int, int>> timeline; // Для візуалізації: {ID процесу, час виконання}

    for (int i = 0; i < n; i++) {
        // Якщо процесор простоює до прибуття наступного процесу
        if (current_time < p[i].arrival_time) {
            timeline.push_back({0, p[i].arrival_time - current_time}); // 0 означає простій
            current_time = p[i].arrival_time;
        }

        p[i].start_time = current_time;
        p[i].completion_time = current_time + p[i].burst_time;
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;

        total_waiting_time += p[i].waiting_time;
        total_turnaround_time += p[i].turnaround_time;
        
        timeline.push_back({p[i].id, p[i].burst_time});
        current_time = p[i].completion_time;
    }

    // Повертаємо оригінальний порядок для виводу таблиці
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
    int time_counter = timeline[0].first == 0 ? 0 : p[0].start_time;
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