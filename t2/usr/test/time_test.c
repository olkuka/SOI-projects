#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <lib.h>

int getgroup(int pid) {
   message m;
   m.m1_i1 = pid;
   return _syscall(MM, GETGROUP, &m);
}

int setscheduler(int time) {
   message m;
   m.m1_i1 = time;
   return _syscall(MM, SETSCHEDULER, &m);
}

int main(int argc, char* argv[]) {
    int time, result;
    pid_t pid = getpid();
    int group = getgroup(pid);

    if(argc != 2) {
       printf("Brak argumentu\n");
       return 0;
    } 
    else {
       time = atoi(argv[1]);
       printf("Kwant czasu dla procesu z grupy A: %d\nOdpowiednio dla procesu z grupy B: %d\n", time, 100-time);
       result = setscheduler(time);
       if (result == 0)
          printf("Aktualny proces (pid: %d) nalezy do grupy %d.\nMozna zmienic czas\n", pid, group);
       else
          printf("Aktualny proces (pid: %d) nalezy do grupy %d.\nNie mozna zmienic czasu\n", pid, group);
 }   
}

