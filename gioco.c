#include "gioco.h"

/** La procedura scartiEccesso viene chiamata quando termina un giro dei turni dei giocatori,
 * e serve per controllare se il giocatore ha piu' carte di quelle consentite, ovvero se ha
 * piu' carte delle pallottole che si hanno attualmente. Vengono stampate le carte della propria mano
 * e il giocatore dovra' scegliere quale carta scartare.
 *
 * @param giocatore giocatore che deve scartare delle carte
 * @param mazzoScarti mazzo di scarti
**/

void scartiEccesso(Giocatore* giocatore, Mazzo* mazzoScarti)
{
    printf("\n%s! Hai %d carte da scartare!\n", giocatore->nome, (giocatore->mano.numCarte - giocatore->pallottole));

    while(giocatore->mano.numCarte > giocatore->pallottole)
    {
        stampaCarte(&giocatore->mano);

        int scelta = NOP;

        do
        {
            printf("\n\nScegli una carta da scartare:");
            scanf("%d", &scelta);
        }while(scelta<0 || scelta>(giocatore->mano.numCarte-1));

        spostaCarta(&giocatore->mano, scelta, mazzoScarti);
    }
}

/** La funzione checkBang controlla se il giocatore a giocare il turno puo' giocare altri bang,
 * e questo dipende dal fatto che abbia o no in gioco l'arma Volcanic, che permette di usare bang illimitati, quando
 * normalmente ogni giocatore puo' giocare un bang per turno. Restituisce un valore booleano in base alle condizioni
 * sopra citate
 *
 * @param giocatore giocatore che gioca il suo turno
 * @param bangGiocati numero di bang giocati in quel turno
 * @return booleano a true o false
**/

bool checkBang(Giocatore* giocatore, int bangGiocati)
{
    bool check = true;

    int idVolcanic = NOP;

    idVolcanic = trovaCartaInGioco(giocatore, VOLCANIC);

    if(idVolcanic == NOP && bangGiocati > 0)
    {
        // se non ha in mano una volcanic e ha giocato gia' un bang non potra' giocarne altri
        check = false;
    }

    return check;
}

/** La funzione checkCartaBlu si occupa di controllare le carte in gioco del giocatore in turno:
 * se questo prova a giocare una carta blu e ha in gioco una carta blu con lo stesso nome, non potra' giocarla.
 * (es: se ha in gioco Barile, non potra' giocare un'altra carta Barile). La funzione restituisce un booleano
 * con valore che dipende dalle condizioni sopra citate.
 *
 * @param giocatore giocatore in turno
 * @param carta id della carta da giocare
 * @return booleano a true o false
**/

bool checkCartaBlu(Giocatore* giocatore, int carta)
{
    bool check = true;

    for(int i=0; i<giocatore->inGioco.numCarte; i++)
    {
        if(comparaNomi(giocatore->mano.carte[carta].nome, giocatore->inGioco.carte[i].nome))
        {
            check = false;
        }
    }

    return check;
}

/** La procedura giocata si occupa del caso in cui un giocatore voglia giocare una carta che ha in mano:
 * controlla i casi in cui non possa giocare determinate carte (es: piu' bang in un turno senza Volcanic in gioco)
 * e chiama le funzioni peer giocare la carta in base al loro tipo.
 *
 * @param party party di giocatori
 * @param nGiocatori numero di giocatori
 * @param giocatore giocatore che vuole giocare una carta
 * @param bangGiocati numero di bang giocati nel turno
 * @param mazzoPesca mazzo di pesca
 * @param mazzoScarti mazzo di scarti
 * @param nomeFile file log
**/

