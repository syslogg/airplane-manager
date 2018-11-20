#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#include "GraphMinPath.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {

	Graph * g = mountGraph();
	int v = 1;
	addVertex(g, 1, &v);
	addVertex(g, 2, &v);
	addVertex(g, 3, &v);
	addVertex(g, 4, &v);

	addEdge(g, 1, 3, 1, 2);
	addEdge(g, 2, 5, 2, 3);
	addEdge(g, 3, 4, 3, 4);
	addEdge(g, 4, 1, 4, 1);
	addEdge(g, 5, 2, 4, 2);

	printGraph(g);

	minPath(g, 1, 2);

	printf("\n\n\nDeu certo!");
	//system("pause");
	getchar();
	return 0;
}
