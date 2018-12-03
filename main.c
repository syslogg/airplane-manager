#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Graph.h"
#include "GraphMinPath.h"

#define BY_STOP 1
#define BY_TIME 2
#define BY_DISTANCE 3
#define BY_TIME_FLIGHT 4

#define bool int
#define false 0
#define true 1

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
	int distance;
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
void trim_both(char *title_p, char * title_tp);
Airport * getAirportByIcao(FlightMap * fm, char * icao);
void selectCost(FlightMap * fm, COST_TYPE c);
void bestCost(FlightMap * fm, char * departure, char * arrival);
void printPath(FlightMap * fm, List * path);
void console(FlightMap * fm, bool write);
void command(FlightMap * fm, char * cmd);
void chooses(FlightMap * fm, COST_TYPE * c, char * derp, char * arri);
void clear();
void airportsAllConnect(FlightMap * fm);

int main(int argc, char *argv[]) {

	FlightMap * fm = (FlightMap *) malloc(sizeof(struct flightMap));
	fm->g = mountGraph();
	fm->airports = list();
	fm->flights = list();
	fm->tempCost = -1;

	loadAirports(fm, "data/airports_t.txt");
	loadFlights(fm, "data/flights_t.txt");

	printf("----====- Gerenciamento de Voos e Aeroportos -====----\n");
	printf("\nUse o comando 'help' para ajuda\n");
	printf("Os aeroportos e rotas ja foram carregados!\n");
	console(fm,true);

	getchar();
	return 0;
} 

void console(FlightMap * fm, bool write) {
    while (write) {
        char input[500];
		printf(">");
		fflush(stdin);
		scanf("%[^\n]s",input);
		fflush(stdin);
		command(fm,input);
        printf("\n");
    }
}

void command(FlightMap * fm, char * cmd) {
    char params[10][100];
	COST_TYPE c = -1;

    if(sscanf(cmd,"bestcost %d %s %s", &c, params[0], params[1]) > 0) {
        clear();
		selectCost(fm,c);
		bestCost(fm,params[0], params[1]);
    } else if(!strcmp(cmd,"flight")) {
        clear();
		char derp[8], arri[8];
		chooses(fm, &c, derp, arri);
		char cmd[100];
		sprintf(cmd,"bestcost %d %s %s", c, derp, arri);
		command(fm, cmd);
    } else if (!strcmp(cmd,"cls")){
		clear();
	} else if (!strcmp(cmd,"print")){
		clear();
		printGraph(fm->g);
		getchar();
	} else if (!strcmp(cmd,"help")) {
		clear();
		printf("---------======- Ajuda - Sistema de Gerenciamento de Voos com Grafos -======---------\n");
		printf("- Comando: flight\n");
		printf("Seleção de menu para calculo do VOO\n\n");
		printf("- Comando: allpath\n");
		printf("Mostra todos os aeroportos que tem conexões com todos. ");
	} else if (!strcmp(cmd,"allpath")) {
		clear();
		airportsAllConnect(fm);
	} else {
		clear();
		printf("Esse comando não existe!\nUse o comando 'help' para ajuda.");
		getchar();
		clear();
	}
	getchar();
}

//TODO: Desenvolver um algoritmo para determinar se existe algum aeroporto a partir do qual é possível atingir todos os outros.
void airportsAllConnect(FlightMap * fm){
	List * listOfIds = weightVertex(fm->g);

	int i, len=length(listOfIds);
	printf("--== Aeroportos que é possivel atingir todos os outros ==--\n\n"); //Alterar pesos para algum valor acima de zero dos pesos
	for(i = 0; i < len; i++) {
		int keyV = getValueInt(listOfIds,i);
		printf("- %s\n", getName(fm->g, i));
	}
}

void chooses(FlightMap * fm, COST_TYPE * c, char * derp, char * arri) {
	int len = length(fm->airports), i;
	int input;
	printf("Escolha o aeroporto de origem:\n\n");
	for (i = 0; i < len;i++) {
		Airport * act = (Airport *) getValue(fm->airports, i);
		printf("%d. %s | %s - %s\n", i+1, act->icao, act->name, act->cityState);
	}

	printf("\n>");
	fflush(stdin);
	scanf("%d", &input);
	fflush(stdin);

	input -= 1;
	strcpy(derp,((Airport *)getValue(fm->airports,input))->icao);
	clear();

	printf("Escolha o aeroporto de destino:\n\n");
	for (i = 0; i < len;i++) {
		Airport * act = (Airport *) getValue(fm->airports, i);
		printf("%d. %s | %s - %s\n", i+1, act->icao, act->name, act->cityState);
	}
	printf("\n>");

	fflush(stdin);
	scanf("%d", &input);
	fflush(stdin);

	input -= 1;
	strcpy(arri, ((Airport *) getValue(fm->airports,input))->icao);
	clear();

	printf("Escolha o tipo de percurso:\n\n");
	printf("1. Por quantidade de paradas.\n");
	printf("2. Pela duração total.\n");
	printf("3. Por distância percorrida.\n");
	printf("4. Por tempo de voo.\n");

	printf("\n\n>");

	fflush(stdin);
	scanf("%d",&input);
	fflush(stdin);
	*c = input;
	clear();
}

