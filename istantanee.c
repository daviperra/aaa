#include "carte.h"

int calcoloDistanza(Giocatore* party, Giocatore* attaccante, Giocatore* bersaglio, int nGiocatori)
{
    int idAttaccante = NOP, idBersaglio = NOP, distanza = 0, distanzaEffettiva = 0, distanzaCircolare = 0;

    for(int i=0; i<nGiocatori; i++)
    {
        if(comparaNomi(attaccante->nome,party[i].nome))
            idAttaccante = i;

        if(comparaNomi(bersaglio->nome,party[i].nome))
            idBersaglio = i;
    }

    distanzaEffettiva = idBersaglio - idAttaccante;

    distanzaCircolare = (nGiocatori - distanzaEffettiva) % nGiocatori;

    distanza = (distanzaEffettiva < distanzaCircolare) ? distanzaEffettiva : distanzaCircolare;

    int idMirino = NOP, idMustang = NOP;

    idMirino = trovaCartaInGioco(attaccante, MIRINO);
    idMustang = trovaCartaInGioco(bersaglio, MUSTANG);

    if(idMirino != NOP)
    {
        if(distanza > 1)
            distanza--;
    }

    if(idMustang != NOP)
    {
        distanza++;
    }

    return distanza;
}

void diligenza(Giocatore* giocatore, Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile)
{
    printf("\n\nStai giocando DILIGENZA.\nPeschi %d carte dall mazzo.\n\n", CARTE_DILIGENZA);
    char messaggio[MEX_DIM] = "";
    strcat(messaggio, giocatore->nome);
    strcat(messaggio, " pesca 2 carte giocando Diligenza.");
    salvaLog(messaggio, nomeFile);
    continuo();
    pulisciSchermo();

    for(int i=0; i<CARTE_DILIGENZA; i++)
    {
        spostaCarta(mazzoPesca, NOP, &giocatore->mano, mazzoScarti, nomeFile);
    }
}

void wellsFargo(Giocatore* giocatore, Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile)
{
    printf("\n\nStai giocando WELLSFARGO.\nPeschi %d carte dall mazzo.\n\n", CARTE_WELLS);
    char messaggio[MEX_DIM] = "";
    strcat(messaggio, giocatore->nome);
    strcat(messaggio, " pesca 3 carte giocando WellsFargo.");
    salvaLog(messaggio, nomeFile);
    continuo();
    pulisciSchermo();
    for(int i=0; i<CARTE_WELLS; i++)
    {
        spostaCarta(mazzoPesca, NOP, &giocatore->mano, mazzoScarti, nomeFile);
    }
}

