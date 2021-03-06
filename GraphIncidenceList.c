#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "generics/List.h"
#include "Graph.h"

typedef struct graph Graph;
typedef struct edge Edge;
typedef struct vertex Vertex;

/*
	--== Incidence List ==--
*/

struct edge {
	int key; //EXT
	int weight; // EXT
	Vertex * from; // EXT_ID
	Vertex * to; // EXT_ID
};

struct vertex {
	int key; // EXT
	char name[60]; // EXT
	void * info; // EXT
};

struct graph {
	List * edges; //List Type: Edges
	List * vertexes; //List Type: Vertex
};

//Declaration
Vertex * findVertex(Graph *g, int key);
Edge * findEdge(Graph *g, int key);
void destroyEdge(Edge * e);
void destroyVertex(Vertex * v);
Edge * findEdgeByVertexes(Graph *g, int src, int dest);

//Public methods

Graph * mountGraph() {
	Graph * g = (Graph *) malloc(sizeof(struct graph));
	g->edges = list(); //List Type: Edges
	g->vertexes = list(); //List Type: Vertex
	return g;
}

void destroyGraph(Graph * g) {
	int len = length(g->edges);
	int i;

	for (i = 0; i < len; i++) {
		destroyEdge(pop(g->edges));
	}

	len = length(g->vertexes);
	for (i = 0; i < len; i++) {
		destroyVertex(pop(g->vertexes));
	}

	destroyList(g->edges);
	destroyList(g->vertexes);

	free(g);
}

List * getEdgeList(Graph * g) {
	//Retornar as ids das arestas
	List * listIds = list();
	int i;
	int len = length(g->edges);

	for (i = 0; i < len; i++) {
		int * keyR = (int *) malloc(sizeof(int));
		*keyR = ((Vertex *) getValue(g->edges, i))->key;
		push(listIds, keyR);
	}
	return listIds;
}

List * getVertexList(Graph * g){
	//Retornar as ids dos vertices
	List * listIds = list();
	int i;
	int len = length(g->vertexes);

	for (i = 0; i < len; i++) {
		int * keyR = (int *) malloc(sizeof(int));
		*keyR = ((Vertex *) getValue(g->vertexes,i))->key;
		push(listIds, keyR);
	}
	return listIds;
}

void addVertex(Graph * g, int key, void * item) {
	if(findVertex(g,key) == NULL) { // If no exist that key
		Vertex * v = (Vertex *) malloc(sizeof(struct vertex));
		v->key = key;
		v->info = item;
		listAdd(g->vertexes,v);
	} else {
		printf("Já existe um vertice com esse ID");
	}

}

void addDoubleEdge(Graph * g, int key1, int key2, int weight, int keyV1, int keyV2) {
	addEdge(g,key1,weight,keyV1,keyV2);
	addEdge(g,key2,weight,keyV2,keyV1);
}

void addEdge(Graph * g, int key,int weight, int keyVFrom, int keyVTo) {
	//TODO: Create code to verify if there's duplicated key
	Edge * e = (Edge *) malloc(sizeof(struct edge));
	Vertex * from = findVertex(g, keyVFrom);
	Vertex * to = findVertex(g, keyVTo);

	e->key = key;
	e->weight = weight;
	e->from = from;
	e->to = to;

	listAdd(g->edges,e);
}

int calculateCost(Graph * g, List * path){
	int i, len = length(path), sum = 0;
	for (i = 0; i < len-1; i++) {
		int keyV = getValueInt(path,i);
		int keyVNext = getValueInt(path,i+1);
		Edge * e = findEdgeByVertexes(g, keyV, keyVNext);
		sum += e->weight;
	}

	return sum;
}

