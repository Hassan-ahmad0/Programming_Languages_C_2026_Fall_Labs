// week5_task4_struct_binary_io.c   (BONUS)
// Name: Hassan  Surname: Ahmad  Student ID: 241ADB154
//
// Task 4: Save and load a struct in binary format with fwrite()/fread()
// Week 5 – Files & Modular Programming
//
// Build: make lab5   Run: ./bin/week5_task4_struct_binary_io

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50

typedef struct Student {
  char name[MAX_NAME_LEN];
  int age;
  float gpa;
} Student;

// Function prototypes (do not change these signatures – the autograder calls
// them directly)
int save_student_binary(const Student* s,
                        const char* filename);  // 0 on success, -1 on error
int load_student_binary(const char* filename,
                        Student* out);  // 0 on success, -1 on error
long file_size(const char* filename);   // bytes, -1 on error

int main(void) {
  Student s1 = {
      0};  // zero-initialised so unused name bytes are not garbage on disk
  strcpy(s1.name, "Alice");
  s1.age = 21;
  s1.gpa = 3.75f;

  Student loaded = {0};
  const char* bin_file = "student.bin";
  const char* txt_file = "student.txt";

  printf("Saving student in binary format...\n");
  if (save_student_binary(&s1, bin_file) != 0) {
    perror(bin_file);
    return EXIT_FAILURE;
  }

  // Save the same record as text ("name age gpa") to txt_file for comparison
  FILE* txt_fp = fopen(txt_file, "w");
  if (txt_fp == NULL) {
    perror(txt_file);
    return EXIT_FAILURE;
  }
  if (fprintf(txt_fp, "%s %d %.2f\n", s1.name, s1.age, s1.gpa) < 0) {
    fclose(txt_fp);
    return EXIT_FAILURE;
  }
  fclose(txt_fp);

  printf("Loading student from binary file...\n");
  if (load_student_binary(bin_file, &loaded) != 0) {
    perror(bin_file);
    return EXIT_FAILURE;
  }

  printf("Loaded student: %s, %d, GPA %.2f\n", loaded.name, loaded.age,
         loaded.gpa);
  printf("Text file size: %ld bytes\n", file_size(txt_file));
  printf("Binary file size: %ld bytes\n", file_size(bin_file));

  return EXIT_SUCCESS;
}

// Open with "wb", fwrite(s, sizeof(Student), 1, fp) must return 1, close,
// return 0/-1
int save_student_binary(const Student* s, const char* filename) {
  if (s == NULL || filename == NULL) {
    return -1;
  }

  FILE* fp = fopen(filename, "wb");
  if (fp == NULL) {
    return -1;
  }

  size_t written = fwrite(s, sizeof(Student), 1, fp);
  fclose(fp);

  return (written == 1) ? 0 : -1;
}

// Open with "rb", fread(out, sizeof(Student), 1, fp) must return 1, close,
// return 0/-1
int load_student_binary(const char* filename, Student* out) {
  if (filename == NULL || out == NULL) {
    return -1;
  }

  FILE* fp = fopen(filename, "rb");
  if (fp == NULL) {
    return -1;
  }

  size_t read_count = fread(out, sizeof(Student), 1, fp);
  fclose(fp);

  return (read_count == 1) ? 0 : -1;
}

// Open with "rb", fseek(fp, 0, SEEK_END), size = ftell(fp), close, return size
// (-1 on error)
long file_size(const char* filename) {
  if (filename == NULL) {
    return -1;
  }

  FILE* fp = fopen(filename, "rb");
  if (fp == NULL) {
    return -1;
  }

  if (fseek(fp, 0, SEEK_END) != 0) {
    fclose(fp);
    return -1;
  }

  long size = ftell(fp);
  fclose(fp);

  return size;
}