#include "carte.h"

bool estrazione(int modalita, Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile)
{
    bool esito = false;
    int idCarta = PRIMO;

    printf("\nEstrazione della carta.\n");
    salvaLog("Viene estratta una carta dal mazzo.", nomeFile);
    continuo();

    Mazzo* temp = (Mazzo*) malloc(sizeof(Mazzo));
    temp->carte = NULL;
    temp->numCarte = 0;

    spostaCarta(mazzoPesca, NOP, temp, mazzoScarti, nomeFile);

    char seme[DIM_NOME_CARTE+1];

    switch(temp->carte[idCarta].seme)
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

    printf("\nCarta estratta: %d di %s\n", temp->carte[PRIMO].num, seme);
    continuo();

    if(modalita == MOD_PRIGIONE || modalita == MOD_BARILE)
    {
        if(temp->carte[idCarta].seme == CUORI)
        {
            esito = true;
        }
    }
    else
    {
        if(temp->carte[idCarta].seme == PICCHE && (temp->carte[idCarta].num>=MIN_DINAMITE && temp->carte[idCarta].num<=MAX_DINAMITE))
        {
            esito = true;
        }
    }

    spostaCarta(temp, idCarta, mazzoScarti, NULL, nomeFile);

    free(temp);

    return esito;
}

void armi(Giocatore* giocatore)
{
    int idCarta = NOP;

    for(int i=0; i<giocatore->inGioco.numCarte; i++)
    {
        if(giocatore->inGioco.carte[i].tipoC == ARMA)
        {
            idCarta = i;
        }
    }

    if(idCarta != NOP)
    {
        if(comparaNomi(giocatore->inGioco.carte[idCarta].nome, VOLCANIC))
            giocatore->gittata = GITTATA_VOLCANIC;

        else if(comparaNomi(giocatore->inGioco.carte[idCarta].nome, SCHOFIELD))
            giocatore->gittata = GITTATA_SCHOFIELD;

        else if(comparaNomi(giocatore->inGioco.carte[idCarta].nome, REMINGTON))
            giocatore->gittata = GITTATA_REMINGTON;

        else if(comparaNomi(giocatore->inGioco.carte[idCarta].nome, CARABINA))
            giocatore->gittata = GITTATA_CARABINA;

        else if(comparaNomi(giocatore->inGioco.carte[idCarta].nome, WINCHESTER))
            giocatore->gittata = GITTATA_WINCHESTER;
    }
}

bool barile(Giocatore* giocatore, int carta, Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile)
{
    bool esito = false;

    if(estrazione(MOD_BARILE, mazzoPesca, mazzoScarti, nomeFile))
    {
        printf("\nEsce CUORI! Il BANG! non ha effetto!\n");

        char messaggio[MEX_DIM] = "L'estrazione va a buon fine, ";
        strcat(messaggio, giocatore->nome);
        strcat(messaggio, " si difende dal Bang! grazie a Barile");
        salvaLog(messaggio, nomeFile);
        esito = true;

        spostaCarta(&giocatore->inGioco, carta, mazzoScarti, NULL, nomeFile);
    }
    else
    {
        printf("\nNon e' uscito CUORI, BARILE non ha effetto!\n");
        salvaLog("L'estrazione non va a buon fine, Barile non ha effetto.", nomeFile);
        spostaCarta(&giocatore->inGioco, carta, mazzoScarti, NULL, nomeFile);
    }

    continuo();

    return esito;
}

bool dinamite(Giocatore* party, int* nGiocatori, Giocatore* giocatore, Mazzo* mazzoPesca, Mazzo* mazzoScarti, int carta, char* nomeFile)
{
    bool check = false;

    Giocatore* dinamite = (Giocatore*) malloc(sizeof(Giocatore));
    strcpy(dinamite->nome, DINAMITE);

    if(!estrazione(MOD_DINAMITE,mazzoPesca, mazzoScarti, nomeFile))
    {
        printf("\nLa DINAMITE esplode!\n");

        spostaCarta(&giocatore->inGioco, carta, mazzoScarti, NULL, nomeFile);

        if(giocatore->pallottole <= DANNO_DINAMITE)
        {
            giocatore->pallottole = 0;

            int idBirra = NOP;
            idBirra = trovaCartaMano(giocatore, BIRRA);

            if(idBirra != NOP)
            {
                printf("\nSei stato colpito!\n");
                char messaggio[MEX_DIM] = "L'estrazione non va a buon fine, ";
                strcat(messaggio, giocatore->nome);
                strcat(messaggio, " subisce il danno di Dinamite.");
                salvaLog(messaggio, nomeFile);

                if(birraExtremis(giocatore, nomeFile))
                {
                    spostaCarta(&giocatore->mano, idBirra, mazzoScarti, NULL, nomeFile);
                    (giocatore->pallottole)++;
                }
                else
                {
                    eliminazione(party, dinamite, giocatore, nGiocatori, mazzoPesca, mazzoScarti, nomeFile);
                    check = true;
                }
            }
        }
        else
        {
            printf("\nSei stato colpito!\n");
            char messaggio[MEX_DIM] = "L'estrazione non va a buon fine, ";
            strcat(messaggio, giocatore->nome);
            strcat(messaggio, " subisce il danno di Dinamite");
            salvaLog(messaggio, nomeFile);
            giocatore->pallottole -= DANNO_DINAMITE;
        }
    }
    else
    {
        printf("\nLa DINAMITE non esplode!\nLa DINAMITE passa al giocatore successivo.\n");
        char messaggio[MEX_DIM] = "L'estrazione va a buon fine, ";
        strcat(messaggio, "la dinamite passa da ");
        strcat(messaggio, giocatore->nome);

        int idGiocatore = NOP;

        for(int i=0; i<(*nGiocatori); i++)
        {
            if(comparaNomi(giocatore->nome, party[i].nome))
                idGiocatore = i;
        }

        if(idGiocatore == (*nGiocatori)-1)
        {
            spostaCarta(&giocatore->inGioco, carta, &party[PRIMO].inGioco, NULL, nomeFile);
            strcat(messaggio, " a ");
            strcat(messaggio, party[PRIMO].nome);
        }
        else
        {
            spostaCarta(&giocatore->inGioco, carta, &party[idGiocatore+1].inGioco, NULL, nomeFile);
            strcat(messaggio, " a ");
            strcat(messaggio, party[idGiocatore+1].nome);
        }

        salvaLog(messaggio, nomeFile);
    }

    free(dinamite);

    continuo();
    pulisciSchermo();

    return check;
}

