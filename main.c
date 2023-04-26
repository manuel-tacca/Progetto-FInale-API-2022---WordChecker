#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define MAX_COMMAND_LENGTH 18
#define RED 'R'
#define BLACK 'B'

typedef struct rb_node {
    char color;
    struct rb_node* left;
    struct rb_node* right;
    struct rb_node* p;
    struct rb_node* next_filtrate;
    char word[];
} rb_node;

struct rb_node T_Nil_Node;
struct rb_node* T_Nil = &T_Nil_Node;

rb_node* dictionary;
rb_node* head = NULL;
rb_node* tail = NULL;

typedef struct wordTable {
    char letter;
    short number;
    short* positions;
}wordTable;

typedef struct discovery {
    char letter;
    char* forbidden;
}Discovery;

typedef struct necessary {
    char letter;
    short int number;
    bool isMax;
}Necessary;

static int x;
bool terminated_input = false;
char command[MAX_COMMAND_LENGTH];
int nOfFiltered = 0;
int max_nOfFiltered = 0;

/*int strcmp( const char* s1, const char* s2 ){

    while ( *s1 && ( *s1 == *s2 ))
        s1++, s2++;

    return *( const unsigned char*)s1 - *(const unsigned char*)s2;
}*/

rb_node* newNode ( char* word ) {

    struct rb_node* new = (rb_node*) malloc( sizeof( rb_node ) + (x+1) * sizeof(char ));

    new->left = NULL;
    new->right = NULL;
    new->p = NULL;
    new->color = RED;
    new->next_filtrate = NULL;
    word[x] = '\0';
    memcpy( new->word, word, x+1);

    return new;
}

void leftRotate( rb_node** T, rb_node* n ) {

    rb_node* y = n->right;
    n->right = y->left;
    if ( y->left != T_Nil ) {
        y->left->p = n;
    }
    y->p = n->p;

    if ( n->p == T_Nil ) {
        *T = y;
    }
    else if ( n == n->p->left ) {
        n->p->left = y;
    }
    else {
        n->p->right = y;
    }
    y->left = n;
    n->p = y;
}

void rightRotate ( rb_node** T, rb_node* n ) {

    rb_node* y = n->left;
    n->left = y->right;
    if ( y->right != T_Nil ) {
        y->right->p = n;
    }

    y->p = n->p;

    if ( n->p == T_Nil ) {
        *T = y;
    }
    else if ( n == n->p->right ) {
        n->p->right = y;
    }
    else {
        n->p->left = y;
    }
    y->right = n;
    n->p = y;
}

void rbInsertFixUp ( rb_node** root, rb_node* new ) {
    rb_node* temp;
    while ( new->p->color == RED ) {
        if ( new->p == new->p->p->left ) {
            temp = new->p->p->right;
            if ( temp->color == RED ) {
                new->p->color = BLACK;
                temp->color = BLACK;
                new->p->p->color = RED;
                new = new->p->p;
            }
            else {
                if ( new == new->p->right ) {
                    new = new->p;
                    leftRotate(root, new);
                }
                new->p->color = BLACK;
                new->p->p->color = RED;
                rightRotate( root, new->p->p );
            }
        }
        else {
            temp = new->p->p->left;
            if ( temp->color == RED ) {
                new->p->color = BLACK;
                new->color = BLACK;
                new->p->p->color = RED;
                new = new->p->p;
            }
            else {
                if ( new == new->p->left ) {
                    new = new->p;
                    rightRotate(root, new);
                }
                new->p->color = BLACK;
                new->p->p->color = RED;
                leftRotate( root, new->p->p );
            }
        }
    }
    root[0]->color = BLACK;
}

rb_node *rbInsert ( rb_node** T, char* word ) {

    rb_node* z = newNode(word);
    rb_node* y = T_Nil;
    rb_node* t = *T;

    while ( t != T_Nil ) {
        y = t;
        if ( strcmp( z->word, t->word) < 0 )
            t = t->left;
        else
            t = t->right;
    }

    z->p = y;

    if ( y == T_Nil )
        *T = z;
    else if ( strcmp(z->word, y->word) < 0 )
        y->left  = z;
    else
        y->right = z;

    z->left  = T_Nil;
    z->right = T_Nil;
    z->color = RED;

    rbInsertFixUp(T,z);

    return z;
}

