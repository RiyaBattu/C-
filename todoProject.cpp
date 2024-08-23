#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

struct Task {
    int id;
    string description;
    bool completed;
    int priority;

    Task(int taskId, const string& desc, int prio) 
        : id(taskId), description(desc), completed(false), priority(prio) {}
};

class ToDoList {
private:
    vector<Task> tasks;
    int nextTaskId;

    // Helper function to find a task by its ID
    int findTaskIndexById(int id) const {
        for (size_t i = 0; i < tasks.size(); ++i) {
            if (tasks[i].id == id) {
                return i;
            }
        }
        return -1;
    }

public:
    ToDoList() : nextTaskId(1) {}

    void addTask(const string& taskDescription, int priority) {
        tasks.emplace_back(nextTaskId++, taskDescription, priority);
        cout << "Task added: " << taskDescription << " with priority " << priority << endl;
    }

    void addMultipleTasks(const vector<pair<string, int>>& taskDetails) {
        for (const auto& detail : taskDetails) {
            addTask(detail.first, detail.second);
        }
    }

    void markTaskAsCompleted(int taskId) {
        int index = findTaskIndexById(taskId);
        if (index != -1) {
            tasks[index].completed = true;
            cout << "Task marked as completed: " << tasks[index].description << endl;
        } else {
            cout << "Task ID not found." << endl;
        }
    }

    void editTask(int taskId, const string& newDescription, int newPriority) {
        int index = findTaskIndexById(taskId);
        if (index != -1) {
            tasks[index].description = newDescription;
            tasks[index].priority = newPriority;
            cout << "Task updated: " << newDescription << " with priority " << newPriority << endl;
        } else {
            cout << "Task ID not found." << endl;
        }
    }

    void viewTasks() const {
        if (tasks.empty()) {
            cout << "No tasks in the to-do list." << endl;
        } else {
            cout << "Current tasks:\n";
            cout << left << setw(5) << "ID" << setw(30) << "Description" 
                 << setw(10) << "Priority" << "Status\n";
            for (const auto& task : tasks) {
                cout << left << setw(5) << task.id 
                     << setw(30) << task.description 
                     << setw(10) << task.priority 
                     << (task.completed ? "Completed" : "Pending") << endl;
            }
        }
    }

    void saveToFile(const string& filename) const {
        ofstream outFile(filename);
        for (const auto& task : tasks) {
            outFile << task.id << ";" << task.description << ";" 
                    << task.priority << ";" << task.completed << endl;
        }
        outFile.close();
        cout << "Tasks saved to " << filename << endl;
    }

    void loadFromFile(const string& filename) {
        ifstream inFile(filename);
        if (!inFile.is_open()) {
            cout << "Error opening file: " << filename << endl;
            return;
        }

        tasks.clear();
        string line;
        while (getline(inFile, line)) {
            int id, priority;
            string description;
            bool completed;

            size_t pos = line.find(';');
            id = stoi(line.substr(0, pos));

            size_t nextPos = line.find(';', pos + 1);
            description = line.substr(pos + 1, nextPos - pos - 1);

            pos = nextPos;
            nextPos = line.find(';', pos + 1);
            priority = stoi(line.substr(pos + 1, nextPos - pos - 1));

            completed = stoi(line.substr(nextPos + 1));

            tasks.emplace_back(id, description, priority);
            tasks.back().completed = completed;
            nextTaskId = max(nextTaskId, id + 1);
        }

        inFile.close();
        cout << "Tasks loaded from " << filename << endl;
    }
};

int main() {
    ToDoList todoList;
    int choice;

    // Load existing tasks from file
    todoList.loadFromFile("tasks.txt");

    do {
        cout << "\nTo-Do List Application\n";
        cout << "1. Add Task\n";
        cout << "2. Add Multiple Tasks\n";
        cout << "3. Mark Task as Completed\n";
        cout << "4. Edit Task\n";
        cout << "5. View Tasks\n";
        cout << "6. Save Tasks to File\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();  // Ignore newline character left in the buffer

        switch (choice) {
        case 1: {
            string taskDescription;
            int priority;
            cout << "Enter task description: ";
            getline(cin, taskDescription);
            cout << "Enter task priority (1-10): ";
            cin >> priority;
            todoList.addTask(taskDescription, priority);
            break;
        }

        case 2: {
            int numTasks;
            cout << "How many tasks do you want to add? ";
            cin >> numTasks;
            cin.ignore(); // Ignore newline character left in the buffer

            vector<pair<string, int>> taskDetails(numTasks);
            for (int i = 0; i < numTasks; ++i) {
                cout << "Task " << i + 1 << " description: ";
                getline(cin, taskDetails[i].first);
                cout << "Task " << i + 1 << " priority (1-10): ";
                cin >> taskDetails[i].second;
                cin.ignore();
            }

            todoList.addMultipleTasks(taskDetails);
            break;
        }

        case 3: {
            int taskId;
            cout << "Enter task ID to mark as completed: ";
            cin >> taskId;
            todoList.markTaskAsCompleted(taskId);
            break;
        }

        case 4: {
            int taskId;
            string newDescription;
            int newPriority;
            cout << "Enter task ID to edit: ";
            cin >> taskId;
            cin.ignore();
            cout << "Enter new description: ";
            getline(cin, newDescription);
            cout << "Enter new priority (1-10): ";
            cin >> newPriority;
            todoList.editTask(taskId, newDescription, newPriority);
            break;
        }

        case 5:
            todoList.viewTasks();
            break;

        case 6:
            todoList.saveToFile("tasks.txt");
            break;

        case 7:
            todoList.saveToFile("tasks.txt");
            cout << "Exiting the application." << endl;
            break;

        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 7);

    return 0;
}