void giocata(Giocatore* party, int *nGiocatori, Giocatore* giocatore, int* bangGiocati, Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile)
{
    int scelta = NOP;

    stampaCarte(&giocatore->mano);

    do
    {
        //scelgo la carta da giocare e conrollo che sia valida

        printf("\nScegli la carta da giocare:\n");
        scanf("%d", &scelta);

        if(scelta<MIN_SCELTA || scelta>giocatore->mano.numCarte)
        {
            printf("\nErrore! Vuoi giocare una carta che non esiste!\n");
        }
        if(!checkBang(giocatore, *bangGiocati) && comparaNomi(giocatore->mano.carte[scelta].nome, BANG))
        {
            printf("\nNon puoi giocare altri BANG! per questo turno.\nPuoi giocare VOLCANIC per giocare quanti BANG! vuoi. Riprova.\n");
        }
        if(!checkCartaBlu(giocatore, scelta))
        {
            printf("\nNon puoi avere piu' carte blu con lo stesso nome in gioco! Riprova.\n");
        }
    }while(scelta<MIN_SCELTA || scelta>giocatore->mano.numCarte || (!checkBang(giocatore, *bangGiocati) && comparaNomi(giocatore->mano.carte[scelta].nome, BANG)) || !checkCartaBlu(giocatore, scelta));

    getchar();

    switch(giocatore->mano.carte[scelta].tipoC)
    {
        case ISTANTANEA:
        case ISTANTANEA_SPECIAL:
        {
            if(comparaNomi(giocatore->mano.carte[scelta].nome, BANG))
            {
                (*bangGiocati)++;
            }
            //gioco carta marrone per istantanea e istantanea special
            giocaCartaMarrone(party, nGiocatori, giocatore, scelta, mazzoPesca, mazzoScarti, nomeFile);
        }
            break;
        case ARMA:
        case EFFETTO:
            // gioco carta blu per armi e effetti
            giocaCartaBlu(party, *nGiocatori, giocatore, scelta, mazzoScarti, nomeFile);
            break;
    }

    pulisciSchermo();
}

/** La procedura salvataggio si occupa di scrivere i dati della partita in corso su un file binario di salvataggio:
 * viene chiamata a inzio di ogni turno di un giocatore e salva sul file: -L'array di giocatori Party;
 * - intero che indica il numero dei giocatori; - memoria occupata dal mazzo di pesca; - memoria occupata dal mazzo di scarti
 * - Le strutture dei giocatori con i loro mazzi mano e inGioco; - intero che identifica il prossimo giocatore a giocare;
 * - intero che indica il turno che si sta giocando.
 *
 * @param party party di giocatori
 * @param nGiocatori numero dei giocatori
 * @param mazzoPesca mazzo di pesca
 * @param mazzoScarti mazzo di scarti
 * @param prossimoGiocatore intero per il prossimo giocatore
 * @param nTurno intero per il turno corrente
 * @param nomeFile nome file su cui scrivere
**/

void salvataggio(Giocatore* party, int* nGiocatori, Mazzo* mazzoPesca, Mazzo* mazzoScarti, int* prossimoGiocatore, int *nTurno, char* nomeFile)
{
    //creo puntatore a file
    FILE* fp = NULL;

    //do il nome del file su cui desidero salvare la partita
    char temp[SAVE_DIM+1];
    strcpy(temp, nomeFile);
    strcat(temp, ".sav");

    //apro il file in modalita' scrittura binaria e controllo non ci siano problemi con l'apertura
    fp = fopen(temp, "wb");
    if(fp == NULL)
    {
        printf("\n\nErrore!\n\n");
        exit(EXIT_FAILURE);
    }

    //scrivo il numero dei giocatori
    fwrite(nGiocatori, sizeof(int), 1, fp);

    //scrivo le strutture dei giocatori e i loro mazzi di carte
    for(int i=0; i<(*nGiocatori); i++)
    {
        fwrite(&party[i], sizeof(Giocatore), 1, fp);
        fwrite(party[i].mano.carte, sizeof(Carta), party[i].mano.numCarte, fp);
        fwrite(party[i].inGioco.carte, sizeof(Carta), party[i].inGioco.numCarte, fp);
    }

    // scrivo il prossimo giocatore
    fwrite(prossimoGiocatore, sizeof(int), 1, fp);

    //scrivo la struttura per il mazzo di pesca
    fwrite(mazzoPesca, sizeof(Mazzo), 1, fp);
    fwrite(mazzoPesca->carte, sizeof(Carta), mazzoPesca->numCarte, fp);

    // scrivo la struttura per il mazzo di scarti
    fwrite(mazzoScarti, sizeof(Mazzo), 1, fp);
    fwrite(mazzoScarti->carte, sizeof(Carta), mazzoPesca->numCarte, fp);

    //scrivo il turno corrente
    fwrite(nTurno, sizeof(int), 1, fp);

    //chiudo il file
    fclose(fp);
}

