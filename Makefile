CC=gcc -w
LIBR=-lm -O3
DEPS = attack.h

all: main

main: main.o $(DEPS)
	$(CC) -o main main.c CPA_Attack.c DPA_Attack.c $(LIBR)

main.o: main.c $(DEPS)
	$(CC) -c main.c $(LIBR)
	
CPA_Attack.o: CPA_Attack.c $(DEPS)
	$(CC) -c CPA_Attack.c $(LIBR)

DPA_Attack.o: DPA_Attack.c $(DEPS)
	$(CC) -c DPA_Attack.c $(LIBR)

clean:
	rm -f *.o main
