#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "sched_algos.h"

void copyTwo(struct process ** copy, struct process ** NP){

  copy[0]->id = NP[0]->id;
  copy[0]->duration = NP[0]->duration;
  copy[0]->priority = NP[0]->priority;
  copy[1]->id = NP[1]->id;
  copy[1]->duration = NP[1]->duration;
  copy[1]->priority = NP[1]->priority;

}

void copyProcesses(struct process ** processes, struct process ** copy, int arrSize){

  for(int i = 0; i < arrSize; i++){

    copy[i] = malloc(sizeof(struct process));
    copy[i]->id = processes[i]->id;
    copy[i]->duration = processes[i]->duration;
    copy[i]->priority = processes[i]->priority;

  }

}

void newProcesses(struct process ** processArr, int arrSize){

    int currIdx = 0;
    while(currIdx < arrSize) {
      processArr[currIdx] = malloc(sizeof(struct process));
      processArr[currIdx]->id = 11 + currIdx;
      processArr[currIdx]->duration = rand() % 5 + 1;
      processArr[currIdx]->priority = rand() % arrSize + 1;
      currIdx++;
    }

}

void fillProcessArr(struct process ** processArr, int arrSize) { //Helper function to fill our array with randomly generated processes.
  int currIdx = 0;
  while(currIdx < arrSize) {
    processArr[currIdx] = malloc(sizeof(struct process));
    processArr[currIdx]->id = currIdx + 1;
    processArr[currIdx]->duration = rand() % 5 + 1;
    processArr[currIdx]->priority = rand() % arrSize + 1;
    currIdx++;
  }
}

void outputProcess(struct process *process) { //Helper function to output a single process.
    printf("Process ID: %i, Duration: %i, Priority: %i \n", process->id, process->duration, process->priority);
}

void outputProcesses(struct process ** processes, int arrSize) {
  int idx = 0;
  while(idx < arrSize) { 
    outputProcess(processes[idx]);
    idx++;
  }
}

void swap(struct process ** p1, int one, int two){

  struct process ** p = malloc(sizeof(struct process *) * 1);
  p[1] = p1[one];
  p1[one] = p1[two];
  p1[two] = p[1];


}

int partition(struct process ** p, int low, int high){

  int pivot = p[high]->duration;

  int i = low - 1;

  for(int j = low; j <= high -1; j++){

    if(p[j]->duration<=pivot){
      i++;
      swap(p, i, j);
    }
  }

  swap(p, i+1, high);

  return(i+1);

}

void quickSort(struct process ** p, int low, int high){

  if(low < high){

    int pi = partition(p, low, high);

    quickSort(p, low, pi-1);
    quickSort(p, pi+1, high);     

  }

}


int main(int argc, char ** argv) {
  srand(time(NULL)); //Seed our random with the current time.
  const int ARR_SIZE = 10; //The size of our array of randomly generated processes.
  const int TIME_SLICE = 2; //The time slice for any preemptive scheduling algorithms.
  struct process ** processes = malloc(sizeof(struct process *) * ARR_SIZE); //Our process array, used in each of our scheduling algorithms.
  int ** priorities = malloc(sizeof(int *) * ARR_SIZE); //Our priorities array, used for priority scheduling. Index i of the priorities array corresponds to the same index of the processes array.

  fillProcessArr(processes, ARR_SIZE);
  //fillPrioritiesArr(priorities, ARR_SIZE); HINT: you will need to fill up the priorities array similarly to how the processes one is filled.
  
  printf("\nOutputting initial process array:\n");
  outputProcesses(processes, ARR_SIZE);
  printf("\n\n");
  
  //Each of the below function calls should NOT change the original processes array. For sorting, you must create a copy of the original array within the function.

  sched_fcfs(processes, ARR_SIZE);
  sched_spn(processes, ARR_SIZE);
  sched_priority(processes, ARR_SIZE, priorities);
  sched_round_robin(processes, ARR_SIZE, TIME_SLICE);
  sched_srt(processes, ARR_SIZE, TIME_SLICE);

  //Final heap cleanup should go here - deallocate all objects on the original array, including the space allocated for the array itself. This goes for the priorities array as well.
}

void sched_fcfs(struct process ** processes, int arrSize) {
  
  printf("First Come First Serve:\n");
  printf("--------------------------------------\n");

  struct process ** copy = malloc(sizeof(struct process *) * arrSize);
  copyProcesses(processes, copy, arrSize);

  outputProcesses(copy, arrSize);

  printf("\n\n");
}

