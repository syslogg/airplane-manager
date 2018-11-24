#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Graph.h"
#include "GraphMinPath.h"

#define BY_STOP 1
#define BY_TIME 2
#define BY_DISTANCE 3
#define BY_TIME_FLIGHT 4

#define COST_TYPE int


struct airport {
	int keyVertex;
	char icao[8];
	char cityState[50];
	char name[200];
	int wait;
};

struct flight {
	int id;
	int time;
	int stopover;
	char departure[8];
	char arrival[8];
};

struct flightMap {
	List * airports;
	List * flights;
	Graph * g;
	COST_TYPE tempCost;
};

typedef struct airport Airport;
typedef struct flight Flight;
typedef struct flightMap FlightMap;

void loadAirports(FlightMap * fm, char * filename);
void loadFlights(FlightMap * fm, char * filename);
void trim_both(char *title_p, char *title_tp);
Airport * getAirportByIcao(FlightMap * fm, char * icao);
void selectCost(FlightMap * fm, COST_TYPE c);
void bestCost(FlightMap * fm, char * departure, char * arrival);

int main(int argc, char *argv[]) {

	FlightMap * fm = (FlightMap *) malloc(sizeof(struct flightMap));
	fm->g = mountGraph();
	fm->airports = list();
	fm->flights = list();
	fm->tempCost = -1;

	loadAirports(fm, "data/airports.txt");
	loadFlights(fm, "data/flights.txt");

	printGraph(fm->g);
	printf("\n\n");
	selectCost(fm,BY_TIME_FLIGHT);
	bestCost(fm, "SBGL", "SBFZ");

	getchar();
	return 0;
}

void selectCost(FlightMap * fm, COST_TYPE c) {
	switch(c) {
		case BY_STOP:
			break;
		case BY_TIME:
			break;
		case BY_DISTANCE:
			break;
		case BY_TIME_FLIGHT:
			fm->tempCost = c;
			int i, len = length(fm->flights);
			for(i = 0; i < len; i++) {
				Flight * f = (Flight *) getValue(fm->flights,i);
				setWeightEdge(fm->g, f->id,f->time);
			}
			break;
	}
}

void bestCost(FlightMap * fm, char * departure, char * arrival) {
	if(fm->tempCost == BY_TIME_FLIGHT) {
		Airport * derp = getAirportByIcao(fm, departure);
		Airport * arriv = getAirportByIcao(fm, arrival);

		/*INICIO REFACTORY*/
		List * listPath = minPath(fm->g, derp->keyVertex, arriv->keyVertex);
		int i, len = length(listPath);
		for (i = 0; i < len; i++) {
			int vKey = getValueInt(listPath,i);
			printf("%s ", getName(fm->g,vKey));
		}
		/*FIM REFACTORY*/
	}

}

void loadAirports(FlightMap * fm, char * filename) {
	FILE * f = fopen(filename, "r");
	char buffer[1023], icao[8], state_uf[50],name[200];
	int timee, i = 0;

	fgets(buffer,sizeof(buffer),f); //Remove header

	while(fgets(buffer,sizeof(buffer),f) != NULL) {
		//| SBFZ | Fortaleza - CE      | Aeroporto Internacional Pinto Martins            | 0              |
		sscanf(buffer, "| %[^|] | %[^|] | %[^|] | %d", icao, state_uf, name, &timee);
		char icao_[8] = "", state_uf_[50] = "", name_[200] = "";
		trim_both(icao,icao_);
		trim_both(state_uf,state_uf_);
		trim_both(name,name_);

		Airport * airport = (Airport *) malloc(sizeof(struct airport));
		airport->keyVertex = i;

		airport->wait = timee;
		strcpy(airport->icao, icao_);
		strcpy(airport->cityState, state_uf_);
		strcpy(airport->name, name_);

		push(fm->airports, airport);
		addVertex(fm->g, i, airport);
		setName(fm->g, i, airport->icao);

		i++;
	}
}

void loadFlights(FlightMap * fm, char * filename) {
	FILE * f = fopen(filename, "r");
	char buffer[1023], departure[8], arrival[8];
	int id, timee, stopover;

	fgets(buffer,sizeof(buffer),f); //Remove header

	while(fgets(buffer,sizeof(buffer),f) != NULL) {
		//| 1  | 4         | 2       | SBFZ    | SBGR    |
		sscanf(buffer, "| %d | %d | %d | %[^|] | %[^|]", &id, &timee, &stopover, departure, arrival);
		char departure_[8] = "",arrival_[8] = "";
		trim_both(departure,departure_);
		trim_both(arrival,arrival_);

		Flight * flight = (Flight *) malloc(sizeof(struct flight));

		flight->time = timee;
		flight->id = id;
		strcpy(flight->departure, departure_);
		strcpy(flight->arrival, arrival_);

		push(fm->flights, flight);
		addEdge(fm->g, id, -1, getAirportByIcao(fm, departure_)->keyVertex, getAirportByIcao(fm, arrival_)->keyVertex);
	}
}

Airport * getAirportByIcao(FlightMap * fm, char * icao) {
	if(fm != NULL){
		int i, len = length(fm->airports);
		for (i = 0; i < len; i++) {
			Airport * a = (Airport *) getValue(fm->airports,i);
			if(!strcmp(icao, a->icao)) {
				return a;
			}
		}
		return NULL;
	}
	return NULL;
}

void trim_both(char * title_p, char * title_tp) {
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