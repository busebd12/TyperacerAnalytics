CC=g++ -std=c++14 -I /usr/loca/boost_1_57_0 -lboost_date_time -lpthread
smallCC=g++ -std=c++14

a.out: main.o TypingRace.o
	$(CC) main.o TypingRace.o

main.o: main.cpp TypingRace.h csv.h
	$(smallCC) -c main.cpp

TypingRace.o: TypingRace.cpp TypingRace.h
	$(smallCC) TypingRace.cpp -c

clean:
	rm a.out