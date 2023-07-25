#include "preparazione.h"

/** La funzione randRange restituisce un numero intero casuale
* compreso tra min e max. Va chiamata la srand() nel main prima
* della sua prima invocazione.
*
* @param min il numero minimo
* @param max il numero massimo
* @return un numero intero casuale compreso tra min e max
**/

int randRange(int min, int max)
{
    int n = min + rand() % (max - min + 1);
    return n;
}

/** La procedura scambioCarte prende in ingresso due variabili struttura
 * Carta e due puntatori alla stessa struttura per realizzare uno scambio.
 *
 * @param carta1 la prima carta
 * @param carta2 la seconda carta
 * @param scambio1 puntatore alla prima carta
 * @param scambio2 puntatore alla seconda carta
**/


void scambioCarte(Carta carta1, Carta carta2, Carta* scambio1, Carta* scambio2)
{
    *scambio1 = carta2;
    *scambio2 = carta1;
}

/** La procedura letturaCarte si occupa della lettura del file di testo
 * "mazzo_bang.txt" e della memorizzazione dei dati su di esso nella struttura
 * Mazzo del mazzo di pesca
 * @param mazzoPesca puntatore alla struttura per il mazzo di pesca
**/

void letturaCarte (Mazzo* mazzoPesca)
{
    FILE *fp = NULL;

    //apertura file di testo
    fp = fopen("mazzo_bang.txt", "r");

    if(fp == NULL)
        exit(EXIT_FAILURE);

    for(int i=0;i<mazzoPesca->numCarte;i++)
    {
        //lettura dei dati da mazzo_bang.txt
        fscanf(fp, "%d %d %d %s\n", &mazzoPesca->carte[i].tipoC, &mazzoPesca->carte[i].num, &mazzoPesca->carte[i].seme,mazzoPesca->carte[i].nome);
    }

    fclose(fp);
}

/** La procedura creaMazzoPesca si occupa di definire
 * i dati della struttura Mazzo per il mazzo di pesca
 * e alloca la memoria per l'array di carte del mazzo.
 * Utilizza la funzione letturaCarte per memorizzare
 * i dati delle carte.
 *
 * @param mazzoPesca puntatore alla struttura per il mazzo di pesca
**/

void creaMazzoPesca(Mazzo* mazzoPesca)
{
    mazzoPesca->tipoM = MAZZO_PESCA;

    mazzoPesca->numCarte = NUMERO_CARTE;

    // allocazione dell'array di carte
    mazzoPesca->carte = (Carta*) malloc(sizeof(Carta) * mazzoPesca->numCarte);

    //verifica avvenuta allocazione
    if(mazzoPesca->carte == NULL)
        exit(EXIT_FAILURE);

    //chiamata a letturaCarte
    letturaCarte(mazzoPesca);
}

/** La procedura creaMazzoScarti si occupa di definire
 * i dati della struttura Mazzo per il mazzo degli scarti.
 *
 * @param mazzoScarti puntatore alla struttura per il mazzo di scarti
**/

void creaMazzoScarti(Mazzo* mazzoScarti)
{
    mazzoScarti->tipoM = MAZZO_SCARTO;

    mazzoScarti->numCarte = 0;

    mazzoScarti->carte = NULL;
}

/** La procedura mescolaCarte si occupa dello scambio dell'ordine delle carte
 * all'interno di una struttura Mazzo. Viene utilizzata per mescolare le carte
 * del mazzo di pesca o del mazzo di scarti quando questo diventa il nuovo mazzo
 * mazzo di pesca.
 *
 * @param mazzo puntatore alla struttura per il mazzo da mescolare
**/

void mescolaCarte(Mazzo* mazzo)
{
    for(int i=0; i<mazzo->numCarte; i++)
    {
        // chiamata a randRange per ottenere un numero casuale da dare alla funzione scambioCarte
        int carta2 = randRange(0, mazzo->numCarte - 1);

        //chiamata a scambioCarte per scambiare d'ordine la carta in posizione i con una carta casuale del mazzo
        scambioCarte(mazzo->carte[i], mazzo->carte[carta2], &mazzo->carte[i], &mazzo->carte[carta2]);
    }
}