/** La funzione caricaSalvataggio si occupa di leggere un file di salvataggio e salvare
 * cio' che vi e' scritto all'interno nelle strutture che serviranno per avviare la partita.
 * Si occupa di allocare la memoria per le strutture su cui verrano trasferiti i dati presenti nel salvataggio.
 * Infine restituisce il puntatore a party al chiamante.
 *
 * @param party puntatore al party di giocatori
 * @param nGiocatori puntatore al numero di giocatori
 * @param mazzoPesca puntatore al mazzo di pesca
 * @param mazzoScarti puntatore al mazzo scarti
 * @param prossimoGiocatore puntatore al prossimo giocatore
 * @param nTurno puntatore al numero del turno
 * @param nomeFile nome file salvataggio da caricare
 * @return puntatore a party
**/

Giocatore* caricaSalvataggio(Giocatore* party, int* nGiocatori, Mazzo* mazzoPesca, Mazzo* mazzoScarti, int* prossimoGiocatore, int* nTurno, char* nomeFile)
{
    //creo puntatore a FILE
    FILE* fp = NULL;

    // do il nome del file da caricare
    char temp[SAVE_DIM+1];
    strcpy(temp, nomeFile);
    strcat(temp, ".sav");

    // apro il file in modalota' lettura binaria e controllo non ci siano problemi ocn l'apertura
    fp = fopen(temp, "rb");
    if(fp == NULL)
    {
        printf("\n\nErrore!\n\n");
        exit(EXIT_FAILURE);
    }

    //leggo il numero dei giocatori
    fread(nGiocatori, sizeof(int), 1, fp);

    //alloco la memoria per l'array di giocatori
    party = (Giocatore*) malloc(sizeof(Giocatore) * (*nGiocatori));
    if(party == NULL)
        exit(EXIT_FAILURE);

   for(int i=0; i<(*nGiocatori); i++)
    {
       //leggo la struttura del giocatore
        fread(&party[i], sizeof(Giocatore), 1, fp);

        // alloco la memoria per il mazzo mano
        party[i].mano.carte = (Carta *) malloc(sizeof(Carta) * party[i].mano.numCarte);
        if(party[i].mano.carte == NULL)
            exit(EXIT_FAILURE);

        //alloco la memoria per il mazzo in gioco
        party[i].inGioco.carte = (Carta *) malloc(sizeof(Carta) * party[i].inGioco.numCarte);
        if(party[i].inGioco.carte == NULL)
            exit(EXIT_FAILURE);

        //leggo i dati dei due mazzi
        fread(party[i].mano.carte, sizeof(Carta), party[i].mano.numCarte, fp);
        fread(party[i].inGioco.carte, sizeof(Carta), party[i].inGioco.numCarte, fp);
    }

   // leggo l'intero per il prossimo giocatore
    fread(prossimoGiocatore, sizeof(int), 1, fp);

   // leggo la struttura per il mazzo di pesca
    fread(mazzoPesca, sizeof(Mazzo), 1, fp);
    // alloco la memoria per l'array di carte del mazzoPesca
    mazzoPesca->carte = (Carta *) malloc(sizeof(Carta) * mazzoPesca->numCarte);
    if(mazzoPesca->carte == NULL)
    {
        exit(EXIT_FAILURE);
    }
    // leggo le carte per il mazzoPesca
    fread(mazzoPesca->carte, sizeof(Carta), mazzoPesca->numCarte, fp);

    //leggo la struttura per il mazzo scarti
    fread(mazzoScarti, sizeof(Mazzo), 1, fp);
    // alloco memoria per l'array di carte del mazzoScarti
    mazzoScarti->carte = (Carta *) malloc(sizeof(Carta) * mazzoScarti->numCarte);
    if(mazzoScarti->carte == NULL)
    {
        exit(EXIT_FAILURE);
    }
    // leggo le carte del mazzo scarti
    fread(mazzoScarti->carte, sizeof(Carta), mazzoScarti->numCarte, fp);

    // chiudo il file
    fclose(fp);

    return party;
}

