#include "gioco.h"

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

        spostaCarta(&giocatore->mano, scelta, mazzoScarti, NULL, NULL);
    }
}

bool checkBang(Giocatore* giocatore, int bangGiocati)
{
    bool check = true;

    int idVolcanic = NOP;

    idVolcanic = trovaCartaInGioco(giocatore, VOLCANIC);

    if(idVolcanic == NOP && bangGiocati > 0)
    {
        check = false;
    }

    return check;
}

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

void giocata(Giocatore* party, int *nGiocatori, Giocatore* giocatore, int* bangGiocati, Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile)
{
    int scelta = NOP;

    stampaCarte(&giocatore->mano);

    do
    {
        printf("\nScegli la carta da giocare:\n");
        scanf("%d", &scelta);

        if(scelta<MIN_SCELTA || scelta>giocatore->mano.numCarte)
        {
            printf("\nErrore! Vuoi giocare una carta che non esiste!\n");
        }
        if(!checkBang(giocatore, *bangGiocati))
        {
            printf("\nNon puoi giocare altri BANG! per questo turno.\nPuoi giocare VOLCANIC per giocare quanti BANG! vuoi. Riprova.\n");
        }
        if(!checkCartaBlu(giocatore, scelta))
        {
            printf("\nNon puoi avere piu' carte blu con lo stesso nome in gioco! Riprova.\n");
        }
    }while(scelta<MIN_SCELTA || scelta>giocatore->mano.numCarte || !checkBang(giocatore, *bangGiocati) || !checkCartaBlu(giocatore, scelta));

    getchar();

    switch(giocatore->mano.carte[scelta].tipoC)
    {
        case ISTANTANEA:
        case ISTANTANEA_SPECIAL:
        {
            giocaCartaMarrone(party, nGiocatori, giocatore, scelta, mazzoPesca, mazzoScarti, nomeFile);
            if(comparaNomi(giocatore->mano.carte[scelta].nome, BANG))
            {
                (*bangGiocati)++;
            }
        }
            break;
        case ARMA:
        case EFFETTO:
            giocaCartaBlu(party, *nGiocatori, giocatore, scelta, mazzoScarti, nomeFile);
            break;
    }

    pulisciSchermo();
}

void salvataggio(Giocatore* party, int* nGiocatori, Mazzo* mazzoPesca, Mazzo* mazzoScarti, int* prossimoGiocatore, int *nTurno, char* nomeFile)
{
    FILE* fp = NULL;

    char temp[SAVE_DIM+1];
    strcpy(temp, nomeFile);
    strcat(temp, ".sav");

    fp = fopen(temp, "wb");
    if(fp == NULL)
    {
        printf("\n\nErrore!\n\n");
        exit(EXIT_FAILURE);
    }

    fwrite(nGiocatori, sizeof(int), 1, fp);

    for(int i=0; i<(*nGiocatori); i++)
    {
        fwrite(&party[i], sizeof(Giocatore), 1, fp);
        fwrite(&party[i].mano.carte, sizeof(Carta), party[i].mano.numCarte, fp);
        fwrite(&party[i].inGioco.carte, sizeof(Carta), party[i].inGioco.numCarte, fp);
    }

    if(prossimoGiocatore != NULL)
    {
        fwrite(prossimoGiocatore, sizeof(int), 1, fp);
    }

    fwrite(mazzoPesca, sizeof(Mazzo), 1, fp);
    fwrite(mazzoPesca, sizeof(Carta), mazzoPesca->numCarte, fp);

    fwrite(mazzoScarti, sizeof(Mazzo), 1, fp);
    fwrite(mazzoScarti, sizeof(Carta), mazzoPesca->numCarte, fp);

    fwrite(nTurno, sizeof(int), 1, fp);

    fclose(fp);
}