void catBalou(Giocatore* party, Giocatore* attaccante, Mazzo* mazzoScarti, int nGiocatori, char* nomeFile)
{
    int bersaglio = NOP;
    bool carteDisponibili = false;

    printf("\nStai giocando CAT BALOU.\n");
    continuo();

    for(int i=0; i<nGiocatori; i++)
    {
        printf("\n%d - %s, %d carte in mano, %d carte in gioco", i, party[i].nome, party[i].mano.numCarte, party[i].inGioco.numCarte);
        if(party[i].mano.numCarte>0 || party[i].inGioco.numCarte>0)
            carteDisponibili = true;
    }

    if(carteDisponibili)
    {
        do {
            printf("\nScegli il giocatore da colpire: ");
            scanf("%d", &bersaglio);

            if(bersaglio < 0 || bersaglio > nGiocatori-1)
                printf("\n\nErrore! Stai provando a colpire un giocatore che non esiste! Riprova.\n");

            if(comparaNomi(attaccante->nome,party[bersaglio].nome))
                printf("\n\nErrore! Non puoi colpirti da solo! Riprova.\n");

        }while(bersaglio < 0 || bersaglio > nGiocatori-1 || comparaNomi(attaccante->nome,party[bersaglio].nome));

        int var1,carta;

        do
        {
            printf("\n\nVuoi colpire la mano del bersaglio o le sue carte in gioco?\n[0 ---> mano, 1 ---> in gioco]:");
            scanf("%d", &var1);

            if(var1<0 || var1>1)
                printf("\n\nErrore! Riprova.");

            if(var1 == 0 && party[bersaglio].mano.numCarte == 0)
                printf("\n\nErrore! Il giocatore non ha carte in mano! Riprova.");

            if(var1 == 1 && party[bersaglio].inGioco.numCarte == 0)
                printf("\n\nErrore! Il giocatore non ha carte in gioco! Riprova.");

        }while(var1<0 || var1>1 || (var1 == 0 && party[bersaglio].mano.numCarte == 0) || (var1 == 1 && party[bersaglio].inGioco.numCarte == 0));

        if(var1 == 0)
        {
            carta = randRange(0, (party[bersaglio].mano.numCarte - 1));

            spostaCarta(&party[bersaglio].mano, carta, mazzoScarti, NULL, nomeFile);

            char messaggio[MEX_DIM] = "";
            strcat(messaggio, attaccante->nome);
            strcat(messaggio, " fa scartare una carta proveniente dalla mano di ");
            strcat(messaggio, party[bersaglio].nome);
            strcat( messaggio, " giocando CatBalou.");
            salvaLog(messaggio, nomeFile);

            printf("\n\nUna carta e' stata scartata dal giocatore colpito!\n\n");
        }

        else if(var1 == 1)
        {
            for(int i=0; i<party[bersaglio].inGioco.numCarte; i++)
            {
                printf("\n%d - %s\n", i, party[bersaglio].inGioco.carte[i].nome);
            }

            do
            {
                printf("\nScegli la carta da far scartare al bersaglio:");

                scanf("%d", &carta);

                if(carta<0 || carta>=party[bersaglio].inGioco.numCarte)
                    printf("\n\nErrore! Hai scelto una carta che non esiste! Riprova.\n");

            }while(carta<0 || carta>=party[bersaglio].inGioco.numCarte);

            spostaCarta(&party[bersaglio].inGioco, carta, mazzoScarti, NULL, nomeFile);

            char messaggio[MEX_DIM] = "";
            strcat(messaggio, attaccante->nome);
            strcat(messaggio, " fa scartare ");
            strcat(messaggio, party[bersaglio].inGioco.carte[carta].nome);
            strcat(messaggio, " a ");
            strcat(messaggio, party[bersaglio].nome);
            strcat(messaggio, " giocando CatBalou.");
            salvaLog(messaggio, nomeFile);

            printf("\n\nLa carta e' stata scartata dal giocatore!\n\n");
        }
    }
    else
    {
        printf("\n\nNessun avversario ha carte in mano o in gioco!! La carta viene giocata senza effetto!\n\n");
        char messaggio[MEX_DIM] = "";
        strcat(messaggio, attaccante->nome);
        strcat(messaggio, " gioca CatBalou senza colpire nessuno.");
        salvaLog(messaggio, nomeFile);
    }

    getchar();
    continuo();
    pulisciSchermo();
}

