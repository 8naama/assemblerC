assembler: main.o firstScan.o dataStructures.o filesGenerator.o
	gcc -pedantic -ansi -Wall -g main.o firstScan.o dataStructures.o filesGenerator.o -o assembler
main.o: assemblerMain.c
	gcc -c -pedantic -ansi -Wall -g assemblerMain.c -o main.o
firstScan.o: firstScan.c
	gcc -c -pedantic -ansi -Wall firstScan.c -o firstScan.o