void rmVertex(Graph * g, int key) {
	IterateList * il = iterableList(g->edges);
	Edge * e;
	while ((e = nextIterable(il)) != NULL) {
		if(e->from->key == key || e->to->key == key) {
			listRemove(g->edges, getIndexIterate(il));
		}
	}
	destroyIterableList(il);

	IterateList * il_ = iterableList(g->vertexes);
	Vertex * v;
	while((e = nextIterable(il_)) != NULL) {
		if(v->key == key){
			listRemove(g->vertexes,getIndexIterate(il_));
		}
	}
	destroyIterableList(il_);
}

void rmEdge(Graph * g, int key) {
	IterateList * il = iterableList(g->edges);
	Edge * e;
	while ((e = nextIterable(il)) != NULL) {
		listRemove(g->edges,getIndexIterate(il));
	}
	destroyIterableList(il);
}

int getWeightEdge(Graph * g, int keyEdge){
	Edge * e = findEdge(g, keyEdge);
	return e->weight;
}

int getVertexByFromKeyEdge(Graph * g, int keyEdge) {
	Edge * e = findEdge(g, keyEdge);
	return e->from->key;
}

int getVertexByToKeyEdge(Graph * g, int keyEdge) {
	Edge * e = findEdge(g, keyEdge);
	return e->to->key;
}

void setWeightEdge(Graph * g, int keyEdge, int w) {
	Edge * e = findEdge(g, keyEdge);
	e->weight = w;
}

void printGraph(Graph * g) {
	int len = length(g->vertexes), i;
	printf("Print the Graph: \n\n");
	printf("Vertexes:\n");
	for (i = 0; i < len; i++) {
		Vertex * v = getValue(g->vertexes,i);
		printf("Name: %s\n", v->name);
	}

	printf("\nEdges:\n\n");
	int lenn = length(g->edges);
	for (i = 0; i < lenn; i++){
		Edge * e = getValue(g->edges, i);
		printf("Key: %d | Weight: %d | From: %s, To: %s \n", e->key, e->weight, e->from->name, e->to->name);
	}
}

char * getName(Graph * g, int keyVertex) {
	Vertex * v = findVertex(g,keyVertex);
	if(v != NULL) {
		return v->name;
	}
	printf("ERROR: Não existe a chave dessa vertice.");
	return NULL;
}

void setName(Graph * g, int keyVertex, char * str) {
	Vertex * v = findVertex(g,keyVertex);
	if(v != NULL) {
		strcpy(v->name, str);
	} else {
		printf("\n\nERROR: Não existe a chave dessa vertice.\n\n");
	}
}

//Private methods
Vertex * findVertex(Graph * g, int key) {
	IterateList * il = iterableList(g->vertexes);
	Vertex * v;
	while ((v = (Vertex *) nextIterable(il)) != NULL) {
		if(v->key == key) 
			return v;
	}
	destroyIterableList(il);
	/*int len = length(g->vertexes);
	int i;
	for (i = 0; i < len; i++) {
		Vertex * act = (Vertex *) getValue(g->vertexes,i);
		if(act->key == key)
			return act;
	}*/
	return NULL;
}

Edge * findEdge(Graph *g, int key) {
	IterateList * il = iterableList(g->edges);
	Edge * e;
	while ((e = (Edge *) nextIterable(il)) != NULL) {
		if(e->key == key)
			return e;
	}
	destroyIterableList(il);
	/*int len = length(g->edges);
	int i;
	for (i = 0; i < len; i++) {
		Edge * act = (Edge *) getValue(g->edges,i);
		if(act->key == key)
			return act;
	}*/
	return NULL;
}

Edge * findEdgeByVertexes(Graph *g, int src, int dest) {
	IterateList * il = iterableList(g->edges);
	Edge * e;
	while ((e = (Edge *) nextIterable(il)) != NULL) {
		if(e->from->key == src && e->to->key == dest)
			return e;
	}
	destroyIterableList(il);
	/*int len = length(g->edges);
	int i;
	for (i = 0; i < len; i++) {
		Edge * act = (Edge *) getValue(g->edges,i);
		
	}*/
	return NULL;
}

void destroyEdge(Edge * e) {
	free(e);
}

void destroyVertex(Vertex * v) {
	free(v);
}