void panico(Giocatore* party, Giocatore* attaccante, int nGiocatori, char* nomeFile)
{
    int bersaglio = NOP;
    bool carteDisponibili = false;

    pulisciSchermo();
    printf("\nStai giocando PANICO!\n");
    continuo();

    int distanza = NOP;

    for(int i=0; i<nGiocatori; i++)
    {
        if(!comparaNomi(attaccante->nome, party[i].nome))
        {
            distanza = calcoloDistanza(party, attaccante, &party[i],nGiocatori);
            if(distanza == GITTATA_COLT_45)
            {
                printf("\n%d - %s, %d carte in mano, %d carte in gioco", i, party[i].nome, party[i].mano.numCarte, party[i].inGioco.numCarte);
            }
            if(party[i].mano.numCarte>0 || party[i].inGioco.numCarte>0)
            {
                carteDisponibili = true;
            }
        }
    }

    if(carteDisponibili)
    {
        do {
            printf("\nScegli il giocatore da colpire: ");
            scanf("%d", &bersaglio);
            getchar();

            distanza = calcoloDistanza(party, attaccante, &party[bersaglio],nGiocatori);

            if(bersaglio<0 || bersaglio > nGiocatori-1)
            {
                printf("\n\nErrore!\nStai provando a colpire un giocatore che non esiste. Riprova.\n");
            }
            else
            {
                if(distanza != GITTATA_COLT_45)
                    printf("\n\nErrore! Non puoi colpire questo giocatore! Riprova.\n");

                if(comparaNomi(attaccante->nome,party[bersaglio].nome))
                    printf("\n\nErrore! Non puoi colpirti da solo! Riprova.\n");
            }

        }while(bersaglio<0 || bersaglio > nGiocatori-1 || distanza != GITTATA_COLT_45 || comparaNomi(attaccante->nome,party[bersaglio].nome));

        int var1,carta;

        do
        {
            printf("\n\nVuoi rubare dalla mano del bersaglio o dalle sue carte in gioco?\n[0 ---> mano, 1 ---> in gioco]:");
            scanf("%d", &var1);

            if(var1<0 || var1>1)
                printf("\n\nErrore! Riprova.");

            if(var1 == 0 && party[bersaglio].mano.numCarte == 0)
                printf("\n\nErrore! Il giocatore non ha carte in mano! Riprova.");

            if(var1 == 1 && party[bersaglio].inGioco.numCarte == 0)
                printf("\n\nErrore! Il giocatore non ha carte in gioco! Riprova.");

        }while(var1<0 || var1>1 || (var1 == 0 && party[bersaglio].mano.numCarte == 0) || (var1 == 1 && party[bersaglio].inGioco.numCarte == 0));

        if(var1 == 0)
        {
            carta = randRange(0, (party[bersaglio].mano.numCarte - 1));

            spostaCarta(&party[bersaglio].mano, carta, &attaccante->mano, NULL, nomeFile);

            char messaggio[MEX_DIM] = "";
            strcat(messaggio, attaccante->nome);
            strcat(messaggio, " ruba una carta dalla mano di ");
            strcat(messaggio, party[bersaglio].nome);
            strcat(messaggio, " giocando Panico.");
            salvaLog(messaggio, nomeFile);

            printf("\n\nHai rubato una carta al giocatore scelto!\n\n");
        }

        else if(var1 == 1)
        {
            do {
                printf("\nScegli la carta da rubare al bersaglio tra:");

                for(int i=0; i<party[bersaglio].inGioco.numCarte; i++)
                {
                    printf("\n%d - %s\n", i, party[bersaglio].inGioco.carte[i].nome);
                }

                scanf("%d", &carta);

                if(carta<0 || carta>=party[bersaglio].inGioco.numCarte)
                    printf("\n\nErrore! Hai scelto una carta che non esiste! Riprova.\n");

            }while(carta<0 || carta>=party[bersaglio].inGioco.numCarte);

            char messaggio[MEX_DIM] = "";
            strcat(messaggio, attaccante->nome);
            strcat(messaggio, " ruba ");
            strcat( messaggio, party[bersaglio].inGioco.carte[carta].nome);
            strcat(messaggio, " a ");
            strcat(messaggio, party[bersaglio].nome);
            strcat( messaggio, " giocando Panico.");
            salvaLog(messaggio, nomeFile);

            spostaCarta(&party[bersaglio].inGioco, carta, &attaccante->mano, NULL, nomeFile);

            printf("\n\nHai rubato la carta!\n\n");
        }
    }
    else
    {
        printf("\n\nNessun avversario ha carte in mano o in gioco!! La carta viene giocata senza effetto!\n\n");
        char messaggio[MEX_DIM] = "";
        strcat(messaggio, attaccante->nome);
        strcat(messaggio, " gioca Panico senza colpire nessuno.");
        salvaLog(messaggio, nomeFile);
    }

    getchar();
    continuo();
    pulisciSchermo();
}

void saloon(Giocatore* party, int nGiocatori, char* nomeFile)
{
    printf("\n\nStai giocando SALOON.\nTutti i giocatori che non hanno pallottole massime ricevono una pallottola extra!\n");
    continuo();
    pulisciSchermo();

    for(int i=0; i<nGiocatori; i++)
    {
        if(party[i].ruolo == SCERIFFO)
        {
            if(party[i].pallottole < HP_SCERIFFO)
            {
                party[i].pallottole++;
                char messaggio[MEX_DIM] = "";
                strcat(messaggio, party[i].nome);
                strcat(messaggio, " recupera una pallottola.");
                salvaLog(messaggio, nomeFile);
            }
        }
        else
        {
            if(party[i].pallottole < HP)
            {
                party[i].pallottole++;
                char messaggio[MEX_DIM] = "";
                strcat(messaggio, party[i].nome);
                strcat(messaggio, " recupera una pallottola.");
                salvaLog(messaggio, nomeFile);
            }
        }
    }
}

