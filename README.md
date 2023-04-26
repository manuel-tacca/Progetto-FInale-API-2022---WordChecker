# Progetto Finale API 2022 - WordChecker
• L'obiettivo del progetto di quest'anno è di realizzare un sistema che,
al suo cuore, controlla la corrispondenza tra le lettere di 2 parole di
ugual lunghezza.
• Le parole sono intese come sequenze di simboli che possono essere
caratteri alfabetici minuscoli (a-z) o maiuscoli (A-Z), cifre numeriche
(0-9), oppure i simboli - (trattino) e _ ("underscore")
• esempio di parola di 20 simboli: djHD1af9fj7g__l-ssOP
• Il sistema legge da standard input una sequenza di informazioni e
istruzioni, e produce delle stringhe in output a seconda dei casi.
• Più precisamente, il sistema legge:
• un valore k, che indica la lunghezza delle parole
• una sequenza (di lunghezza arbitraria) di parole, ognuna di lunghezza k, che
costituisce l'insieme delle parole ammissibili
• si dia pure per scontato che la sequenza di parole non contenga duplicati
• A quel punto, viene letta da standard input una sequenza di "partite",
in cui l'inizio di ogni nuova partita è marcato dal comando (letto
sempre da input) +nuova_partita
• Le sequenze di stringhe in input per ogni partita (successive al comando
+nuova_partita) sono fatte nel seguente modo:
• parola di riferimento (di lunghezza k caratteri)
• si assuma che la parola di riferimento appartenga all'insieme di parole ammissibili
• numero n massimo di parole da confrontare con la parola di riferimento
• sequenza di parole (ognuna di k caratteri) da confrontare con la parola di riferimento
• Ogni tanto, nella sequenza di stringhe in input, può comparire il comando
+stampa_filtrate, il cui effetto è spiegato in seguito
• Inoltre, sia durante una partita, che tra una partita e l'altra, possono
comparire i comandi +inserisci_inizio e +inserisci_fine che racchiudono tra
di loro una sequenza di nuove parole da aggiungere all'insieme delle parole
ammissibili
• le parole aggiunte sono anch'esse di lunghezza k, e si dà sempre per scontato che
non ci siano parole duplicate (neanche rispetto alle parole già presenti nell'insieme di
quelle ammissibili)
• Per ogni parola letta p, da confrontare con la parola di riferimento r, il
programma scrive su stdout una sequenza di k caratteri fatta nella
seguente maniera:
• nel seguito, indichiamo con p[1], p[2], … p[k] i caratteri della parola p, con r[1], r[2],
… r[k] quelli della parola r, e con res[1], res[2], … res[k] quelli della sequenza
stampata
• Per ogni 1 ≤ i ≤ k, si ha che
• res[i] è il carattere ‘+’ se l'i-esimo carattere di p è uguale all'i-esimo carattere di r
• cioè se vale che p[i] = r[i], quindi p[i] è "in posizione corretta"
• res[i] è il carattere ‘/’ se p[i] non compare da nessuna parte in r
• res[i] è il carattere ‘|’ se p[i] compare in r, ma non in posizione i-esima; tuttavia, se in
r compaiono ni istanze di p[i], se ci è il numero di istanze del carattere p[i] che sono in
posizione corretta (chiaramente ci ≤ ni) e se ci sono prima del carattere i-esimo in p
almeno ni-ci caratteri uguali a p[i] che sono in posizione scorretta, allora res[i] deve
essere / invece di |
• Se da standard input viene letta una parola che non appartiene all'insieme
di quelle ammissibili, il programma scrive su standard output la stringa
not_exists (NB: non viene contato come tentativo)
• Se invece viene letta la parola r (cioè se p = r), allora il programma scrive ok
(senza stampare il risultato dettagliato del confronto) e la partita termina
• Se, dopo avere letto n parole ammissibili (con n, si ricordi, numero
massimo di parole da confrontare con r), nessuna di queste era uguale a r,
il programma scrive ko (dopo avere stampato il risultato del confronto
dell'ultima parola), e la partita termina
• Dopo che la partita è finita:
• Non ci possono essere altre parole da confrontare (ma ci potrebbe essere
l'inserimento di nuove parole ammissibili)
• Se in input c'è il comando +nuova_partita, ha inizio una nuova partita
• Quando, durante una partita, da input si legge il comando
+stampa_filtrate, il programma deve produrre in output, in ordine
lessicografico, l'insieme delle parole ammissibili che sono compatibili con i
vincoli appresi fino a quel momento nella partita, scritte una per riga
• si noti che i vincoli appresi riguardano, per ogni simbolo:
  - se il simbolo non appartiene a r
  - posti in cui quel simbolo deve comparire in r
  - posti in cui quel simbolo non può comparire in r
  - numero minimo di volte che il simbolo compare in r
  - numero esatto di volte che il simbolo compare in r
• si noti che il vincolo 5 è più forte del vincolo 4
• l'ordine dei simboli (usato per stabilire l'ordine lessicografico delle parole) è quello
specificato dallo standard ASCII
• Inoltre, dopo ogni confronto, il programma deve stampare in output il
numero di parole ammissibili ancora compatibili con i vincoli appresi
