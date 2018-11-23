#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Graph.h"
#include "GraphMinPath.h"


struct airport {
	char icao[8];
	char cityState[50];
	char name[200];
	int wait;
};

struct flight {
	int id;
	int time;
	int stopover;
	char * departure;
	char * arrival;
};

struct flightMap {
	List * airports;
	List * flights;
};

typedef struct airport Airport;
typedef struct flight Flight;
typedef struct flightMap FlightMap;

void loadAirports();
void loadFlights();
void trim_both(char *title_p, char *title_tp);

int main(int argc, char *argv[]) {

	/*Graph * g = mountGraph();
	int v = 1;

	addVertex(g, 1, &v);
	addVertex(g, 2, &v);
	addVertex(g, 3, &v);
	addVertex(g, 4, &v);

	addEdge(g, 1, 1, 1, 2);
	addEdge(g, 2, 2, 2, 3);
	addEdge(g, 3, 2, 1, 3);
	addEdge(g, 4, 3, 3, 4);

	printGraph(g);

	List * path = minPath(g, 1, 3);

	printf("\n\n\nDeu certo!");*/
	FlightMap * fm = (FlightMap *) malloc(sizeof(struct flightMap));
	Graph * g = mountGraph();
	fm->airports = list();
	fm->flights = list();

	loadAirports(fm,g,"data/airports.txt");

	printGraph(g);

	getchar();
	return 0;
}

void loadAirports(FlightMap * fm, Graph * g, char * filename) {
	FILE * f = fopen(filename, "r");
	char buffer[1023], icao[8], state_uf[50],name[200];
	int timee, i = 0;

	fgets(buffer,sizeof(buffer),f); //Remove header

	while(fgets(buffer,sizeof(buffer),f) != NULL) {
		//| SBFZ | Fortaleza - CE      | Aeroporto Internacional Pinto Martins            | 0              |
		sscanf(buffer,"| %[^|] | %[^|] | %[^|] | %d", icao, state_uf, name, &timee);
		char icao_[8] = "", state_uf_[50] = "", name_[200] = ""; 
		trim_both(icao,icao_);
		trim_both(state_uf,state_uf_);
		trim_both(name,name_);

		Airport * airport = (Airport *) malloc(sizeof(struct airport));

		airport->wait = timee;
		strcpy(airport->icao, icao_);
		strcpy(airport->cityState, state_uf_);
		strcpy(airport->name, name_);

		push(fm->airports,airport);
		addVertex(g,i,airport);
		setName(g,i,airport->icao);

		i++;
	}
}

void loadFlights(char * filename) {
	FILE * file = open(filename,"r");
}

void trim_both(char *title_p, char *title_tp) {
    int flag = 0;

    // from left
    while(*title_p) {
        if(!isspace((unsigned char) *title_p) && flag == 0) {
            *title_tp++ = *title_p;
            flag = 1;
        }
        title_p++;
        if(flag == 1) {
            *title_tp++ = *title_p;
        }
    }

    // from right
    while(1) {
        title_tp--;
        if(!isspace((unsigned char) *title_tp) && flag == 0) {
            break;
        }
        flag = 0;
        *title_tp = '\0';
    }
}