void birra(Giocatore* giocatore, char* nomeFile)
{
    printf("\n\nStai giocando BIRRA.\n\n");
    continuo();

    if(giocatore->ruolo == SCERIFFO)
    {
        if(giocatore->pallottole < HP_SCERIFFO)
        {
            printf("\nRecuperi una pallottola!\n");
            char messaggio[MEX_DIM] = "";
            strcat(messaggio, giocatore->nome);
            strcat(messaggio, " riceve una pallottola.");
            salvaLog(messaggio, nomeFile);
            giocatore->pallottole++;
        }
        else
        {
            printf("\nHai il massimo di pallottole! BIRRA non ha avuto alcun effetto!\n");
            char messaggio[MEX_DIM] = "";
            strcat(messaggio, giocatore->nome);
            strcat(messaggio, " ha il massimo di pallottole, Birra non ha effetto.");
            salvaLog(messaggio, nomeFile);
        }
    }
    else
    {
        if(giocatore->pallottole < HP)
        {
            printf("\nRecuperi una vita!\n");
            char messaggio[MEX_DIM] = "";
            strcat(messaggio, giocatore->nome);
            strcat(messaggio, " recupera una pallottola.");
            salvaLog(messaggio, nomeFile);
            giocatore->pallottole++;
        }
        else
        {
            printf("\nHai il massimo di pallottole! BIRRA non ha avuto alcun effetto!\n");
            char messaggio[MEX_DIM] = "";
            strcat(messaggio, giocatore->nome);
            strcat(messaggio, " ha il massimo di pallottole, Birra non ha effetto.");
            salvaLog(messaggio, nomeFile);
        }
    }

    continuo();
    pulisciSchermo();
}

bool birraExtremis(Giocatore* giocatore, char* nomeFile)
{
    int scelta = NOP;
    do {
        printf("\n\nVuoi giocare BIRRA? \n[0 = si, 1 = no]:");
        scanf("%d", &scelta);

        if (scelta < 0 || scelta > 1)
            printf("\nErrore! Riprova.");
    } while (scelta < 0 || scelta > 1);

    if(scelta == 0)
    {
        char messaggio[MEX_DIM] = "";
        strcat(messaggio, giocatore->nome);
        strcat(messaggio, " gioca Birra in extremis.");
        salvaLog(messaggio, nomeFile);

        getchar();
        continuo();
        pulisciSchermo();
        return true;
    }
    else
    {
        getchar();
        continuo();
        pulisciSchermo();
        return false;
    }
}

void mancato()
{
    printf("\n\nStai giocando MANCATO!\nIl BANG! non ha effetto!\n\n");
    continuo();
}

void danno(Giocatore* party, int* nGiocatori, Giocatore* attaccante, int bersaglio, Mazzo* mazzoScarti, Mazzo* mazzoPesca, char* nomeFile)
{
    int idMancato = NOP;
    int idBirra = NOP;

    idMancato = trovaCartaMano(&party[bersaglio], MANCATO);
    idBirra = trovaCartaMano(&party[bersaglio], BIRRA);

    if(idMancato != NOP)
    {
        pulisciSchermo();
        int scelta = NOP;
        do {
            printf("\n\n%s! Vuoi giocare MANCATO? \n[0 = si, 1 = no]:", party[bersaglio].nome);
            scanf("%d", &scelta);

            if (scelta < 0 || scelta > 1)
                printf("\nErrore! Riprova.");
        } while (scelta < 0 || scelta > 1);

        if (scelta == 0)
        {
            spostaCarta(&party[bersaglio].mano, idMancato, mazzoScarti, NULL, nomeFile);
            char messaggio[MEX_DIM] = "";
            strcat(messaggio, party[bersaglio].nome);
            strcat(messaggio, " si difende con Mancato.");
            salvaLog(messaggio, nomeFile);
            mancato();
        }
        else
        {
            printf("\nSei stato colpito!\n");
            char messaggio[MEX_DIM] = "";
            strcat(messaggio, party[bersaglio].nome);
            strcat(messaggio, " viene colpito.");
            salvaLog(messaggio, nomeFile);
            (party[bersaglio].pallottole)--;
            if((party[bersaglio].pallottole) == 0 && idBirra != NOP)
            {
                if(birraExtremis(&party[bersaglio], nomeFile))
                {
                    spostaCarta(&party[bersaglio].mano, idBirra, mazzoScarti, NULL, nomeFile);
                    (party[bersaglio].pallottole)++;
                }
                else
                {
                    eliminazione(party, attaccante, &party[bersaglio], nGiocatori, mazzoPesca, mazzoScarti, nomeFile);
                }
            }
            else if((party[bersaglio].pallottole) == 0)
            {
                eliminazione(party, attaccante, &party[bersaglio], nGiocatori, mazzoPesca, mazzoScarti, nomeFile);
            }
        }
    }
    else
    {
        pulisciSchermo();
        printf("\n%s, sei stato colpito!\n", party[bersaglio].nome);
        char messaggio[MEX_DIM] = "";
        strcat(messaggio, party[bersaglio].nome);
        strcat(messaggio, " viene colpito dal Bang.");
        salvaLog(messaggio, nomeFile);
        (party[bersaglio].pallottole)--;
        if((party[bersaglio].pallottole) == 0 && idBirra != NOP)
        {
            if(birraExtremis(&party[bersaglio], nomeFile))
            {
                spostaCarta(&party[bersaglio].mano, idBirra, mazzoScarti, NULL, nomeFile);
                (party[bersaglio].pallottole)++;
            }
            else
            {
                eliminazione(party, attaccante, &party[bersaglio], nGiocatori, mazzoPesca, mazzoScarti, nomeFile);
            }
        }
        else if((party[bersaglio].pallottole) == 0)
        {
            eliminazione(party, attaccante, &party[bersaglio], nGiocatori, mazzoPesca, mazzoScarti, nomeFile);
        }
    }

    continuo();
    pulisciSchermo();
}