void selectCost(FlightMap * fm, COST_TYPE c) {
	int i, len;
	fm->tempCost = c;

	switch(c) {
		case BY_STOP:
			len = length(fm->flights);
			for(i = 0; i < len; i++) {
				Flight * f = (Flight *) getValue(fm->flights,i);
				setWeightEdge(fm->g, f->id,f->stopover + 1);
			}
			break;
		case BY_TIME:
			len = length(fm->flights);
			for(i = 0; i < len; i++) {
				Flight * f = (Flight *) getValue(fm->flights,i);
				Airport * arriv = getAirportByIcao(fm, f->arrival);
				setWeightEdge(fm->g, f->id,f->time + arriv->wait);
			}
			break;
		case BY_DISTANCE:
			len = length(fm->flights);
			for(i = 0; i < len; i++) {
				Flight * f = (Flight *) getValue(fm->flights,i);
				setWeightEdge(fm->g, f->id,f->distance);
			}
			break;
		case BY_TIME_FLIGHT:
			len = length(fm->flights);
			for(i = 0; i < len; i++) {
				Flight * f = (Flight *) getValue(fm->flights,i);
				setWeightEdge(fm->g, f->id,f->time);
			}
			break;
	}
}

void bestCost(FlightMap * fm, char * departure, char * arrival) {
	Airport * derp = getAirportByIcao(fm, departure);
	Airport * arriv = getAirportByIcao(fm, arrival);

	if(fm->tempCost == BY_STOP){
		List * path = minPath(fm->g, derp->keyVertex, arriv->keyVertex);
		if(path != NULL) {
			int cost = calculateCost(fm->g,path) - 1;
			printPath(fm, path);
			printf("\nQuantidade de paradas do Voo: %d", cost);
		} else {
			printf("\nNao ha voos para chegar nesse destino.");
		}
	} else if(fm->tempCost == BY_TIME) {
		List * path = minPath(fm->g, derp->keyVertex, arriv->keyVertex);
		if(path != NULL) {
			int cost = calculateCost(fm->g,path) - arriv->wait;
			printPath(fm, path);
			printf("\nTempo total para chegar ao destino: %d", cost);
		} else {
			printf("\nNao ha voos para chegar nesse destino.");
		}
	} else if (fm->tempCost == BY_DISTANCE) {
		List * path = minPath(fm->g, derp->keyVertex, arriv->keyVertex);
		if(path != NULL) {
			int cost = calculateCost(fm->g,path);
			printf("\nDistancia Percorrida: %d", cost);
			printPath(fm,path);
		} else {
			printf("\nNao ha voos para chegar nesse destino.");
		}
	} else if(fm->tempCost == BY_TIME_FLIGHT) {
		List * path = minPath(fm->g, derp->keyVertex, arriv->keyVertex);
		if(path != NULL) {
			int cost = calculateCost(fm->g,path);
			printf("\nTempo de Voo: %d", cost);
			printPath(fm,path);
		} else {
			printf("\nNao ha voos para chegar nesse destino.");
		}
	}

}

void printPath(FlightMap * fm, List * path) {
	printf("\nPercurso: ");
	IterateList * il = iterableList(path);
	int * key;
	while ((key = (int *)nextIterable(il)) != NULL) {
		printf("%s ", getName(fm->g, *key));
	}
}

void loadAirports(FlightMap * fm, char * filename) {
	FILE * f = fopen(filename, "r");
	char buffer[1023], icao[8], state_uf[50],name[200];
	int timee, i = 0;

	fgets(buffer,sizeof(buffer),f); //Remove header

	while(fgets(buffer,sizeof(buffer),f) != NULL) {
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
	char buffer[1023], departure[64] = "", arrival[64] = "";
	int id, timee, stopover, distance;

	fgets(buffer,sizeof(buffer),f); //Remove header

	while(fgets(buffer,sizeof(buffer),f) != NULL) {
		sscanf(buffer, "| %d | %d | %d | %d | %[^|] | %[^|] |", &id, &timee, &distance, &stopover, departure, arrival);
		char departure_[64] = "",arrival_[64] = "";
		trim_both(departure, departure_);
		trim_both(arrival,arrival_);

		Flight * flight = (Flight *) malloc(sizeof(struct flight));

		flight->time = timee;
		flight->id = id;
		flight->distance = distance;
		flight->stopover = stopover;
		strcpy(flight->departure, departure_);
		strcpy(flight->arrival, arrival_);

		push(fm->flights, flight);
		addEdge(fm->g, id, -1, getAirportByIcao(fm, departure_)->keyVertex, getAirportByIcao(fm, arrival_)->keyVertex);
	}
}

Airport * getAirportByIcao(FlightMap * fm, char * icao) {
	if(fm != NULL){
		IterateList * il = iterableList(fm->airports);
		Airport * a;

		while ((a = (Airport *) nextIterable(il)) != NULL) {
			if(!strcmp(icao, a->icao)) {
				return a;
			}
		}

		destroyIterableList(il);
		return NULL;
	}
	return NULL;
}

void trim_both(char * title_p, char * title_tp) {
    int flag = 0;
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
    while(1) {
        title_tp--;
        if(!isspace((unsigned char) *title_tp) && flag == 0) {
            break;
        }
        flag = 0;
        *title_tp = '\0';
    }
}

void clear(){
    #ifdef WIN32
		system("CLS");
	#else
		system("clear");
	#endif
}