/**La funzione checkMazzoPesca controlla se il mazzo di pesca non ha
 * piu' carte al suo interno
 *
 * @param mazzoPesca puntatore a struttura per il mazzo di pesca
 * @return valore booleano per il controllo
**/

bool checkMazzoPesca(Mazzo* mazzoPesca)
{
    if(mazzoPesca->numCarte == 0)
        return true;

    else return false;
}

/** La procedura cambio mazzo viene chiamata successivamente al
 * controllo su mazzoPesca. se questo e' vuoto mazzoScarti viene mescolato
 * e messo al posto del mazzoPesca. La funzione gestisce la memoria dei due mazzi
 * tramite chiamate a funzioni malloc, realloc e free.
 *
 * @param mazzoScarti puntatore al mazzo degli scarti
 * @param mazzoPesca puntatore al mazzo di pesca vuoto
 * @param nomeFile puntatore alla stringa del nome del file log dove verra' segnato l'evento
**/

void cambioMazzo(Mazzo* mazzoScarti, Mazzo* mazzoPesca, char* nomeFile)
{
    if(mazzoPesca->carte == NULL)
    {
        //allocazione di un puntatore temporaneo alla dimensione di mazzoScarti
        Carta* temp = (Carta*) malloc(sizeof(Carta) * mazzoScarti->numCarte);

        if(temp != NULL)
        {
            //se il puntatore e' valido viene assegnato all'array di carte del mazzoPesca
            mazzoPesca->carte = temp;
        }

        //controllo di avvenuta allocazione
        if(mazzoPesca->carte == NULL)
            exit(EXIT_FAILURE);

        //mazzoPesca prende la dimensione di mazzoScarti
        mazzoPesca->numCarte = mazzoScarti->numCarte;
    }

    //chiamata a mescolaCarte
    mescolaCarte(mazzoScarti);

    for(int i=0; i<mazzoScarti->numCarte; i++)
    {
        //assegno le carte all'array del mazzoPesca
        mazzoPesca->carte[i] = mazzoScarti->carte[i];
    }

    //libero la memoria del vecchio mazzo di scarti e lo imposto come vuoto
    free(mazzoScarti->carte);
    mazzoScarti->carte = NULL;
    mazzoScarti->numCarte = 0;

    // report sul log dell'avvenuta operazione
    salvaLog("Mazzo di pesca vuoto, cambio mazzo.", nomeFile);
}

/** La procedura daiCarta si occupa di spostare le carte dal mazzo di pesca
 * alle mani dei giocatori o a mazzi temporanei di supporto per il gioco.
 * Viene passato mazzoScarti per gestire il caso in cui il mazzoPesca rimanga
 * senza carte al suo interno, in quel caso verrà chiamata cambioMazzo.
 *
 * @param mazzo1 puntatore alla struttura del mazzo di provenienza
 * @param mazzo2 puntatore alla struttura del mazzo di destinazione
 * @param mazzoPesca puntatore alla struttura per il mazzo di scarti
 * @param nomeFile nome del file log
**/

