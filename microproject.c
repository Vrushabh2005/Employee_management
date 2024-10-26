#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for task node (binary tree)
typedef struct TaskNode {
    int taskId;
    char taskName[100];
    struct TaskNode* left;
    struct TaskNode* right;
} TaskNode;

// Structure for employee (linked list)
typedef struct Employee {
    int id;
    char name[100];
    TaskNode* taskRoot;  
    struct Employee* next;
} Employee;

// Function declarations
Employee* addEmployee(Employee* head, int id, const char* name);
Employee* updateEmployee(Employee* head, int id, const char* name);
Employee* deleteEmployee(Employee* head, int id);
Employee* searchEmployee(Employee* head, int id);
void displayEmployees(Employee* head);
TaskNode* assignTask(TaskNode* root, int taskId, const char* taskName);
void viewTasks(TaskNode* root);
void freeEmployeeList(Employee* head);
void freeTaskTree(TaskNode* root);

int main() {
    Employee* head = NULL;
    int choice, id, taskId;
    char name[100], taskName[100];

    while (1) {
        printf("\nEmployee Management System\n");
        printf("1. Add Employee\n");
        printf("2. Update Employee\n");
        printf("3. Delete Employee\n");
        printf("4. Search Employee\n");
        printf("5. Display Employees\n");
        printf("6. Assign Task\n");
        printf("7. View Tasks for an Employee\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter Employee ID: ");
                scanf("%d", &id);
                // Check if the employee exists first
                Employee* existingEmp = searchEmployee(head, id);
                if (existingEmp) {
                    printf("Error: Employee with ID %d already exists. Name: %s\n", existingEmp->id, existingEmp->name);
                } else {
                    printf("Enter Employee Name: ");
                    scanf("%s", name);
                    head = addEmployee(head, id, name);
                    printf("Employee added successfully!\n");
                }
                break;

            case 2:
                printf("Enter Employee ID to update: ");
                scanf("%d", &id);
                printf("Enter new Employee Name: ");
                scanf("%s", name);
                head = updateEmployee(head, id, name);
                printf("Employee details updated.\n");
                break;

            case 3:
                printf("Enter Employee ID to delete: ");
                scanf("%d", &id);
                head = deleteEmployee(head, id);
                break;

            case 4:
                printf("Enter Employee ID to search: ");
                scanf("%d", &id);
                Employee* emp = searchEmployee(head, id);
                if (emp) {
                    printf("Employee found: ID: %d, Name: %s\n", emp->id, emp->name);
                } else {
                    printf("Employee not found.\n");
                }
                break;

            case 5:
                displayEmployees(head);
                break;

            case 6:
                printf("Enter Employee ID to assign task: ");
                scanf("%d", &id);
                emp = searchEmployee(head, id);
                if (emp) {
                    printf("Enter Task ID: ");
                    scanf("%d", &taskId);
                    printf("Enter Task Name: ");
                    scanf("%s", taskName);
                    emp->taskRoot = assignTask(emp->taskRoot, taskId, taskName);
                    printf("Task assigned successfully to Employee ID %d.\n", id);
                } else {
                    printf("Employee not found.\n");
                }
                break;

            case 7:
                printf("Enter Employee ID to view tasks: ");
                scanf("%d", &id);
                emp = searchEmployee(head, id);
                if (emp) {
                    printf("Tasks for Employee ID %d:\n", id);
                    viewTasks(emp->taskRoot);
                } else {
                    printf("Employee not found.\n");
                }
                break;

            case 8:
                freeEmployeeList(head);
                exit(0);

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

// Add a new employee to the linked list
Employee* addEmployee(Employee* head, int id, const char* name) {
    Employee* current = head;
    // Check if employee ID already exists
    while (current != NULL) {
        if (current->id == id) {
            printf("Error: Employee with ID %d already exists.\n", id);
            return head;  // Return unchanged list if duplicate ID is found
        }
        current = current->next;
    }

    Employee* newEmp = (Employee*)malloc(sizeof(Employee));
    newEmp->id = id;
    strcpy(newEmp->name, name);
    newEmp->taskRoot = NULL;
    newEmp->next = head;
    return newEmp;
}

// Update the name of an employee
Employee* updateEmployee(Employee* head, int id, const char* name) {
    Employee* current = head;
    while (current != NULL) {
        if (current->id == id) {
            strcpy(current->name, name);
            return head;
        }
        current = current->next;
    }
    printf("Employee not found.\n");
    return head;
}

// Delete an employee from the linked list
Employee* deleteEmployee(Employee* head, int id) {
    Employee* current = head;
    Employee* prev = NULL;

    while (current != NULL) {
        if (current->id == id) {
            if (prev == NULL) {
                head = current->next;
            } else {
                prev->next = current->next;
            }
            freeTaskTree(current->taskRoot);  // Free associated task tree
            free(current);
            return head;
        }
        prev = current;
        current = current->next;
    }
    printf("Employee not found.\n");
    return head;
}

// Search for an employee by ID
Employee* searchEmployee(Employee* head, int id) {
    Employee* current = head;
    while (current != NULL) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Display all employees
void displayEmployees(Employee* head) {
    Employee* current = head;
    if (current == NULL) {
        printf("No employees to display.\n");
        return;
    }
    while (current != NULL) {
        printf("ID: %d, Name: %s\n", current->id, current->name);
        current = current->next;
    }
}

// Assign a task to an employee's task tree
TaskNode* assignTask(TaskNode* root, int taskId, const char* taskName) {
    if (root == NULL) {
        TaskNode* newNode = (TaskNode*)malloc(sizeof(TaskNode));
        newNode->taskId = taskId;
        strcpy(newNode->taskName, taskName);
        newNode->left = newNode->right = NULL;
        return newNode;
    }
    if (taskId == root->taskId) {    
        printf("Task with ID %d already exists. Updating the task name to %s.\n", taskId, taskName);
        strcpy(root->taskName, taskName);  
    } else if (taskId < root->taskId) {
        root->left = assignTask(root->left, taskId, taskName);
    } else {
        root->right = assignTask(root->right, taskId, taskName);
    }
    return root;
}

// View tasks (in-order traversal of the task tree)
void viewTasks(TaskNode* root) {
    if (root == NULL) {
        return;
    }
    viewTasks(root->left);
    printf("Task ID: %d, Task Name: %s\n", root->taskId, root->taskName);
    viewTasks(root->right);
}

// Free the employee list and associated task trees
void freeEmployeeList(Employee* head) {
    Employee* current = head;
    while (current != NULL) {
        Employee* next = current->next;
        freeTaskTree(current->taskRoot);
        free(current);
        current = next;
    }
}

// Free the task tree recursively
void freeTaskTree(TaskNode* root) {
    if (root == NULL) {
        return;
    }
    freeTaskTree(root->left);
    freeTaskTree(root->right);
    free(root);
}