/** La procedura
 *
 * @param party
 * @param nGiocatori
 * @param mazzoPesca
 * @param mazzoScarti
 * @param prossimoGiocatore
 * @param nTurno
 * @param nomeFile
**/

void chiudiGioco(Giocatore* party, int* nGiocatori, Mazzo* mazzoPesca, Mazzo* mazzoScarti, int* prossimoGiocatore, int *nTurno, char* nomeFile)
{
    salvaLog("Partita salvata.", nomeFile);
    salvataggio(party, nGiocatori, mazzoPesca, mazzoScarti, prossimoGiocatore, nTurno, nomeFile);

    free(mazzoPesca->carte);
    free(mazzoScarti->carte);
    free(nomeFile);

    for(int i=0; i<(*nGiocatori); i++)
    {
        free(party[i].mano.carte);
        free(party[i].inGioco.carte);
    }

    free(party);
}

void turno(Giocatore* party, int* nGiocatori, Giocatore* giocatore, int* nTurno, Mazzo* mazzoPesca, Mazzo* mazzoScarti, int giocatoreAttuale, int* prossimoGiocatore, char* nomeFile)
{
    char nomeRuolo[15];

    switch(giocatore->ruolo)
    {
        case SCERIFFO:
            strcpy(nomeRuolo, "SCERIFFO");
            break;
        case VICESCERIFFO:
            strcpy(nomeRuolo, "VICESCERIFFO");
            break;
        case FUORILEGGE:
            strcpy(nomeRuolo, "FUORILEGGE");
            break;
        case RINNEGATO:
            strcpy(nomeRuolo, "RINNEGATO");
            break;
    }

    salvataggio(party, nGiocatori, mazzoPesca, mazzoScarti, &giocatoreAttuale, nTurno, nomeFile);

    char messaggio[MEX_DIM] = "E' il turno di ";
    strcat(messaggio, giocatore->nome);
    strcat( messaggio, ".");
    salvaLog(messaggio, nomeFile);
    continuo();
    printf("\n%s! Peschi due carte dal mazzo.\n", giocatore->nome);
    continuo();
    daiCarta(mazzoPesca, &giocatore->mano, mazzoScarti, nomeFile);
    daiCarta(mazzoPesca, &giocatore->mano, mazzoScarti, nomeFile);

    int idDinamite = NOP;
    bool eliminazioneDinamite = false;

    idDinamite = trovaCartaInGioco(giocatore, DINAMITE);

    if(idDinamite != NOP)
    {
        eliminazioneDinamite = dinamite(party, nGiocatori, giocatore, mazzoPesca, mazzoScarti, idDinamite, nomeFile);
    }

    if(!eliminazioneDinamite)
    {
        int scelta = NOP, bangGiocati = 0;
        bool checkPrigione = true;

        do
        {
            pulisciSchermo();

            if(checkFine(party, *nGiocatori) != NOP)
            {
                scelta = SKIP;
            }
            else
            {
                int i=0, idPrigione = NOP, nGiocate = 0;

                idPrigione = trovaCartaInGioco(giocatore, PRIGIONE);

                if(idPrigione != NOP) {
                    printf("\n\nSei in prigione!\nProcedi con l'estrazione per evadere e giocare il turno!\n\n");
                    continuo();
                    checkPrigione = prigione(giocatore, mazzoPesca, mazzoScarti, nomeFile);
                }
                if(checkPrigione)
                {
                    printf("\n\nTurno %d - %s - %s - %d pallottole.\n\n", (*nTurno), giocatore->nome, nomeRuolo, giocatore->pallottole);

                    printf("\n\nGioca il tuo turno!\n\n");

                    if(checkBang(giocatore, bangGiocati))
                    {
                        printf("%d - Gioca una carta.\n", i);
                    }
                    else
                    {
                        printf("%d - Gioca una carta. (Escluso BANG!)\n", i);
                    }
                    i++;
                    printf("%d - Guarda la tua mano.\n", i);
                    i++;
                    printf("%d - Guarda le tue carte in gioco.\n", i);
                    i++;
                    printf("%d - Controlla le distanze.\n", i);
                    i++;
                    printf("%d - Guarda le carte in gioco degli altri giocatori.\n", i);
                    i++;
                    printf("%d - Passa il turno.\n", i);
                    i++;
                    printf("%d - Chiudi il gioco.\n", i);
                    i++;

                    printf("\nScegli l'azione da effettuare:");
                    scanf("%d", &scelta);
                }
                else
                {
                    scelta = SKIP;
                }

                if(scelta<MIN_SCELTA || scelta>MAX_SCELTA)
                {
                    getchar();
                    printf("\nErrore! Riprova.\n");
                    continuo();
                    pulisciSchermo();
                }

                switch(scelta)
                {
                    case GIOCATA:
                    {
                        pulisciSchermo();
                        giocata(party, nGiocatori, giocatore, &bangGiocati, mazzoPesca, mazzoScarti, nomeFile);
                        continuo();
                        pulisciSchermo();
                    }
                        break;
                    case CONTROLLO_MANO:
                    {
                        getchar();
                        if(giocatore->mano.numCarte !=0)
                        {
                            stampaCarte(&giocatore->mano);
                        }
                        else
                        {
                            printf("\nNon hai carte in mano!\n");
                        }
                        continuo();
                        pulisciSchermo();
                    }
                        break;
                    case CONTROLLO_INGIOCO:
                    {
                        getchar();
                        if(giocatore->inGioco.numCarte !=0)
                        {
                            stampaCarte(&giocatore->inGioco);
                        }
                        else
                        {
                            printf("\nNon hai carte in gioco!\n");
                        }
                        continuo();
                        pulisciSchermo();
                    }
                        break;
                    case CONTROLLO_DISTANZE:
                    {
                        getchar();

                        int idGiocatore = NOP;

                        for(int j=0; j<(*nGiocatori); j++)
                        {
                            if(comparaNomi(giocatore->nome, party[j].nome))
                            {
                                idGiocatore = j;
                            }
                        }

                        for(int k=0; k<(*nGiocatori); k++)
                        {
                            int distanza = calcoloDistanza(party, &party[idGiocatore], &party[k], *nGiocatori);

                            if(!comparaNomi(giocatore->nome, party[k].nome))
                            {
                                if(party[k].ruolo == SCERIFFO)
                                {
                                    printf("\n%s (SCERIFFO) - %d\n", party[k].nome, distanza);
                                }
                                else
                                {
                                    printf("\n%s - %d\n", party[k].nome, distanza);
                                }
                            }
                        }

                        continuo();
                        pulisciSchermo();
                    }
                        break;
                    case CONTROLLO_ALTRI:
                    {
                        int scelta2 = NOP;

                        for(int j=0; j<(*nGiocatori); j++)
                        {
                            if(!comparaNomi(giocatore->nome, party[j].nome))
                            {
                                if(party[j].ruolo == SCERIFFO)
                                {
                                    printf("\n %d - %s (SCERIFFO)\n", j, party[j].nome);
                                }
                                else
                                {
                                    printf("\n%d - %s\n", j, party[j].nome);
                                }
                            }
                        }

                        do {
                            printf("\nScegli il giocatore di cui vuoi vedere le carte in gioco:\n");
                            scanf("%d", &scelta2);

                            if(scelta2<MIN_SCELTA || scelta2>=(*nGiocatori))
                            {
                                printf("\nErrore! Il giocatore che hai selezionato non esiste.\n");
                            }
                        }while(scelta2<MIN_SCELTA || scelta2>=(*nGiocatori));

                        getchar();
                        if(party[scelta2].inGioco.numCarte!=0)
                        {
                            stampaCarte(&party[scelta2].inGioco);
                        }
                        else
                        {
                            printf("\nIl giocatore non ha carte in gioco!\n");
                        }

                        continuo();
                        pulisciSchermo();
                    }
                        break;
                    case CLOSE:
                    {
                        int chiusura = NOP;

                        do
                        {
                            printf("\n\nSei sicuro di voler chiudere il gioco?"
                                   "\nSe non hai ancora effettuato nessuna mossa perderai il turno!"
                                   "\nAssicurati di aver fatto tutte le mosse che volevi prima di chiudere il gioco!"
                                   "\n[0 = chiudi il gioco, 1 = torna indietro]\n\n");
                            scanf("%d", &chiusura);
                        }while(chiusura<0 || chiusura>1);

                        if(chiusura == 0)
                        {
                            pulisciSchermo();
                            chiudiGioco(party, nGiocatori, mazzoPesca, mazzoScarti, prossimoGiocatore, nTurno, nomeFile);
                        }
                    }
                }
            }
        }while(scelta<MIN_SCELTA || scelta>MAX_SCELTA || scelta != SKIP);

        if(checkFine(party, *nGiocatori) == NOP)
        {
            if(scelta == SKIP)
            {
                printf("\n\nSalti il turno!\n\n");
                char messaggio2[MEX_DIM] = "";
                strcat(messaggio2, giocatore->nome);
                strcat(messaggio2, " salta il turno.");
                salvaLog(messaggio2, nomeFile);
                continuo();
            }
        }
    }

    pulisciSchermo();
}

