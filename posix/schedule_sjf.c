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

// finds task with the shortest burst time
Task *pickNextTask() {
  
  // if list is empty, nothing to do
  if (!head)
    return NULL;

  struct node *temp;
  temp = head;
  struct task *best_sofar = temp->task; 

  while (temp != NULL) {
    if(best_sofar->burst > temp->task->burst){

      best_sofar = temp->task;

    }  // if two tasks with the same burst time will choose the one who comes first lexigraphically                               
    if (best_sofar->burst == temp->task->burst && comesBefore(temp->task->name, best_sofar->name)){ 
    
      best_sofar = temp->task;

    }

    temp = temp->next;

  }

  delete (&head, best_sofar);
  return best_sofar;

}


void schedule(){
  
  printf("\nShortest Job First:\n");  
  int time = 0; 

  while(head != NULL){

    struct task t = *pickNextTask();
    run(&t, t.burst);
    time += t.burst; 
    printf("Time is now: %d\n", time);

  }

}