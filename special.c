#include "carte.h"

void duello(Giocatore* party,int* nGiocatori, Giocatore* sfidante, Mazzo* mazzoScarti, Mazzo* mazzoPesca, char* nomeFile)
{
    printf("\n\nStai giocando DUELLO.\n\n");
    continuo();

    for(int i=0; i<(*nGiocatori); i++)
    {
        if(!comparaNomi(sfidante->nome, party[i].nome))
        {
            printf("%d - %s.\n", i, party[i].nome);
        }
    }

    int idBersaglio = NOP;

    do
    {
        printf("\nScegli il giocatore da sfidare:");
        scanf("%d", &idBersaglio);

        if(idBersaglio<0 || idBersaglio>=(*nGiocatori))
            printf("\nErrore! Stai sfidando un giocatore che non esiste! Riprova.\n");
        if(comparaNomi(sfidante->nome, party[idBersaglio].nome))
            printf("\nErrore! Non puoi sfidare te stesso! Riprova.\n");
    }while(idBersaglio<0 || idBersaglio>=(*nGiocatori) || comparaNomi(sfidante->nome, party[idBersaglio].nome));

    char messaggio[MEX_DIM] = "";
    strcat(messaggio, party[idBersaglio].nome);
    strcat(messaggio, " viene sfidato a duello.");
    salvaLog(messaggio, nomeFile);

    printf("\n%s! %s ti sfida a duello!\n" , party[idBersaglio].nome, sfidante->nome);
    continuo();
    pulisciSchermo();

    printf("\n\nINIZIO DUELLO!\n\n");

    int esito = NOP;

    do
    {
        pulisciSchermo();

        int cartaBersaglio = NOP, cartaSfidante = NOP, i = 0, j = 0;

        printf("\nTocca a %s!\n", party[idBersaglio].nome);

        continuo();

        while(i<party[idBersaglio].mano.numCarte && cartaBersaglio == NOP)
        {
            if(comparaNomi(party[idBersaglio].mano.carte[i].nome, BANG))
            {
                cartaBersaglio = i;
            }
            i++;
        }

        if(cartaBersaglio != NOP)
        {
            printf("\n%s gioca un BANG! e si difende. La sfida continua.\n", party[idBersaglio].nome);
            continuo();
            pulisciSchermo();

            printf("\nTocca a %s!\n", sfidante->nome);
            continuo();

            while (j < sfidante->mano.numCarte && cartaSfidante == -1)
            {
                if(comparaNomi(sfidante->mano.carte[j].nome, BANG))
                {
                    cartaSfidante = j;
                }
                j++;
            }

            if(cartaSfidante != NOP)
            {
                printf("\n%s gioca un BANG! e si difende. La sfida continua.\n", sfidante->nome);
                continuo();
            }
            else
            {
                esito = 1;
                printf("\n%s non ha piu' BANG! a disposizione!\n", sfidante->nome);
                continuo();
            }
        }
        else
        {
            esito = 0;
            printf("\n%s non ha piu' BANG! a disposizione!\n", party[idBersaglio].nome);
            continuo();
        }

        if(cartaBersaglio != NOP && cartaSfidante != NOP)
        {
            spostaCarta(&party[idBersaglio].mano, cartaBersaglio, mazzoScarti, NULL, nomeFile);
            spostaCarta(&sfidante->mano, cartaBersaglio, mazzoScarti, NULL, nomeFile);
        }

        pulisciSchermo();

    }while(esito == NOP);

    int idBirra = NOP;

    if(esito == 0)
    {
        printf("\n%s! Sei stato colpito!\n", party[idBersaglio].nome);
        char messaggio2[MEX_DIM] = "";
        strcat(messaggio2, party[idBersaglio].nome);
        strcat(messaggio2, " perde il duello e subisce danno.");
        salvaLog(messaggio2, nomeFile);
        (party[idBersaglio].pallottole)--;

        idBirra = trovaCartaMano(&party[idBersaglio], BIRRA);

        if(party[idBersaglio].pallottole == 0 && idBirra != NOP)
        {
            if (birraExtremis(&party[idBersaglio], nomeFile))
            {
                spostaCarta(&sfidante->mano, idBirra, mazzoScarti, NULL, nomeFile);
                birra(&party[idBersaglio], nomeFile);
            }
            else
            {
                eliminazione(party, sfidante, &party[idBersaglio], nGiocatori, mazzoPesca, mazzoScarti, nomeFile);
            }
        }
    }
    else
    {
        printf("\n%s! Sei stato colpito!\n", sfidante->nome);
        char messaggio2[MEX_DIM] = "";
        strcat(messaggio2, sfidante->nome);
        strcat(messaggio2, " perde il duello e subisce danno.");
        salvaLog(messaggio2, nomeFile);

        idBirra = trovaCartaMano(sfidante, BIRRA);

        if (sfidante->pallottole == 0 && idBirra != NOP)
        {
            if (birraExtremis(sfidante, nomeFile))
            {
                spostaCarta(&sfidante->mano, idBirra, mazzoScarti, NULL, nomeFile);
                birra(sfidante, nomeFile);
            }
            else
            {
                eliminazione(party, sfidante, &party[idBersaglio], nGiocatori, mazzoPesca, mazzoScarti, nomeFile);
            }
        }
    }
}