void bang(Giocatore* party, Giocatore* attaccante,Mazzo* mazzoScarti,Mazzo* mazzoPesca, int* nGiocatori, char* nomeFile)
{
    printf("\n\nStai giocando BANG!\n\n");

    int bersaglio = NOP;
    int distanza = NOP;

    for(int i=0; i<(*nGiocatori); i++)
    {
        distanza = calcoloDistanza(party,attaccante,&party[i], *nGiocatori);

        if(distanza <= attaccante->gittata && distanza != 0)
        {
            printf("%d - %s\n", i, party[i].nome);
        }
    }

    do
    {
        printf("\nScegli il giocatore da colpire:");
        scanf("%d", &bersaglio);

        distanza = calcoloDistanza(party, attaccante, &party[bersaglio], *nGiocatori);

        if(bersaglio<0 || bersaglio>=(*nGiocatori))
            printf("\nErrore! Stai provando a colpire un giocatore che non estiste! Riprova.\n");
        if(comparaNomi(attaccante->nome, party[bersaglio].nome))
            printf("\nErrore! Non puoi colpirti da solo! Riprova.\n");
        if(distanza>attaccante->gittata)
            printf("\nErrore! Il giocatore che vuoi colpire e' troppo lontano! Riprova.\n");
    }while(bersaglio<0 || bersaglio>(*nGiocatori) || comparaNomi(attaccante->nome, party[bersaglio].nome) || distanza>attaccante->gittata);

    char messaggio[MEX_DIM] = "";
    strcat(messaggio, attaccante->nome);
    strcat(messaggio, " gioca un Bang su ");
    strcat(messaggio, party[bersaglio].nome);
    strcat(messaggio, ".");
    salvaLog(messaggio, nomeFile);

    int idBarile = NOP;

    idBarile = trovaCartaInGioco(&party[bersaglio], BARILE);

    if(idBarile != NOP)
    {
        pulisciSchermo();
        printf("\n%s! Hai in gioco BARILE! Procedi con l'estrazione.\n", party[bersaglio].nome);
        char messaggio2[MEX_DIM] = "";
        strcat(messaggio2, party[bersaglio].nome);
        strcat(messaggio2, " ha Barile in gioco.");
        salvaLog(messaggio2, nomeFile);
        continuo();

        if(!barile(&party[bersaglio], idBarile, mazzoPesca, mazzoScarti, nomeFile))
        {
            danno(party, nGiocatori, attaccante, bersaglio, mazzoScarti, mazzoPesca, nomeFile);
        }
    }

    continuo();
    pulisciSchermo();
}

