#include "task_list.h"

Task_List createTaskList() {
    Task_List list;

    // Attributes
    list.pHead = NULL;
    list.pTail = NULL;
    list.size  = 0;

    // Methods
    list.createTaskNode  = &createTaskNode;
    list.isTaskListEmpty = &isTaskListEmpty;
    list.insertTask      = &insertTask;
    list.removeTask      = &removeTask;
    list.length          = &length;

    return list;
}

Task_Node* createTaskNode(Task task) {
    Task_Node* node = (Task_Node*) malloc(sizeof(Task_Node));

    node->task = task;
    node->pNext = NULL;

    return node;
}

int isTaskListEmpty(Task_List* list) {
	if(list->pHead == NULL)
		return TRUE;
	return FALSE;
}

void insertTask(Task_List* list, Task task) {
	Task_Node* node = list->createTaskNode(task);

	if(list->pHead == NULL) {
		list->pHead = node;
		list->pTail = list->pHead;
	} else {
		list->pTail->pNext = node;
		list->pTail = list->pTail->pNext;
	}

    list->size++;
}

Task removeTask(Task_List* list) {
	Task task;

	if(list->pHead == NULL) {
        printf("The list is empty.");
        exit(0);
    }

    task = list->pHead->task;

    if(list->pHead == list->pTail) { 
    	free(list->pHead);
    	list->pHead = NULL;
    	list->pTail = NULL;
    } else {
    	Task_Node* temp = list->pHead;
    	list->pHead = list->pHead->pNext;

        free(temp);
        temp = NULL;
    }

    list->size--;
    return task;
}

int length(Task_List* list) {
    if(list->pHead == NULL) {
        return 0;
    }

    Task_Node* temp = list->pHead;
    unsigned size = 0;

    while(temp != NULL) {
        temp = temp->pNext;
        size++;
    }

    return size;
}

void printTask(Task task) {
    printf("a: %f\n",      task.local_a);
    printf("b: %f\n",      task.local_b);
    printf("fa: %f\n",     task.local_fa);
    printf("fb: %f\n",     task.local_fb);
    printf("area: %f\n\n", task.local_area);
}