void rbDispose ( rb_node* root ) {
    if (root->left != T_Nil)
        rbDispose(root->left);
    if (root->right != T_Nil) {
        rbDispose(root->right);
    }
    free(root);
}

rb_node* rbSearch ( rb_node** T, const char* word ) {
    rb_node* n = *T;
    int cmp_result;
    do {
        cmp_result = strcmp( n->word, word );
        if ( cmp_result == 0 ){
            return n;
        }
        else if ( cmp_result > 0 ) {
            n = n->left;
        }
        else {
            n = n->right;
        }
    }while (n != T_Nil);
    return T_Nil;
}

void rbPrint ( rb_node* root ) {
    if (root->left != T_Nil)
        rbPrint(root->left);
    printf("%s\n", root->word);
    if (root->right != T_Nil)
        rbPrint(root->right);
}

void printFiltered ( rb_node *node ){ // TODO: develop this function in a non-recursive way
    if (node != NULL) {
        printf("%s\n", node->word);
        if (node->next_filtrate == NULL)
            return;
        else
            printFiltered(node->next_filtrate);
    }
    else {
        rbPrint(dictionary);
    }
}

bool filterWord (char* toFilter, char* att,  const char* result, Necessary *necessary_tmp, short nti) {

    for (int i = 0; i < x; i++) {
        if (result[i] == '+') {
            if (toFilter[i] != att[i]) {
                nOfFiltered--;
                return false;
            }
        }
        else if (result[i] == '/') {
            if ( strchr(toFilter, att[i]) != NULL ) {
                nOfFiltered--;
                return false;
            }
        }
        else if ( result[i] == '-') {
            if ( (toFilter[i] == att[i]) || (strchr(toFilter, att[i] ) == NULL) ) {
                nOfFiltered--;
                return false;
            }
        }
        else if ( result[i] == '|') {
            if ( toFilter[i] == att[i] || (strchr(toFilter, att[i] ) == NULL) ) {
                nOfFiltered--;
                return false;
            }
        }
    }
    for (int i = 0; i < x; ++i) {
        for (int j = 0; j < nti; ++j) {
            if (toFilter[i] == necessary_tmp[j].letter) {
                necessary_tmp[j].number--;
                if (necessary_tmp[j].isMax && necessary_tmp[j].number < 0) {
                    nOfFiltered--;
                    return false;
                }
                break;
            }
        }
    }
    for (int i = 0; i < nti; ++i) {
        if ( necessary_tmp[i].isMax == false ) {
            if (necessary_tmp[i].number > 0) {
                nOfFiltered--;
                return false;
            }
        }
        else if (necessary_tmp[i].isMax == true) {
            if (necessary_tmp[i].number != 0) {
                nOfFiltered--;
                return false;
            }
        }
    }
    return true;
}

void rbFirstFilter ( rb_node* node, char* att, wordTable *att_wt, const char* result, Necessary* necessary_tmp, short nti ) { //TODO: develop this function in a non-recursive way
    if( node->left != T_Nil ) {
        rbFirstFilter(node->left, att, att_wt, result , necessary_tmp, nti);
    }
    Necessary necessary_copy[nti];
    for (int i = 0; i < nti; i++) {
        necessary_copy[i].letter = necessary_tmp[i].letter;
        necessary_copy[i].number = necessary_tmp[i].number;
        necessary_copy[i].isMax = necessary_tmp[i].isMax;
    }
    bool toKeep = filterWord( node->word, att, result, necessary_copy, nti);
    if ( toKeep ) {
        if (head == NULL) {
            head = node;
            tail = node;
        } else {
            rb_node *tmp = tail;
            tmp->next_filtrate = node;
            tail = node;
            tail->next_filtrate = NULL;
        }
    }
    if( node->right != T_Nil ) {
        rbFirstFilter(node->right, att, att_wt, result , necessary_tmp, nti);
    }
}

