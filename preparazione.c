#include "preparazione.h"

int randRange(int min, int max)
{
    int n = min + rand() % (max - min + 1);
    return n;
}

void scambioCarte(Carta carta1, Carta carta2, Carta* scambio1, Carta* scambio2)
{
    *scambio1 = carta2;
    *scambio2 = carta1;
}

void letturaCarte (Mazzo* mazzoPesca)
{
    FILE *fp = NULL;

    fp = fopen("mazzo_bang.txt", "r");

    if(fp == NULL)
        exit(EXIT_FAILURE);

    for(int i=0;i<mazzoPesca->numCarte;i++)
    {
        fscanf(fp, "%d %d %d %s\n", &mazzoPesca->carte[i].tipoC, &mazzoPesca->carte[i].num, &mazzoPesca->carte[i].seme,mazzoPesca->carte[i].nome);
    }

    fclose(fp);
}

void creaMazzoPesca(Mazzo* mazzoPesca)
{
    mazzoPesca->tipoM = MAZZO_PESCA;

    mazzoPesca->numCarte = NUMERO_CARTE;

    mazzoPesca->carte = (Carta*) malloc(sizeof(Carta) * mazzoPesca->numCarte);

    if(mazzoPesca->carte == NULL)
        exit(EXIT_FAILURE);

    letturaCarte(mazzoPesca);
}

void creaMazzoScarti(Mazzo* mazzoScarti)
{
    mazzoScarti->tipoM = MAZZO_SCARTO;

    mazzoScarti->numCarte = 0;

    mazzoScarti->carte = NULL;
}

void mescolaCarte(Mazzo* mazzo)
{
    for(int i=0; i<mazzo->numCarte; i++)
    {
        int carta2 = randRange(0, mazzo->numCarte - 1);

        scambioCarte(mazzo->carte[i], mazzo->carte[carta2], &mazzo->carte[i], &mazzo->carte[carta2]);
    }
}

void spostaCarta(Mazzo* mazzo1, int carta, Mazzo* mazzo2, Mazzo* mazzoScarti, char* nomeFile)
{
    if(mazzo1->tipoM == MAZZO_PESCA && checkMazzoPesca(mazzo1) && mazzoScarti != NULL)
    {
        cambioMazzo(mazzoScarti, mazzo1, nomeFile);
    }

    int aumento = mazzo2->numCarte + 1;
    Carta* temp = (Carta*) realloc(mazzo2->carte, sizeof(Carta) * aumento);

    if (temp != NULL)
    {
        mazzo2->carte = temp;
        if(mazzo1->tipoM == MAZZO_PESCA)
        {
            mazzo2->carte[mazzo2->numCarte] = mazzo1->carte[PRIMO];
        }
        else
        {
            mazzo2->carte[mazzo2->numCarte] = mazzo1->carte[carta];
        }
        mazzo2->numCarte++;

        int decremento = mazzo1->numCarte - 1;

        int indice = NOP;

        if(mazzo1->tipoM == MAZZO_PESCA)
        {
            indice = PRIMO;
        }
        else if(carta != NOP)
        {
            indice = carta;
        }

        for (int i = indice; i < decremento; i++)
        {
            mazzo1->carte[i] = mazzo1->carte[i + 1];
        }

        if (decremento == 0)
        {
            free(mazzo1->carte);
            mazzo1->carte = NULL;
        }
        else
        {
            temp = (Carta*) realloc(mazzo1->carte, sizeof(Carta) * decremento);
            if (temp != NULL)
            {
                mazzo1->carte = temp;
            }
            mazzo1->numCarte--;

        }
    }
}

bool checkMazzoPesca(Mazzo* mazzoPesca)
{
    if(mazzoPesca->numCarte == 0)
        return true;

    else return false;
}

