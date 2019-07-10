#	Projekt:
#		PROJEKT IFJ18 - Tvorba vlastního pøekladaèe

#	Autor:
#		Petr Urbánek - Xurban 70

#	Soubor:
#		symtable.h

#	Varianta:
#		Tým 85, varianta 1



CC = gcc
CFLAGS =-O0 -std=c99 -g -Wall -Werror -pedantic


main: main.c error.c parser.h str.h list.h instruction.h symtable.h scanner.h expression_list.h precedence_analysis.h stack.h
	$(CC) $(CFLAGS) main.c list.c list.h instruction.h instruction.c symtable.c symtable.h stack.c stack.h error.h error.c parser.c parser.h str.c str.h scanner.c scanner.h expression_list.h expression_list.c precedence_analysis.h precedence_analysis.c -o $@

List: list.h str.h  error.h 
	$(CC) $(CFLAGS) str.h str.c list.c list.h error.h error.c -o $@

symtable: symtable.h error.h main.c
	$(CC) $(CFLAGS) main.c symtable.c symtable.h error.h error.c -o $@

instruction: instruction.h str.h error.h list.h main.c
	$(CC) $(CFLAGS) main.c instruction.c instruction.h str.h str.c list.c list.h error.h error.c -o $@

Error: error.c main.c
	$(CC) $(CFLAGS) main.c error.c error.h -o $@

Parser: parser.h error.h str.h symtable.h scanner.h main.c precedence_analysis.h expression_list.h stack.h
	$(CC) $(CFLAGS) main.c parser.c parser.h symtable.c symtable.h error.c error.h str.c str.h scanner.c scanner.h precedence_analysis.c precedence_analysis.h expression_list.h expression_list.c stack.c stack.h -o $@ 

String: str.h error.h main.c 
	$(CC) $(CFLAGS) main.c str.h str.c error.h error.c -o $@

Scanner: scanner.h error.h str.h main.c
	$(CC) $(CFLAGS) main.c scanner.h scanner.c str.h str.c error.h error.c -o $@

Precedence_analysis: precedence_analysis.h error.h parser.h list.h scanner.h str.h stack.h
	$(CC) $(CFLAGS) main.c stack.h stack.c precedence_analysis.h precedence_analysis.c scanner.h scanner.c str.h str.c error.h error.c  -o $@

		

run: main
	./main

clean:
	rm -f *.o symtable List Precedence_analysis Expres  instruction Error Parser String Scanner main IFJ-PROJEKT.zip

zip: *.c *.h Makefile
	zip IFJ-PROJEKT.zip *.c *.h Makefile


