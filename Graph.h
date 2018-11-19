#include "generics/List.h"

typedef struct graph Graph;

//Mount Graph
Graph * mountGraph();

void destroyGraph(Graph * g);

void addVertex(Graph * g, int key, void * item);

void addEdge(Graph * g, int key,int weight, int keyVFrom, int keyVTo);

void rmVertex(Graph * g, int key);

void rmEdge(Graph * g, int key);

List * getEdgeList(Graph * g);

List * getVertexList(Graph * g);

int getWeightEdge(Graph * g, int keyEdge);

int getVertexByFromKeyEdge(Graph * g, int keyEdge);

int getVertexByToKeyEdge(Graph * g, int keyEdge);

void setWeightEdge(Graph * g, int keyEdge, int w);

/*
int key; //EXT
int weight; // EXT
Vertex * from; // EXT_ID
Vertex * to; // EXT_ID
*/


//TODO: List of vertex and edges with voids pointer