void gatling(Giocatore* party, Giocatore* attaccante,Mazzo* mazzoScarti,Mazzo* mazzoPesca, int* nGiocatori, char* nomeFile)
{
    printf("\n\nStai giocando GATLING\n\n");
    char messaggio[MEX_DIM] = "";
    strcat(messaggio, attaccante->nome);
    strcat(messaggio, " gioca Gatling.");
    salvaLog(messaggio, nomeFile);
    continuo();

    for(int i=0; i<(*nGiocatori); i++)
    {
        pulisciSchermo();
        printf("\n\n%s! %s ha giocato GATLING.\n\n", party[i].nome, attaccante->nome);

        if(!comparaNomi(attaccante->nome, party[i].nome))
        {
            int idBarile = NOP;

            idBarile = trovaCartaInGioco(&party[i], BARILE);

            if(idBarile != NOP)
            {
                printf("\nHai in gioco BARILE! Procedi con l'estrazione.\n");
                char messaggio2[MEX_DIM] = "";
                strcat(messaggio2, party[i].nome);
                strcat(messaggio2, " ha Barile in gioco.");
                salvaLog(messaggio2, nomeFile);
                continuo();

                if(!barile(&party[i], idBarile, mazzoPesca, mazzoScarti, nomeFile))
                {
                    danno(party, nGiocatori, attaccante, i, mazzoScarti, mazzoPesca, nomeFile);
                }
            }
            else
            {
                danno(party, nGiocatori, attaccante, i, mazzoScarti, mazzoPesca, nomeFile);
            }
        }

        continuo();
    }
}

void giocaCartaMarrone(Giocatore* party, int* nGiocatori, Giocatore* giocatore, int carta, Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile)
{
    if(comparaNomi(giocatore->mano.carte[carta].nome, DILIGENZA))
    {
        diligenza(giocatore, mazzoPesca, mazzoScarti, nomeFile);
    }
    else if(comparaNomi(giocatore->mano.carte[carta].nome, PANICO))
    {
        panico(party, giocatore, *nGiocatori, nomeFile);
    }
    else if(comparaNomi(giocatore->mano.carte[carta].nome, BANG))
    {
        bang(party, giocatore, mazzoScarti, mazzoPesca, nGiocatori, nomeFile);
    }
    else if(comparaNomi(giocatore->mano.carte[carta].nome, GATLING))
    {
        gatling(party, giocatore, mazzoScarti, mazzoPesca, nGiocatori, nomeFile);
    }
    else if(comparaNomi(giocatore->mano.carte[carta].nome, WELLS))
    {
        wellsFargo(giocatore, mazzoPesca, mazzoScarti, nomeFile);
    }
    else if(comparaNomi(giocatore->mano.carte[carta].nome, CATBALOU))
    {
        catBalou(party, giocatore, mazzoScarti, *nGiocatori, nomeFile);
    }
    else if(comparaNomi(giocatore->mano.carte[carta].nome, BIRRA))
    {
        birra(giocatore, nomeFile);
    }
    else if(comparaNomi(giocatore->mano.carte[carta].nome, SALOON))
    {
        char messaggio[MEX_DIM] = "";
        strcat(messaggio, giocatore->nome);
        strcat(messaggio, " gioca Saloon.");
        salvaLog(messaggio, nomeFile);
        saloon(party, *nGiocatori, nomeFile);
    }
    else if(comparaNomi(giocatore->mano.carte[carta].nome, INDIANI))
    {
        char messaggio[MEX_DIM] = "";
        strcat(messaggio, giocatore->nome);
        strcat(messaggio, " gioca Indiani.");
        salvaLog(messaggio, nomeFile);
        indiani(party, nGiocatori, giocatore, mazzoScarti, mazzoPesca, nomeFile);
    }
    else if(comparaNomi(giocatore->mano.carte[carta].nome, DUELLO))
    {
        char messaggio[MEX_DIM] = "";
        strcat(messaggio, giocatore->nome);
        strcat(messaggio, " gioca Duello.");
        salvaLog(messaggio, nomeFile);
        duello(party, nGiocatori, giocatore, mazzoScarti, mazzoPesca, nomeFile);
    }
    else if(comparaNomi(giocatore->mano.carte[carta].nome, EMPORIO))
    {
        char messaggio[MEX_DIM] = "";
        strcat(messaggio, giocatore->nome);
        strcat(messaggio, " gioca Emporio.");
        salvaLog(messaggio, nomeFile);
        emporio(party, *nGiocatori, giocatore, mazzoPesca, mazzoScarti, nomeFile);
    }

    spostaCarta(&giocatore->inGioco, carta, mazzoScarti, NULL, nomeFile);
}