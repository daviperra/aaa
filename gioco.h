#include "carte.h"

#define MIN_SCELTA 0
#define MAX_SCELTA 6
#define GIOCATA 0
#define CONTROLLO_MANO 1
#define CONTROLLO_INGIOCO 2
#define CONTROLLO_DISTANZE 3
#define CONTROLLO_ALTRI 4
#define SKIP 5
#define CLOSE 6

#define VITTORIA_RINNEGATO 0
#define VITTORIA_SCERIFFO 1
#define VITTORIA_FUORILEGGE 2
#define SAVE_DIM 20
#define LOG_DIM 20
#define NUOVA_PARTITA 0
#define CARICA_PARTITA 1
#define CHIUDI 2

void scartiEccesso(Giocatore* giocatore, Mazzo* mazzoScarti);

bool checkBang(Giocatore* giocatore, int bangGiocati);

bool checkCartaBlu(Giocatore* giocatore, int carta);

void giocata(Giocatore* party, int *nGiocatori, Giocatore* giocatore, int* bangGiocati, Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile);

void salvataggio(Giocatore* party, int* nGiocatori, Mazzo* mazzoPesca, Mazzo* mazzoScarti, int* prossimoGiocatore, int *nTurno, char* nomeFile);

Giocatore* caricaSalvataggio(Giocatore* party, int* nGiocatori, Mazzo* mazzoPesca, Mazzo* mazzoScarti, int* prossimoGiocatore, int* nTurno, char* nomeFile);

void chiudiGioco(Giocatore* party, int* nGiocatori, Mazzo* mazzoPesca, Mazzo* mazzoScarti, int* prossimoGiocatore, int *nTurno, char* nomeFile);

void turno(Giocatore* party, int* nGiocatori, Giocatore* giocatore, int* nTurno, Mazzo* mazzoPesca, Mazzo* mazzoScarti, int giocatoreAttuale, int* prossimoGiocatore, char* nomeFile);

int checkFine(Giocatore* party, int nGiocatori);

void finePartita(char* nomeFile, int esito);

void partita(Giocatore* party, int* nGiocatori, Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile, int prossimoGiocatore, int nTurno);

bool controlloStringa(char* stringa);

char* creaSalvataggio();

void nuovaPartita();

void caricaPartita();

void menu();