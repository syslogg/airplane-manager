#include <stdio.h>
#include <stdlib.h>

#include "List.h"

typedef struct list List;
typedef struct nodeList NodeList;
typedef struct iterateList IterateList;

struct nodeList {
    void * b;
    NodeList * next;
};

struct list {
    NodeList * first;
    NodeList * last;
    int len;
};

struct iterateList {
    NodeList * act;
    int index;
};

//Declaring
NodeList * createNode(void * item);

//Public
List * list() {
    List * l = (List *) malloc(sizeof(struct list));
    l->first = NULL;
    l->last = NULL;
    l->len = 0;

    return l;
}

//Public
void destroyList(List * l) {
    listClean(l);
    free(l);
}

//Public
IterateList * iterableList(List * l) {
    if(l != NULL) {
        IterateList * il = (IterateList *) malloc(sizeof(struct iterateList));
        il->act = l->first;
        il->index = 0;
        return il;
    }
    return NULL;
}

//Public
void * nextIterable(IterateList * il) {
    if (il != NULL && il->act != NULL) {
        void * r = il->act->b;
        il->index++;
        il->act = il->act->next;
        return r;
    }
    return NULL;
}

int getIndexIterate(IterateList * il) {
    return il->index;
}

//Public
void destroyIterableList(IterateList * il) {
    if (il != NULL) {
        free(il);
    }
}

//Public
void listClean(List * l) {
    NodeList * aux = l->first;
    while (aux != NULL) {
        NodeList * aux2 = aux->next;
        //free(aux->b);
        free(aux);
        aux = aux2;
    }
    l->first = NULL;
    l->last = NULL;
    l->len = 0;
}

//Public
void listAdd(List * l, void * item) {
    NodeList * newNode = createNode(item);
    
    if(l->first == NULL) {
        l->first = newNode;
    } else {
        l->last->next = newNode;
    }

    l->last = newNode;
    l->len++;
}

//Public
void listRemove(List * l, int i) {
    NodeList * aux = l->first;
    NodeList * prev = NULL;

    if(i >= l->len) {
        printf("Error");
        return;
    }

    int count = 0;
    for (count=0; i != count; count++) {
        prev = aux;
        aux = aux->next;
    }

    if(i == 0) {
        l->first = aux->next;
    } else if (i == l->len-1) {
        l->last = prev;
        prev->next = NULL;
    } else {
        prev->next = aux->next;
    }

    free(aux);
    
    l->len--;
}

//Public
void setItem(List * l, int i, void * item) {
    if (i >= l->len){
		printf("Error: Index passou do limite");
		return;	
	}

    int count = 0;

    if(l != NULL) {
        NodeList * aux = l->first;
        while (aux != NULL) {
		if(i == count) {
			aux->b = item;
		}
		count++;
		aux = aux->next;
	}

    }
}

//Public
void * getValue(List * l, int index) {
	if (index >= l->len){
		printf("Error: Index passou do limite");
		return NULL;	
	}
	
	NodeList * aux = l->first;
	int count = 0;
	while (aux != NULL) {
		if(index == count) {
			return aux->b;
		}
		count++;
		aux = aux->next;
	}

    return NULL;
}

//Public
int getValueInt(List * l, int index) {
	void * value = getValue(l,index);
	if(value != NULL) {
		return * (int *) value;
	}
	return -1;
}

//Public
char * getValueStr(List * l, int index) {
    void * value = getValue(l,index);
	if(value != NULL) {
		return (char *) value;
	}
	return "";
}

//Public
void push(List * l, void * item) {
    listAdd(l,item);
}

void reverseList(List * l) {
    if(l != NULL) {
        NodeList * aux = l->first;
        NodeList * prev = NULL;
        NodeList * next = NULL;

        while (aux != NULL){
            next = aux->next;
            aux->next = prev;
            prev = aux;
            aux = next;
        }

        NodeList * n = l->first;
        l->first = l->last;
        l->last = n;
    }
}

//Public
void * pop(List * l) {
    NodeList * aux = l->first;
    NodeList * prev = NULL;

    while (aux->next != NULL) {
        prev = aux;
        aux = aux->next;
    }
    void * b = aux->b;
    free(aux);
    l->last = prev;
    
    return b;
}

//Public
int length(List * l) {
    return l->len;
}

//Public
void * * getAllList(List * l) {
	void * * listArray = (void * *) malloc(l->len * sizeof(struct nodeList));
	//void * listArray[l->len];
	int i;
	NodeList * aux = l->first;
	for (i = 0; i < l->len; i++) {
		listArray[i] = aux->b;
		aux = aux->next;
	}
	return listArray;
}

//Private
NodeList * createNode(void * item) {
    NodeList * n = (NodeList *) malloc(sizeof(struct nodeList));
    n->next = NULL;
    n->b = item;
    return n;
}







