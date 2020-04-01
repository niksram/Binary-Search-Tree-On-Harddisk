GFLAG=-g

a.out: A4_PES1201801972.o A4_client_PES1201801972.o
	gcc $(GFLAG) A4_PES1201801972.o A4_client_PES1201801972.o

A4_client_PES1201801972.o: assignment_4.h A4_client_PES1201801972.c
	gcc -c assignment_4.h A4_client_PES1201801972.c

A4_PES1201801972.o: assignment_4.h A4_PES1201801972.c
	gcc -c assignment_4.h A4_PES1201801972.c

clean:
	rm -f A4_PES1201801972.o A4_client_PES1201801972.o assignment_4.h.gch
