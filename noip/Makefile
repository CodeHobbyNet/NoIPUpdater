all: Noip

Noip: Noip.o SQLite IP.cpp
	g++ -o NoIP Noip.o sqlite3.o -ldl -lpthread -lcurl

SQLite: sqlite3.c
	gcc -c sqlite3.c

Noip.o: Noip.cpp IP.cpp
	g++ -c Noip.cpp

clean:
	rm Noip.o sqlite3.o NoIP *~
