#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credentials.txt"

struct Student {
    int roll;
    char name[50];
    float marks;
};

char currentRole[10];
char currentUser[50];

// FUNCTION DECLARATIONS
void mainMenu();
void adminMenu();
void staffMenu();
void guestMenu();

int loginSystem();
void addStudent();
void searchStudent();
void displayStudents();
void updateStudent();
void deleteStudent();

int main() {
    if (loginSystem()) {
        mainMenu();
    } else {
        printf("\nLogin Failed. Exiting...\n");
    }
    return 0;
}

// LOGIN SYSTEM
int loginSystem() {
    char username[20], password[20];
    char fileUser[20], filePass[20], fileRole[10];

    printf("======== Login Screen =========\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if (!fp) {
        printf("Error: credentials.txt not found!\n");
        return 0;
    }

    while (fscanf(fp, "%s %s %s", fileUser, filePass, fileRole) == 3) {
        if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) {
            strcpy(currentRole, fileRole);
            strcpy(currentUser, fileUser);
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// MAIN MENU
void mainMenu() {
    if (strcmp(currentRole, "ADMIN") == 0)
        adminMenu();
    else if (strcmp(currentRole, "STAFF") == 0)
        staffMenu();
    else
        guestMenu();
}

// ADMIN MENU
void adminMenu() {
    int choice;
    while (1) {
        printf("\n==== ADMIN MENU =====\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Logout\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: printf("Logging out...\n"); return;
            default: printf("Invalid choice\n");
        }
    }
}

// STAFF MENU (With UPDATE allowed)
void staffMenu() {
    int choice;
    while (1) {
        printf("\n==== STAFF MENU =====\n");
        printf("1. Display Students\n");
        printf("2. Search Student\n");
        printf("3. Update Student\n");
        printf("4. Logout\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: updateStudent(); break;
            case 4: printf("Logging out...\n"); return;
            default: printf("Invalid choice\n");
        }
    }
}

// GUEST MENU
void guestMenu() {
    int choice;
    while (1) {
        printf("\n==== GUEST MENU =====\n");
        printf("1. Display Students\n");
        printf("2. Search Student\n");
        printf("3. Logout\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: printf("Logging out...\n"); return;
            default: printf("Invalid choice\n");
        }
    }
}

// ADD STUDENT
void addStudent() {
    FILE *fp = fopen(STUDENT_FILE, "a");
    struct Student st;

    if (!fp) {
        printf("Error opening file\n");
        return;
    }

    printf("Enter Roll No: ");
    scanf("%d", &st.roll);

    printf("Enter Name: ");
    scanf("%s", st.name);

    printf("Enter Marks: ");
    scanf("%f", &st.marks);

    fprintf(fp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    fclose(fp);

    printf("Student Added Successfully!\n");
}

// DISPLAY STUDENTS
void displayStudents() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    struct Student st;

    if (!fp) {
        printf("No Student records found\n");
        return;
    }

    printf("\nRoll\tName\tMarks\n");
    printf("---------------------------\n");

    while (fscanf(fp, "%d %s %f", &st.roll, st.name, &st.marks) == 3) {
        printf("%d\t%s\t%.2f\n", st.roll, st.name, st.marks);
    }
    fclose(fp);
}

// SEARCH STUDENT
void searchStudent() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    struct Student st;
    int roll, found = 0;

    if (!fp) {
        printf("Error opening file\n");
        return;
    }

    printf("Enter Roll to search: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %s %f", &st.roll, st.name, &st.marks) == 3) {
        if (st.roll == roll) {
            printf("Record found\n");
            printf("Roll: %d\nName: %s\nMarks: %.2f\n", st.roll, st.name, st.marks);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Record not found\n");

    fclose(fp);
}

// UPDATE STUDENT
void updateStudent() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    struct Student st;
    int roll, found = 0;

    if (!fp || !temp) {
        printf("Error opening file\n");
        return;
    }

    printf("Enter Roll to update: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %s %f", &st.roll, st.name, &st.marks) == 3) {
        if (st.roll == roll) {
            printf("Enter new Name: ");
            scanf("%s", st.name);
            printf("Enter new Marks: ");
            scanf("%f", &st.marks);
            found = 1;
        }
        fprintf(temp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("Record Updated Successfully!\n");
    else
        printf("Record not found\n");
}

// DELETE STUDENT
void deleteStudent() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    struct Student st;
    int roll, found = 0;

    if (!fp || !temp) {
        printf("Error opening file\n");
        return;
    }

    printf("Enter Roll to delete: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %s %f", &st.roll, st.name, &st.marks) == 3) {
        if (st.roll == roll) {
            found = 1;
            continue;
        }
        fprintf(temp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("Record Deleted Successfully!\n");
    else
        printf("Record not found\n");
}
