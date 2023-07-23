#include "preparazione.h"

#define GITTATA_COLT_45 1
#define GITTATA_VOLCANIC 1
#define GITTATA_SCHOFIELD 2
#define GITTATA_REMINGTON 3
#define GITTATA_CARABINA 4
#define GITTATA_WINCHESTER 5
#define VOLCANIC "Volcanic"
#define SCHOFIELD "Schofield"
#define REMINGTON "Remington"
#define CARABINA "Carabina"
#define WINCHESTER "Winchester"

#define MOD_PRIGIONE 0
#define MOD_BARILE 1
#define MOD_DINAMITE 2
#define MIN_DINAMITE 2
#define MAX_DINAMITE 9
#define DANNO_DINAMITE 3

void armi(Giocatore* giocatore);

void estrazione(int modalita, int carta, Mazzo* mazzoPesca, Mazzo* mazzoScarti, Giocatore* party, int nGiocatori, Giocatore* giocatore);

void barile();