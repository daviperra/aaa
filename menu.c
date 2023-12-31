#include "gioco.h"

bool controlloStringa(char* stringa)
{
    bool check = true;

    for(int i=0; i<strlen(stringa); i++)
    {
        if(!((stringa[i] >= '0' && stringa[i] <= '9') || (stringa[i] >= 'A' && stringa[i] <= 'Z') || (stringa[i] >= 'a' && stringa[i] <= 'z') || stringa[i] == '_'))
        {
            check = false;
        }
    }

    return check;
}

char* creaSalvataggio()
{
    char temp[LOG_DIM];
    char temp1[SAVE_DIM+1];
    int scelta;

    do
    {

        printf("\nInserisci il nome del nuovo file di salvataggio:");
        scanf(" %[^\n]s", temp);

        if(!controlloStringa(temp))
        {
            printf("\nErrore! Nome non valido,\n(usa caratteri alfanumerici o il trattino basso). Riprova.\n");
        }

        strcpy(temp1, temp);
        strcat(temp1, ".sav");

        FILE* fp = NULL;

        fp = fopen(temp1, "rb");

        if(fp != NULL)
        {
            printf("\nUn file di salvataggio con questo nome e' gia' esistente.\nVuoi sovrascriverlo?. [0 = si, 1 = no]");
            scanf("%d", &scelta);
        }

        fclose(fp);
    }while(!controlloStringa(temp) || scelta == 1);

    char* nomeFile = (char*) malloc(sizeof(char) * strlen(temp)+1);
    strcpy(nomeFile, temp);

    FILE* fp = NULL;

    fp = fopen(temp1, "wb");

    if(fp == NULL)
    {
        exit(EXIT_FAILURE);
    }

    fclose(fp);

    FILE* log = NULL;
    strcat( temp, "_log.txt");
    log = fopen(temp, "w");
    if(log == NULL)
    {
        exit(EXIT_SUCCESS);
    }
    fprintf(log, "Nuovo salvataggio.\n");
    fclose(log);

    pulisciSchermo();

    return nomeFile;
}

void nuovaPartita()
{
    pulisciSchermo();
    Giocatore* party = NULL;
    int nGiocatori = NOP;
    char* nomeFile = NULL;
    Mazzo* mazzoPesca;
    Mazzo* mazzoScarti;
    mazzoPesca = (Mazzo*) malloc(sizeof(Mazzo));
    mazzoScarti = (Mazzo*) malloc(sizeof(Mazzo));

    nomeFile = creaSalvataggio();
    party = nuovoParty(&nGiocatori, nomeFile);
    creaMazzoPesca(mazzoPesca);
    creaMazzoScarti(mazzoScarti);
    rivelaRuoli(party, nGiocatori, nomeFile);
    stampaNomi(party, nGiocatori);
    mescolaCarte(mazzoPesca);
    distribuisciCarte(party, nGiocatori, mazzoPesca, mazzoScarti, nomeFile);

    partita(party, &nGiocatori, mazzoPesca, mazzoScarti, nomeFile, NOP, NOP);

    free(mazzoPesca->carte);
    free(mazzoScarti->carte);
    free(nomeFile);

    for(int i=0; i<nGiocatori; i++)
    {
        free(party[i].mano.carte);
        free(party[i].inGioco.carte);
    }

    free(party);
}

void caricaPartita()
{
    bool trovato = false;

    char temp[SAVE_DIM];
    char temp1[SAVE_DIM];

    Giocatore* party = NULL;
    int nGiocatori = NOP;
    Mazzo* mazzoPesca = NULL;
    Mazzo* mazzoScarti = NULL;
    int prossimoGiocatore = NOP;
    int nTurno = NOP;

    do {
        printf("\nScrivi il nome del file che vuoi caricare:");
        scanf("%[^\n]s", temp);

        strcpy(temp1, temp);
        strcat(temp1, ".sav");

        FILE* fp = NULL;
        fp = fopen(temp1, "rb");
        if(fp != NULL)
        {
            trovato = true;
            fclose(fp);
        }
        else
        {
            printf("\nErrore! Il file che vuoi caricare non esiste, Riprova.\n");
        }
    }while(!trovato);


    char* nomeFile = (char*) malloc(sizeof(char) * strlen(temp)+1);
    strcpy(nomeFile, temp);

    party = caricaSalvataggio(party, &nGiocatori, mazzoPesca, mazzoScarti, &prossimoGiocatore, &nTurno, nomeFile);

    partita(party, &nGiocatori, mazzoPesca, mazzoScarti, nomeFile, prossimoGiocatore, nTurno);

    free(mazzoPesca->carte);
    free(mazzoScarti->carte);
    free(mazzoPesca);
    free(mazzoScarti);
    free(nomeFile);

    for(int i=0; i<nGiocatori; i++)
    {
        free(party[i].mano.carte);
        free(party[i].inGioco.carte);
    }

    free(party);
}

void menu()
{
    int scelta = NOP;

    printf("\n\n");
    printf("  BBBBB    AAAA   N   N   GGGG    !!\n");
    printf("  B   BB  A    A  NN  N  G        !!\n");
    printf("  BBBBB   AAAAAA  N N N  G  GG    !!\n");
    printf("  B   BB  A    A  N  NN  G    G     \n");
    printf("  BBBBB   A    A  N   N   GGG     !!\n");
    printf("\n\n");

    printf("\n%d - Nuova Partita", NUOVA_PARTITA);
    printf("\n%d - Carica Partita", CARICA_PARTITA);
    printf("\n%d - Chiudi\n\n", CHIUDI);

    do
    {
        printf("Seleziona:");
        scanf("%d", &scelta);
        getchar();

        if(scelta< 0 || scelta > CHIUDI)
        {
            printf("\n\nErrore! Riprova.\n\n");
        }
    }while(scelta< 0 || scelta > CHIUDI);



    switch(scelta)
    {
        case NUOVA_PARTITA:
        {
            continuo();
            nuovaPartita();
        }
            break;
        case CARICA_PARTITA:
        {
            pulisciSchermo();
            caricaPartita();
        }
            break;
        case CHIUDI:
            break;
    }
}