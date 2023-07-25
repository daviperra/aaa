#include "carte.h"

/** La procedura duello gestisce la giocata della carta duello da parte di un giocatore.
 * Il giocatore che gioca la carta duello sceglie un avversario da sfidare indipendentemente
 * dalla distanza. Dopo aver scelto il giocatore da sfidare inizia il duello: i due giocatori
 * scarteranno a turno un bang dalla propria mano fino a che uno dei due non avra' piu' carte bang
 * disponibili. Il giocatore che perde il duello perde un punto vita e se si trova a zero punti vita
 * potra' giocare una birra in extremis o essere eliminato.
 *
 * @param party party di giocatori
 * @param nGiocatori numero di giocatori
 * @param sfidante giocatore che usa duello
 * @param mazzoScarti mazzo scarti
 * @param mazzoPesca mazzo pesca per eventuali bonus
 * @param nomeFile nome file log
**/

void duello(Giocatore* party,int* nGiocatori, Giocatore* sfidante, Mazzo* mazzoScarti, Mazzo* mazzoPesca, char* nomeFile)
{
    printf("\n\nStai giocando DUELLO.\n\n");
    continuo();

    //scelta giocatore da sfidare
    for(int i=0; i<(*nGiocatori); i++)
    {
        if(!comparaNomi(sfidante->nome, party[i].nome))
        {
            if(party[i].ruolo == SCERIFFO)
            {
                printf("%d - %s (SCERIFFO).", i, party[i].nome);
            }
            else
            {
                printf("%d - %s.\n", i, party[i].nome);
            }
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

    getchar();
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

        // cerco bang da scartare nella mano del bersaglio
        cartaBersaglio = trovaCartaMano(&party[idBersaglio], BANG);

        // se viene trovato un bang nella mano del bersaglio la sfida continua
        if(cartaBersaglio != NOP)
        {
            printf("\n%s gioca un BANG! e si difende. La sfida continua.\n", party[idBersaglio].nome);
            spostaCarta(&party[idBersaglio].mano, cartaBersaglio, mazzoScarti);

            continuo();
            pulisciSchermo();

            printf("\nTocca a %s!\n", sfidante->nome);
            continuo();

            // cerco bang da scartare nella mano dello sfidante
            cartaSfidante = trovaCartaMano(&party[idBersaglio], BANG);

            // se si trova un bang nella mano dello sfidante la sfida continua
            if(cartaSfidante != NOP)
            {
                printf("\n%s gioca un BANG! e si difende. La sfida continua.\n", sfidante->nome);
                spostaCarta(&sfidante->mano, cartaSfidante, mazzoScarti);
                continuo();
            }
            // se non si trova un bang nella mano dello sfidante si aggiorna la variabile esito per terminare il duello
            else
            {
                esito = 1;
                printf("\n%s non ha piu' BANG! a disposizione!\n", sfidante->nome);
                continuo();
            }
        }
            // se non si trova un bang nella mano del bersaglio si aggiorna la variabile esito per terminare il duello
        else
        {
            esito = 0;
            printf("\n%s non ha piu' BANG! a disposizione!\n", party[idBersaglio].nome);
            continuo();
        }

        pulisciSchermo();

    }while(esito == NOP);

    int idBirra = NOP;
    // gestisco il danno recato al bersaglio
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
                spostaCarta(&sfidante->mano, idBirra, mazzoScarti);
                birra(&party[idBersaglio], nomeFile);
            }
            else
            {
                eliminazione(party, sfidante, &party[idBersaglio], nGiocatori, mazzoPesca, mazzoScarti, nomeFile);
            }
        }
    }
    // gestisco il danno arrecato al bersaglio
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
                spostaCarta(&sfidante->mano, idBirra, mazzoScarti);
                birra(sfidante, nomeFile);
            }
            else
            {
                eliminazione(party, sfidante, sfidante, nGiocatori, mazzoPesca, mazzoScarti, nomeFile);
            }
        }
    }
}