void indiani(Giocatore* party, int* nGiocatori, Giocatore* attaccante, Mazzo* mazzoScarti, Mazzo* mazzoPesca, char* nomeFile)
{
    printf("\n\nStai giocando INDIANI!\n\n");
    continuo();

    for(int i=0; i<(*nGiocatori); i++)
    {
        pulisciSchermo();

        if(!comparaNomi(attaccante->nome, party[i].nome))
        {
            printf("\n%s ha giocato INDIANI!\n", attaccante->nome);

            int idBang = NOP;
            idBang = trovaCartaInGioco(&party[i], BARILE);

            if(idBang != NOP)
            {
                printf("\n%s, la tua carta BANG! viene giocata per difenderti.\n", party[i].nome);
                char messaggio[MEX_DIM] = "";
                strcat(messaggio, party[i].nome);
                strcat(messaggio, " si difende dagli indiani.");
                salvaLog(messaggio, nomeFile);

                spostaCarta(&party[i].mano, idBang, mazzoScarti, NULL, nomeFile);
                continuo();
            }
            else
            {
                printf("\n%s, sei stato colpito!\n", party[i].nome);
                char messaggio[MEX_DIM] = "";
                strcat(messaggio, party[i].nome);
                strcat(messaggio, " viene colpito dagli indiani.");
                salvaLog(messaggio, nomeFile);
                party[i].pallottole--;

                int idBirra = NOP;

                idBirra = trovaCartaMano(&party[i], BIRRA);

                if(party[i].pallottole == 0 && idBirra != NOP)
                {
                    if(birraExtremis(&party[i], nomeFile))
                    {
                        spostaCarta(&party[i].mano, idBirra, mazzoScarti, NULL, nomeFile);
                        birra(&party[i], nomeFile);
                    }
                    else
                    {
                        eliminazione(party, attaccante, &party[i], nGiocatori, mazzoPesca, mazzoScarti, nomeFile);
                    }
                }
                else if(party[i].pallottole == 0)
                {
                    eliminazione(party, attaccante, &party[i], nGiocatori, mazzoPesca, mazzoScarti, nomeFile);
                }
                continuo();
            }
        }
    }
}

void emporio(Giocatore* party,int nGiocatori, Giocatore* attaccante, Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile)
{
    printf("\n\n%s ha giocato EMPORIO.\nOgni giocatore deve scegliere una carta.\n", attaccante->nome);
    continuo();
    pulisciSchermo();

    Mazzo temp;
    temp.carte = NULL;
    temp.numCarte = 0;

    for(int i=0; i<nGiocatori; i++)
    {
        spostaCarta(mazzoPesca, NOP, &temp, mazzoScarti, nomeFile);
    }

    int scelta = NOP;

    stampaCarte(&temp);

    do
    {
        printf("\n%s! Scegli una carta da pescare!\n", attaccante->nome);
        scanf("%d", &scelta);

        if(scelta<0 || scelta>temp.numCarte)
            printf("\nErrore! La carta che hai scelto non esiste! Riprova.\n");
    }while(scelta<0 || scelta>temp.numCarte);

    if(scelta != NOP)
    {
        spostaCarta(&temp, scelta, &attaccante->mano, NULL, nomeFile);
        char messaggio[MEX_DIM] = "";
        strcat(messaggio, attaccante->nome);
        strcat(messaggio, " pesca una carta da Emporio.");
        salvaLog(messaggio, nomeFile);
        printf("\nHai pescato la carta!\n");
    }

    continuo();

    for(int i=0; i<nGiocatori; i++)
    {
        pulisciSchermo();

        if(!comparaNomi(attaccante->nome, party[i].nome))
        {
            scelta = NOP;

            stampaCarte(&temp);

            do
            {
                printf("\n%s! Scegli una carta da pescare!\n", party[i].nome);
                scanf("%d", &scelta);

                if(scelta<0 || scelta>temp.numCarte)
                    printf("\nErrore! La carta che hai scelto non esiste! Riprova.\n");
            }while(scelta<0 || scelta>temp.numCarte);

            if(scelta != NOP)
            {
                spostaCarta(&temp, scelta, &party[i].mano, NULL, nomeFile);
                char messaggio[MEX_DIM] = "";
                strcat(messaggio, party[i].nome);
                strcat(messaggio, " pesca una carta da Emporio.");
                salvaLog(messaggio, nomeFile);
                printf("\nHai pescato la carta!\n");
            }

            continuo();
        }
    }

    free(temp.carte);
}