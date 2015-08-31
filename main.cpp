#include "CardPlayer.h"
#include "CardTable.h"

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
using namespace std;

int mygetch( ) {
	struct termios oldt,newt;
	int ch;
	tcgetattr( STDIN_FILENO, &oldt );
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newt );
	ch = getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
	return ch;
}

int getIndex(char c)
{
	switch(c)
	{
		case '`':
			return 0;
		case '1':
			return 1;
		case '2':
			return 2;
		case '3':
			return 3;
		case '4':
			return 4;
		case '5':
			return 5;
		case '6':
			return 6;
		case '7':
			return 7;
		case '8':
			return 8;
		case '9':
			return 9;
		case '0':
			return 10;
		case '-':
			return 11;
		case '=':
			return 12;
		case 8:
			return 13;
		default:
			return 13;
	}
}

int test()
{
#define SIZE_TIAO  14
#define SIZE_BING  0
#define SIZE_WAN  0
	int tiao[SIZE_TIAO] = {1, 2, 2, 3, 3, 3, 4, 4, 5, 8, 9, 9, 6, 7};
	int bing[SIZE_BING] = {};
	int wan[SIZE_WAN] = {};

	CardPlayer		player;
	CardTable		table;

	MJCard card;
	card.type = MJTYPE_TIAO;
	card.value = 7;
	player.setBaida(card);
	int i = 0 ;
	for(i = 0; i < SIZE_TIAO; i++)
	{
		card.type = MJTYPE_TIAO;
		card.value = tiao[i];
		player.addCard(card);
	}
	for(i = 0; i < SIZE_BING; i++)
	{
		card.type = MJTYPE_BING;
		card.value = bing[i];
		player.addCard(card);
	}
	for(i = 0; i < SIZE_WAN; i++)
	{
		card.type = MJTYPE_WAN;
		card.value = wan[i];
		player.addCard(card);
	}

	player.dump();
	//player.PrintAllCards();

	bool ret = player.checkHu();
	if(ret) player.showHu();
	printf("ret: %d\n", ret);


	return 0;
}

int game()
{
	CardPlayer		player;
	CardTable		table;
	player.setTable(&table);
	int				score = 0;

	MJCard card;
	bool game = true;
	while(game)
	{
		unsigned int seed = time(0);
		unsigned int in;
		bool ret = false;
		char c;
		table.init(0, seed);
		player.reset();

		card = table.getCard();
		player.setBaida(card);
		for(int i = 0 ; i < 13 ; i++)
		{

			card = table.getCard();
			player.addCard(card);
		}
		if(0)
		{
			player.reset();
			card.type = MJTYPE_BING;
			card.value = 7;
			player.setBaida(card);

			card.type = MJTYPE_TIAO;
			card.value = 1;
			player.addCard(card);
			card.value = 2;
			player.addCard(card);
			card.value = 3;
			player.addCard(card);
			card.value = 3;
			player.addCard(card);
			card.value = 3;
			player.addCard(card);
			card.value = 4;
			player.addCard(card);
			card.value = 4;
			player.addCard(card);
			card.value = 5;
			player.addCard(card);
			card.value = 5;
			player.addCard(card);
			card.value = 6;
			player.addCard(card);
			card.type = MJTYPE_WAN;
			card.value = 5;
			player.addCard(card);
			card.value = 6;
			player.addCard(card);
			card.value = 7;
			player.addCard(card);
		}
		printf("\033[%dA", (40));
		printf("\033[2J");
		printf("\n");
		player.showGuid();
		player.showCards();
		//player.dump();
		printf("rest cards: %d\n", table.getCardNum());
		while(1)
		{
			/*
			   printf("press any key to get a new card...\n");
			   mygetch();
			 */
			card = table.getCard();
			if(0)
			{
				card.type = MJTYPE_WAN;
				card.value = 7;
			}
			if(card.type == -1 && card.value == -1)
			{
				printf("zhua huang le...\n");
				break;
			}
			player.addTmpCard(card);
			printf("\033[%dA", (40));
			printf("\033[2J");
			printf("\n");
			player.showGuid();
			player.showCards();
			//player.dump();
			printf("rest cards: %d\n", table.getCardNum());
			player.rmTmpCard();

			player.addCard(card);
			ret = player.checkHu();
			if(ret == true)
			{
				player.showHu();
				printf("press 'n' jixu. qita HU.\n");
				c = mygetch();
				if(c != 'n')
					break;
			}
			player.rmCard(card);

			while(1)
			{
				printf("press key\n");
				c = mygetch();
				in = getIndex(c);
				// this is the card we just got
				if(in == 13)
				{
					if(player.isBaida(card))
					{
						printf("baida cannot be drop\n");
						continue;
					}
					break;
				}
				ret = player.rmCard(in);
				if(ret == true)
				{
					player.addCard(card);
					printf("\033[%dA", (40));
					printf("\033[2J");
					printf("\n");
					player.showGuid();
					player.showCards();
					//player.dump();
					printf("reset cards: %d\n", table.getCardNum());
					ret = player.checkTing();
					if(ret)
					{
						printf("press anykey...\n");
						mygetch();
					}
					break;
				}
				printf("\nrmCard failed: %d\n", in);
			}
		}

		printf("press any key continue, press 'q' to quit.\n");
		c = mygetch();
		if(c == 'q'){ game = false;}
	}

	return 0;
}
int main(int argc, char* argv[])
{
	return game();
	//return test();
}

