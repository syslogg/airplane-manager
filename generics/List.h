/* Criacao de estrutura de lista, Trabalho de Estrutura de Dados 2 */

/* setting */
typedef struct list List;
typedef struct iterateList IterateList;

List * list();

void destroyList(List * l);

IterateList * iterableList(List * l);

void destroyIterableList(IterateList * l);

void * nextIterable(IterateList * il);

int getIndexIterate(IterateList * il);

// TODO: Criar novo list add com numeros inteiros (definir tipos de listas para facilitar a implementacoes de outras atividades)
void listAdd(List * l, void * item);

void listRemove(List * l, int i);

void setItem(List * l, int i, void * item);

void listClean(List * l);

void push(List * l, void * item);
void * pop(List * l);

void * getValue(List * l, int index);

int getValueInt(List * l, int index);

char * getValueStr(List * l, int index);

int length(List * l);

void * * getAllList(List * l);

void reverseList(List * l);

int interateList(List * l);


//TODO:
/*
    Order function with comparator
    Create other list implementation:
        - Array List
        - Double Linked List
        - Tree List (With AVL)
    Other implementations:
        - HashMap
*/