void daiCarta(Mazzo* mazzo1, Mazzo* mazzo2, Mazzo* mazzoScarti, char* nomeFile)
{
    // controllo se mazzoPesca e' vuoto
    if(mazzo1->tipoM == MAZZO_PESCA && checkMazzoPesca(mazzo1) && mazzoScarti != NULL)
    {
        //nel caso sia vuoto chiamo cambioMazzo
        cambioMazzo(mazzoScarti, mazzo1, nomeFile);
    }

    //alloco un vettore temporaneo alla dimensione del mazzo di destinazione aumentato di uno
    int aumento = mazzo2->numCarte + 1;
    Carta* temp = realloc(mazzo2->carte, sizeof(Carta) * aumento);

    if (temp != NULL)
    {
        //se l'allocazione e' avvenuta correttamente assegno il puntatore al mazzo di destinazione
        mazzo2->carte = temp;

        //mi assicuro che venga pescata la prima carta
        mazzo2->carte[mazzo2->numCarte] = mazzo1->carte[PRIMO];


        //aumento la dimensione di numCarte
        mazzo2->numCarte++;

        /*
         * nel prossimo blocco gestisco la memoria del mazzo di provenienza, chiamando realloc per
         * diminuire la dimensione del mazzo di provenienza. Il mazzo di provenienza viene
         * riordinato dopo la rimozione della carta
         */
        int decremento = mazzo1->numCarte - 1;

        for (int i = PRIMO; i < decremento; i++)
        {
            mazzo1->carte[i] = mazzo1->carte[i + 1];
        }

        // se il mazzo di provenienza non ha piu' carte lo imposto come vuoto
        if (decremento == 0)
        {
            free(mazzo1->carte);
            mazzo1->carte = NULL;
        }
        else
        {
            Carta* temp2 = realloc(mazzo1->carte, sizeof(Carta) * decremento);
            if (temp2 != NULL)
            {
                mazzo1->carte = temp2;
            }
            mazzo1->numCarte--;
        }
    }
}

/** La procedura spostaCarta si occupa del passaggio delle carte tra i mazzi del giocatore o da
 * uno di questi due al mazzo di scarti. Viene gestita la memoria dei mazzi coinvolti a ogni passaggio.
 *
 * @param mazzo1 mazzo di provenienza
 * @param carta id della carta da spostare
 * @param mazzo2 mazzo di destinazione
**/

void spostaCarta(Mazzo* mazzo1, int carta, Mazzo* mazzo2)
{
    //alloco un vettore temporaneo alla dimensione del mazzo di destinazione aumentato di uno
    int aumento = mazzo2->numCarte + 1;
    Carta* temp = realloc(mazzo2->carte, sizeof(Carta) * aumento);

    if (temp != NULL)
    {
        //se l'allocazione e' avvenuta correttamente assegno il puntatore al mazzo di destinazione
        mazzo2->carte = temp;

        //se la carta viene passata da un mazzo all'altro uso l'intero che la identifica
        mazzo2->carte[mazzo2->numCarte] = mazzo1->carte[carta];

        //aumento la dimensione di numCarte
        mazzo2->numCarte++;

        /*
         * nel prossimo blocco gestisco la memoria del mazzo di provenienza, chiamando realloc per
         * diminuire la dimensione del mazzo di provenienza. Il mazzo di provenienza viene
         * riordinato dopo la rimozione della carta
         */
        int decremento = mazzo1->numCarte - 1;

        for (int i = carta; i < decremento; i++)
        {
            mazzo1->carte[i] = mazzo1->carte[i + 1];
        }

        // se il mazzo di provenienza non ha piu' carte lo imposto come vuoto
        if (decremento == 0)
        {
            free(mazzo1->carte);
            mazzo1->carte = NULL;
        }
        else
        {
            Carta* temp2 = realloc(mazzo1->carte, sizeof(Carta) * decremento);
            if (temp2 != NULL)
            {
                mazzo1->carte = temp2;
            }
            mazzo1->numCarte--;
        }
    }
}

/** La procedura stampa carte procede a rendere visibili all'utente le carte di un mazzo
 * a seconda della necessita'.
 *
 * @param mazzo puntatore al mazzo da stampare
**/

void stampaCarte(Mazzo* mazzo)
{
    for(int i=0; i<mazzo->numCarte; i++)
    {
        char tipo[DIM_NOME_CARTE+1];

        char seme[DIM_NOME_CARTE+1];

        switch(mazzo->carte[i].seme)
        {
            case QUADRI:
                strcpy(seme, S_QUADRI);
                break;
            case PICCHE:
                strcpy(seme, S_PICCHE);
                break;
            case CUORI:
                strcpy(seme, S_CUORI);
                break;
            case FIORI:
                strcpy(seme, S_FIORI);
                break;
        }

        switch(mazzo->carte[i].tipoC)
        {
            case ISTANTANEA:
            case ISTANTANEA_SPECIAL:
                strcpy(tipo, S_MARRONI);
                break;
            case ARMA:
            case EFFETTO:
                strcpy(tipo, S_BLU);
        }

        printf("%d - %d di %s - \"%s\", %s\n", i, mazzo->carte[i].num, seme, mazzo->carte[i].nome, tipo);
    }
}