void rbFilter ( rb_node* current, char* att, wordTable *att_wt, const char* result, Necessary *necessary_tmp, short nti) {
    bool toKeep;
    rb_node *precedent = NULL;
    do {
        Necessary necessary_copy[nti];
        for (int i = 0; i < nti; i++) {
            necessary_copy[i].letter = necessary_tmp[i].letter;
            necessary_copy[i].number = necessary_tmp[i].number;
            necessary_copy[i].isMax = necessary_tmp[i].isMax;
        }
        toKeep = filterWord( current->word, att, result, necessary_copy, nti);
        if ( toKeep ) {
            precedent = current;
            current = precedent->next_filtrate;
        }
        else {
            if ( precedent == NULL ) { //if precedent == NULL, it means that we want to remove the head of the list
                head = current->next_filtrate;
                current->next_filtrate = NULL;
                precedent = NULL;
                current = head;
            }
            else if ( tail == current ) { // id tail == current, it means that we want to remove the tail of the list
                tail = precedent;
                precedent->next_filtrate = NULL;
                current = NULL;
            }
            else { // here we want to remove an intermediate node of the list
                precedent->next_filtrate = current->next_filtrate;
                current->next_filtrate = NULL;
                current = precedent->next_filtrate;
            }
        }
        if (current == NULL)
            break;
    } while ( 1 );
}

int readCommand() {
    short int i = 0;
    int result;
    command[i] = '+';

    do {
        i++;
        command[i] = getc_unlocked(stdin);
    } while( command[i] != '\n');
    command[i] = '\0';

    if( strcmp( command, "+inserisci_inizio" ) == 0 ){
        result = 1;
    }
    else if( strcmp( command, "+inserisci_fine" ) == 0 ){
        result = 2;
    }
    else if( strcmp( command, "+nuova_partita" ) == 0 ){
        result = 3;
    }
    else if( strcmp( command, "+stampa_filtrate" ) == 0 ){
        result = 4;
    }
    else {
        result = -1;
    }
    memset(&command, '\0', MAX_COMMAND_LENGTH);
    return result;
}

void readWord( char* word ) {
    short int i = 0;
    do {
        i++;
        word[i] = getc_unlocked(stdin);
    } while( word[i] != '\n' );
    word[i] = '\0';
}

int firstRead() {

    char w;
    char word[x+1];
    int c_res;
    command[0] = '\0';
    dictionary = T_Nil;

    do {
        w = getc_unlocked(stdin);
        if ( w == EOF ){
            terminated_input = true;
            return -1;
        }
        else if (w == '+') {
            c_res = readCommand();
            return c_res;
        }
        else {
            word[0] = w;
            readWord( word );
            rbInsert( &dictionary, word );
            max_nOfFiltered++;
        }
    } while( 1 );
}

bool filterInsertFirst(char word[x], Discovery discovery[x], Necessary *necessary, short necessary_index, char *forbidden) {

    //if (word[0] == 'r')
    //printf("BREAK TO DEBUG!\n");

    for (int i = 0; i < x; i++) {
        if (discovery[i].letter != '?' && word[i] != discovery[i].letter) {
            return false;
        }
        if (strchr(forbidden, word[i]) != NULL)
            return false;
        if (discovery[i].forbidden != NULL && strchr(discovery[i].forbidden, word[i]) != NULL)
            return false;
        for (int j = 0; j < necessary_index; ++j) {
            if (word[i] == necessary[j].letter) {
                necessary[j].number--;
                if (necessary[j].isMax && necessary[j].number < 0)
                    return false;
                break;
            }
        }
    }
    for (int i = 0; i < necessary_index; ++i) { // TODO: maybe this cycle could be optimized in some way with a wordTable (?)
        if (necessary[i].isMax) {
            if (necessary[i].number != 0)
                return false;
        }
        else if (!necessary[i].isMax) {
            if (necessary[i].number > 0)
                return false;
        }
    }
    return true;
}