/** La procedura Indiani gestisce la giocata della carta Indiani:
 * il giocatore che gioca questa carta sottopone gli avversari alla possibile perdita di un punto vita,
 * questi infatti saranno costretti a giocare un bang per difendersi dagli indiani e nel caso non abbiano
 * carte bang da scartare subiranno danno. Nel caso le sue pallottole si esauriscono potra' usare una birra
 * in extremis, oppure verra' eliminato.
 *
 * @param party party di giocatori
 * @param nGiocatori numero di giocatori
 * @param attaccante giocatore che chiama indiani
 * @param mazzoScarti mazzo di scarti
 * @param mazzoPesca mazzo di pesca per eventuali bonus
 * @param nomeFile file di log
**/

void indiani(Giocatore* party, int* nGiocatori, Giocatore* attaccante, Mazzo* mazzoScarti, Mazzo* mazzoPesca, char* nomeFile)
{
    printf("\n\nStai giocando INDIANI!\n\n");
    continuo();

    for(int i=0; i<(*nGiocatori); i++)
    {
        pulisciSchermo();

        // tutti i giocatori tranne l'attaccante dovranno scartare un bang per difendersi
        if(!comparaNomi(attaccante->nome, party[i].nome))
        {
            printf("\n%s ha giocato INDIANI!\n", attaccante->nome);

            int idBang = NOP;

            //trovo la carta bang nella mani del giocatore
            idBang = trovaCartaMano(&party[i], BANG);

            if(idBang != NOP)
            {
                printf("\n%s, la tua carta BANG! viene giocata per difenderti.\n", party[i].nome);
                char messaggio[MEX_DIM] = "";
                strcat(messaggio, party[i].nome);
                strcat(messaggio, " si difende dagli indiani.");
                salvaLog(messaggio, nomeFile);

                spostaCarta(&party[i].mano, idBang, mazzoScarti);
                continuo();
            }
            //gestisco il caso in cui il giocatore subisca danno
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
                        spostaCarta(&party[i].mano, idBirra, mazzoScarti);
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

/** La procedura Emporio gestisce la giocata della carta Emporio da parte di un giocatore.
 * Viene creato un array di carte temporaneo in cui inserire carte dal mazzoPesca e ogni giocatore a
 * turno dovra' scegliere una carta da pescare.
 *
 * @param party party di giocatori
 * @param nGiocatori numero di giocatori
 * @param attaccante giocatore che gioca emporio
 * @param mazzoPesca mazzo di pesca
 * @param mazzoScarti mazzo di scarti da passare se il mazzo pesca e' vuoto
 * @param nomeFile nome file log
**/

void emporio(Giocatore* party,int nGiocatori, Giocatore* attaccante, Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile)
{
    printf("\n\n%s ha giocato EMPORIO.\nOgni giocatore deve scegliere una carta.\n", attaccante->nome);
    continuo();
    pulisciSchermo();

    //creazioe struttura mazzo temporanea
    Mazzo temp;
    //array di carte inizializzato a NULL
    temp.carte = NULL;
    temp.numCarte = 0;

    //passaggio carte dal mazzoPesca al mazzo temporaneo
    for(int i=0; i<nGiocatori; i++)
    {
        daiCarta(mazzoPesca, &temp, mazzoScarti, nomeFile);
    }

    int scelta = NOP;

    stampaCarte(&temp);

    //scelta della carta da parte di chi gioca emporio
    do
    {
        printf("\n%s! Scegli una carta da pescare!\n", attaccante->nome);
        scanf("%d", &scelta);

        if(scelta<0 || scelta>temp.numCarte)
            printf("\nErrore! La carta che hai scelto non esiste! Riprova.\n");
    }while(scelta<0 || scelta>temp.numCarte);

    if(scelta != NOP)
    {
        spostaCarta(&temp, scelta, &attaccante->mano);
        char messaggio[MEX_DIM] = "";
        strcat(messaggio, attaccante->nome);
        strcat(messaggio, " pesca una carta da Emporio.");
        salvaLog(messaggio, nomeFile);
        printf("\nHai pescato la carta!\n");
    }

    continuo();

    // scelta della carta da pescare degli altri giocatori
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
                spostaCarta(&temp, scelta, &party[i].mano);
                char messaggio[MEX_DIM] = "";
                strcat(messaggio, party[i].nome);
                strcat(messaggio, " pesca una carta da Emporio.");
                salvaLog(messaggio, nomeFile);
                printf("\nHai pescato la carta!\n");
            }

            continuo();
        }
    }
}
