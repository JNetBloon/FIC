all: clean executable valgrind

executable: listas.o p0.o
	gcc -Wall -o p0 listas.o p0.o

%.o: %.c
	gcc -Wall -Wno-stringop-overflow -c -g $<

execute:
	./p0

valgrind: clean executable
	mkdir valgrind
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind/valgrind-report.txt ./p0

clean:
	rm -f *.o *.txt p0
	if [ -d "valgrind" ]; then rm -r valgrind; fi
