#include "carte.h"

/** La funzione estrazione prende una carta dal mazzo di pesca, la controlla
 * per restituire un valore di verita' a seconda della modalita' di estrazione e poi la scarta.
 *
 * @param modalita intero che indica la modalita' con cui avviene l'estrazione
 * @param mazzoPesca mazzo di pesca
 * @param mazzoScarti mazzo di scarti
 * @param nomeFile nome del file log
 * @return booleano a true o false
**/

bool estrazione(int modalita, Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile)
{
    bool esito = false;
    int idCarta = PRIMO;

    printf("\nEstrazione della carta.\n");
    salvaLog("Viene estratta una carta dal mazzo.", nomeFile);
    continuo();

    //allocazione mazzo temporaneo
    Mazzo* temp = (Mazzo*) malloc(sizeof(Mazzo));
    temp->carte = NULL;
    temp->numCarte = 0;

    daiCarta(mazzoPesca, temp, mazzoScarti, nomeFile);

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
        /*se l'estrazione e' chiamata da prigione o barile viene
        * controllato se la carta ha come seme cuori*/
         if(temp->carte[idCarta].seme == CUORI)
        {
            esito = true;
        }
    }
    else
    {
        /*se l'estrazione e' chiamata da dinamite controlla se la carta estratta
         * e' compresa tra il 2 e il 9 di picche*/
        if(temp->carte[idCarta].seme == PICCHE && (temp->carte[idCarta].num>=MIN_DINAMITE && temp->carte[idCarta].num<=MAX_DINAMITE))
        {
            esito = true;
        }
    }

    spostaCarta(temp, idCarta, mazzoScarti);

    //libero mazzo temporaneo
    free(temp);

    return esito;
}

/** La procedura armi viene chiamata dopo che un'arma e' stata giocata
 * e assegna al giocatore la gittata fissata per quell'arma.
 *
 * @param giocatore giocatore che gioca arma
**/

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

/** La funzione barile controlla, tramite l'estrazione di una carta dal mazzo
 * se Barile puo' effettuare la sua funzione o no. Restituisce il booleano per il controllo.
 *
 * @param giocatore giocatore che ha Barile in gioco
 * @param carta carta Barile da scartare
 * @param mazzoPesca mazzo pesca
 * @param mazzoScarti mazzo scarti
 * @param nomeFile nome del file log
 * @return booleano a true o false
**/

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

        spostaCarta(&giocatore->inGioco, carta, mazzoScarti);
    }
    else
    {
        printf("\nNon e' uscito CUORI, BARILE non ha effetto!\n");
        salvaLog("L'estrazione non va a buon fine, Barile non ha effetto.", nomeFile);
        spostaCarta(&giocatore->inGioco, carta, mazzoScarti);
    }

    continuo();

    return esito;
}

/** La funzione dinamite controlla, tramite l'estrazione di una carta dal mazzo
 * se la dinamite esplode o no. Se si, fa danno al giocatore che puo' giocare una birra
 * in extremis nel caso in cui i suoi punti vita scendono a zero. Puo' anche venire eliminato
 * e il colpevole dell'eliminazione non sarà nessuno, in quel caso verrà restituito true cosicche'
 * il giocatore eliminato non possa giocare per errore il proprio turno. Se non viene eliminato restituisce false.
 * Se invece la dinamite non esplode questa viene passata al giocatore successivo nel party.
 *
 * @param giocatore giocatore che ha Barile in gioco
 * @param carta carta Barile da scartare
 * @param mazzoPesca mazzo pesca
 * @param mazzoScarti mazzo scarti
 * @param nomeFile nome del file log
 * @return booleano a true o false
**/

bool dinamite(Giocatore* party, int* nGiocatori, Giocatore* giocatore, Mazzo* mazzoPesca, Mazzo* mazzoScarti, int carta, char* nomeFile)
{
    bool check = false;

    //creo giocatore temporaneo per non chiamare malus o bonus in eliminazione
    Giocatore* dinamite = (Giocatore*) malloc(sizeof(Giocatore));
    strcpy(dinamite->nome, DINAMITE);

    if(!estrazione(MOD_DINAMITE,mazzoPesca, mazzoScarti, nomeFile))
    {
        printf("\nLa DINAMITE esplode!\n");

        spostaCarta(&giocatore->inGioco, carta, mazzoScarti);

        /*se il giocatore ha meno punti vita rispetto al danno arrecato dalla
         *dinamite puo' giocare una birra in extremis se la ha
         * se no subisce il danno e viene eliminato*/
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
                    spostaCarta(&giocatore->mano, idBirra, mazzoScarti);
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


        //dinamite passa al giocatore successivo
        if(idGiocatore == (*nGiocatori)-1)
        {
            spostaCarta(&giocatore->inGioco, carta, &party[PRIMO].inGioco);
            strcat(messaggio, " a ");
            strcat(messaggio, party[PRIMO].nome);
        }
        else
        {
            spostaCarta(&giocatore->inGioco, carta, &party[idGiocatore+1].inGioco);
            strcat(messaggio, " a ");
            strcat(messaggio, party[idGiocatore+1].nome);
        }

        salvaLog(messaggio, nomeFile);
    }

    //liberazione giocatore temporaneo
    free(dinamite);

    continuo();
    pulisciSchermo();

    return check;
}