/** La procedura continuo serve per bloccare l'esecuzione del programma
 * per farla ripartire dopo l'input dell'utente.
**/

void continuo()
{
    printf("\nPremi invio per continuare:");
    getchar();
}

/**La funzione comparaNomi prende come parametri due stringhe e le compara
 * chiamando la funzione strcmp().
 *
 * @param nome1 puntatore a stringa per il primo nome
 * @param nome2 puntatore a stringa per il secondo nome
 * @return booleano true se le due stringhe sono uguali, false in caso contrario
**/

bool comparaNomi(char* nome1, char* nome2)
{
    if(strcmp(nome1,nome2) == 0)
        return true;

    else return false;
}

/** La procedura creaGiocatori si occupa della creazione dei giocatori,
 * chiede all'utente il nome da inserire e procede a inserire i dati del giocatore
 * all'interno della struttura
 *
 * @param party puntatore all'array party di giocatori
 * @param nGiocatori numero dei giocatori che partecipano
 * @param nomeFile puntatore al nome del file log
**/

void creaGiocatori(Giocatore** party, int nGiocatori, char* nomeFile)
{
    //allocazione del party per mezzo di puntatore temporaneo a Giocatore
    Giocatore* temp = (Giocatore*) malloc(nGiocatori * sizeof(Giocatore));

    if(temp != NULL)
    {
        *party = temp;

        for(int i=0; i<nGiocatori; i++)
        {

            //prendo input dall'utente e controllo che il nome del nuovo giocatore sia diverso dai precedenti
            bool check = false;
            do
            {
                printf("\nInserisci il nome del %d giocatore:", i+1);
                scanf(" %50[^\n]s",(*party)[i].nome);

                if(i!=0)
                {
                    for(int j=0; j<i; j++)
                    {
                        if(comparaNomi((*party)[i].nome, (*party)[j].nome))
                            check = true;
                    }
                }

                if(check)
                {
                    printf("\nErrore! Per favore inserisci un nome diverso dai precedenti\n");
                }
            }while(check);

            //dati dei mazzi inMano e inGioco
            (*party)[i].mano.carte = NULL;
            (*party)[i].mano.tipoM = GIOC_MANO;
            (*party)[i].mano.numCarte = 0;

            (*party)[i].inGioco.carte = NULL;
            (*party)[i].inGioco.tipoM = GIOC_GIOCO;
            (*party)[i].inGioco.numCarte = 0;

            //gittata del giocatore impostata inzialmente a 1
            (*party)[i].gittata = GITTATA_COLT_45;

            //report sul log di avvenuta operazione
            char messaggio[MEX_DIM] = "Creato giocatore: ";
            strcat(messaggio, (*party)[i].nome);

            salvaLog(messaggio, nomeFile);

            continuo();
            pulisciSchermo();
        }
    }
}

/** La procedura generaRuoli si occupa dell'assegnazione del ruolo ai giocatori
 * tramite un ciclo che in base al numero di giocatori si interrompe finche' i ruoli
 * assegnati non sono conformi a quelli stabiliti dalle specifiche del gioco.
 *
 * @param party puntatore al party di giocatori
 * @param nGiocatori numero di giocatori
 * @param nomeFile nome del file log
**/

