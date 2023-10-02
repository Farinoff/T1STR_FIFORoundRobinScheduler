#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Task {
    char id;
    int computation_time;
    int arrival_time;
    int priority;
    int policy;
    int remaining_time;

    Task(char _id, int _computation_time, int _arrival_time, int _priority, int _policy) : id(_id), computation_time(_computation_time), arrival_time(_arrival_time), priority(_priority), policy(_policy), remaining_time(_computation_time) {}
};

bool compareArrivalTime(const Task& task1, const Task& task2) {
    return task1.arrival_time < task2.arrival_time;
}

bool comparePriority(const Task& task1, const Task& task2) {
    if (task1.priority < task2.priority) {
        return true;
    } else if (task1.priority > task2.priority) {
        return false;
    } else {
        // Se as prioridades forem iguais, compare os IDs em ordem alfabética
        return task1.id < task2.id;
    }
}

string schedulefiforr(vector<Task>& tasks){
    sort(tasks.begin(), tasks.end(), compareArrivalTime);
    string execution_order;
    int current_time = 0;
    char last_executed_id = '\0';
    char last_executed_rr = '\0';
    while (!tasks.empty()){
        sort(tasks.begin(), tasks.end(), compareArrivalTime);
        Task current_task = tasks.front();
         tasks.erase(tasks.begin());
        while (current_time < current_task.arrival_time){
            execution_order +='.';
            current_time ++;
        }

        //pega o mais prioritario e que esteja dentro do tempo
        sort(tasks.begin(), tasks.end(), comparePriority);
        for (auto it = tasks.begin(); it != tasks.end(); ++it) {
        const Task& tarefa = *it;
            if (current_task.id == last_executed_rr){
                if (tarefa.priority <= current_task.priority && tarefa.arrival_time <= current_time && tarefa.id != last_executed_rr) {
                    tasks.push_back(current_task);
                    current_task = tarefa;
                    tasks.erase(it);

                   break;
                } 
            } else if (tarefa.priority < current_task.priority && tarefa.arrival_time <= current_time && tarefa.id != last_executed_id && tarefa.policy == 1) {
                    tasks.push_back(current_task);
                    current_task = tarefa;
                    tasks.erase(it);
            } 
        }
        sort(tasks.begin(), tasks.end(), compareArrivalTime);
        //dentro da fifo preempcao
        if (current_task.policy == 1){ // FIFO
            for (int i = 0; i < current_task.computation_time; i++){
                for (auto it = tasks.begin(); it != tasks.end(); ++it) {
                const Task& tarefa = *it;
                    if (tarefa.priority < current_task.priority && tarefa.arrival_time <= current_time) {
                        tasks.push_back(current_task);
                        current_task = tarefa;
                        tasks.erase(it);
                        current_task.computation_time = current_task.remaining_time; // Defina o tempo de computação restante
                    break;
                    }
                }
        execution_order += current_task.id;
        last_executed_id = current_task.id;
        current_time++;
        current_task.remaining_time--;

        if (current_task.remaining_time <= 0) {
            break; // A tarefa foi concluída
            }
        }
        
        if (current_task.remaining_time > 0){
            tasks.push_back(current_task);  // Adicionar a tarefa de volta à fila
            sort(tasks.begin(), tasks.end(), compareArrivalTime);
           } 
        } 
        
        else if (current_task.policy == 2) { // Round Robin

            int quantum_time = 1;
            int execution_time = min(quantum_time, current_task.remaining_time);
            
            for (int i = 0; i < execution_time; i++) {
            execution_order += current_task.id;
            last_executed_id = current_task.id;
            last_executed_rr = current_task.id;
            current_time++;
            current_task.remaining_time--;

            }
            
            if (current_task.remaining_time > 0){
                tasks.push_back(current_task);  // Adicionar a tarefa de volta à fila
                sort(tasks.begin(), tasks.end(), compareArrivalTime);
            }    
        }
    }
    return execution_order;
}

//recebe tarefas
vector<Task> readTasks() {
    vector<Task> tasks;
    char current_id = 'A';

    int num_tasks;
    cin >> num_tasks;

    for (int i = 0; i < num_tasks; i++) {
        int computation_time, arrival_time, priority, policy;
        cin >> computation_time >> arrival_time >> priority >> policy;

        if (current_id <= 'Z') {
            Task task(current_id, computation_time, arrival_time, priority, policy);
            tasks.push_back(task);
            current_id++;
        } else {
            cout << "Reached maximum task count (26 tasks, A to Z).\n";
            break;
        }
    }

    return tasks;
}

int main() {
    while (true) {
        vector<Task> original_tasks = readTasks();
        if (original_tasks.empty()) {
            break; 
        }

        string schedule = schedulefiforr(original_tasks);
        cout << endl << "Execution Order: " << schedule << endl << endl;
    }
    return 0;
}