rb_node *rbGetBeforeNode (rb_node* src) {
    if (src->left != T_Nil) {
        src = src->left;
        while (src->right != T_Nil) {
            src = src->right;
        }
        return src;
    }
    else {
        if (src == src->p->right) {
            src = src->p;
            return src;
        }
        else {
            while (src == src->p->left)
                src = src->p;
            src = src->p;
            return src;
        }
    }
}

void insertStartMatch(Discovery *discovery, Necessary *necessary, short necessary_index, char *forbidden) {

    char w;
    char word[x+1];
    bool toInsert;
    Necessary necessary_copy[necessary_index];

    do {
        w = getc_unlocked(stdin);
        if (w == '+' && readCommand() == 2)
            return;
        word[0] = w;
        readWord(word);
        rb_node *new = rbInsert(&dictionary, word);
        max_nOfFiltered++;
        for (int i = 0; i < necessary_index; i++) {
            necessary_copy[i].letter = necessary[i].letter;
            necessary_copy[i].number = necessary[i].number;
            necessary_copy[i].isMax = necessary[i].isMax;
        }
        toInsert = filterInsertFirst(word, discovery, necessary_copy, necessary_index, forbidden);
        if (toInsert) {
            nOfFiltered++;
            rb_node *src = new;
            if (strcmp(new->word, head->word) < 0) {
                rb_node *tmp = head;
                head = new;
                new->next_filtrate = tmp;
            } else if (strcmp(new->word, tail->word) > 0) {
                rb_node *tmp = tail;
                tail = new;
                new->next_filtrate = NULL;
                tmp->next_filtrate = new;
            } else {
                do {
                    src = rbGetBeforeNode(src);
                    if (src->next_filtrate != NULL) {
                        rb_node* tmp = src->next_filtrate;
                        src->next_filtrate = new;
                        new->next_filtrate = tmp;
                        break;
                    }
                }while (1);
            }
        }
    } while (true);
}

short int insertStartNotMatch( bool firstTry ) {

    char w;
    char word[x];
    int c_res;
    command[0] = '\0';

    do {
        w = getc_unlocked(stdin);
        if ( w == EOF ){
            terminated_input = true;
            return -1;
        }
        else if (w == '+') {
            c_res = readCommand();
            if ( c_res == 2 ){
                return 1;
            }
        }
        else {
            word[0] = w;
            readWord( word );
            rbInsert( &dictionary, word );
            max_nOfFiltered++;
            if ( firstTry )
                nOfFiltered++;
        }
    } while ( 1 );
}

short int readAttempt(char* attempt, Discovery *discovery, Necessary *necessary, short necessary_index, char *forbidden, bool firstTry ) {
    char w;
    int r;
    do {
        w = getc_unlocked(stdin);

        if ( w == EOF ){
            return -1;
        }
        else if ( w == '+' ){
            r = readCommand();
            if ( r == 1 ) {
                if (!firstTry)
                    insertStartMatch(discovery, necessary, necessary_index, forbidden);
                else
                    insertStartNotMatch( firstTry );
            }
            else if ( r == 4 ){
                printFiltered(head);
            }
        }
        else {
            attempt[0] = w;
            readWord(attempt);

            if ( rbSearch( &dictionary, attempt ) == T_Nil ) {
                printf("not_exists\n");
            }
            else {
                return 1;
            }
        }
    }while (1);
}

short int calculateWT( const char* word, wordTable* wt ){

    int nOfLetters = 0;

    for (int i = 0; i < x; ++i) {
        int f = -1;
        for (int j = 0; j < nOfLetters; ++j) {
            if (word[i] == wt[j].letter) {
                f = j;
                break;
            }
        }
        if ( f != -1 ){
            wt[f].number++;
            wt[f].positions[wt[f].number - 1] = i;
        }
        else{
            wt[nOfLetters].letter = word[i];
            wt[nOfLetters].number = 1;
            wt[nOfLetters].positions = (short int*) malloc(x * sizeof( short int ) );
            memset(wt[nOfLetters].positions, (char)-1, x* sizeof(short int));
            wt[nOfLetters].positions[ wt[ nOfLetters ].number - 1 ] = i;
            nOfLetters++;
        }
    }

    return nOfLetters;
}