void generaRuoli(Giocatore* party, int nGiocatori, char* nomeFile)
{
    int sceriffo ,vicesceriffo, fuorilegge, rinnegato;

    //massimo a 1 per sceriffo e rinnegato, variabile per gli altri
    int maxSceriffo = 1, maxVice, maxFuorilegge, maxRinnegato = 1;

    switch(nGiocatori)
    {
        case 4:
        {
            maxVice = 0;
            maxFuorilegge = 2;
            break;
        }
        case 5:
        {
            maxVice = 1;
            maxFuorilegge = 2;
            break;
        }
        case 6:
        {
            maxVice = 1;
            maxFuorilegge = 3;
            break;
        }
        case 7:
        {
            maxVice = 2;
            maxFuorilegge = 3;
            break;
        }
    }

    do
    {
        sceriffo = vicesceriffo = fuorilegge = rinnegato = 0;

        for(int i = 0; i<nGiocatori; i++)
        {
            //assegna un ruolo casuale al giocatore
            party[i].ruolo = randRange(MIN_RUOLI, MAX_RUOLI);

            //aumenta il numero di giocatori attualmente con quel ruolo
            switch (party[i].ruolo)
            {
                case SCERIFFO:
                    sceriffo++;
                    break;
                case VICESCERIFFO:
                    vicesceriffo++;
                    break;
                case FUORILEGGE:
                    fuorilegge++;
                    break;
                case RINNEGATO:
                    rinnegato++;
                    break;
            }
        }
        //il ciclo non si interrompe finche' i ruoli non sono confrotmi
    }while(sceriffo != maxSceriffo || vicesceriffo != maxVice || fuorilegge != maxFuorilegge || rinnegato != maxRinnegato);

    // report sul log
    salvaLog("Ruoli assegnati ai giocatori", nomeFile);
}

/** La procedura lifePoints assegna un numero di punti vita designato per il ruolo del giocatore.
 *
 * @param party puntatore a party di giocatori
 * @param nGiocatori numero di giocatori
 * @param nomeFile nome del file log
**/

void lifePoints(Giocatore* party, int nGiocatori, char* nomeFile)
{
    for(int i = 0; i < nGiocatori; i++)
    {
        if(party[i].ruolo == ID_SCERIFFO)
            party[i].pallottole = 1;
        else
            party[i].pallottole = HP;
    }

    salvaLog("Pallottole assegnate ai giocatori.", nomeFile);
}

/** La procedura rivelaRuoli si occupa di comunicare al giocatore il ruolo che gli e' stato
 * casualmente affidato
 *
 * @param party party di giocatori
 * @param nGiocatori numero di giocatori
 * @param nomeFile nome del file log
**/

void rivelaRuoli(Giocatore* party, int nGiocatori, char* nomeFile)
{

    // stampo prima lo sceriffo poi gli altri giocatori
    int idSceriffo = -1;

    for(int i=0; i<nGiocatori; i++)
    {
        if(party[i].ruolo == SCERIFFO)
            idSceriffo = i;
    }

    printf("\n%s! Sei lo SCERIFFO!\nElimina tutti i FUORILEGGE e il RINNEGATO.\nHai %d pallottole.\n", party[idSceriffo].nome, HP_SCERIFFO);
    char messaggio[MEX_DIM] = "";
    strcat(messaggio, party[idSceriffo].nome);
    strcat(messaggio, " e' lo sceriffo.");
    salvaLog(messaggio, nomeFile);

    continuo();
    pulisciSchermo();

    for(int i=0; i<nGiocatori; i++)
    {
        if(party[i].ruolo != SCERIFFO)
        {
            if(party[i].ruolo == VICESCERIFFO)
            {
                printf("\n%s! Sei un VICESCERIFFO!\nProteggi lo SCERIFFO, elimina tutti i FUORILEGGE e il RINNEGATO.\nHai %d pallottole.\n", party[i].nome, HP);
                char messaggio2[MEX_DIM] = "";
                strcat(messaggio2, party[i].nome);
                strcat(messaggio2, " e' un vicesceriffo.");
                salvaLog(messaggio2, nomeFile);
            }
            else if(party[i].ruolo == FUORILEGGE)
            {
                printf("\n%s! Sei un FUORILEGGE!\nElimina lo SCERIFFO.\nHai %d pallottole.\n", party[i].nome, HP);
                char messaggio2[MEX_DIM] = "";
                strcat(messaggio2, party[i].nome);
                strcat(messaggio2, " e' un fuorilegge.");
                salvaLog(messaggio2, nomeFile);
            }
            else if(party[i].ruolo == RINNEGATO)
            {
                printf("\n%s! Sei il RINNEGATO!\nRimani l'unico giocatore in gioco.\nHai %d pallottole.\n", party[i].nome, HP);
                char messaggio2[MEX_DIM] = "";
                strcat(messaggio2, party[i].nome);
                strcat(messaggio2, " e' il rinnegato.");
                salvaLog(messaggio2, nomeFile);
            }

            continuo();
            pulisciSchermo();
        }
    }
}

