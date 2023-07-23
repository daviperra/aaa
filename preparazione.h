#include "strutture.h"

#define NUMERO_CARTE 80
#define S_CUORI "Cuori"
#define S_PICCHE "Picche"
#define S_FIORI "Fiori"
#define S_QUADRI "Quadri"
#define S_MARRONI "Marrone"
#define S_BLU "Blu"
#define DINAMITE "Dinamite"
#define GITTATA_COLT_45 1
#define NOP (-1)
#define PRIMO 0
#define BONUS 2
#define MEX_DIM 100

#define MIN_RUOLI 0
#define MAX_RUOLI 3
#define ID_SCERIFFO 0
#define HP_SCERIFFO 5
#define HP 4

int randRange(int min, int max);

void scambioCarte(Carta carta1, Carta carta2, Carta* scambio1, Carta* scambio2);

void letturaCarte (Mazzo* mazzoPesca);

void creaMazzoPesca(Mazzo* mazzoPesca);

void creaMazzoScarti(Mazzo* mazzoScarti);

void mescolaCarte(Mazzo* mazzo);

bool checkMazzoPesca(Mazzo* mazzoPesca);

void spostaCarta(Mazzo* mazzo1, int carta, Mazzo* mazzo2, Mazzo* mazzoScarti, char* nomeFile);

void cambioMazzo(Mazzo* mazzoScarti, Mazzo* mazzoPesca, char* nomeFile);

void stampaCarte(Mazzo* mazzo);

void continuo();

bool comparaNomi(char* nome1, char* nome2);

void creaGiocatori(Giocatore** party, int nGiocatori, char* nomeFile);

void generaRuoli(Giocatore* party, int nGiocatori, char* nomeFile);

void lifePoints(Giocatore* party, int nGiocatori, char* nomeFile);

void rivelaRuoli(Giocatore* party, int nGiocatori, char* nomeFile);

void distribuisciCarte(Giocatore* giocatori, int nGiocatori, Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile);

Giocatore* nuovoParty(int* nGiocatori, char* nomeFile);

void stampaNomi(Giocatore* party, int nGiocatori);

int trovaCartaMano(Giocatore* giocatore, char* nomeCarta);

int trovaCartaInGioco(Giocatore* giocatore, char* nomeCarta);

bool checkBonus(Giocatore* eliminato);

bool checkMalus(Giocatore* attaccante, Giocatore* eliminato);

void bonus(Giocatore* attaccante, Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile);

void malus(Giocatore* attaccante, Mazzo* mazzoScarti, char* nomeFile);

void eliminazione(Giocatore* party,Giocatore* attaccante, Giocatore* eliminato, int* nGiocatori,Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile);

void pulisciSchermo();

void salvaLog(char* messaggio, char* nomeFile);