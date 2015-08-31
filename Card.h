#ifndef _CARD_H
#define _CARD_H
#include <iostream>
#include <vector>
#include <algorithm>
#include <string.h>
#include <stdio.h>
using namespace std;

#define MJTYPE_UNDEFINE			-1
#define MJTYPE_BEGIN			0
#define MJTYPE_TIAO				0
#define MJTYPE_BING				1
#define MJTYPE_WAN				2
#define MJTYPE_BAIDA			3
#define MJTYPE_END				4

#define BAIDA 0

struct MJCard
{
		char type;
		char value;
		char flag;
};

class CardGroup
{
public:
		bool jiang;
		int baida;
		int type;
		vector< int > cards;
};
#endif
