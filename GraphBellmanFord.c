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
void relax(VertexBellmanFord * source, VertexBellmanFord * dest, int w);
VertexBellmanFord * findVertexBf(BellmanFord * bf, int key);
int bellmanFord(Graph * g, BellmanFord * bf, int keyFrom);
List * mountPath(BellmanFord * bf, int keySrc, int keyDest);
int weightInfinity (BellmanFord * bf);


/*
|	---=== Bellman Ford ===---    |
*/

// Public methods

List * minPath(Graph * g, int keyFrom, int keyTo) {
    //Imprimir o caminho
    BellmanFord * bf = initAlgorithms(g, keyFrom);
    
    if(bellmanFord(g, bf, keyFrom)){
        //Pega os anteriores a partir do destino
        return mountPath(bf,keyFrom,keyTo);
    }
    return NULL;
}

List * weightVertex(Graph * g)  {
    List * vertexesList = getVertexList(g);
    int i = 0, len = length(vertexesList);
    List * result = list();
    for (i = 0; i < len; i++) {
        int * key = (int *) malloc(sizeof(int));
        *key = getValueInt(vertexesList, i);

        BellmanFord * bf = initAlgorithms(g, *key);
        if(bellmanFord(g, bf, *key)){
            if(weightInfinity(bf)){
                listAdd(result, key);
            }
        }
    }
        
    return result;
}

// Private methods

int weightInfinity (BellmanFord * bf) {
    int i = 0, len = length(bf->vertexes);
    for (i = 0; i < len; i++) {
        VertexBellmanFord * act = (VertexBellmanFord *) getValue(bf->vertexes, i);
        if(act->weight == INT_MAX) {
            return 0;
        }
    }
    return 1;
}

List * mountPath(BellmanFord * bf, int keySrc, int keyDest) {

    int keyAct = keyDest;
    List * path = list();

    int * v = (int *) malloc(sizeof(int));
    *v = keyDest;
    listAdd(path, v);

    while (keySrc != keyAct) {
        VertexBellmanFord * act = findVertexBf(bf,keyAct);
        int * v = (int *) malloc(sizeof(int));
        *v = act->keyPrev;
        listAdd(path, v);
        keyAct = act->keyPrev;
    }
    
    reverseList(path);

    return path;
}

int bellmanFord(Graph * g, BellmanFord * bf, int keyFrom) {
    List * vertexes = bf->vertexes;
    int i, lenVertex = length(vertexes);

    List * edges = bf->edges;
    int lenEdge = length(edges);

    for (i = 0; i < lenVertex; i++) { // Roda a quantidade de vertice
        //Roda a quantidade de arestas

        int e;
        for (e = 0; e < lenEdge; e++) {
            //VertexBellmanFord * vbf = findVertex(bf, );
            int edgeId = getValueInt(edges, e);
            int srcId = getVertexByFromKeyEdge(g, edgeId), destId = getVertexByToKeyEdge(g,edgeId);
            VertexBellmanFord * src = findVertexBf(bf,srcId);
            VertexBellmanFord * dest = findVertexBf(bf,destId);
            //Relaxar o programa
            relax(src,dest, getWeightEdge(g,edgeId));
        }
    }

    //Verificar se há ciclo
    for (i = 0; i < lenEdge; i++) {
        int edgeId = getValueInt(edges, i);
        int srcId = getVertexByFromKeyEdge(g, edgeId), destId = getVertexByToKeyEdge(g,edgeId);
        VertexBellmanFord * src = findVertexBf(bf,srcId);
        VertexBellmanFord * dest = findVertexBf(bf,destId);

        if(dest->weight > (src->weight == INT_MAX ? INT_MAX : src->weight + getWeightEdge(g,edgeId))) {
            printf("| %d |",dest->weight);
            return 0;
        }
    }
    return 1;

}

VertexBellmanFord * findVertexBf(BellmanFord * bf, int key){
    int len = length(bf->vertexes);
	int i;
	for (i = 0; i < len; i++) {
		VertexBellmanFord * act = (VertexBellmanFord *) getValue(bf->vertexes,i);
		if(act->keyVertex == key)
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
        listAdd(bf->vertexes, vertexBf);
    }

    List * edgesList = getEdgeList(g);
    len = length(edgesList);
    //Transfere os ids dos o struct
    for (i = 0; i < len; i++) {
        int * keyEdge = (int *) malloc(sizeof(int)); //TODO: Implementar na TAD LISTA
        *keyEdge = getValueInt(edgesList, i);
        listAdd(bf->edges, keyEdge);
    }

    return bf;
}

void relax(VertexBellmanFord * source, VertexBellmanFord * dest, int w) {
    if(dest->weight > (source->weight == INT_MAX ? INT_MAX : source->weight + w)) {
        dest->weight = source->weight + w;
        dest->keyPrev = source->keyVertex;
    }
}