/** La funzione prigione viene chiamata quando un giocatore a inizio del proprio turno ha in gioco
 * una carta prigione. Procede all'estrazione di una carta dal mazzo, se l'estrazione ha esito positivo
 * il giocatore esce di prigione e gioca il proprio turno, se no lo salta. Viene restituito
 * valore di verita' che dipende dall'esito dell'estrazione.
 *
 * @param giocatore
 * @param mazzoPesca
 * @param mazzoScarti
 * @param nomeFile
 * @return
**/

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

    //scarto la carta prigione
    spostaCarta(&giocatore->inGioco, idPrigione, mazzoScarti);

    continuo();
    pulisciSchermo();
    return esito;
}

/** La procedura giocaBarile permette di mettere la carta Barile tra le proprie
 * carte in gioco, cosi' da potersi difendere nel caso si e' vittima di un bang.
 *
 * @param giocatore giocatore che gioca barile
 * @param carta id della carta barile
 * @param nomeFile nome file log
**/

void giocaBarile(Giocatore* giocatore, int carta, char* nomeFile)
{
    printf("\n\nStai giocando BARILE.\nLa carta e' ora in gioco!\n\n");
    char messaggio[MEX_DIM] = "";
    strcat(messaggio, giocatore->nome);
    strcat(messaggio, "gioca Barile.");
    salvaLog(messaggio, nomeFile);
    continuo();
    spostaCarta(&giocatore->mano, carta, &giocatore->inGioco);
}

/** La procedura giocaDinamite permette di mettere la carta Dinamite tra le proprie
 * carte in gioco, cosi' da poter passare la dinamite e arrecare danno a altri
 * giocatori in caso si sia fortunati con l'estrazione.
 *
 * @param giocatore giocatore che gioca dinamite
 * @param carta id della carta dinamite
 * @param nomeFile nome file log
**/

void giocaDinamite(Giocatore* giocatore, int carta, char* nomeFile)
{
    printf("\n\nStai giocando DINAMITE.\nLa carta e' ora in gioco!\n");
    char messaggio[MEX_DIM] = "";
    strcat(messaggio, giocatore->nome);
    strcat(messaggio, "gioca Dinamite.");
    salvaLog(messaggio, nomeFile);
    continuo();
    spostaCarta(&giocatore->mano, carta, &giocatore->inGioco);
}

/** La procedura giocaBarile permette di mettere la carta Prigione tra le
 * carte in gioco di un avversario a scelta, cosi' da metterlo in prigione e a costringerlo a estrarre
 * per poter giocare il proprio tunro.
 *
 * @param party party di giocatori
 * @param nGiocatori numero dei giocatori
 * @param giocatore giocatore che gioca Prigione
 * @param carta id della carta prigione
 * @param nomeFile nome del file log
**/

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
        spostaCarta(&giocatore->mano, carta, &party[scelta].inGioco);
    }
}

/** La procedura giocaArma permette di mettere in gioco una carta arma cosi' da
 * poter aumentare la propria gittata in base all'arma giocata. Se ha un'altra arma in gioco
 * questa verra' scartata per far posto alla nuova arma.
 *
 * @param giocatore giocatore che gioca arma
 * @param carta id carta arma
 * @param mazzoScarti mazzo di scarti
 * @param nomeFile nome file log
**/

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
        spostaCarta(&giocatore->inGioco, idArma, mazzoScarti);
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

    spostaCarta(&giocatore->mano, carta, &giocatore->inGioco);

    continuo();
    pulisciSchermo();
}

/** La procedura giocaCartaBlu permette di mettere in gioco una carta tra armi e effetti.
 * Nel caso in cui sia un'arma a essere giocata verra' chiamata giocaArma e poi armi per modificare
 * la gittata del giocatore. Se viene giocata prigione, dinamite o barile verra' chiamata la rispettiva
 * procedura, se no giocaCartaBlu si occupera' di mettere in gioco le carte mirino o mustang.
 *
 * @param party party di giocatori
 * @param nGiocatori numero di giocatori
 * @param giocatore giocatore a giocare la carta
 * @param carta id della carta da giocare
 * @param mazzoScarti mazzo di scarti
 * @param nomeFile nome file log
**/

void giocaCartaBlu(Giocatore* party, int nGiocatori, Giocatore* giocatore, int carta, Mazzo* mazzoScarti, char* nomeFile)
{
    // pulisco il buffer di input
    getchar();

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

            // metto mustang in gioco
            spostaCarta(&giocatore->mano, carta, &giocatore->inGioco);

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

            // metto mirino in gioco
            spostaCarta(&giocatore->mano, carta, &giocatore->inGioco);

            continuo();
            pulisciSchermo();
        }
    }
}
