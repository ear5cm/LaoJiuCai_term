ljc : CardPlayer.o CardTable.o main.o
	g++ -o ljc -g CardPlayer.o CardTable.o main.o

CardPlayer.o: CardPlayer.cpp
	g++ -c -g CardPlayer.cpp

CardPlayerManage.o: CardTable.cpp
	g++ -c -g CardTable.cpp

main.o: main.cpp
	g++ -c -g main.cpp

clean : 
	rm ljc *.o