bool prigione(Giocatore* giocatore, Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile)
{
    bool esito = false;
    int idPrigione = NOP;

    idPrigione = trovaCartaInGioco(giocatore, PRIGIONE);

    if(idPrigione != NOP)
    {
        if(estrazione(MOD_PRIGIONE, mazzoPesca, mazzoScarti, nomeFile))
        {
            printf("\nEsce CUORI! Scarti la carta e evadi di prigione!\n");
            char messaggio[MEX_DIM] = "L'estrazione va a buon fine, ";
            strcat(messaggio, giocatore->nome);
            strcat(messaggio, "evade di prigione.");
            salvaLog(messaggio, nomeFile);
            esito = true;
        }
        else
        {
            printf("\nNon esce CUORI! Scarti la carta!\n");
            salvaLog("L'estrazione non va a buon fine.", nomeFile);
        }
    }

    spostaCarta(&giocatore->inGioco, idPrigione, mazzoScarti, NULL, nomeFile);

    continuo();
    pulisciSchermo();
    return esito;
}

void giocaBarile(Giocatore* giocatore, int carta, char* nomeFile)
{
    printf("\n\nStai giocando BARILE.\nLa carta e' ora in gioco!\n\n");
    char messaggio[MEX_DIM] = "";
    strcat(messaggio, giocatore->nome);
    strcat(messaggio, "gioca Barile.");
    salvaLog(messaggio, nomeFile);
    continuo();
    spostaCarta(&giocatore->mano, carta, &giocatore->inGioco, NULL, nomeFile);
}

void giocaDinamite(Giocatore* giocatore, int carta, char* nomeFile)
{
    printf("\n\nStai giocando DINAMITE.\nLa carta e' ora in gioco!\n");
    char messaggio[MEX_DIM] = "";
    strcat(messaggio, giocatore->nome);
    strcat(messaggio, "gioca Dinamite.");
    salvaLog(messaggio, nomeFile);
    continuo();
    spostaCarta(&giocatore->mano, carta, &giocatore->inGioco, NULL, nomeFile);
}

void giocaPrigione(Giocatore* party, int nGiocatori, Giocatore* giocatore, int carta, char* nomeFile)
{
    printf("\n\nStai giocando PRIGIONE.\n\n");

    for(int i=0; i<nGiocatori; i++)
    {
        if(party[i].ruolo != SCERIFFO && !comparaNomi(giocatore->nome, party[i].nome))
        {
            printf("\n%d - %s.", i, party[i].nome);
        }
    }

    int scelta = NOP;

    do
    {
        printf("\nScegli il giocatore da mettere in prigione:");
        scanf("%d", &scelta);

        if(scelta<0 || scelta>= nGiocatori)
        {
            printf("\nErrore! Riprova.\n");
        }
        else if(party[scelta].ruolo == SCERIFFO)
        {
            printf("\nNon puoi mettere in prigione lo SCERIFFO! Riprova.\n");
        }
        else if(comparaNomi(giocatore->nome, party[scelta].nome))
        {
            printf("\nNon puoi mettere te stesso in prigione! Riprova.\n");
        }
    }while(scelta<0 || scelta>= nGiocatori || party[scelta].ruolo == SCERIFFO || comparaNomi(giocatore->nome, party[scelta].nome));

    if(scelta != NOP)
    {
        printf("\n%s VA IN PRIGIONE!\n", party[scelta].nome);
        char messaggio[MEX_DIM] = "";
        strcat(messaggio, giocatore->nome);
        strcat(messaggio, " gioca Prigione su ");
        strcat(messaggio, party[scelta].nome);
        salvaLog(messaggio, nomeFile);
        spostaCarta(&giocatore->mano, carta, &party[scelta].inGioco, NULL, nomeFile);
    }
}

