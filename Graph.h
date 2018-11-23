#include "generics/List.h"

typedef struct graph Graph;

//Mount Graph
Graph * mountGraph();

void destroyGraph(Graph * g);

void addVertex(Graph * g, int key, void * item);

void addEdge(Graph * g, int key,int weight, int keyVFrom, int keyVTo);

void addDoubleEdge(Graph * g, int key1, int key2, int weight, int keyV1, int keyV2);

void rmVertex(Graph * g, int key);

void rmEdge(Graph * g, int key);

List * getEdgeList(Graph * g);

List * getVertexList(Graph * g);

int getWeightEdge(Graph * g, int keyEdge);

int getVertexByFromKeyEdge(Graph * g, int keyEdge);

int getVertexByToKeyEdge(Graph * g, int keyEdge);

void setWeightEdge(Graph * g, int keyEdge, int w);

void printGraph(Graph * g);

char * getName(Graph * g, int keyVertex);

void setName(Graph * g, int keyVertex, char * str);