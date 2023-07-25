#include "carte.h"

/** La funzione calcoloDistanza gestisce il calcolo delle distanze tra i giocatori, contando anche la
 * circolarita' dell'array party, cerchera' l'id dell'attaccante e del giocatore nell'array, e calcolera'
 * la distanza in base alla distanza effettiva o la distanza circolare. Tiene conto anche delle giocate di
 * Mirinio o Mustang.
 *
 * @param party party di giocatori
 * @param attaccante giocatore attaccante
 * @param bersaglio giocatore bersaglio
 * @param nGiocatori numero di giocatori
 * @return intero che indica la distanza tra l'attaccante e il bersaglio
**/

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

    // controllo la distanza "lineare" e quella circolare e vedo qual è la distanza minima tra le due
    distanzaEffettiva = abs(idAttaccante - idBersaglio);

    distanzaCircolare = abs((nGiocatori - distanzaEffettiva) % nGiocatori);

    distanza = (distanzaEffettiva < distanzaCircolare) ? distanzaEffettiva : distanzaCircolare;

    // gestisco i casi in cui attaccante abbia im gioco Mirino o bersaglio abbia in gioco Mustang
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

/** La procedura diligenza si occupa della gestione della carta Diligenza.
 * Quando un giocatore gioca questa carta ha il diritto di pescare 2 carte dal mazzo di pesca.
 *
 * @param giocatore giocatore che gioca Diligenza
 * @param mazzoPesca mazzo di pesca
 * @param mazzoScarti mazzo di scarti da passare se mazzoPesca e' vuoto
 * @param nomeFile nome file log
**/

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
        daiCarta(mazzoPesca, &giocatore->mano, mazzoScarti, nomeFile);
    }
}

/** La procedura wellsFargo si occupa della gestione della carta Wells Fargo.
 * Quando un giocatore gioca questa carta ha il diritto di pescare 3 carte dal mazzo di pesca.
 *
 * @param giocatore giocatore che gioca Wells Fargo
 * @param mazzoPesca mazzo di pesca
 * @param mazzoScarti mazzo di scarti da passare se mazzoPesca e' vuoto
 * @param nomeFile nome file log
**/

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
        daiCarta(mazzoPesca, &giocatore->mano, mazzoScarti, nomeFile);
    }
}

/** La procedura CatBalou si occupa della giocata della carta CatBalou.
 * Quando un giocatore gioca questa carta sceglie un bersaglio che sara' obbligato
 * a scartare una carta e gli viene descritta la quantita' di carte in mano o in gioco
 * dei suoi avversari. A questo punto sceglie se colpire
 * la mano del giocatore o le sue carte in gioco: se decide di colpire la mano, una carta casuale verra' scartata
 * dalla mano del bersaglio, se no verra' chiesto al giocatore di scegliere una carta da far scartare al suo avversario.
 *
 * @param party party di giocatori
 * @param attaccante giocatore che gioca CatBalou
 * @param mazzoScarti mazzo scarti
 * @param nGiocatori numero dei giocatori
 * @param nomeFile nome file log
**/

