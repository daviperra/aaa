#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define DIM_NOME_CARTE 14
#define DIM_NICK 49

typedef enum {MAZZO_PESCA, MAZZO_SCARTO, GIOC_MANO, GIOC_GIOCO} TipoMazzo;

typedef enum {ISTANTANEA = 1, ISTANTANEA_SPECIAL, ARMA, EFFETTO} TipoCarta;

typedef enum {CUORI, QUADRI, FIORI, PICCHE} Seme;

typedef struct
{
    char nome[DIM_NOME_CARTE+1];
    TipoCarta tipoC;
    int num;
    Seme seme;
} Carta;

typedef struct
{
    TipoMazzo tipoM;
    int numCarte;
    Carta *carte;
} Mazzo;

typedef enum {SCERIFFO, VICESCERIFFO, FUORILEGGE, RINNEGATO}Ruolo;

typedef struct giocatore
{
    char nome[DIM_NICK+1];
    Ruolo ruolo;
    int pallottole;
    Mazzo mano;
    Mazzo inGioco;
    int gittata;
} Giocatore;