/** La procedura distribuisciCarte viene chiamata all'inizio di una nuova partita
 * e si occupa di distribuire le carte dal mazzo di pesca alle mani dei giocatori.
 * Verranno date tante carte quante i punti vita del giocatore
 *
 * @param party party di giocatori
 * @param nGiocatori numero di giocatori
 * @param mazzoPesca mazzo di pesca
 * @param mazzoScarti mazzo di scarti
 * @param nomeFile nome file log
**/

void distribuisciCarte(Giocatore* party, int nGiocatori, Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile)
{
    for(int i=0; i<nGiocatori; i++)
    {
        printf("\n\n%s!\nRiceverai delle carte dal mazzo di pesca, tante quante sono le tue pallottole!\n\n", party[i].nome);

        do
        {
            daiCarta(mazzoPesca, &party[i].mano, mazzoScarti, nomeFile);
        }while(party[i].mano.numCarte != party[i].pallottole);

        continuo();
        pulisciSchermo();
    }

    salvaLog("Carte distribuite ai giocatori.", nomeFile);
}

/** La funzione nuovoParty crea il party di giocatori e chiama le funzioni per
 * la creazione dei giocatori, assegnazione dei ruoli e dei punti vita.
 *
 * @param nGiocatori puntatore all'intero per nGiocatori
 * @param nomeFile nome del file log
 * @return puntatore al nuovo Party
**/

Giocatore* nuovoParty(int* nGiocatori, char* nomeFile)
{
    Giocatore* party = NULL;

    /* utente sceglie il numero di giocatori partecipanti alla partita
     *  e conrolla che siano in un numero conforme alle regole*/

    do
    {
        printf("\nIniziamo! Inserisci il numero di giocatori -->");
        scanf("%d", nGiocatori);

        if((*nGiocatori)<4 || (*nGiocatori)>7)
            printf("Errore\n");
    }while((*nGiocatori)<4 || (*nGiocatori)>7);

    continuo();
    pulisciSchermo();

    creaGiocatori(&party, *nGiocatori, nomeFile);

    //controllo che i giocatori siano stati creati
    if(party == NULL)
        exit(EXIT_FAILURE);

    generaRuoli(party, *nGiocatori, nomeFile);

    lifePoints(party, *nGiocatori, nomeFile);

    salvaLog("Party di giocatori creato.", nomeFile);

    return party;
}

/** La procedura stampaNomi si occupa di rendere visibli all'utente i nomi del party
 * di giocatori appena creato
 *
 * @param party party di giocatori
 * @param nGiocatori numero di giocatori
 */

void stampaNomi(Giocatore* party, int nGiocatori)
{
    printf("\nParty giocatori:\n\n");

    for(int i=0; i<nGiocatori; i++)
    {
        if(party[i].ruolo == SCERIFFO)
        {
            printf("\n%s  -  SCERIFFO",party[i].nome);
        }
        else
        {
            printf("\n%s",party[i].nome);
        }
    }

    continuo();
    pulisciSchermo();
}

/** La funzione trovaCartaMano cerca all'interno della mano del giocatore
 * la carta identificata dall'intero passato come parametro
 *
 * @param giocatore puntatore al giocatore
 * @param nomeCarta id della carta da cercare
 * @return la posizione della carta all'interno della mano
**/