void cambioMazzo(Mazzo* mazzoScarti, Mazzo* mazzoPesca, char* nomeFile)
{
    if(mazzoPesca->carte == NULL)
    {
        Carta* temp = (Carta*) malloc(sizeof(Carta) * mazzoScarti->numCarte);

        if(temp != NULL)
        {
            mazzoPesca->carte = temp;
        }

        if(mazzoPesca->carte == NULL)
            exit(EXIT_FAILURE);

        mazzoPesca->numCarte = mazzoScarti->numCarte;
    }

    mescolaCarte(mazzoScarti);

    for(int i=0; i<mazzoScarti->numCarte; i++)
    {
        mazzoPesca->carte[i] = mazzoScarti->carte[i];
    }

    free(mazzoScarti->carte);
    mazzoScarti->carte = NULL;
    mazzoScarti->numCarte = 0;

    salvaLog("Mazzo di pesca vuoto, cambio mazzo.", nomeFile);
}

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

void continuo()
{
    printf("\nPremi invio per continuare:");
    getchar();
}

bool comparaNomi(char* nome1, char* nome2)
{
    if(strcmp(nome1,nome2) == 0)
        return true;

    else return false;
}

void creaGiocatori(Giocatore** party, int nGiocatori, char* nomeFile)
{
    Giocatore* temp = (Giocatore*) malloc(nGiocatori * sizeof(Giocatore));

    if(temp != NULL)
    {
        *party = temp;

        for(int i=0; i<nGiocatori; i++)
        {
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

            (*party)[i].mano.carte = NULL;
            (*party)[i].mano.tipoM = GIOC_MANO;
            (*party)[i].mano.numCarte = 0;

            (*party)[i].inGioco.carte = NULL;
            (*party)[i].inGioco.tipoM = GIOC_GIOCO;
            (*party)[i].inGioco.numCarte = 0;

            (*party)[i].gittata = GITTATA_COLT_45;

            char messaggio[MEX_DIM] = "Creato giocatore: ";
            strcat(messaggio, (*party)[i].nome);

            salvaLog(messaggio, nomeFile);

            continuo();
            pulisciSchermo();
        }
    }
}

void generaRuoli(Giocatore* party, int nGiocatori, char* nomeFile)
{
    int sceriffo ,vicesceriffo, fuorilegge, rinnegato;

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
            party[i].ruolo = randRange(MIN_RUOLI, MAX_RUOLI);

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
    }while(sceriffo != maxSceriffo || vicesceriffo != maxVice || fuorilegge != maxFuorilegge || rinnegato != maxRinnegato);

    salvaLog("Ruoli assegnati ai giocatori", nomeFile);
}

void lifePoints(Giocatore* party, int nGiocatori, char* nomeFile)
{
    for(int i = 0; i < nGiocatori; i++)
    {
        if(party[i].ruolo == ID_SCERIFFO)
            party[i].pallottole = HP_SCERIFFO;
        else
            party[i].pallottole = HP;
    }

    salvaLog("Pallottole assegnate ai giocatori.", nomeFile);
}

