/*
    Projekt:
        PROJEKT IFJ18 - Tvorba vlastního pøekladaèe

    Autor:
        Martin Kladňák - xkladn00

    Soubor:
        scanner.c

    Varianta:
        Tým 85, varianta 1
*/

#ifndef SCANNER_C
#define SCANNER_C
#include "scanner.h"
#define POCET_KLICOVYCH_SLOV 
//Vsechna klicova slova
char* keywords[] = {
    "def", "do", "else", "end", "if", "not", "nill", "then", "while"
};
char *fce[] = {
    "inputs", "inputi", "inputf", "print", "length", "substr", "ord", "chr"
};
// Vraci hodnotu preddefinovano fce pokud ji najde

int isfce(char *slovo) {
    int COUNT_FCE = 8;
    int tmp = 34;
    for (int i = 0; i < COUNT_FCE; i++)

        if ((strcmp(slovo, fce[i])) == 0) {
            return (tmp + i);
        }
    return 99;
}
// Vraci hodnotu klicoveho slovo pokud ji najde

int iskeyword(char *slovo) {
    int COUNT_KEYWORD = 9;
    int tmp = 11;
    for (int i = 0; i < COUNT_KEYWORD; i++)

        if ((strcmp(slovo, keywords[i])) == 0) {
            return (tmp + i);
        }
    return 99;
}
/*
int main() {
    int k;
    FILE *file = fopen("test", "r");
    struct token *tmptoken;

    while (1) {

        tmptoken = newToken();
        if ((k = get_token(tmptoken, file)) == 2) {
            printf("%d ", tmptoken->typ);
            printf("Konec souboru\n");
            break;
        }
        printf("%d ", tmptoken->typ);
        printf("%s\n", tmptoken->data);
    }
    return 0;
}*/
//Vratim se o jeden znak zpet v souboru

void ungetchar(int znak, FILE * file) {
    if (znak == 10) {

        ungetc(znak, file);
    } else
        if (!isspace(znak)) {
        ungetc(znak, file);
    }
}

void ungettoken(struct token *tokenptr, FILE * file) {
    int i = 32; // mezera
    ungetc(i, file);
    for (i = tokenptr->length; tokenptr->length >= 0; tokenptr->length--)
        if (tokenptr->data[tokenptr->length] != '\0')
            ungetc(tokenptr->data[tokenptr->length], file);
}

//Zjisti token