int trovaCartaMano(Giocatore* giocatore, char* nomeCarta)
{
    // se la carta non viene trovata ritorna NOP
    int carta = NOP;

    for(int i=0; i<giocatore->mano.numCarte; i++)
    {
        if(comparaNomi(nomeCarta, giocatore->mano.carte[i].nome))
        {
            carta = i;
        }
    }

    return carta;
}

/** La funzione trovaCartaInGioco cerca tra le carte in gioco del giocatore
 * la carta identificata dall'intero passato come parametro
 *
 * @param giocatore puntatore al giocatore
 * @param nomeCarta id della carta da cercare
 * @return la posizione della carta tra le carte in gioco
**/

int trovaCartaInGioco(Giocatore* giocatore, char* nomeCarta)
{
    //se la carta non viene torvata ritorna NOP
    int carta = NOP;

    for(int i=0; i<giocatore->inGioco.numCarte; i++)
    {
        if(comparaNomi(nomeCarta, giocatore->inGioco.carte[i].nome))
        {
            carta = i;
        }
    }

    return carta;
}

/** La funzione checkBonus ritorna un valore booleano a seconda
 * del fatto che ci siano o meno le condizioni perche' venga assegnato un bonus
 * a chi ha eliminato il gioatore
 *
 * @param eliminato puntatore al giocatore eliminato
 * @return booleano a true o false
**/

bool checkBonus(Giocatore* eliminato)
{
    bool check = false;
    if(eliminato->ruolo == FUORILEGGE)
        check = true;

    return check;
}

/** La funzione checkMalus ritorna un valore booleano a seconda
 * del fatto che ci siano o meno le condizioni perche' venga assegnato un malus
 * a chi ha eliminato il giocatore
 *
 * @param eliminato puntatore al giocatore eliminato
 * @return booleano a true o false
**/

bool checkMalus(Giocatore* attaccante, Giocatore* eliminato)
{
    bool check = false;
    if(attaccante->ruolo == SCERIFFO && eliminato->ruolo == VICESCERIFFO)
        check = true;

    return check;
}

/** La procedura bonus assegna un bonus a chi elimina un fuorilegge.
 *
 * @param attaccante giocatore che ha eliminato il fuorilegge
 * @param mazzoPesca mazzo di pesca
 * @param mazzoScarti mazzo scarti
 * @param nomeFile nome file log
 */

void bonus(Giocatore* attaccante, Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile)
{
    char messaggio[MEX_DIM] = "";
    strcat(messaggio, attaccante->nome);
    strcat(messaggio, " ha eliminato un fuorilegge, riceve la ricompensa sulla taglia.");
    salvaLog(messaggio, nomeFile);

    printf("\n%s! Hai eliminato un fuorilegge,\nVieni premiato per le tue gesta!\nRicevi una ricompensa per la taglia sul giocatore eliminato!\n", attaccante->nome);
    continuo();
    pulisciSchermo();

    //la ricompensa e' pescare due carte dal mazzoPesca
    for(int i=0; i < BONUS; i++)
    {
        daiCarta(mazzoPesca, &attaccante->mano, mazzoScarti, nomeFile);
    }
}

/** La procedura malus assegna un malus a allo sceriffo che elimina un suo vice.
 *
 * @param attaccante sceriffo che ha eliminato il vice
 * @param mazzoScarti mazzo scarti
 * @param nomeFile nome file log
 */

void malus(Giocatore* attaccante, Mazzo* mazzoScarti, char* nomeFile)
{
    char messaggio[MEX_DIM] = "";
    strcat(messaggio, attaccante->nome);
    strcat(messaggio, " ha eliminato un vicesceriffo, perde tutte le sue carte.");
    salvaLog(messaggio, nomeFile);

    printf("\n%s! Hai eliminato un tuo vice,\nverrai disonorato e perderai tutti i tuoi averi!\nNon mollare e rifatti un nome!\n", attaccante->nome);
    continuo();
    pulisciSchermo();

    //il malus consiste nello scartare tutte le carte dal proprio mazzo
    for(int i = 0; i<attaccante->mano.numCarte; i++)
    {
        spostaCarta(&attaccante->mano, i, mazzoScarti);
    }
}