Giocatore* caricaSalvataggio(Giocatore* party, int* nGiocatori, Mazzo* mazzoPesca, Mazzo* mazzoScarti, int* prossimoGiocatore, int* nTurno, char* nomeFile)
{
    FILE* fp = NULL;

    char temp[SAVE_DIM+1];
    strcpy(temp, nomeFile);
    strcat(temp, ".sav");

    fp = fopen(temp, "rb");
    if(fp == NULL)
    {
        printf("\n\nErrore!\n\n");
        exit(EXIT_FAILURE);
    }

    fread(nGiocatori, sizeof(int), 1, fp);

    party = (Giocatore*) malloc(sizeof(Giocatore) * (*nGiocatori));
    if(party == NULL)
        exit(EXIT_FAILURE);

    for(int i=0; i<(*nGiocatori); i++)
    {
        fread(&party[i], sizeof(Giocatore), 1, fp);

        party[i].mano.carte = (Carta *) malloc(sizeof(Carta) * party[i].mano.numCarte);
        if(party[i].mano.carte == NULL)
            exit(EXIT_FAILURE);

        party[i].inGioco.carte = (Carta *) malloc(sizeof(Carta) * party[i].inGioco.numCarte);
        if(party[i].inGioco.carte == NULL)
            exit(EXIT_FAILURE);

        fread(party[i].mano.carte, sizeof(Carta), party[i].mano.numCarte, fp);
        fread(party[i].inGioco.carte, sizeof(Carta), party[i].inGioco.numCarte, fp);
    }

    fread(prossimoGiocatore, sizeof(int), 1, fp);

    mazzoPesca = (Mazzo *) malloc(sizeof(Mazzo));
    fread(mazzoPesca, sizeof(Mazzo), 1, fp);
    mazzoPesca->carte = (Carta *) malloc(sizeof(Carta) * mazzoPesca->numCarte);
    if(mazzoPesca->carte == NULL)
    {
        exit(EXIT_FAILURE);
    }
    fread(mazzoPesca->carte, sizeof(Carta), mazzoPesca->numCarte, fp);

    mazzoScarti = (Mazzo *) malloc(sizeof(Mazzo));
    fread(mazzoScarti, sizeof(Mazzo), 1, fp);
    mazzoScarti->carte = (Carta *) malloc(sizeof(Carta) * mazzoScarti->numCarte);
    if(mazzoScarti->carte == NULL)
    {
        exit(EXIT_FAILURE);
    }
    fread(mazzoScarti->carte, sizeof(Carta), mazzoScarti->numCarte, fp);

    fclose(fp);

    return party;
}

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

    exit(EXIT_SUCCESS);
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

    if((*nTurno) != 1)
    {
        salvataggio(party, nGiocatori, mazzoPesca, mazzoScarti, &giocatoreAttuale, nTurno, nomeFile);
        salvaLog("Salvataggio completato.", nomeFile);
    }

    printf("\n\nTurno %d - %s - %s - %d pallottole.\n\n", (*nTurno), giocatore->nome, nomeRuolo, giocatore->pallottole);
    char messaggio[MEX_DIM] = "E' il turno di ";
    strcat(messaggio, giocatore->nome);
    strcat( messaggio, ".");
    salvaLog(messaggio, nomeFile);
    continuo();
    printf("\n%s! Peschi due carte dal mazzo.\n", giocatore->nome);
    continuo();
    spostaCarta(mazzoPesca, NOP, &giocatore->mano, mazzoScarti, nomeFile);
    spostaCarta(mazzoPesca, NOP, &giocatore->mano, mazzoScarti, nomeFile);

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

            int i=0, idPrigione = NOP, nGiocate = 0;

            idPrigione = trovaCartaInGioco(giocatore, PRIGIONE);

            if(idPrigione != NOP) {
                printf("\n\nSei in prigione!\nProcedi con l'estrazione per evadere e giocare il turno!\n\n");
                continuo();
                checkPrigione = prigione(giocatore, mazzoPesca, mazzoScarti, nomeFile);
            }
            if(checkPrigione)
            {
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
        }while(scelta<MIN_SCELTA || scelta>MAX_SCELTA || scelta != SKIP);

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
                    turno(party, nGiocatori, &party[idSceriffo], &i, mazzoPesca, mazzoScarti, idSceriffo, &primo, nomeFile);
                }
            }
            else
            {
                if(checkFine(party, *nGiocatori) == NOP)
                {
                    turno(party, nGiocatori, &party[idSceriffo], &i, mazzoPesca, mazzoScarti, idSceriffo, &primo+1, nomeFile);
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
                            turno(party, nGiocatori, &party[k], &i, mazzoPesca, mazzoScarti, k, &k+1, nomeFile);
                        }
                    }
                    else
                    {
                        if(checkFine(party, *nGiocatori) == NOP)
                        {
                            turno(party,nGiocatori, &party[k], &i, mazzoPesca, mazzoScarti, k, &idSceriffo, nomeFile);
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
                        turno(party, nGiocatori, &party[idSceriffo], &i, mazzoPesca, mazzoScarti, idSceriffo, &primo, nomeFile);
                    }
                }
                else
                {
                    if(checkFine(party, *nGiocatori) == NOP)
                    {
                        turno(party, nGiocatori, &party[idSceriffo], &i, mazzoPesca, mazzoScarti, idSceriffo, &primo+1, nomeFile);
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
                                turno(party, nGiocatori, &party[k], &i, mazzoPesca, mazzoScarti, k, &k+1, nomeFile);
                            }
                        }
                        else
                        {
                            if(checkFine(party, *nGiocatori) == NOP)
                            {
                                turno(party,nGiocatori, &party[k], &i, mazzoPesca, mazzoScarti, k, &idSceriffo, nomeFile);
                            }
                        }
                    }
                }
            }
        }

        for(int j=0; j<(*nGiocatori); j++)
        {
            scartiEccesso(&party[j], mazzoScarti);
        }

        i++;
    }while(checkFine(party, *nGiocatori) == NOP);

    if(checkFine(party, *nGiocatori) != NOP)
    {
        finePartita(nomeFile, checkFine(party, *nGiocatori));
    }
}