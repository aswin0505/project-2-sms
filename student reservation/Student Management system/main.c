#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[50];
    int age;
    float grades;
} Student;

void addStudent(FILE *file);
void modifyStudent(FILE *file);
void deleteStudent(FILE *file);
void viewStudents(FILE *file);
void menu();

int main() {
    menu();
    return 0;
}

void menu() {
    int choice;
    FILE *file;

    do {
        printf("\nStudent Management System\n");
        printf("1. Add Student\n");
        printf("2. Modify Student\n");
        printf("3. Delete Student\n");
        printf("4. View Students\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        file = fopen("students.dat", "r+b");
        if (file == NULL && choice != 1) {
            printf("Error opening file!\n");
            continue;
        }

        switch (choice) {
            case 1: addStudent(file); break;
            case 2: modifyStudent(file); break;
            case 3: deleteStudent(file); break;
            case 4: viewStudents(file); break;
            case 5: fclose(file); break;
            default: printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 5);
}

void addStudent(FILE *file) {
    Student student;
    printf("Enter Student ID: ");
    scanf("%d", &student.id);
    printf("Enter Name: ");
    scanf("%s", student.name);
    printf("Enter Age: ");
    scanf("%d", &student.age);
    printf("Enter Grades: ");
    scanf("%f", &student.grades);

    fseek(file, 0, SEEK_END);
    fwrite(&student, sizeof(Student), 1, file);
    fclose(file);
    printf("Student added successfully!\n");
}

void modifyStudent(FILE *file) {
    Student student;
    int id, found = 0;

    printf("Enter Student ID to modify: ");
    scanf("%d", &id);

    while (fread(&student, sizeof(Student), 1, file)) {
        if (student.id == id) {
            found = 1;
            printf("Enter new Name: ");
            scanf("%s", student.name);
            printf("Enter new Age: ");
            scanf("%d", &student.age);
            printf("Enter new Grades: ");
            scanf("%f", &student.grades);
            fseek(file, -sizeof(Student), SEEK_CUR);
            fwrite(&student, sizeof(Student), 1, file);
            printf("Student modified successfully!\n");
            break;
        }
    }
    if (!found) {
        printf("Student ID not found!\n");
    }
    fclose(file);
}

void deleteStudent(FILE *file) {
    Student student;
    FILE *tempFile;
    int id, found = 0;

    printf("Enter Student ID to delete: ");
    scanf("%d", &id);

    tempFile = fopen("temp.dat", "w+b");
    while (fread(&student, sizeof(Student), 1, file)) {
        if (student.id != id) {
            fwrite(&student, sizeof(Student), 1, tempFile);
        } else {
            found = 1;
        }
    }
    fclose(file);
    fclose(tempFile);
    remove("students.dat");
    rename("temp.dat", "students.dat");

    if (found) {
        printf("Student deleted successfully!\n");
    } else {
        printf("Student ID not found!\n");
    }
}

void viewStudents(FILE *file) {
    Student student;

    printf("\nStudent Records:\n");
    while (fread(&student, sizeof(Student), 1, file)) {
        printf("ID: %d, Name: %s, Age: %d, Grades: %.2f\n", student.id, student.name, student.age, student.grades);
    }
    fclose(file);
}
