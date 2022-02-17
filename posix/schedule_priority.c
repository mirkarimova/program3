#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"

struct node *head = NULL;

// adds to the head of the list 
void add(char *name, int priority, int burst){

    struct task *t = malloc(sizeof(struct task));
    t->name = name; 
    t->priority = priority;
    t->burst = burst; 
    
    if(head == NULL){
      
      head =  malloc(sizeof(struct node));
      head->task = t; 
      head->next = NULL; 
        
    }else{

      insert(&head, t);

    }
    
}

bool comesBefore(char *a, char *b) { return strcmp(a, b) < 0; }

// picks task with the highest priority to run
Task *pickNextTask() {
  
  if (!head)
    return NULL;

  
  struct node *temp;
  temp = head;
  struct task *best_sofar = temp->task; 

  while (temp != NULL) {
    if(best_sofar->priority < temp->task->priority){

      best_sofar = temp->task;

    }  // if same priority then will choose the one that comes first lexigraphically
    if (best_sofar->priority == temp->task->priority && comesBefore(temp->task->name, best_sofar->name)){ 

      best_sofar = temp->task;

    }

    temp = temp->next;

  }
  
  delete (&head, best_sofar);
  return best_sofar;

}


void schedule(){
  
  printf("\nPriority:\n");  
  int time = 0; 

  while(head != NULL){
    struct task t = *pickNextTask();
    run(&t, t.burst);
    time += t.burst; 
    printf("Time is now: %d\n", time);
  }

}