#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "generics/List.h"
#include "Graph.h"

//Data Structure
struct vertexBellmanFord {
    int keyVertex;
    int keyPrev;
    int weight;
};

struct bellmanFord {
    List * vertexes; //List of vertexBellmanFord
    List * edges;    // List of Key
};

typedef struct vertexBellmanFord VertexBellmanFord;
typedef struct bellmanFord BellmanFord;


//Declaracoes
BellmanFord * initAlgorithms(Graph * g,int keyVertexSource);
void relax(VertexBellmanFord * source, VertexBellmanFord * dest);
VertexBellmanFord * findVertex(BellmanFord * bf, int key);


/*
|	---=== Bellman Ford ===---    |
*/

// Public methods

void minPath(Graph * g, int keyFrom, int keyTo) {
    //Imprimir o caminho
    BellmanFord * bf = initAlgorithms(g, keyFrom);
    bellmanFord(g, bf, keyFrom, keyTo);
    printf("Deu certo!");
}

// Private methods

void bellmanFord(Graph * g, BellmanFord * bf, int keyFrom, int keyTo) {
    List * vertexes = getVertexList(bf->vertexes);
    int i, len = length(bf->vertexes);

    for (i = 0; i < len; i++) { // Roda a quantidade de vertice
        //Roda a quantidade de arestas
        List * edges = getEdgeList(bf->edges);
        int lenI = length(edges), e;

        for (e = 0; e < lenI; e++) {
            VertexBellmanFord * vbf = findVertex(bf, getValueInt(edges, e));
            //Relaxar o programa
            printf("TESTE");
        }

    }
}

VertexBellmanFord * findVertex(BellmanFord * bf, int key){
    int len = length(bf->vertexes);
	int i;
	for (i = 0; i < len; i++) {
		Vertex * act = (VertexBellmanFord *) getValue(bf->vertexes,i);
		if(act->key == key)
			return act;
	}
	return NULL;
}

BellmanFord * initAlgorithms(Graph * g, int keyVertexSource) {
    BellmanFord * bf = (BellmanFord *) malloc(sizeof(struct bellmanFord));
    
    bf->vertexes = list();
    bf->edges = list();
    
    List * vertexesList = getVertexList(g);
    int i, len = length(vertexesList);
    
    //Seta para infinito os nos e a origem seta para 0
    for (i = 0; i < len; i++) {
        int keyVertex = getValueInt(vertexesList, i);
        VertexBellmanFord * vertexBf = (VertexBellmanFord *) malloc(sizeof(struct vertexBellmanFord));

        vertexBf->keyVertex = keyVertex;
        vertexBf->keyPrev = -1;

        if(keyVertexSource == keyVertex) {
            vertexBf->weight = 0;
        } else {
            vertexBf->weight = INT_MAX;
        }
        listAdd(bf->vertexes, bf);
    }

    List * edgesList = getEdgeList(g);
    len = length(edgesList);
    //Transfere os ids dos o struct
    for (i = 0; i < len; i++) {
        int keyEdge = getValueInt(edgesList, i);
        listAdd(bf->edges, &keyEdge);
    }

    return bf;
}

void relax(VertexBellmanFord * source, VertexBellmanFord * dest) {

}

