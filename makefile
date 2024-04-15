assembler: main.o macro.o firstScan.o secondScan.o dataStructures.o filesGenerator.o global.o
	gcc -pedantic -ansi -Wall -g main.o macro.o firstScan.o secondScan.o dataStructures.o filesGenerator.o global.o -o assembler
main.o: assemblerMain.c
	gcc -c -pedantic -ansi -Wall -g assemblerMain.c -o main.o
macro.o: macro.c
	gcc -c -pedantic -ansi -Wall macro.c -o macro.o
firstScan.o: firstScan.c
	gcc -c -pedantic -ansi -Wall firstScan.c -o firstScan.o
secondScan.o: secondScan.c
	gcc -c -pedantic -ansi -Wall secondScan.c -o secondScan.o
global.o: global.c
	gcc -c -pedantic -ansi -Wall global.c -o global.o
dataStructures.o: dataStructures.c
	gcc -c -pedantic -ansi -Wall dataStructures.c -o dataStructures.o
filesGenerator.o: filesGenerator.c
	gcc -c -pedantic -ansi -Wall filesGenerator.c -o filesGenerator.o