int checkFine(Giocatore* party, int nGiocatori)
{
    int esito = NOP;
    int nSceriffo = 0, nFuorilegge = 0, nVice = 0, nRinnegato = 0;

    for(int i=0; i<nGiocatori; i++)
    {
        switch(party[i].ruolo)
        {
            case SCERIFFO:
                nSceriffo++;
                break;
            case VICESCERIFFO:
                nVice++;
                break;
            case FUORILEGGE:
                nFuorilegge++;
                break;
            case RINNEGATO:
                nRinnegato++;
                break;
        }
    }

    if(nSceriffo == 0)
    {
        if(nGiocatori>4)
        {
            if(nFuorilegge == 0)
            {
                esito = VITTORIA_RINNEGATO;
            }
            else
            {
                esito = VITTORIA_FUORILEGGE;
            }
        }
        else
        {
            if(nFuorilegge == 0 && nVice == 0)
            {
                esito = VITTORIA_RINNEGATO;
            }
            else
            {
                esito = VITTORIA_FUORILEGGE;
            }
        }
    }
    else
    {
        if(nFuorilegge == 0 && nRinnegato == 0)
        {
            esito = VITTORIA_SCERIFFO;
        }
    }

    return esito;
}

void finePartita(char* nomeFile, int esito)
{
    printf("\n\nFINE PARTITA!!!\n\n");

    continuo();

    if(esito == VITTORIA_SCERIFFO)
    {
        salvaLog("Fine partita, vincono sceriffo e i vice.", nomeFile);
        printf("\nI fuorilegge e il rinnegato sono stati eliminati!"
               "\nVincono lo sceriffo e i suoi vice!!!\n");
    }
    else if(esito == VITTORIA_FUORILEGGE)
    {
        salvaLog("Fine partita, vincono i fuorilegge.", nomeFile);
        printf("\nLo sceriffo e' stato eliminato!"
               "\nVincono i fuorilegge!!!\n");
    }
    else if(esito == VITTORIA_RINNEGATO)
    {
        salvaLog("Fine partita, vince il rinnegato.", nomeFile);
        printf("\nTutti i giocatori tranne il rinnegato sono stati eliminati!"
               "\nVince il rinnegato!!!\n");
    }

    continuo();
    pulisciSchermo();
}

