typedef Graph struct graph;

//Mount Graph
Graph mountGraph();

void destroyGraph(Graph g);

void addVertex(Graph * g, int key);

void addEdge(Graph * g, int key);

void rmVertex(Graph * g, int key);

void rmEdge(Graph * g, int key);

List * getEdgeList(Graph * g); //Priority

List * getVertexList(Graph * g); //TODO: Method

int getWeightEdge(Graph * g, int keyVertex);  //TODO: Method

int getFromKeyEdge(Graph * g, int keyVertex);  //TODO: Method

int getToKeyEdge(Graph * g, int keyVertex);  //TODO: Method

/*
int key; //EXT
int weight; // EXT
Vertex * from; // EXT_ID
Vertex * to; // EXT_ID
*/


//TODO: List of vertex and edges with voids pointer


