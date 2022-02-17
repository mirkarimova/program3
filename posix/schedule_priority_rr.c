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

// adds task next to task furthest down the list that has the same priority 
void addEnd(char *name, int priority, int burst){
    
    struct node *temp;
    temp = head;
    
    struct task *t = malloc(sizeof(struct task));
    t->name = name; 
    t->priority = priority;
    t->burst = burst; 
    

    while (temp != NULL) {
        if((temp->task->priority == t->priority) && (temp->next == NULL)){
            
            struct node *newNode = malloc(sizeof(struct node));
            struct node *temp2; 
            temp2 = temp;
            newNode->task = t;
            newNode->next = NULL;
            temp->next = newNode; 
            break; 
            
        }
        if((temp->task->priority == t->priority) && (temp->next->task->priority != t->priority)){
            
            struct node *newNode = malloc(sizeof(struct node));
            struct node *temp2; 
            temp2 = temp;
            newNode->task = t;
            newNode->next = temp2->next;
            temp->next = newNode; 
            break; 
            
        }

        temp = temp->next;

    }
    
}

// updates burst time on tasks after running that still have time left 
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

// checks to see if there are multiple tasks with the same priority
bool multiplePriorities(Task task){
  
  struct node *temp; 
  temp = head;

   while (temp != NULL) {
        if(temp->task->priority == task.priority && temp->task->name != task.name){
            return 1;
        }      
    temp = temp->next;
   } 

   return 0;
}

bool comesBefore(char *a, char *b) { return strcmp(a, b) < 0; }


// will pick next task at the head of the list 
Task *pickNextTask() {
 
  if (!head){
    return NULL;
  }

    struct node *temp;
    temp = head;
    struct task *best_sofar = temp->task; 

    delete (&head, best_sofar);
    return best_sofar;

}

// helper function to swap two nodes
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

// helper function to sort by priority
void findMax(struct node *head) {
  struct node *temp;
  temp = head;
  struct node *best_sofar = temp; 

  while (temp != NULL) {

    if(best_sofar->task->priority < temp->task->priority){

        best_sofar = temp;

    }      

    temp = temp->next;

   } 

    swap(head, best_sofar);
    
}


// helpfer function to sort lexigraphically tasks with same priorities in ilst 
void findMin(struct node *head) {

  struct node *temp;
  temp = head;
  struct node *best_sofar = temp; 

  while (temp != NULL) {

    if(best_sofar->task->priority == temp->task->priority && comesBefore(temp->task->name, best_sofar->task->name)){

        swap(temp, best_sofar);

    }      

    temp = temp->next;

   } 
}

void sortList(struct node *head){

  struct node *temp;
  temp = head;
  
  while (temp != NULL) {

    findMax(temp); // sorts list by priority
    temp = temp->next;

  }
  
  temp = head; 

  while(temp != NULL){

    findMin(temp); // then sorts lexigraphically with same priorities
    temp = temp->next;

  }
}

void schedule(){

    printf("\nPriority Round Robin:\n");
    int time = 0; 
    sortList(head);
    

    while(head != NULL){
        
        struct task t = *pickNextTask();

        // if multiple priorities will only run for 10 and then will be placed after last task with same priority in list
        if(multiplePriorities(t)){
            
            if(t.burst >= 10){
                run(&t, 10);
                time += 10;
                updateBurst(head, t);
                addEnd(t.name, t.priority, t.burst - 10);
            }
            else if(t.burst > 0) {
                run(&t, t.burst);
                time += t.burst; 
            }
            if(t.burst > 0){
                printf("Time is now: %d\n", time);
            }
        } // if the only task with that priority will run the whole time
        else{
            if(t.burst > 0){
            run(&t, t.burst);
            time += t.burst;
            printf("Time is now: %d\n", time);
            }
        }
    }
}