void giocaArma(Giocatore* giocatore, int carta, Mazzo* mazzoScarti, char* nomeFile)
{
    if(comparaNomi(giocatore->mano.carte[carta].nome, VOLCANIC))
    {
        printf("\n\nStai giocando VOLCANIC\nAvrai gittata pari a %d\ne potrai giocare quanti BANG! vuoi a ogni turno.\n\n", GITTATA_VOLCANIC);
    }
    else if(comparaNomi(giocatore->mano.carte[carta].nome, SCHOFIELD))
    {
        printf("\n\nStai giocando SCHOFIELD.\nAvrai gittata pari a %d.\n\n", GITTATA_SCHOFIELD);
    }
    else if(comparaNomi(giocatore->mano.carte[carta].nome, REMINGTON))
    {
        printf("\n\nStai giocando REMINGTON.\nAvrai gittata pari a %d.\n\n", GITTATA_REMINGTON);
    }
    else if(comparaNomi(giocatore->mano.carte[carta].nome, CARABINA))
    {
        printf("\n\nStai giocando CARABINA.\nAvrai gittata pari a %d.\n\n", GITTATA_CARABINA);
    }
    else if(comparaNomi(giocatore->mano.carte[carta].nome, WINCHESTER))
    {
        printf("\n\nStai giocando WINCHESTER.\nAvrai gittata pari a %d.\n\n", GITTATA_WINCHESTER);
    }

    int idArma = NOP;

    for(int i=0; i<giocatore->inGioco.numCarte; i++)
    {
        if(giocatore->inGioco.carte[i].tipoC == ARMA)
        {
            idArma = i;
        }
    }

    if(idArma != NOP)
    {
        printf("\n\nHai un'altra arma in gioco, questa viene scartata e sostituita con l'arma da giocare.\n\n");
        spostaCarta(&giocatore->inGioco, idArma, mazzoScarti, NULL, nomeFile);
        char messaggio[MEX_DIM] = "";
        strcat(messaggio, giocatore->nome);
        strcat(messaggio, " scarta ");
        strcat(messaggio, giocatore->inGioco.carte[idArma].nome);
        strcat(messaggio, " per giocare la nuova arma.");
        salvaLog(messaggio, nomeFile);
    }
    else
    {
        printf("\n\nLa carta di tipo arma e' ora in gioco!\n\n");
        char messaggio[MEX_DIM] = "";
        strcat(messaggio, giocatore->nome);
        strcat(messaggio, "gioca ");
        strcat(messaggio, giocatore->mano.carte[carta].nome);
        strcat(messaggio, ".");
        salvaLog(messaggio, nomeFile);
    }

    spostaCarta(&giocatore->mano, carta, &giocatore->inGioco, NULL, nomeFile);

    continuo();
    pulisciSchermo();
}

void giocaCartaBlu(Giocatore* party, int nGiocatori, Giocatore* giocatore, int carta, Mazzo* mazzoScarti, char* nomeFile)
{
    if(giocatore->mano.carte[carta].tipoC == ARMA)
    {
        giocaArma(giocatore, carta, mazzoScarti, nomeFile);
        armi(giocatore);
    }
    else if(giocatore->mano.carte[carta].tipoC == EFFETTO)
    {
        if(comparaNomi(giocatore->mano.carte[carta].nome, DINAMITE))
        {
            giocaDinamite(giocatore, carta, nomeFile);
        }
        else if(comparaNomi(giocatore->mano.carte[carta].nome, BARILE))
        {
            giocaBarile(giocatore, carta, nomeFile);
        }
        else if(comparaNomi(giocatore->mano.carte[carta].nome, PRIGIONE))
        {
            giocaPrigione(party, nGiocatori, giocatore, carta, nomeFile);
        }
        else if(comparaNomi(giocatore->mano.carte[carta].nome, MUSTANG))
        {
            printf("\n\nStai giocando MUSTANG.\nLa carta e' ora in gioco, i nemici ti vedranno a una distanza più grande!\n\n");
            char messaggio[MEX_DIM] = "";
            strcat(messaggio, giocatore->nome);
            strcat(messaggio, " gioca Mustang.");
            salvaLog(messaggio, nomeFile);
            spostaCarta(&giocatore->mano, carta, &giocatore->inGioco, NULL, nomeFile);

            continuo();
            pulisciSchermo();
        }
        else if(comparaNomi(giocatore->mano.carte[carta].nome, MIRINO))
        {
            printf("\n\nStai giocando MIRINO.\nLa carta e' ora in gioco, vedrai i nemici a una distanza più piccola!\n\n");
            char messaggio[MEX_DIM] = "";
            strcat(messaggio, giocatore->nome);
            strcat(messaggio, " gioca Mirino.");
            salvaLog(messaggio, nomeFile);
            spostaCarta(&giocatore->mano, carta, &giocatore->inGioco, NULL, nomeFile);

            continuo();
            pulisciSchermo();
        }
    }
}