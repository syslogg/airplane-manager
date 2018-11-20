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
	void * info; // EXT
};

struct graph {
	List * edges; //List Type: Edges
	List * vertexes; //List Type: Vertex
};

//Declaration
Vertex * findVertex(Graph *g, int key);
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
	return g->edges;
}

List * getVertexList(Graph * g){
	return g->vertexes;
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
	printf("Nao est� implementado");
}

void rmEdge(Graph * g, int key) {
	//TODO: Method
	printf("Nao está implementado");
}

//Private methods

Vertex * findVertex(Graph *g, int key) {
	int len = length(g->vertexes);
	int i;
	for (i = 0; i < len; i++) {
		Vertex * act = (Vertex *) getValue(g->vertexes,i);
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
