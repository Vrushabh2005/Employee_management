#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Task {
    int taskId;
    char taskName[100];
    struct Task* next;
} Task;

typedef struct Employee {
    int id;
    char name[100];
    Task* taskList;  
    struct Employee* left;
    struct Employee* right;
} Employee;

Employee* addEmployee(Employee* root, int id, const char* name);
Employee* searchEmployee(Employee* root, int id);
Task* addTask(Task* head, int taskId, const char* taskName);
void displayEmployees(Employee* root);
void displayTasks(Task* head);
void freeTaskList(Task* head);
void freeEmployeeTree(Employee* root);

int main() {
    Employee* root = NULL;
    int choice, id, taskId;
    char name[100], taskName[100];

    while (1) {
        printf("\nEmployee Management System\n");
        printf("1. Add Employee\n");
        printf("2. Assign Task to Employee\n");
        printf("3. View All Employees\n");
        printf("4. View Tasks for an Employee\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter Employee ID: ");
                scanf("%d", &id);
                printf("Enter Employee Name: ");
                scanf("%s", name);
                root = addEmployee(root, id, name);
                printf("Employee added successfully!\n");
                break;
            case 2:
                printf("Enter Employee ID to assign task: ");
                scanf("%d", &id);
                Employee* emp = searchEmployee(root, id);
                if (emp) {
                    printf("Enter Task ID: ");
                    scanf("%d", &taskId);
                    printf("Enter Task Name: ");
                    scanf("%s", taskName);
                    emp->taskList = addTask(emp->taskList, taskId, taskName);
                    printf("Task assigned successfully to Employee ID %d.\n", id);
                } else {
                    printf("Employee not found.\n");
                }
                break;
            case 3:
                displayEmployees(root);
                break;
            case 4:
                printf("Enter Employee ID to view tasks: ");
                scanf("%d", &id);
                emp = searchEmployee(root, id);
                if (emp) {
                    printf("Tasks for Employee ID %d:\n", id);
                    displayTasks(emp->taskList);
                } else {
                    printf("Employee not found.\n");
                }
                break;
            case 5:
                freeEmployeeTree(root);
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

Employee* addEmployee(Employee* root, int id, const char* name) {
    if (root == NULL) {
        Employee* newEmp = (Employee*)malloc(sizeof(Employee));
        newEmp->id = id;
        strcpy(newEmp->name, name);
        newEmp->taskList = NULL;  // Initialize task list to NULL
        newEmp->left = newEmp->right = NULL;
        return newEmp;
    }

    if (id < root->id) {
        root->left = addEmployee(root->left, id, name);
    } else if (id > root->id) {
        root->right = addEmployee(root->right, id, name);
    } else {
        printf("Employee with ID %d already exists.\n", id);
    }

    return root;
}

Employee* searchEmployee(Employee* root, int id) {
    if (root == NULL || root->id == id) {
        return root;
    }

    if (id < root->id) {
        return searchEmployee(root->left, id);
    } else {
        return searchEmployee(root->right, id);
    }
}

Task* addTask(Task* head, int taskId, const char* taskName) {
    Task* newTask = (Task*)malloc(sizeof(Task));
    newTask->taskId = taskId;
    strcpy(newTask->taskName, taskName);
    newTask->next = head;  
    return newTask;
}
void displayEmployees(Employee* root) {
    if (root == NULL) {
        return;
    }
    displayEmployees(root->left);
    printf("Employee ID: %d, Name: %s\n", root->id, root->name);
    displayEmployees(root->right);
}

void displayTasks(Task* head) {
    if (head == NULL) {
        printf("No tasks assigned.\n");
        return;
    }

    Task* current = head;
    while (current != NULL) {
        printf("Task ID: %d, Task Name: %s\n", current->taskId, current->taskName);
        current = current->next;
    }
}

void freeTaskList(Task* head) {
    Task* current = head;
    while (current != NULL) {
        Task* nextTask = current->next;
        free(current);
        current = nextTask;
    }
}

// Function to free the employee tree and all tasks for each employee
void freeEmployeeTree(Employee* root) {
    if (root == NULL) {
        return;
    }

    freeEmployeeTree(root->left);
    freeEmployeeTree(root->right);

    freeTaskList(root->taskList);  // Free tasks for the current employee
    free(root);
}