void catBalou(Giocatore* party, Giocatore* attaccante, Mazzo* mazzoScarti, int nGiocatori, char* nomeFile)
{
    int bersaglio = NOP;
    bool carteDisponibili = false;

    printf("\nStai giocando CAT BALOU.\n");
    continuo();

    // rendo visibili le carte in mano e in gioco degli avversari
    for(int i=0; i<nGiocatori; i++)
    {
        printf("\n%d - %s, %d carte in mano, %d carte in gioco", i, party[i].nome, party[i].mano.numCarte, party[i].inGioco.numCarte);
        if(party[i].mano.numCarte>0 || party[i].inGioco.numCarte>0)
            carteDisponibili = true;
    }

    //se qualcuno ha delle carte da scartare viene chiesto di sceglierlo
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

        //se sceglie di colpire una mano una carta casuale verra' fatta scartare al bersaglio
        if(var1 == 0)
        {
            carta = randRange(0, (party[bersaglio].mano.numCarte - 1));

            spostaCarta(&party[bersaglio].mano, carta, mazzoScarti);

            char messaggio[MEX_DIM] = "";
            strcat(messaggio, attaccante->nome);
            strcat(messaggio, " fa scartare una carta proveniente dalla mano di ");
            strcat(messaggio, party[bersaglio].nome);
            strcat( messaggio, " giocando CatBalou.");
            salvaLog(messaggio, nomeFile);

            printf("\n\nUna carta e' stata scartata dal giocatore colpito!\n\n");
        }

        // se sceglie di colpire le carte in gioco scegliera' la carta da scartare
        else if(var1 == 1)
        {
            // rendo visibili le carte in gioco del bersaglio
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

            spostaCarta(&party[bersaglio].inGioco, carta, mazzoScarti);

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
    // se nessuno ha carte in mano o in gioco viene giocata la carta senza effetto
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

/** La procedura panico si gestisce la giocata della carta Panico! da parte di un giocatore.
 * La carta Panico! permette di rubare una carta a un giocatore a distanza 1 da chi gioca la carta,
 * viene poi chiesto al giocatore se vuole rubare una carta dalla mano o dal mazzo in gioco del giocatore
 * desiderato. Se vuole rubare una carta dalla mano del giocatore bersaglio questa verra' scelta casualmente
 * mentre se sceglie di rubare la carta dal mazzo in gioco dovra' scegliere quale carta rubare. La carta finira'
 * poi nella sua mano.
 *
 * @param party party di giocatori
 * @param attaccante giocatore che gioca la carta
 * @param nGiocatori numero di giocatori
 * @param nomeFile nome file log
**/

void panico(Giocatore* party, Giocatore* attaccante, int nGiocatori, char* nomeFile)
{
    int bersaglio = NOP;
    // controllo se i giocatori hanno carte
    bool carteDisponibili = false;

    pulisciSchermo();
    printf("\nStai giocando PANICO!\n");
    continuo();

    int distanza = NOP;

    for(int i=0; i<nGiocatori; i++)
    {
        if(!comparaNomi(attaccante->nome, party[i].nome))
        {
            //se la distanza e' pari a uno viene consetita la scelta del bersaglio
            distanza = calcoloDistanza(party, attaccante, &party[i],nGiocatori);
            if(distanza == GITTATA_COLT_45)
            {
                printf("\n%d - %s, %d carte in mano, %d carte in gioco", i, party[i].nome, party[i].mano.numCarte, party[i].inGioco.numCarte);
            }
            if(party[i].mano.numCarte>0 || party[i].inGioco.numCarte>0)
            {
                //aggiorno variabile di controllo
                carteDisponibili = true;
            }
        }
    }

    if(carteDisponibili)
    {
        do {
            // scelta del giocatore da colpire
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
            // scelta del mazzo da cui rubare la carta
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
            // un numero casuale viene estratto per decidere quale carta verra' rubata dalla mano del bersaglio
            carta = randRange(0, (party[bersaglio].mano.numCarte - 1));

            spostaCarta(&party[bersaglio].mano, carta, &attaccante->mano);

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
                // scelta della carta da rubare dal mazzo delle carte in gioco
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

            spostaCarta(&party[bersaglio].inGioco, carta, &attaccante->mano);

            printf("\n\nHai rubato la carta!\n\n");
        }
    }
    else
    {
        // se nessuno ha carte in gioco o in mano panico! viene giocata senza effetto
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

/** La procedura saloon gestisce la giocata della carta Saloon da parte del giocatore.
 * La carta Saloon permette a tutti i giocatori di recupare una vita a meno che questi non abbiano
 * gia' il massimo di pallotole consentite per il proprio ruolo.
 *
 * @param party
 * @param nGiocatori
 * @param nomeFile
**/

void saloon(Giocatore* party, int nGiocatori, char* nomeFile)
{
    printf("\n\nStai giocando SALOON.\nTutti i giocatori che non hanno pallottole massime ricevono una pallottola extra!\n");
    continuo();
    pulisciSchermo();

    for(int i=0; i<nGiocatori; i++)
    {
        //controllo se lo sceriffo non ha gia' 5 pallottole
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
            //controllo se il giocatore a ricevere il punto vita non ha gia' 4 pallottole
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

/** La procedura Birra permette di gestire la giocata della carta Birra da parte di un
 * giocatore. La carta Birra permette di recuperare una pallottola a meno che non si abbia gia' il massimo
 * di pallottole consentite per il proprio ruolo.
 *
 * @param giocatore giocatore che gioca Birra
 * @param nomeFile file log
**/

void birra(Giocatore* giocatore, char* nomeFile)
{
    printf("\n\nStai giocando BIRRA.\n\n");
    continuo();

    if(giocatore->ruolo == SCERIFFO)
    {
        // se lo sceriffo non ha il massimo di pallottole ne recupera una
        if(giocatore->pallottole < HP_SCERIFFO)
        {
            printf("\nRecuperi una pallottola!\n");
            char messaggio[MEX_DIM] = "";
            strcat(messaggio, giocatore->nome);
            strcat(messaggio, " riceve una pallottola.");
            salvaLog(messaggio, nomeFile);
            giocatore->pallottole++;
        }
        //se ha il massimo di pallottole gioca Birra senza effetto
        else
        {
            printf("\nHai il massimo di pallottole! BIRRA non ha avuto alcun effetto!\n");
            char messaggio[MEX_DIM] = "";
            strcat(messaggio, giocatore->nome);
            strcat(messaggio, " ha il massimo di pallottole, Birra non ha effetto.");
            salvaLog(messaggio, nomeFile);
        }
    }
    // se il giocatore non ha il massimo di pallottole ne recupera una
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
        //se ha il massimo di pallottole gioca Birra senza effetto
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

/** La funzione birraExtremis viene chiamata quando al giocatore vengono sottratte tutte
 * le pallottole, per salvarsi puo' ancora giocare una carta Birra, se ne ha una, e scartarla per
 * recuperare una pallottola. La funzione restituisce una variabile booleana a seconda che il giocatore
 * decida di giocare la Birra oppure no.
 *
 * @param giocatore giocatore che potrebbe giocare la Birra
 * @param nomeFile file di log
 * @return booleano a true o false
**/

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
        // se sceglie di giocare la Birra recupera una vita
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
        // se sceglie di non giocare la Birra verra' eliminato
        getchar();
        continuo();
        pulisciSchermo();
        return false;
    }
}

/** La procedura mancato viene chiamata quando un giocatore e vittima di un bang e sceglie di giocare
 * un Mancato per difendersi. La procedura si occupa di scartare la carta.
 *
 * @param giocatore giocatore che gioca Mancato
 * @param carta id carta da scartare
 * @param mazzoScarti mazzo degli scarti
 * @param nomeFile nome file log
 */

void mancato(Giocatore* giocatore, int carta, Mazzo* mazzoScarti, char* nomeFile)
{
    printf("\n\nStai giocando MANCATO!\nIl BANG! non ha effetto!\n\n");

    spostaCarta(&giocatore->mano, carta, mazzoScarti);

    char messaggio[MEX_DIM] = "";
    strcat(messaggio, giocatore->nome);
    strcat(messaggio, " si difende con Mancato.");
    salvaLog(messaggio, nomeFile);

    continuo();
}

void danno(Giocatore* party, int* nGiocatori, Giocatore* attaccante, int bersaglio, Mazzo* mazzoScarti, Mazzo* mazzoPesca, char* nomeFile)
{
    int idMancato = NOP;
    int idBirra = NOP;

    // cerco la carta mancato nella mano del bersaglio
    idMancato = trovaCartaMano(&party[bersaglio], MANCATO);
    idBirra = trovaCartaMano(&party[bersaglio], BIRRA);

    if(idMancato != NOP)
    {
        // il giocatore sceglie se giocare mancato
        int scelta = NOP;
        do {
            printf("\n\n%s! Vuoi giocare MANCATO? \n[0 = si, 1 = no]:", party[bersaglio].nome);
            scanf("%d", &scelta);

            if (scelta < 0 || scelta > 1)
                printf("\nErrore! Riprova.");
        } while (scelta < 0 || scelta > 1);

        if (scelta == 0)
        {
            mancato(&party[bersaglio], idMancato, mazzoScarti, nomeFile);
        }
        else
        {
            //giocatore viene colpito ma puo' giocare birra in extremis se ha zero pallottole
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
                    spostaCarta(&party[bersaglio].mano, idBirra, mazzoScarti);
                    (party[bersaglio].pallottole)++;
                }
                else
                {
                    // se ha zero pallottole viene eliminato
                    eliminazione(party, attaccante, &party[bersaglio], nGiocatori, mazzoPesca, mazzoScarti, nomeFile);
                }
            }
            else if((party[bersaglio].pallottole) == 0)
            {
                //se non ha birre da giocare viene eliminato
                eliminazione(party, attaccante, &party[bersaglio], nGiocatori, mazzoPesca, mazzoScarti, nomeFile);
            }
        }
    }
    else
    {
        // se non ha mancato da giocare viene colpito e sceglie se vuole giocare  birra in extremis o essere eliminato
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
                spostaCarta(&party[bersaglio].mano, idBirra, mazzoScarti);
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

/** La procedura bang si occupa della gestione della carta Bang!.
 * Un giocatore gioca il bang su un altro che puo' raggiungere in base alla propria gittata.
 * Viene controllato se il bersaglio ha barile , in caso positivo estrarra' una carta per vedere se potra' difendersi dal bang
 * potra' ancora giocare un Mancato per difendersi e schivare il colpo oppure subire danno e
 * perdere una pallottola. Se ha zero pallottole a seguito del bang e non ha una Birra
 * per salvarsi viene eliminato.
 *
 * @param party party di giocatori
 * @param attaccante giocatore che spara
 * @param mazzoScarti mazzo scarti per gestire eventuali malus
 * @param mazzoPesca mazzo pesca per gestire eventuali bonus
 * @param nGiocatori numero di giocatori
 * @param nomeFile file log
**/

void bang(Giocatore* party, Giocatore* attaccante,Mazzo* mazzoScarti,Mazzo* mazzoPesca, int* nGiocatori, char* nomeFile)
{
    printf("\n\nStai giocando BANG!\n\n");

    int bersaglio = NOP;
    int distanza = NOP;

    for(int i=0; i<(*nGiocatori); i++)
    {
        //calcolo la distanza dagli altri giocatori per stampare chi si puo' colpire
        distanza = calcoloDistanza(party,attaccante,&party[i], *nGiocatori);

        // non si puo' colpire da solo
        if(distanza <= attaccante->gittata && distanza != 0)
        {
            if(party[i].ruolo == SCERIFFO)
            {
                printf("%d - %s (SCERIFFO)\n", i, party[i].nome);
            }
            else
            {
                printf("%d - %s\n", i, party[i].nome);
            }
        }
    }

    do
    {
        //scelta del bersaglio
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

    getchar();

    char messaggio[MEX_DIM] = "";
    strcat(messaggio, attaccante->nome);
    strcat(messaggio, " gioca un Bang su ");
    strcat(messaggio, party[bersaglio].nome);
    strcat(messaggio, ".");
    salvaLog(messaggio, nomeFile);

    // controllo se il bersaglio ha barile in gioco
    int idBarile = NOP;

    idBarile = trovaCartaInGioco(&party[bersaglio], BARILE);

    if(idBarile != NOP)
    {
        //estrazione per barile
        pulisciSchermo();
        printf("\n%s! Hai in gioco BARILE! Procedi con l'estrazione.\n", party[bersaglio].nome);
        char messaggio2[MEX_DIM] = "";
        strcat(messaggio2, party[bersaglio].nome);
        strcat(messaggio2, " ha Barile in gioco.");
        salvaLog(messaggio2, nomeFile);
        continuo();

        //se l'estrazione non ha buon fine si e' soggetti alla possibilita' di prendere danno
        if(!barile(&party[bersaglio], idBarile, mazzoPesca, mazzoScarti, nomeFile))
        {
            danno(party, nGiocatori, attaccante, bersaglio, mazzoScarti, mazzoPesca, nomeFile);
        }
        else
        {
            char messaggio3[MEX_DIM] = "";
            strcat(messaggio3, party[bersaglio].nome);
            strcat(messaggio3, " si difende grazie a Barile.");
            salvaLog(messaggio3, nomeFile);
            continuo();
            printf("\nTi difendi dal Bang! grazie a Barile.\n");
        }
    }
    else
    {
        danno(party, nGiocatori, attaccante, bersaglio, mazzoScarti, mazzoPesca, nomeFile);
    }
}

/** La procedura gatling gestisce l'utilizzo della carta Gatling da parte del giocatore.
 * La carta Gatling permette di colpire con un bang tutti i giocatori a prescindere dalla distanza.
 * I giocatori potranno difendersi con Barile o Mancato, opppure potranno giocare una Birra per salvarsi o essere eliminati
 *
 * @param party party di giocatori
 * @param attaccante giocatore che usa Gatling
 * @param mazzoScarti mazzo di scarti
 * @param mazzoPesca mazzo di pesca
 * @param nGiocatori numero di giocatori
 * @param nomeFile nome file log
**/

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

        // il giocatore che gioca gatling non puo' colpirsi da solo
        if(!comparaNomi(attaccante->nome, party[i].nome))
        {
            // controllo se il bersaglio ha un barile in gioco
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
                    //se l'estrazione non va a buon fine chiamo danno
                    danno(party, nGiocatori, attaccante, i, mazzoScarti, mazzoPesca, nomeFile);
                }
                else
                {
                    char messaggio3[MEX_DIM] = "";
                    strcat(messaggio3, party[i].nome);
                    strcat(messaggio3, " si difende grazie a Barile.");
                    salvaLog(messaggio3, nomeFile);
                    continuo();
                    printf("\nTi difendi dal Bang! grazie a Barile.\n");
                }
            }
            // se non ha barile in gioco chiamo danno
            else
            {
                danno(party, nGiocatori, attaccante, i, mazzoScarti, mazzoPesca, nomeFile);
            }
        }

        continuo();
    }
}

/** La procedura giocaCartaMarrone si occupa di chiamare le funzioni che gestiscono l'effetto della carta giocata
 * e di scartarle, in quanto le carte marroni vengono scartate subito dopo aver esercitato il proprio effetto.
 *
 * @param party party di giocatori
 * @param nGiocatori numero dei giocatori
 * @param giocatore giocatore che gioca una carta
 * @param carta id della carta giocata
 * @param mazzoPesca mazzo di pesca
 * @param mazzoScarti mazzo di scarti
 * @param nomeFile nome file log
**/

void giocaCartaMarrone(Giocatore* party, int* nGiocatori, Giocatore* giocatore, int carta, Mazzo* mazzoPesca, Mazzo* mazzoScarti, char* nomeFile)
{
    // controllo quale funzione invocare in base alla carta giocata
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

    spostaCarta(&giocatore->mano, carta, mazzoScarti);
}
