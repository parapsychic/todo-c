#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#define SIZE 1024

char home[SIZE];

void help(){
  printf("Todo [USAGE]\ntodo [..text]\t: Add a todo\ntodo -d\t\t: Delete a Todo \n");
}

char * getAbsolutePath(const char *filename){
  char * new_str ;
  if((new_str = malloc(strlen(home)+strlen(filename)+1)) != NULL){
      new_str[0] = '\0';  
      strcat(new_str,home);
      strcat(new_str,filename);
  } else {
      printf("ERROR ALLOCATING MEMORY\n");
  }

  return new_str;
}

int deleteTask(const char *id){
  const char * mainfilename = getAbsolutePath("/TODOLIST");
  const char * tempfilename = getAbsolutePath("/.temp");
  FILE * todo = fopen(mainfilename, "r");
  FILE * temp = fopen(tempfilename, "w");
  int lineToDelete;

  char line[SIZE];
  int num = 1;
  while (fgets(line , SIZE, todo) != NULL) {
    printf("%d %s", num, line);
    num++;
  }
  if(strcmp(id, "") == 0){

    printf("Enter a number to delete (non-numeric to cancel): ");
    scanf("%d", &lineToDelete);
  }
  else {
    lineToDelete = atoi(id);
  }

  if (lineToDelete > num) {
    printf("🙃 There are no accidents. But I can't delete something that does not exists!\n");
    return  -1;
  }

  rewind(todo);
  
  
  num = 1;
  while (fgets(line , SIZE, todo) != NULL) {
    if(num != lineToDelete) 
      fputs(line, temp);
    num++;
  }

  fclose(todo);
  fclose(temp);

  remove(mainfilename);
  rename(tempfilename, mainfilename);
  printf("Deleted task %d\n", lineToDelete);
  return 0;
}

int addTask(int count, char **text){

  FILE * todo = fopen(getAbsolutePath("/TODOLIST"), "a");

  fprintf(todo, "- ");
  for (int i = 1; i < count ; i++) {
    fprintf(todo, "%s ", text[i]);
  }
  fprintf(todo, "\n");
  fclose(todo);
  return 0;
}

int main(int argc, char ** argv){

 strcpy(home,  getenv("HOME"));

  if(argc < 2) {
    help();
    return -1;
  }

  if (argv[1][0] == '-') {
    if(argv[1][1] == 'd'){
      if(argc > 2)
        deleteTask(argv[2]);
      else
       deleteTask("");
      return 0;
    }
    help();
    return -1;
  }

  addTask(argc, argv);

  return 0;
}