/** La procedura eliminazione si occupa dell'eliminazione del giocatore eliminato dall'array party.
 * Viene controllato se chi ha causato l'eliminazione riceve bonus o malus.
 *
 * @param party array party
 * @param attaccante chi ha eliminato il giocatore
 * @param eliminato il giocatore eliminato
 * @param nGiocatori numero di giocatori
 * @param mazzoPesca mazzo di pesca
 * @param mazzoScarti mazzo di scarti
 * @param nomeFile nome file log
 */

void eliminazione(Giocatore* party, Giocatore* attaccante, Giocatore* eliminato, int* nGiocatori,Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile)
{
    printf("\n\n%s E' STATO ELIMINATO!!!!\n\n", eliminato->nome);
    char messaggio2[MEX_DIM] = "";
    strcat(messaggio2, eliminato->nome);
    strcat(messaggio2, " e' eliminato dal gioco.");
    salvaLog(messaggio2, nomeFile);
    continuo();
    pulisciSchermo();

    printf("\nOk\n");
    continuo();
    //scarto tutte le carte dell'eliminato
    for(int i=0; i<eliminato->mano.numCarte; i++)
    {
        spostaCarta(&eliminato->mano, i, mazzoScarti);
    }
    for(int i=0; i<eliminato->inGioco.numCarte; i++)
    {
        spostaCarta(&eliminato->inGioco, i, mazzoScarti);
    }

    printf("\nSCartate caarte\n");
    continuo();
    //controllo per eventuali bonus o malus
    if(!comparaNomi(attaccante->nome, eliminato->nome) || !comparaNomi(attaccante->nome, DINAMITE))
    {
        if(checkBonus(eliminato))
            bonus(attaccante, mazzoPesca, mazzoScarti, nomeFile);
        else if(checkMalus(attaccante, eliminato))
            malus(attaccante, mazzoScarti, nomeFile);
    }

    printf("\nok bonus\n");
    continuo();
    //eliminazione dell'eliminato dal party
    int idEliminato = NOP;

    for(int i=0; i<*nGiocatori; i++)
    {
        if(comparaNomi(eliminato->nome, party[i].nome))
        {
            idEliminato = i;
        }
    }


    free(eliminato->inGioco.carte);
    free(eliminato->mano.carte);

    printf("\nok liberazione mazzi\n");
    continuo();
    if(idEliminato != NOP)
    {
        if(idEliminato != (*nGiocatori)-1)
        {
            for(int j = idEliminato; j<(*nGiocatori-1); j++)
            {
                party[j] = party[j+1];
            }
        }

        (*nGiocatori)--;

        printf("\nOk riordinamento\n");
        continuo();


        // gestione della memoria dell'array party
        Giocatore* temp = realloc(party, sizeof(Giocatore) * (*nGiocatori));

        if(temp != NULL)
        {
            party = temp;
            printf("\nok realloc\n");
            continuo();
        }
    }
}

/** La procedura salvaLog scrive tutti gli avvenimenti dell partite su un file di testo.
 * ogni file log ha lo stesso nome del file di salvataggio a cui e' associato.
 *
 * @param messaggio stringa da scrivere sul log
 * @param nomeFile nome del file di salvataggio a cui e' associato il log
**/

void salvaLog(char* messaggio, char* nomeFile)
{
    char* nomeLog = (char*) malloc(sizeof(char) * (strlen(nomeFile)+1));
    strcpy(nomeLog, nomeFile);

    FILE* fp = NULL;

    strcat(nomeLog, "_log.txt");

    fp = fopen(nomeLog, "a");
    if(fp == NULL)
    {
        printf("\nErrore\n");
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "%s\n", messaggio);
    fclose(fp);

    free(nomeLog);
}

/**La procedura pulisciSchermo si occupa di chiamare la funzione system
 * per pulire l'output sul terminale a seconda della necessita'.
 * System() avra' un comando diverso a seconda del sistema operativo
 */

void pulisciSchermo()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