void rivelaRuoli(Giocatore* party, int nGiocatori, char* nomeFile)
{
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

void distribuisciCarte(Giocatore* party, int nGiocatori, Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile)
{
    for(int i=0; i<nGiocatori; i++)
    {
        printf("\n\n%s!\nRiceverai delle carte dal mazzo di pesca, tante quante sono le tue pallottole!\n\n", party[i].nome);

        do
        {
            spostaCarta(mazzoPesca, NOP, &party[i].mano, mazzoScarti, nomeFile);
        }while(party[i].mano.numCarte != party[i].pallottole);

        continuo();
        pulisciSchermo();
    }

    salvaLog("Carte distribuite ai giocatori.", nomeFile);
}

Giocatore* nuovoParty(int* nGiocatori, char* nomeFile)
{
    Giocatore* party = NULL;

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

    if(party == NULL)
        exit(EXIT_FAILURE);

    generaRuoli(party, *nGiocatori, nomeFile);

    lifePoints(party, *nGiocatori, nomeFile);

    salvaLog("Party di giocatori creato.", nomeFile);

    return party;
}

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

int trovaCartaMano(Giocatore* giocatore, char* nomeCarta)
{
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

int trovaCartaInGioco(Giocatore* giocatore, char* nomeCarta)
{

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

bool checkBonus(Giocatore* eliminato)
{
    bool check = false;
    if(eliminato->ruolo == FUORILEGGE)
        check = true;

    return check;
}

bool checkMalus(Giocatore* attaccante, Giocatore* eliminato)
{
    bool check = false;
    if(attaccante->ruolo == SCERIFFO && eliminato->ruolo == FUORILEGGE)
        check = true;

    return check;
}

void bonus(Giocatore* attaccante, Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile)
{
    char messaggio[MEX_DIM] = "";
    strcat(messaggio, attaccante->nome);
    strcat(messaggio, " ha eliminato un fuorilegge, riceve la ricompensa sulla taglia.");
    salvaLog(messaggio, nomeFile);

    printf("\n%s! Hai eliminato un fuorilegge,\nVieni premiato per le tue gesta!\nRicevi una ricompensa per la taglia sul giocatore eliminato!\n", attaccante->nome);
    continuo();
    pulisciSchermo();

    for(int i=0; i < BONUS; i++)
    {
        spostaCarta(mazzoPesca, NOP, &attaccante->mano, mazzoScarti, nomeFile);
    }
}

void malus(Giocatore* attaccante, Mazzo* mazzoScarti, char* nomeFile)
{
    char messaggio[MEX_DIM] = "";
    strcat(messaggio, attaccante->nome);
    strcat(messaggio, " ha eliminato un vicesceriffo, perde tutte le sue carte.");
    salvaLog(messaggio, nomeFile);

    printf("\n%s! Hai eliminato un tuo vice,\nverrai disonorato e perderai tutti i tuoi averi!\nNon mollare e rifatti un nome!\n", attaccante->nome);
    continuo();
    pulisciSchermo();

    for(int i = 0; i<attaccante->mano.numCarte; i++)
    {
        spostaCarta(&attaccante->mano, i, mazzoScarti, mazzoScarti, nomeFile);
    }
}

void eliminazione(Giocatore* party, Giocatore* attaccante, Giocatore* eliminato, int* nGiocatori,Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile)
{
    printf("\n\n%s E' STATO ELIMINATO!!!!\n\n", eliminato->nome);
    char messaggio2[MEX_DIM] = "";
    strcat(messaggio2, eliminato->nome);
    strcat(messaggio2, " e' eliminato dal gioco.");
    salvaLog(messaggio2, nomeFile);
    continuo();
    pulisciSchermo();

    for(int i=0; i<eliminato->mano.numCarte; i++)
    {
        spostaCarta(&eliminato->mano, i, mazzoScarti, NULL, nomeFile);
    }
    for(int i=0; i<eliminato->inGioco.numCarte; i++)
    {
        spostaCarta(&eliminato->inGioco, i, mazzoScarti, NULL, nomeFile);
    }

    if(!comparaNomi(attaccante->nome, eliminato->nome) || !comparaNomi(attaccante->nome, DINAMITE))
    {
        if(checkBonus(eliminato))
            bonus(attaccante, mazzoPesca, mazzoScarti, nomeFile);
        else if(checkMalus(attaccante, eliminato))
            malus(attaccante, mazzoScarti, nomeFile);
    }

    int idEliminato = NOP;

    for(int i=0; i<*nGiocatori; i++)
    {
        if(comparaNomi(eliminato->nome, party[i].nome))
        {
            idEliminato = i;
        }
    }

    if(idEliminato != NOP)
    {
        if(idEliminato != (*nGiocatori)-1)
        {
            for(int j = idEliminato; j<(*nGiocatori-1); j++)
            {
                party[j] = party[j+1];
            }
        }

        (*nGiocatori) --;

        free(eliminato->inGioco.carte);
        free(eliminato->mano.carte);
        free(eliminato);

        Giocatore* temp = realloc(party, sizeof(Giocatore) * (*nGiocatori));

        if(temp != NULL)
        {
            party = temp;
        }
    }
}

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

void pulisciSchermo()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}