bool check(int ids[], int checkingID, int arrSize){

    bool saw = true;

    for(int i = 0; i < arrSize; i++){
      if(ids[i] == checkingID)
        saw = false;

    }

    return saw;

}

void sched_round_robin(struct process ** processes, int arrSize, int timeSlice) {

  printf("Round Robin\n");
  printf("--------------------------------------\n\n");

  bool notDone = true;
  int i = 0, j = 0;
  int ids[10];

  struct process ** copy = malloc(sizeof(struct process *) * arrSize);
  copyProcesses(processes, copy, arrSize);
  struct process ** done = malloc(sizeof(struct process *) * arrSize);

  while(j!=10){

    for(int i = 0; i < arrSize; i ++){
      copy[i]->duration = copy[i]->duration - timeSlice;

      if(copy[i]->duration <= 0 && check(ids, copy[i]->id, j)){
        ids[j] = copy[i]->id;
        done[j] = malloc(sizeof(struct process));
        done[j]->id = copy[i]->id;
        done[j]->duration = 0;
        done[j]->priority = copy[i]->priority;
        j++;
      }

      if( i == arrSize-1){
        i == 0;
      }

    }

  }

  printf("Sorted order: \n");
  outputProcesses(done, j);


  printf("\n\n");
  
}

void sched_spn(struct process ** processes, int arrSize) {
  printf("Shortest Process Next:\n");
  printf("--------------------------------------\n");

  struct process ** spn = malloc(sizeof(struct process *) * arrSize);
  copyProcesses(processes, spn, arrSize);

  struct process ** NP = malloc(sizeof(struct process *) * 2);

  fillProcessArr(NP, 2);

 //Sorting first then outputting first two
  printf("\n");
  printf("Sorted, two processes completed: \n");
  quickSort(spn, 0, arrSize-1);
  outputProcesses(spn, arrSize);
  printf("\n\n");

}

void sched_srt(struct process ** processes, int arrSize, int timeSlice) {
  printf("Shortest Remaining Time:\n\n");
  printf("--------------------------------------\n");

  struct process ** spn = malloc(sizeof(struct process *) * arrSize);
  copyProcesses(processes, spn, arrSize);

  struct process ** NP = malloc(sizeof(struct process *) * 2);

  fillProcessArr(NP, 2);

 //Sorting first then outputting first two
  printf("\n");
  printf("Sorted, two processes completed: \n");
  quickSort(spn, 0, arrSize-1);
  outputProcesses(spn, 2);
  printf("\n\n");

  //Making two new processes
  printf("Two new processes incoming: \n");
  newProcesses(NP, 2);
  outputProcesses(NP, 2);
  printf("\n");

  //copying NP processes to Copy
  copyTwo(spn, NP);

  //Sorting with the newly added processes from NP
  printf("Sorted again: \n");
  quickSort(spn, 0, arrSize-1);
  outputProcesses(spn, arrSize);

  printf("\n\n");
}

int priorityPartition(struct process ** p, int low, int high){

  int pivot = p[high]->priority;

  int i = low - 1;

  for(int j = low; j <= high -1; j++){

    if(p[j]->priority<=pivot){
      i++;
      swap(p, i, j);
    }
  }

  swap(p, i+1, high);

  return(i+1);

}

void priorityQuickSort(struct process ** p, int low, int high){

  if(low < high){

    int pi = priorityPartition(p, low, high);

    priorityQuickSort(p, low, pi-1);
    priorityQuickSort(p, pi+1, high);     

  }

}


void sched_priority(struct process ** processes, int arrSize, int **priorities) {
  printf("Priority:\n");
  printf("--------------------------------------\n");

  struct process ** copy = malloc(sizeof(struct process *) * arrSize);
  copyProcesses(processes, copy, arrSize);
  struct process ** NP = malloc(sizeof(struct process *) * 2);

  fillProcessArr(NP, 2);
  
  //Sorting first then outputting first two
  printf("\n");
  printf("Sorted, two processes completed: \n");
  priorityQuickSort(copy, 0, arrSize-1);
  outputProcesses(copy, 2);
  printf("\n\n");

  //Making two new processes
  printf("Two new processes incoming: \n");
  newProcesses(NP, 2);
  outputProcesses(NP, 2);
  printf("\n");

  //copying NP processes to Copy
  copyTwo(copy, NP);

  //Sorting with the newly added processes from NP
  printf("Sorted again: \n");
  priorityQuickSort(copy, 0, arrSize-1);
  outputProcesses(copy, arrSize);

  printf("\n\n");
}