int get_token(struct token *tokenptr, FILE * file) {
    //pomocne promene
    char check_end_blok[] = "****";
    char end_block[] = "=end";
    char start_block[] ="begin";
    int count_char = 0;
    int collum = 0;
    int c, tmp;
    int key_words;
    tstav stav = S_START;
    bool string_literal = false;
    bool exponent = false;
    bool digit = false;
    bool desetine_cislo = false;
    bool zero = false;
    bool second_zero = false;

    tmp = 0;
    while (1) {
        c = getc(file);

        collum++;
        switch (stav) {
            case S_START: //Analyzuji prvni znak noveho tokenu
                if ((isalpha(c) && islower(c)) || (c == '_')) {
                    stav = S_IDENTIFIKATOR;
                    if (strAddChar(tokenptr, c) == 1)
                        return INTERNAL_ERROR;
                } else if (isdigit(c)) {
                    if (strAddChar(tokenptr, c) == 1)
                        return INTERNAL_ERROR;
                    if (c == '0')
                        zero = true;
                    stav = S_INTEGER;
                    break;
                } else if (c == -1) {
                    tokenptr->typ = END_OF_FILE;
                    return 2;
                }//Dvouoperatorove operace
                else if (c == '=') {
                    if (strAddChar(tokenptr, c) == 1) {
                        return INTERNAL_ERROR;
                    }
                    stav = S_ROVNO;
                } else if (c == '<') {
                    if (strAddChar(tokenptr, c) == 1) {
                        return INTERNAL_ERROR;
                    }
                    stav = S_MENSI;
                } else if (c == '>') {
                    if (strAddChar(tokenptr, c) == 1) {
                        return INTERNAL_ERROR;
                    }
                    stav = S_VETSI;
                } else if (c == '!') {
                    if (strAddChar(tokenptr, c) == 1)
                        return INTERNAL_ERROR;
                    stav = S_RUZNE;

                }//jednooperátorové akce
                else if (c == '(') {
                    tokenptr->typ = OPERATOR_LEVA_ZAVORKA;
                    if (strAddChar(tokenptr, c) == 1)
                        return INTERNAL_ERROR;
                    return 0;
                } else if (c == ')') {
                    tokenptr->typ = OPERATOR_PRAVA_ZAVORKA;
                    if (strAddChar(tokenptr, c) == 1) {
                        return INTERNAL_ERROR;
                    }
                    return 0;
                } else if (c == '+') {
                    tokenptr->typ = OPERATOR_PLUS;
                    if (strAddChar(tokenptr, c) == 1)
                        return INTERNAL_ERROR;
                    return 0;
                } else if (c == '-') {
                    tokenptr->typ = OPERATOR_MINUS;
                    if (strAddChar(tokenptr, c) == 1)
                        return INTERNAL_ERROR;
                    return 0;
                } else if (c == '*') {
                    tokenptr->typ = OPERATOR_KRAT;

                    if (strAddChar(tokenptr, c) == 1)
                        return INTERNAL_ERROR;
                    return 0;

                } else if (c == '/') {
                    tokenptr->typ = OPERATOR_DELENO;
                    if (strAddChar(tokenptr, c) == 1)
                        return INTERNAL_ERROR;
                    return 0;
                } else if (c == ',') {
                    tokenptr->typ = OPERATOR_CARKA;
                    if (strAddChar(tokenptr, c) == 1)
                        return INTERNAL_ERROR;
                    return 0;
                } else if (c == '#') {
                    stav = S_COMENT;
                    break;
                } else if (c == '"') {
                    stav = S_STRING;
                    break;

                } else if (c == '\n') {
                    tokenptr->typ = EOL;
                    if (strAddChar(tokenptr, c) == 1)
                        return INTERNAL_ERROR;
                    stav = S_END;
                    break;
                    //mezery preskakuji
                } else if (isspace(c)) {
                    stav = S_START;
                    break;
                }//Jiny stav neni povoleny
                else {
                    stav = S_CHYBA;
                    break;
                }
                break;

            case S_VETSI:
            {
                if (c == '=') {
                    if (strAddChar(tokenptr, c) == 1)
                        return INTERNAL_ERROR;
                    tokenptr->typ = OPERATOR_BEQ;
                    stav = S_END;
                    break;
                } else {
                    if (strAddChar(tokenptr, c) == 1)
                        return INTERNAL_ERROR;
                    tokenptr->typ = OPERATOR_BTN;
                    stav = S_END;
                    break;
                }
            }
            case S_MENSI:
            {
                if (c == '=') {
                    if (strAddChar(tokenptr, c) == 1)
                        return INTERNAL_ERROR;
                    tokenptr->typ = OPERATOR_LEQ;
                    stav = S_END;
                    break;
                } else {
                    tokenptr->typ = OPERATOR_LTN;
                    ungetchar(c, file);
                    return 0;
                }
            }
            case S_ROVNO:
            {
                if (c == '=') {
                    if (strAddChar(tokenptr, c) == 1)
                        return INTERNAL_ERROR;
                    tokenptr->typ = OPERATOR_EQ;
                    stav = S_END;
                    break;
                } else if (c == start_block[count_char] && (collum == (count_char + 2))) { //resi aby =begin byl na zacatku souboru
                    count_char++;
                    if (c == 'n') {

                        stav = S_BLOK_COMENT;
                        break;
                    }
                    break;
                   
                } else if (collum < 3) {
                    tokenptr->typ = OPERATOR_PRIRAZOVACI;
                    ungetchar(c, file);
                    return 0;
                } else error_exit(SYNTAX_ERROR, "CHyba syntaxe");
            }
            case S_RUZNE: 
            {
                if (c == '=') { 
                    if (strAddChar(tokenptr, c) == 1)
                        return INTERNAL_ERROR;
                    tokenptr->typ = OPERATOR_NEQ;
                    stav = S_END;
                    break;
                } else { //zadna jina moznost neexistuje
                    ungetchar((char) c, file);  
                    stav = S_CHYBA;
                    break;
                }
            }
            case S_IDENTIFIKATOR: 
            {
                if ((isalpha(c) || isdigit(c)) || c == '_') {
                    if (strAddChar(tokenptr, c) == 1)
                        return INTERNAL_ERROR;
                    break;
                } else if (c == '?' || c == '!') {
                    if (strAddChar(tokenptr, c) == 1)
                        return INTERNAL_ERROR;
                    tokenptr->typ = IDENTIFIKATOR;
                    c = getc(file);
                    if (isdigit(c) || isalpha(c) || c == '_')
                        error_exit(LEXICAL_ERROR, "Chyba lexému");
                    ungetchar(c, file);

                    return 0;

                } else { //klicova slova
                    key_words = iskeyword(tokenptr->data);
                    if (key_words != 99) {
                        stav = S_END;
                        tokenptr->typ = key_words;
                        ungetchar(c, file);
                        stav = S_END;
                        break;
                    } else //fce
                        key_words = isfce(tokenptr->data);
                    if (key_words != 99) {
                        stav = S_END;
                        tokenptr->typ = key_words;
                        ungetchar(c, file);
                        stav = S_END;
                        break;
                    } else {//dostal jsem znak kterym koncim
                        tokenptr->typ = IDENTIFIKATOR;
                        ungetchar(c, file);
                        stav = S_END;
                    }
                    break;
                }
            }
            case S_END: // konec pro vetsinu stavu
            {
                while ((tokenptr->typ == EOL) && (c == '\n'))
                    c = getc(file);
                ungetchar(c, file);
                return 0;
            }
            case S_COMENT: //komentare radkovy
            {
                if (c == -1) {
                    ungetchar(c, file);
                    stav = S_END_OF_FILE;
                    break;
                }
                if (c == '\n') {
                    tokenptr->typ = EOL;
                    stav = S_START;
                }
                break;
            }
            case S_BLOK_COMENT:
            {
                int n = 4;
                for (int count_char = 0; count_char < (n - 1); count_char++) //=end = 4 
                {
                    check_end_blok[count_char] = check_end_blok[count_char + 1]; //vkladam si znaku do promene
                }
                check_end_blok[n - 1] = c;
                if (strcmp(check_end_blok, end_block) == 0) {//jestli nactene znaky rovnaji =end
                    c = getc(file);
                    if (c == '\n' || c== ' '){
                    stav = S_CEKEJ;
                    tokenptr->data[0] = '\0';
                    tokenptr->length = 0;
                    break;
                    }
                    else ungetchar(c,file);
                    break;
                }
                if (c == -1)
                {
                    return INTERNAL_ERROR;
                }
                    break;
            }
            case S_CEKEJ: // cekam dokud nedojde '\n nebo EOF' ostatni znaky "vyhodim"
            {
                if (c == '\n') {
                    stav = S_START;
                    collum = 0;
                    count_char = 0;

                } else if (c == -1)
                    stav = S_END_OF_FILE;
                break;
            }
            case S_STRING: //textovy retezec
            {
                if (string_literal == true) {//prevadim hexadecimalni cislo na decimalni a äbych vedel jaka je to hodnota v tabulce
                    if ((isdigit(c) || c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F' || (c >= 97 && c <= 102)) == 1) {
                        tmp = tmp * 16; //AF == 10*16+15 = 175

                        if (isalpha(c)) {
                            if (isupper(c)) {//velke pismena
                                c = c % 65;
                                c = c + 10;
                            } else { //male pismena
                                c = c % 97;
                                c = c + 10;
                            }
                        } else c = c % 48;
                        tmp = tmp + c;

                        if (strAddChar(tokenptr, tmp) == 1)
                            return INTERNAL_ERROR;
                        string_literal = false;
                        tmp = 0;
                        break;
                    } else {
                        string_literal = false;
                        if (strAddChar(tokenptr, tmp) == 1)
                            return INTERNAL_ERROR;
                        tmp = 0;
                        ungetchar(c, file);
                        break;
                    }
                } else if (c == '"') {
                    stav = S_END;
                    tokenptr->typ = STRING;
                    break;
                } else if (c == '\\') {
                    stav = S_LOM_STRING;
                    break;
                } else {
                    if (strAddChar(tokenptr, c) == 1)
                        return INTERNAL_ERROR;
                    else if (c == '\n') {
                        stav = S_CHYBA;
                        break;
                    }
                    break;
                }
            }
            case S_LOM_STRING: //hledam 
            {
                if (c == '\n') {
                    stav = S_CHYBA;
                    break;
                }
                if (c == 'n') {
                    c = '\n'; //newline
                    if (strAddChar(tokenptr, c) == 1)
                        return INTERNAL_ERROR;
                    stav = S_STRING;
                    break;
                } else if (c == 't') //horizontal tab
                {
                    c = 9;
                    if (strAddChar(tokenptr, c) == 1)
                        return INTERNAL_ERROR;
                    stav = S_STRING;
                    break;
                } else if (c == 's') {
                    c = 32; //space
                    if (strAddChar(tokenptr, c) == 1)
                        return INTERNAL_ERROR;
                    stav = S_STRING;
                    break;
                } else if (c == 'x') {
                    stav = S_X_STRING;
                    break;
                } else if (c == '"') {
                    if (strAddChar(tokenptr, c) == 1)
                        return INTERNAL_ERROR;
                    stav = S_STRING;
                    break;
                } else {
                    stav = S_STRING;
                    if (strAddChar(tokenptr, c) == 1)
                        return INTERNAL_ERROR;
                    break;
                }
            }
            case S_X_STRING:
            {
                if (c == '\n')
                    return LEXICAL_ERROR;//prevadim hexadecimalni cislo na decimalni a abych vedel jaka je to hodnota v tabulce
                if ((isdigit(c) || c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F' || (c >= 97 && c <= 102)) == 1) {
                    if (isalpha(c)) {
                        if (isupper(c)) {
                            c = c % 65;
                            c = c + 10;
                        } else {
                            c = c % 97;
                            c = c + 10;
                        }
                    } else c = c % 48;
                    tmp = c;
                    string_literal = true;
                    stav = S_STRING;
                    break;
                } else {
                    stav = S_CHYBA;
                    break;
                }
            }
            case S_INTEGER:
            {

                if (c == '0' && zero == true) {
                    stav = S_CHYBA;
                    break;
                } else {
                    zero = false;
                    if (isdigit(c)) {
                        if (strAddChar(tokenptr, c) == 1)
                            return INTERNAL_ERROR;
                        break;
                    } else if (c == 'e' || c == 'E') {
                        if (strAddChar(tokenptr, c) == 1)
                            return INTERNAL_ERROR;
                        stav = S_EXPONENT;
                        break;
                    } else if (c == '.') {
                        stav = S_DESETINY;
                        if (strAddChar(tokenptr, c) == 1)
                            return INTERNAL_ERROR;
                        break;
                    } else {
                        if (second_zero == true)
                            return LEXICAL_ERROR;
                        else if (isalpha(c)){
                            stav = S_CHYBA;
                          break;
                        }
                      
                        tokenptr->typ = INTEGER;
                        ungetchar(c, file);
                        stav = S_END;
                        break;
                    }
                }
            }
            case S_EXPONENT:
            {
                if ((c == '-' || c == '+') && exponent == false) {
                    if (strAddChar(tokenptr, c) == 1)
                        return INTERNAL_ERROR;
                    exponent = true;
                    break;
                } else if (isdigit(c)) {
                    exponent = true;
                    digit = true;
                    if (strAddChar(tokenptr, c) == 1)
                        return INTERNAL_ERROR;
                    break;
                } else if (digit == false) {

                    error_exit(LEXICAL_ERROR, "Chyba alokace");
                    break;
                } else {
                    if (desetine_cislo == true)
                        tokenptr->typ = DOUBLE_DOTTED_EXPONENT;
                    else if (isalpha(c)){
                        stav = S_CHYBA;
                    
                    break;
                    }
                    else {
                        tokenptr->typ = DOUBLE_EXPONENT;
                        ungetchar(c, file); //nejsu scount_charjisty
                        stav = S_END;
                        break;
                    }
                }

            }
            case S_DESETINY:
            {
                if (isdigit(c)) {
                    desetine_cislo = true;
                    if (strAddChar(tokenptr, c) == 1)
                        return INTERNAL_ERROR;
                    break;
                } else if (desetine_cislo == true && (c == 'e' || c == 'E')) {
                    stav = S_EXPONENT;
                    if (strAddChar(tokenptr, c) == 1)
                        stav = INTERNAL_ERROR;
                    break;
                } else if (isalpha(c))
                    error_exit(LEXICAL_ERROR, "IDENTIFIKATOR za cislem");
                else {
                    tokenptr->typ = DOUBLE;
                    ungetchar(c, file); //nejsu scount_charjisty
                    stav = S_END;
                    break;
                }
            }

            case S_CHYBA: //Chyba lexemu
            {
                return LEXICAL_ERROR;
            }
            case S_END_OF_FILE: //konec souboru
            {

                return 2;
            }
            default:
            {
                return LEXICAL_ERROR;
            }
        }
    }
}
#endif
