#include "preparazione.h"

#define DILIGENZA "Diligenza"
#define PANICO "Panico!"
#define BANG "Bang!"
#define WELLS "WellsFargo"
#define CATBALOU "CatBalou"
#define BIRRA "Birra"
#define MANCATO "Mancato"
#define SALOON "Saloon"
#define GATLING "Gatling"

#define INDIANI "Indiani"
#define EMPORIO "Emporio"
#define DUELLO "Duello"

#define VOLCANIC "Volcanic"
#define SCHOFIELD "Schofield"
#define REMINGTON "Remington"
#define CARABINA "Carabina"
#define WINCHESTER "Winchester"

#define MIRINO "Mirino"
#define MUSTANG "Mustang"
#define BARILE "Barile"
#define PRIGIONE "Prigione"

#define CARTE_WELLS 3
#define CARTE_DILIGENZA 2
#define MOD_PRIGIONE 0
#define MOD_BARILE 1
#define MOD_DINAMITE 2
#define MIN_DINAMITE 2
#define MAX_DINAMITE 9
#define DANNO_DINAMITE 3

#define GITTATA_COLT_45 1
#define GITTATA_VOLCANIC 1
#define GITTATA_SCHOFIELD 2
#define GITTATA_REMINGTON 3
#define GITTATA_CARABINA 4
#define GITTATA_WINCHESTER 5

int calcoloDistanza(Giocatore* party, Giocatore* attaccante, Giocatore* bersaglio, int nGiocatori);

void diligenza(Giocatore* player, Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile);

void wellsFargo(Giocatore* player, Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile);

void catBalou(Giocatore* party, Giocatore* attaccante, Mazzo* mazzoScarti, int nGiocatori, char* nomeFile);

void panico(Giocatore* party, Giocatore* attaccante, int nGiocatori, char* nomeFile);

void saloon(Giocatore* party, int nGiocatori, char* nomeFile);

void birra(Giocatore* giocatore, char* nomeFile);

bool birraExtremis(Giocatore* giocatore, char* nomeFile);

void mancato();

void danno(Giocatore* party, int *nGiocatori, Giocatore* attaccante, int bersaglio, Mazzo* mazzoScarti, Mazzo* mazzoPesca, char* nomeFile);

void bang(Giocatore* party, Giocatore* attaccante,Mazzo* mazzoScarti,Mazzo* mazzoPesca, int *nGiocatori, char* nomeFile);

void gatling(Giocatore* party, Giocatore* attaccante,Mazzo* mazzoScarti,Mazzo* mazzoPesca, int *nGiocatori, char* nomeFile);

void duello(Giocatore* party,int *nGiocatori, Giocatore* sfidante, Mazzo* mazzoScarti, Mazzo* mazzoPesca, char* nomeFile);

void indiani(Giocatore* party, int *nGiocatori, Giocatore* attaccante, Mazzo* mazzoScarti, Mazzo* mazzoPesca, char* nomeFile);

void emporio(Giocatore* party,int nGiocatori, Giocatore* attaccante, Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile);

bool estrazione(int modalita, Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile);

void armi(Giocatore* giocatore);

bool barile(Giocatore* giocatore, int carta, Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile);

bool dinamite(Giocatore *party, int *nGiocatori, Giocatore* giocatore, Mazzo* mazzoPesca, Mazzo* mazzoScarti, int carta, char* nomeFile);

bool prigione(Giocatore* giocatore, Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile);

void giocaCartaMarrone(Giocatore* party, int *nGiocatori, Giocatore* giocatore, int carta, Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile);

void giocaBarile(Giocatore* giocatore, int carta, char* nomeFile);

void giocaDinamite(Giocatore* giocatore, int carta, char* nomeFile);

void giocaPrigione(Giocatore* party, int nGiocatori, Giocatore* giocatore, int carta, char* nomeFile);

void giocaArma(Giocatore* giocatore, int carta, Mazzo* mazzoScarti, char* nomeFile);

void giocaCartaBlu(Giocatore* party, int nGiocatori, Giocatore* giocatore, int carta, Mazzo* mazzoScarti, char* nomeFile);