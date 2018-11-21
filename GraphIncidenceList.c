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

void rmVertex(Graph * g, int key) {
	//TODO: Method
	printf("Nao está implementado");
}

void rmEdge(Graph * g, int key) {
	//TODO: Method
	printf("Nao está implementado");
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
		printf("Key: %d\n", v->key);
	}

	printf("\nEdges:\n\n");

	int lenn = length(g->edges);
	for (i = 0; i < lenn; i++){
		Edge * e = getValue(g->edges, i);
		printf("Key: %d | From: %d, To: %d \n", e->key, e->from->key, e->to->key);
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
	int len = length(g->vertexes);
	int i;
	for (i = 0; i < len; i++) {
		Vertex * act = (Vertex *) getValue(g->vertexes,i);
		if(act->key == key)
			return act;
	}
	return NULL;
}

Edge * findEdge(Graph *g, int key) {
	int len = length(g->edges);
	int i;
	for (i = 0; i < len; i++) {
		Edge * act = (Edge *) getValue(g->edges,i);
		if(act->key == key)
			return act;
	}
	return NULL;
}

void destroyEdge(Edge * e) {
	free(e);
}

void destroyVertex(Vertex * v) {
	free(v);
}