void disposeWT( wordTable* wt, short index ){
    for ( int i = 0; i < index; i++ ) {
        if (wt[i].positions != NULL) {
            free(wt[i].positions);
            wt[i].positions = NULL;
        }
        wt[i].number = 0;
        wt[i].letter = '\0';
    }
    free(wt);
    wt = NULL;
}

void buildDiscovery(Discovery discovery[x]) {
    for (int i = 0; i < x; i++) {
        discovery[i].letter = '?';
        discovery[i].forbidden = NULL;
        //discovery[i].forbidden = (char*) malloc( x + 1 );
        //memset(discovery[i].forbidden, '\0', x + 1);
    }
}

void buildNecessary (Necessary *necessary, short n) { // maybe I don't have to initialize anything
    for (int i = 0; i < n; ++i) {
        necessary[i].letter = '\0';
        necessary[i].number = 0;
        necessary[i].isMax = false;
    }
}

short *necessaryInsert(Necessary *n, short *ni, char l, bool isMax, short number) {
    short int exist = -1;
    for (int i = 0; i < *ni; ++i) {
        if (n[i].letter == l) {
            exist = i;
            if (n[i].isMax)
                return ni;
            else
                break;
        }
    }
    if (exist == -1) {
        (*ni) = (*ni) + 1;
        n[*ni-1].letter = l;
        n[*ni-1].number = number;
        if (isMax) {
            n[*ni-1].isMax = true;
        } else {
            n[*ni-1].isMax = false;
        }
    }
    else {
        if ( number > n[exist].number )
            n[exist].number = number;
        if ( isMax )
            n[exist].isMax = isMax;
    }

    return ni;
}

