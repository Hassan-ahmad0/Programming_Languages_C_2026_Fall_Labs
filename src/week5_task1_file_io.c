// week5_task1_file_io.c
// Name: Hassan  Surname: Ahmad  Student ID: 241ADB154
//
// Task 1: Read and write data from text files
// Week 5 – Files & Modular Programming
//
// Build: make lab5   Run: ./bin/week5_task1_file_io

#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_FILENAME "data.txt"
#define LINE_LEN 256

int main(int argc, char* argv[]) {
  FILE* fp = NULL;
  const char* filename = DEFAULT_FILENAME;
  char line[LINE_LEN];
  int line_count = 0;

  // Optional filename from command-line arguments
  if (argc > 1) {
    filename = argv[1];
  }

  // 1 & 2. Open file for writing and check for NULL
  printf("Writing lines to %s...\n", filename);
  fp = fopen(filename, "w");
  if (fp == NULL) {
    perror(filename);
    return EXIT_FAILURE;
  }

  // 3. Write lines to the file
  fprintf(fp, "Hello, file I/O in C!\n");
  fprintf(fp, "This is another line.\n");

  // 4. Close the file
  fclose(fp);

  // 5. Open the file for reading and check for NULL
  fp = fopen(filename, "r");
  if (fp == NULL) {
    perror(filename);
    return EXIT_FAILURE;
  }

  // 6. Read contents line by line and print them
  printf("Reading contents:\n");
  while (fgets(line, sizeof(line), fp) != NULL) {
    printf("%s", line);
    line_count++;
  }

  // 7. Close the file
  fclose(fp);

  // Print total count of lines read
  printf("Lines read: %d\n", line_count);

  return EXIT_SUCCESS;
}