void partita(Giocatore* party, int* nGiocatori, Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile, int prossimoGiocatore, int nTurno)
{
    int i;
    int primo = 0;

    if(nTurno == NOP)
    {
        i = 1;
    }
    else
    {
        i = nTurno;
    }

    do
    {
        char messaggio[MEX_DIM];
        sprintf(messaggio, "Turno numero %d:",i);
        salvaLog(messaggio, nomeFile);

        int idSceriffo = NOP;

        for(int j = 0; j<*nGiocatori; j++)
        {
            if(party[j].ruolo == SCERIFFO)
                idSceriffo = j;
        }

        if(prossimoGiocatore == NOP)
        {
            if(idSceriffo != 0)
            {
                if(checkFine(party, *nGiocatori) == NOP)
                {
                    prossimoGiocatore = primo;
                    printf("");
                    turno(party, nGiocatori, &party[idSceriffo], &i, mazzoPesca, mazzoScarti, idSceriffo, &prossimoGiocatore, nomeFile);
                }
            }
            else
            {
                if(checkFine(party, *nGiocatori) == NOP)
                {
                    prossimoGiocatore = primo+1;
                    turno(party, nGiocatori, &party[idSceriffo], &i, mazzoPesca, mazzoScarti, idSceriffo, &prossimoGiocatore, nomeFile);
                }
            }

            for(int k=0; k<(*nGiocatori); k++)
            {
                if(k != idSceriffo)
                {
                    if(k<(*nGiocatori-1))
                    {
                        if(checkFine(party, *nGiocatori) == NOP)
                        {
                            prossimoGiocatore = k+1;
                            turno(party, nGiocatori, &party[k], &i, mazzoPesca, mazzoScarti, k, &prossimoGiocatore, nomeFile);
                        }
                    }
                    else
                    {
                        if(checkFine(party, *nGiocatori) == NOP)
                        {
                            prossimoGiocatore = idSceriffo;
                            turno(party,nGiocatori, &party[k], &i, mazzoPesca, mazzoScarti, k, &prossimoGiocatore, nomeFile);
                        }
                    }
                }
            }
        }
        else
        {
            if(prossimoGiocatore == idSceriffo)
            {
                if(idSceriffo != 0)
                {
                    if(checkFine(party, *nGiocatori) == NOP)
                    {
                        prossimoGiocatore = primo;
                        turno(party, nGiocatori, &party[idSceriffo], &i, mazzoPesca, mazzoScarti, idSceriffo, &prossimoGiocatore, nomeFile);
                    }
                }
                else
                {
                    if(checkFine(party, *nGiocatori) == NOP)
                    {
                        prossimoGiocatore = primo+1;
                        turno(party, nGiocatori, &party[idSceriffo], &i, mazzoPesca, mazzoScarti, idSceriffo, &prossimoGiocatore, nomeFile);
                    }
                }
            }
            for(int k=0; k<(*nGiocatori); k++)
            {
                if(k != idSceriffo)
                {
                    if(prossimoGiocatore == k)
                    {
                        if(k<(*nGiocatori-1))
                        {
                            if(checkFine(party, *nGiocatori) == NOP)
                            {
                                prossimoGiocatore = k+1;
                                turno(party, nGiocatori, &party[k], &i, mazzoPesca, mazzoScarti, k, &prossimoGiocatore, nomeFile);
                            }
                        }
                        else
                        {
                            if(checkFine(party, *nGiocatori) == NOP)
                            {
                                prossimoGiocatore = idSceriffo;
                                turno(party,nGiocatori, &party[k], &i, mazzoPesca, mazzoScarti, k, &prossimoGiocatore, nomeFile);
                            }
                        }
                    }
                }
            }
        }

        for(int j=0; j<(*nGiocatori); j++)
        {
            scartiEccesso(&party[j], mazzoScarti);
            pulisciSchermo();
        }

        i++;
    }while(checkFine(party, *nGiocatori) == NOP);

    if(checkFine(party, *nGiocatori) != NOP)
    {
        finePartita(nomeFile, checkFine(party, *nGiocatori));
        return;
    }
}