bool calculateResult(wordTable* reference_wt, short int ref_wt_index, const char* reference, const char* attempt, wordTable *attempt_wt, short int att_wt_index, char* result, Discovery *discovery, Necessary *necessary, short int *necessary_index,  char* forbidden) {

    short int exists_in_reference, exists_in_attempt;
    bool correct_guess = true;

    for ( int i = 0; i < x; i++ ) {
        if (result[i] != '+' && result[i] != '|' && result[i] != '/' && result[i] != '-') {
            exists_in_reference = -1;
            exists_in_attempt = -1;
            if (reference[i] == attempt[i]) {
                result[i] = '+';
                if( discovery[i].letter == '?'){
                    discovery[i].letter = attempt[i];
                    if (discovery[i].forbidden != NULL) {
                        free(discovery[i].forbidden);
                        discovery[i].forbidden = NULL;
                    }
                }
            }
            else {
                correct_guess = false;
                for (int j = 0; j < ref_wt_index; ++j) {
                    if (reference_wt[j].letter == attempt[i]) {
                        exists_in_reference = j;
                        break;
                    }
                }
                if (exists_in_reference != -1) {
                    for (int j = 0; j < att_wt_index; ++j) {
                        if (attempt_wt[j].letter == attempt[i]) {
                            exists_in_attempt = j;
                            break;
                        }
                    }
                    if (reference_wt[exists_in_reference].number >= attempt_wt[exists_in_attempt].number) {
                        result[i] = '|';
                        necessary_index = necessaryInsert(necessary, necessary_index, attempt[i], false, attempt_wt[exists_in_attempt].number); // same, but the number is not the maximum
                        if (discovery[i].letter == '?') {
                            if (discovery[i].forbidden == NULL) {
                                discovery[i].forbidden = malloc(x);
                                memset(discovery[i].forbidden, '\0', x);
                                char toConc[2] = { attempt[i], '\0' };
                                strcat(discovery[i].forbidden, toConc);
                            }
                            else {
                                if (strchr(discovery[i].forbidden, attempt[i]) == NULL) {
                                    char toConc[2] = { attempt[i], '\0' };
                                    strcat(discovery[i].forbidden, toConc);
                                }
                            }
                        }
                    }
                    else {
                        short int ni = reference_wt[exists_in_reference].number;
                        short int ci = 0, si = 0;

                        for (int j = 0; j < attempt_wt[exists_in_attempt].number; ++j) {
                            if (reference[attempt_wt[exists_in_attempt].positions[j]] == attempt[i]) {
                                ci++;
                                result[attempt_wt[exists_in_attempt].positions[j]] = '+';
                                if (discovery[attempt_wt[exists_in_attempt].positions[j]].letter == '?') {
                                    discovery[attempt_wt[exists_in_attempt].positions[j]].letter = attempt[i];
                                    if (discovery[attempt_wt[exists_in_attempt].positions[j]].forbidden != NULL) {
                                        free(discovery[attempt_wt[exists_in_attempt].positions[j]].forbidden);
                                        discovery[attempt_wt[exists_in_attempt].positions[j]].forbidden = NULL;
                                    }
                                }
                            }
                            else if (attempt_wt[exists_in_attempt].positions[j] < i)
                                si++;
                        }
                        if (si < (ni-ci)) {
                            result[i] = '|';
                            if (discovery[i].letter == '?') {
                                if (discovery[i].forbidden == NULL) {
                                    discovery[i].forbidden = malloc(x);
                                    memset(discovery[i].forbidden, '\0', x);
                                    char toConc[2] = { attempt[i], '\0' };
                                    strcat(discovery[i].forbidden, toConc);
                                }
                                else {
                                    if (strchr(discovery[i].forbidden, attempt[i]) == NULL) {
                                        char toConc[2] = { attempt[i], '\0' };
                                        strcat(discovery[i].forbidden, toConc);
                                    }
                                }
                            }
                            necessary_index = necessaryInsert(necessary, necessary_index, attempt[i], true, ni);
                        }
                        else {
                            result[i] = '-';
                            necessary_index = necessaryInsert(necessary, necessary_index, attempt[i], true, ni);
                            if (discovery[i].letter == '?') {
                                if (discovery[i].forbidden == NULL) {
                                    discovery[i].forbidden = malloc(x);
                                    memset(discovery[i].forbidden, '\0', x);
                                    char toConc[2] = { attempt[i], '\0' };
                                    strcat(discovery[i].forbidden, toConc);
                                }
                                else {
                                    if (strchr(discovery[i].forbidden, attempt[i]) == NULL) {
                                        char toConc[2] = { attempt[i], '\0' };
                                        strcat(discovery[i].forbidden, toConc);
                                    }
                                }
                            }
                        }
                    }
                }
                else {
                    result[i] = '/';
                    if (strchr(forbidden, attempt[i]) == NULL) {
                        char toConc[2] = {attempt[i], '\0'};
                        strcat(forbidden, toConc);
                    }
                }
            }
        }
    }
    result[x] = '\0';
    return correct_guess;
}

void printResult(char *result) { for ( int i = 0; i < x; i++ ) { if(result[i] == '-'){ putc_unlocked('/', stdout); continue;} putc_unlocked(result[i], stdout); } printf("\n"); memset(result, '\0', x); }

void disposeDiscovery(Discovery *discovery) {
    for (int i = 0; i < x; ++i) {
        if (discovery[i].forbidden != NULL && discovery[i].letter == '?') {
            free(discovery[i].forbidden);
            discovery[i].forbidden = NULL;
        }
        discovery[i].letter = '\0';
    }
    discovery = NULL;
}

void restoreFiltered(rb_node *node) {
    if (node == NULL)
        return;
    rb_node *curr = node;
    rb_node *tmp;
    do {
        tmp = curr->next_filtrate;
        curr->next_filtrate = NULL;
        curr = tmp;
    }while (curr != NULL && curr->next_filtrate != NULL);
    head = NULL;
    tail = NULL;
}

