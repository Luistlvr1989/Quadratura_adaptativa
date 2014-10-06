#ifndef TASK_LIST_H
#define TASK_LIST_H

#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */

#define TRUE 1
#define FALSE 0

typedef struct Task {
	double local_a;
	double local_b;
	double local_fa;
	double local_fb;
	double local_area;
} Task;

typedef struct Task_Node {
	Task task;
	struct Task_Node* pNext;
} Task_Node;

typedef struct Task_List {
	Task_Node* pHead;
	Task_Node* pTail;
	unsigned size;

	Task_Node* (*createTaskNode) (Task task);
	int (*isTaskListEmpty) (struct Task_List*);
	void (*insertTask) (struct Task_List*, Task task);
	Task (*removeTask) (struct Task_List*);
	int (*length) (struct Task_List*);
} Task_List;

Task_List createTaskList();

Task_Node* createTaskNode(Task task);

int isTaskListEmpty(Task_List* list);

void insertTask(Task_List* list, Task task);

Task removeTask(Task_List* list);

int length(Task_List* list);

void printTask(Task task);

#endif // TASK_LIST_H