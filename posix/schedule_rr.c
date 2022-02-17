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
    // it adds to the head of the list
    if(head == NULL){

        head =  malloc(sizeof(struct node));
        head->task = t; 
        head->next = NULL; 

    }else{

        insert(&head, t);

    }

}

// adds tasks to the end of the list 
void addEnd(char *name, int priority, int burst){
    
    struct node *temp;
    temp = head;
    
    struct task *t = malloc(sizeof(struct task));
    t->name = name; 
    t->priority = priority;
    t->burst = burst; 
    

    while (temp != NULL) {
        if(temp->next == NULL){
            struct node *newNode = malloc(sizeof(struct node));
            newNode->task = t;
            newNode->next = NULL;
            temp->next = newNode; 
            break; 
        }
        temp = temp->next;
    }
    
}

// updates burst times for tasks that did not complete in 10 time quantum
void updateBurst(struct node *head, Task task) {
    struct node *temp;
    temp = head;

    while (temp != NULL) {

        if(strcmp(task.name, temp->task->name) == 0){

            temp->task->burst = temp->task->burst - 10; 
            break;

        }
        
        temp = temp->next;

    }
}


bool comesBefore(char *a, char *b) { return strcmp(a, b) < 0; }

// picks task from the head of the list 
Task *pickNextTask() {
  // if list is empty, nothing to do
  if (!head){

    return NULL;

  }
  
    struct node *temp;
    temp = head;
    struct task *best_sofar = temp->task; 

    delete (&head, best_sofar);
    return best_sofar;

}

// helper function that swaps two nodes to sort the list 
void swap(struct node *head, struct node *min){
  
   struct task temp; 
   temp.name = head->task->name; 
   temp.priority = head->task->priority; 
   temp.burst = head->task->burst; 
   
   head->task->name = min->task->name; 
   head->task->priority = min->task->priority;
   head->task->burst = min->task->burst; 

   min->task->name = temp.name; 
   min->task->priority = temp.priority;
   min->task->burst = temp.burst;
   
}

// helper function to help sort the list 
void findMin(struct node *head) {

  struct node *temp;
  temp = head;
  struct node *best_sofar = temp; 

  while (temp != NULL) {

    if (comesBefore(temp->task->name, best_sofar->task->name)){
      best_sofar = temp;
    }

    temp = temp->next;

  } 

    swap(head, best_sofar);

}

// will sort list lexigraphically
void sortList(struct node *head){
  
  struct node *temp;
  temp = head;

  while (temp != NULL) {
    findMin(temp);
    temp = temp->next;
  }

}

void schedule(){
    
    printf("\nRound Robin:\n");
    int time = 0; 
    sortList(head);

    while(head != NULL){

        struct task t = *pickNextTask();
        
        // if burst greater than 10 will need to update burst time and add to end of list so task can run again
        if(t.burst >= 10){

            run(&t, 10);
            time += 10;
            updateBurst(head, t);
            addEnd(t.name, t.priority, t.burst - 10);

        } // otherwise will not add to the end of the list 
        else if(t.burst > 0) {

            run(&t, t.burst);
            time += t.burst; 

        } 
        if(t.burst > 0){
            printf("Time is now: %d\n", time);
        }
    }
}