short int newGame() {

    nOfFiltered = max_nOfFiltered;

    char reference_word[x+1], attempt_word[x+1], result[x+1];
    short int nOfAttempts = -1;
    short int read_attempt_result = -1;
    short int ref_wt_index = 0;
    bool gameEnd = false; // Can become true if I guess correctly or if nOfAttempts = 0 or if EOF is reached
    bool firstTry = true;
    wordTable* reference_wt = (wordTable*) malloc( x * sizeof(wordTable) );

    memset(result, '\0', x);

    //building of the discovery structure
    Discovery discovery[x];
    buildDiscovery(discovery);

    //building of forbidden_letters
    char forbidden_letters[64-x];
    forbidden_letters[0] = '\0';

    char w = getc_unlocked(stdin);
    reference_word[0] = w;
    readWord(reference_word); // I'm reading the reference word. It must be in the dictionary so no control is needed
    if (scanf("%hd\n", &nOfAttempts) == 0) {}
    ref_wt_index = calculateWT(reference_word, reference_wt); // I'm creating the word table of reference word

    //building of the necessary structure
    Necessary necessary[ref_wt_index];
    short int necessary_index = 0;
    buildNecessary(necessary, ref_wt_index);

    do {
        read_attempt_result = readAttempt( attempt_word, discovery, necessary, necessary_index, forbidden_letters, firstTry ); // I'm reading an attempt word, if the word is not in the dictionary, print "not exists", if instead of an attempt I read a command, return 0, if the attempt is valid, return 1.
        wordTable* attempt_wt = (wordTable*) malloc( x * sizeof(wordTable) );
        short int att_wt_index = calculateWT(attempt_word, attempt_wt); // I'm creating the word table of reference word
        if ( read_attempt_result == -1 ) {
            return -1;
        }
        else {
            Necessary necessary_tmp[ref_wt_index];
            buildNecessary(necessary_tmp, ref_wt_index);
            gameEnd = calculateResult( reference_wt, ref_wt_index, reference_word, attempt_word, attempt_wt, att_wt_index, result, discovery, necessary, &necessary_index, forbidden_letters );

            if (firstTry && !gameEnd) {
                rbFirstFilter(dictionary, attempt_word, attempt_wt, result, necessary, necessary_index);
                firstTry = false;
            }
            else if (!gameEnd) {
                rbFilter(head, attempt_word, attempt_wt, result, necessary, necessary_index);
            }
            if ( !gameEnd ) {
                nOfAttempts--;
                printResult(result);
                if ( nOfAttempts == 0 ) {
                    printf("%d\n", nOfFiltered);
                    printf("ko\n");
                    gameEnd = true;
                }
                else {
                    printf("%d\n", nOfFiltered);
                }
            }
            else {
                printf("ok\n");
            }
        }
        disposeWT(attempt_wt, att_wt_index);
    } while ( !gameEnd );
    disposeWT(reference_wt, ref_wt_index);
    disposeDiscovery(discovery);
    restoreFiltered(head);
    return 1;
}

int main() {

    int c_res, ng_outcome;
    char w;

    if (scanf("%d\n", &x)){}

    c_res = firstRead();

    if ( c_res == 1 ) {
        if ( insertStartNotMatch(false) == -1 )
            terminated_input = true;
    }
    if ( c_res == 3 ) {
        ng_outcome = newGame();
        if ( ng_outcome == -1 )
            terminated_input = true;
    }
    if ( c_res == 4 ) {
        if (dictionary != NULL)
            rbPrint(dictionary);
    }

    while ( !terminated_input ) {
        if( ( w = getc_unlocked(stdin) ) == EOF ) {
            terminated_input = true;
        }
        else if ( w == '+' ){
            c_res = readCommand();
            if ( c_res == 1 ) {
                if ( insertStartNotMatch(false) == -1 )
                    terminated_input = true;
            }
            if ( c_res == 3 ) {
                ng_outcome = newGame();
                if ( ng_outcome == -1 )
                    terminated_input = true;
            }
            if ( c_res == 4 ) {
                if ( dictionary != NULL ) {
                    rbPrint(dictionary);
                }
            }
        }
    }

    rbDispose(dictionary);
    return 0;
}