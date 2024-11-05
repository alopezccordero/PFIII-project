#include <iostream>
#include <string>

using namespace std;

// Base class representing a general Task
class Task {
public:
    string taskName;
    int taskPriority;
    bool taskCompleted;

    Task() : taskName(""), taskPriority(0), taskCompleted(false) {}
    Task(string name, int priority) : taskName(name), taskPriority(priority), taskCompleted(false) {}

    virtual ~Task() {}
};

// Derived class to manage the to-do list with arrays and dynamic memory
class ToDoList {
private:
    Task** tasksArray;           // Pointer to array of Task pointers (dynamic memory)
    Task** completedArray;   // Pointer to array of completed tasks
    int taskCount;           // Current number of tasks
    int completedCount;      // Current number of completed tasks
    int taskSize;        // Current capacity of task array
    int completedSize;   // Current capacity of completed tasks array

public:
    ToDoList() : taskCount(0), completedCount(0), taskSize(10), completedSize(10) {
        tasksArray = new Task*[taskSize];              // Allocate memory for task array
        completedArray = new Task*[completedSize]; // Allocate memory for completed tasks array
    }

    ~ToDoList() {
        for (int i = 0; i < taskCount; ++i) {
            delete tasksArray[i];  // Free memory for each task
        }
        for (int i = 0; i < completedCount; ++i) {
            delete completedArray[i];  // Free memory for completed tasks
        }
        delete[] tasksArray;           // Free memory for task array
        delete[] completedArray;   // Free memory for completed tasks array
    }

    void addTask() {
        if (taskCount == taskSize) {
            resizeTaskArray();
        }

        string taskName;
        int taskPriority;
        
        cout << "Enter task name: ";
        cin.ignore();
        getline(cin, taskName);

        cout << "Enter task priority (lower number = higher priority): ";
        cin >> taskPriority;
        while (taskPriority <= 0) {
            cout << "Please enter a valid priority number (It must be greater than 0): ";
                cin >> taskPriority;
        }

        tasksArray[taskCount] = new Task(taskName, taskPriority); // Create new task dynamically
        ++taskCount;

        sortTasks(); // Sort tasks after adding a new one
    }

    void completeTask() {
        if (taskCount == 0) {
            cout << "No tasks to complete!" << endl;
            return;
        }

        displayTasks();

        int taskIndex;
        cout << "Enter the number of the task to mark as completed: ";
        cin >> taskIndex;

        if (taskIndex > 0 && taskIndex <= taskCount) {
            tasksArray[taskIndex - 1]->taskCompleted = true;
            moveToCompleted(taskIndex - 1);
        } else {
            cout << "Invalid task number!" << endl;
        }
    }

    void displayTasks() {
        if (taskCount == 0) {
            cout << "No tasks to display!" << endl;
            return;
        }

        cout << "\n--- Pending Tasks ---" << endl;
        for (int i = 0; i < taskCount; ++i) {
            cout << i + 1 << ". " << tasksArray[i]->taskName << " (Priority: " << tasksArray[i]->taskPriority << ")" << endl;
        }
    }

    void displayCompletedTasks() {
        if (completedCount == 0) {
            cout << "No completed tasks to display!" << endl;
            return;
        }

        cout << "\n--- Completed Tasks ---" << endl;
        for (int i = 0; i < completedCount; ++i) {
            cout << completedArray[i]->taskName << " (Priority: " << completedArray[i]->taskPriority << ")" << endl;
        }
    }

private:
    void resizeTaskArray() {
        // Dynamically resize the task array when capacity is reached
        taskSize *= 2;
        Task** newTasks = new Task*[taskSize];

        for (int i = 0; i < taskCount; ++i) {
            newTasks[i] = tasksArray[i]; // Copy existing tasks to new array
        }

        delete[] tasksArray;  // Free old memory
        tasksArray = newTasks; // Point to the new larger array
    }

    void resizeCompletedArray() {
        // Dynamically resize the completed tasks array
        completedSize *= 2;
        Task** newCompletedTasks = new Task*[completedSize];

        for (int i = 0; i < completedCount; ++i) {
            newCompletedTasks[i] = completedArray[i]; // Copy completed tasks
        }

        delete[] completedArray; // Free old memory
        completedArray = newCompletedTasks; // Point to new array
    }

    void moveToCompleted(int taskIndex) {
        // Move task to completedTasks array
        if (completedCount == completedSize) {
            resizeCompletedArray();
        }

        completedArray[completedCount] = tasksArray[taskIndex]; // Move pointer
        ++completedCount;

        // Shift remaining tasks to fill the gap
        for (int i = taskIndex; i < taskCount - 1; ++i) {
            tasksArray[i] = tasksArray[i + 1];
        }
        --taskCount;
    }

    void sortTasks() {
        // Simple selection sort to sort tasks based on priority (higher priority first)
        for (int i = 0; i < taskCount - 1; ++i) {
            int maxIndex = i;
            for (int j = i + 1; j < taskCount; ++j) {
                if (tasksArray[j]->taskPriority <= tasksArray[maxIndex]->taskPriority) {
                    maxIndex = j;
                }
            }
            swap(tasksArray[i], tasksArray[maxIndex]);
        }
    }
};

int main() {
    ToDoList todoList;
    int choice;

    do {
        cout << "\n--- To-Do List Menu ---" << endl;
        cout << "1. Add Task" << endl;
        cout << "2. Complete Task" << endl;
        cout << "3. View Pending Tasks" << endl;
        cout << "4. View Completed Tasks" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            todoList.addTask();
            break;
        case 2:
            todoList.completeTask();
            break;
        case 3:
            todoList.displayTasks();
            break;
        case 4:
            todoList.displayCompletedTasks();
            break;
        case 5:
            cout << "Exiting program..." << endl;
            break;
        default:
            cout << "Invalid choice, try again!" << endl;
        }
    } while (choice != 5);

    return 0;
}
