// week5_task3_student_management_system.c
// Name: Hassan  Surname: Ahmad  Student ID: 241ADB154
//
// Task 3: Mini-project – Student management system with file persistence
// Week 5 – Files & Modular Programming
//
// Build: make lab5   Run: ./bin/week5_task3_student_management_system

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LEN 50
#define LINE_LEN 256
#define DATA_FILE "students.txt"

typedef struct Student {
  char name[NAME_LEN];
  int id;
  float gpa;
} Student;

// Function prototypes (do not change these signatures – the autograder calls
// them directly)
int load_students(Student arr[], int max);  // returns number of records loaded
int save_students(const Student arr[], int count);  // 0 on success, -1 on error
void add_student(Student arr[], int* count);
void list_students(const Student arr[], int count);
int read_line(const char* prompt, char* buf, size_t size);

int main(void) {
  Student students[MAX_STUDENTS];
  int count = 0;
  char input[LINE_LEN];
  int running = 1;

  count = load_students(students, MAX_STUDENTS);
  printf("Loaded %d student(s) from %s\n", count, DATA_FILE);

  while (running) {
    printf("\n=== Student Management System ===\n");
    printf("1. List students\n");
    printf("2. Add student\n");
    printf("3. Save and Exit\n");

    int choice;
    if (read_line("Select an option: ", input, sizeof input) != 0) {
      choice = 3;
    } else {
      char* end;
      long value = strtol(input, &end, 10);
      choice = (end != input && *end == '\0') ? (int)value : 0;
    }

    switch (choice) {
      case 1:
        list_students(students, count);
        break;
      case 2:
        add_student(students, &count);
        break;
      case 3:
        if (save_students(students, count) != 0) {
          perror(DATA_FILE);
        }
        printf("Data saved. Goodbye!\n");
        running = 0;
        break;
      default:
        printf("Invalid option. Try again.\n");
    }
  }

  return EXIT_SUCCESS;
}

int read_line(const char* prompt, char* buf, size_t size) {
  printf("%s", prompt);
  fflush(stdout);
  if (fgets(buf, (int)size, stdin) == NULL) {
    return -1;
  }
  size_t len = strcspn(buf, "\r\n");
  if (buf[len] == '\0' && len == size - 1) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
  }
  buf[len] = '\0';
  return 0;
}

int load_students(Student arr[], int max) {
  if (arr == NULL || max <= 0) {
    return 0;
  }

  FILE* fp = fopen(DATA_FILE, "r");
  if (fp == NULL) {
    return 0;
  }

  char line[LINE_LEN];
  int count = 0;

  while (count < max && fgets(line, sizeof(line), fp) != NULL) {
    Student temp = {0};
    if (sscanf(line, "%49[^,],%d,%f", temp.name, &temp.id, &temp.gpa) == 3) {
      if (temp.name[0] != '\0' && temp.name[0] != '\n' &&
          temp.name[0] != '\r') {
        arr[count] = temp;
        count++;
      }
    }
  }

  fclose(fp);
  return count;
}

int save_students(const Student arr[], int count) {
  if (arr == NULL || count < 0) {
    return -1;
  }

  FILE* fp = fopen(DATA_FILE, "w");
  if (fp == NULL) {
    return -1;
  }

  for (int i = 0; i < count; i++) {
    if (fprintf(fp, "%s,%d,%.2f\n", arr[i].name, arr[i].id, arr[i].gpa) < 0) {
      fclose(fp);
      return -1;
    }
  }

  fclose(fp);
  return 0;
}

void add_student(Student arr[], int* count) {
  if (arr == NULL || count == NULL) {
    return;
  }

  if (*count >= MAX_STUDENTS) {
    printf("Cannot add more students (limit reached).\n");
    return;
  }

  char name_buf[LINE_LEN];
  char id_buf[LINE_LEN];
  char gpa_buf[LINE_LEN];

  if (read_line("Enter name: ", name_buf, sizeof(name_buf)) != 0) {
    return;
  }
  if (read_line("Enter ID: ", id_buf, sizeof(id_buf)) != 0) {
    return;
  }
  if (read_line("Enter GPA: ", gpa_buf, sizeof(gpa_buf)) != 0) {
    return;
  }

  if (name_buf[0] == '\0' || strlen(name_buf) >= NAME_LEN ||
      strchr(name_buf, ',') != NULL) {
    printf("Invalid input. Student not added.\n");
    return;
  }

  char* end_id = NULL;
  long id_val = strtol(id_buf, &end_id, 10);
  if (id_buf[0] == '\0' || end_id == id_buf || *end_id != '\0') {
    printf("Invalid input. Student not added.\n");
    return;
  }

  char* end_gpa = NULL;
  float gpa_val = strtof(gpa_buf, &end_gpa);
  if (gpa_buf[0] == '\0' || end_gpa == gpa_buf || *end_gpa != '\0' ||
      gpa_val < 0.0f || gpa_val > 4.0f) {
    printf("Invalid input. Student not added.\n");
    return;
  }

  strcpy(arr[*count].name, name_buf);
  arr[*count].id = (int)id_val;
  arr[*count].gpa = gpa_val;
  (*count)++;

  printf("Student added successfully!\n");
}

void list_students(const Student arr[], int count) {
  if (arr == NULL || count <= 0) {
    printf("No students found.\n");
    return;
  }

  printf("%-5s %-20s %s\n", "ID", "Name", "GPA");
  for (int i = 0; i < count; i++) {
    printf("%-5d %-20s %.2f\n", arr[i].id, arr[i].name, arr[i].gpa);
  }
}