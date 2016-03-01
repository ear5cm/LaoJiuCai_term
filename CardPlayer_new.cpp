#include "CardPlayer.h"
#include "CardTable.h"

#define v1 group->cards[0]
#define v2 group->cards[1]
#define v3 group->cards[2]
#define v4 group->cards[3]
#define v5 group->cards[4]
#define v6 group->cards[5]
#define v7 group->cards[6]
#define v8 group->cards[7]
#define v9 group->cards[8]
#define addcard(v) cg->cards.push_back(v)
CardGroup* checkGroup(CardGroup* group);

static int depth = 0;
static int debug = 0;

CardPlayer::CardPlayer()
{
}

CardPlayer::~CardPlayer()
{
}

void CardPlayer::setTable(CardTable* table)
{
	m_table = table;
}

void CardPlayer::reset()
{
	for(int i = 0 ; i < 6 ; i++ )
	{
		m_MyPAIVec[i].clear();
		m_ChiPAIVec[i].clear();
		m_PengPAIVec[i].clear();
		m_GangPAIVec[i].clear();
	}
}

bool CardPlayer::addTmpCard(MJCard card) {
	m_tmpCard.type = card.type;
	m_tmpCard.value = card.value;
	return true;
}

bool CardPlayer::rmTmpCard() {
	m_tmpCard.type = -1;
	m_tmpCard.value = -1;
	return -1;
}

void CardPlayer::addCard(MJCard card)
{
	int p_Type = card.type;
	int p_Value = card.value;
	if(isBaida(card)) p_Type = MJTYPE_BAIDA;
	bool t_Find = false;
	vector<	int >::iterator Iter;
	for(Iter = m_MyPAIVec[p_Type].begin();Iter !=m_MyPAIVec[p_Type].end(); Iter++)
	{
		if((*Iter)>p_Value)
		{
			m_MyPAIVec[p_Type].insert(Iter,p_Value);
			t_Find = true;
			break;
		}

	}

	if(t_Find==false)
	{
		m_MyPAIVec[p_Type].push_back(p_Value);
	}
}

bool CardPlayer::rmCard(int index)
{
	vector< int >::iterator Iter;
	int count = 0;
	for(int i = MJTYPE_BEGIN; i < MJTYPE_BAIDA; i++)
	{
		count = m_MyPAIVec[i].size();
		if(index >= count)
		{
			index -= count;
			continue;
		}
		count = 0;
		for(Iter = m_MyPAIVec[i].begin();Iter !=m_MyPAIVec[i].end(); Iter++)
		{
			if(index == count)
			{
				m_MyPAIVec[i].erase(Iter);
				return true;
			}
			count++;
		}
	}
	return false;
}

bool CardPlayer::rmCard(MJCard card)
{
	int p_Type = card.type;
	int p_Value = card.value;
	if(isBaida(card)) p_Type = MJTYPE_BAIDA;
	vector<	int >::iterator Iter;
	for(Iter = m_MyPAIVec[p_Type].begin();Iter !=m_MyPAIVec[p_Type].end(); Iter++)
	{
		if((*Iter) == p_Value)
		{
			m_MyPAIVec[p_Type].erase(Iter);
			break;
		}

	}

	return false;
}

void CardPlayer::setBaida(MJCard card)
{
	m_baida = card;
	m_baidaVec.clear();
	MJCard c = m_baida;
	int value1 = m_baida.value;
	int value2;
	if(value1 == 1) value2 = 9;
	else value2 = value1 - 1;
	c.value = value2;
	m_baidaVec.push_back(m_baida);
	m_baidaVec.push_back(c);
}

bool CardPlayer::isBaida(MJCard card)
{
	vector< MJCard >::iterator it;
	for(it = m_baidaVec.begin(); it != m_baidaVec.end(); it++)
	{
		if(card.type == it->type && card.value == it->value)
		{
			return true;
		}
	}
	return false;
}

bool CardPlayer::checkTing()
{
	MJCard card;
	int i;
	int j;
	bool ret = false;
	for(i = MJTYPE_TIAO; i < MJTYPE_BAIDA; i++)
	{
		for(j = 1; j < 10; j++)
		{
			card.type = i;
			card.value = j;
			if(!isBaida(card))
			{
				addCard(card);
				bool tmp = checkHu();
				rmCard(card);
				if(tmp)
				{
					if(ret == false)
					{
						printf("====================================================================\n");
					}
					addTmpCard(card);
					dump();
					rmTmpCard();
					printf("mode: ");
					showHu();
					printf("====================================================================\n");
				}
				ret |= tmp;

			}
		}
	}
	return ret;
}

bool CardPlayer::checkHu()
{
	CardGroup cg;
	CardGroup tiao;
	CardGroup bing;
	CardGroup wan;
	CardGroup* ptiao = NULL;
	CardGroup* pbing = NULL;
	CardGroup* pwan = NULL;
	CardGroup* tcg = NULL;
	cg.jiang = false;
	cg.baida = m_MyPAIVec[MJTYPE_BAIDA].size();

	depth = 0;
	if(m_MyPAIVec[MJTYPE_TIAO].size() > 0)
	{
		cg.type = MJTYPE_TIAO;
		cg.cards = m_MyPAIVec[MJTYPE_TIAO];
		tcg = checkGroup(&cg);
		//printf("tcg size TIAO: %ld\n", tcg->cards.size());
		if(tcg == NULL)
		{
			return false;
		}
		cg.jiang |= tcg->jiang;
		cg.baida -= tcg->baida;
		ptiao = tcg;
	}
	if(m_MyPAIVec[MJTYPE_BING].size() > 0)
	{
		cg.type = MJTYPE_BING;
		cg.cards = m_MyPAIVec[MJTYPE_BING];
		//debug = 1;
		tcg = checkGroup(&cg);
		//printf("tcg size BING: %ld\n", tcg->cards.size());
		//debug = 0;
		if(tcg == NULL)
		{
			delete ptiao;
			return false;
		}
		cg.jiang |= tcg->jiang;
		cg.baida -= tcg->baida;
		pbing = tcg;
	}
	if(m_MyPAIVec[MJTYPE_WAN].size() > 0)
	{
		cg.type = MJTYPE_WAN;
		cg.cards = m_MyPAIVec[MJTYPE_WAN];
		tcg = checkGroup(&cg);
		//printf("tcg size WAN: %ld\n", tcg->cards.size());
		if(tcg == NULL)
		{
			delete ptiao;
			delete pbing;
			return false;
		}
		cg.jiang |= tcg->jiang;
		cg.baida -= tcg->baida;
		pwan = tcg;
	}
	if (ptiao) {
		tiao.jiang = ptiao->jiang;
		tiao.baida = ptiao->baida;
		tiao.type = MJTYPE_TIAO;
		tiao.cards = ptiao->cards;
		delete ptiao;
	}
	if (pbing) {
		bing.jiang = pbing->jiang;
		bing.baida = pbing->baida;
		bing.type = MJTYPE_BING;
		bing.cards = pbing->cards;
		delete pbing;
	}
	if (pwan) {
		wan.jiang = pwan->jiang;
		wan.baida = pwan->baida;
		wan.type = MJTYPE_WAN;
		wan.cards = pwan->cards;
		delete pwan;
	}
	m_tiao = tiao;
	m_bing = bing;
	m_wan = wan;
	//vector<	int >::iterator it;
	printf("call depth is %d\n", depth);
	return true;
}

void CardPlayer::showHu()
{
	CardGroup tiao = m_tiao;
	CardGroup bing = m_bing;
	CardGroup wan = m_wan;
	vector<	int >::iterator it;
	int idx = 0;
	for(it = tiao.cards.begin(); it != tiao.cards.end(); it++)
	{
		if(*it == 0)
		{
			printf("\033[45;37m%d\033[0m ", m_MyPAIVec[MJTYPE_BAIDA][idx++]); 
		}
		else
		{
			printf("\033[40;32m%d\033[0m ", *it); 
		}
		//printf("tiao\n");
	}
	for(it = bing.cards.begin(); it != bing.cards.end(); it++)
	{
		if(*it == 0)
		{
			printf("\033[45;37m%d\033[0m ", m_MyPAIVec[MJTYPE_BAIDA][idx++]); 
		}
		else
			printf("\033[40;34m%d\033[0m ", *it); 
		//printf("bing\n");
	}
	for(it = wan.cards.begin(); it != wan.cards.end(); it++)
	{
		if(*it == 0)
		{
			printf("\033[45;37m%d\033[0m ", m_MyPAIVec[MJTYPE_BAIDA][idx++]); 
		}
		else
			printf("\033[40;31m%d\033[0m ", *it); 
		//printf("wan\n");
	}
	for(;idx < m_MyPAIVec[MJTYPE_BAIDA].size(); idx++)
	{
		printf("\033[45;37m%d\033[0m ", m_MyPAIVec[MJTYPE_BAIDA][idx++]); 
		//printf("baida\n");
	}
	printf("\n");
}

void CardPlayer::showGuid()
{
	/*
	   printf("\033[40;32mTIAO\033[0m\t"); 
	   printf("\033[40;34mBING\033[0m\t"); 
	   printf("\033[40;31mWAN\033[0m\t"); 
	   printf("\033[45;37mBAIDA\033[0m\t"); 
	   printf("\033[45;37m%d\033[0m", m_baida.value); 
	   if(m_baida.type == MJTYPE_TIAO)
	   printf("\033[45;37mTIAO\033[0m"); 
	   if(m_baida.type == MJTYPE_BING)
	   printf("\033[45;37mBING\033[0m"); 
	   if(m_baida.type == MJTYPE_WAN)
	   printf("\033[45;37mWAN\033[0m"); 
	   printf("\n");
	 */
	for(int i = 0; i < 14; i++)
	{
		if(i == 0) printf("  `  ");
		else if(i < 10)
			printf("  %d  ", i);
		else if(i == 10) printf("  0  ");
		else if(i == 11) printf("  -  ");
		else if(i == 12) printf("  +  ");
		else if(i == 13) printf("  <-  ");
	}
	printf("\n");
}

void CardPlayer::showValue(MJCard card)
{
	int value = card.value;
	int type = card.type;
	bool baida = isBaida(card);
	switch(value)
	{
		case 1:
			if(baida)
				printf("\033[45;37m  1 \033[0m "); 
			else
				printf("\033[47;30m  1 \033[0m "); 
			break;
		case 2:
			if(baida)
				printf("\033[45;37m  2 \033[0m "); 
			else
				printf("\033[47;30m  2 \033[0m "); 
			break;
		case 3:
			if(baida)
				printf("\033[45;37m  3 \033[0m "); 
			else
				printf("\033[47;30m  3 \033[0m "); 
			break;
		case 4:
			if(baida)
				printf("\033[45;37m  4 \033[0m "); 
			else
				printf("\033[47;30m  4 \033[0m "); 
			break;
		case 5:
			if(baida)
				printf("\033[45;37m  5 \033[0m "); 
			else
				printf("\033[47;30m  5 \033[0m "); 
			break;
		case 6:
			if(baida)
				printf("\033[45;37m  6 \033[0m "); 
			else
				printf("\033[47;30m  6 \033[0m "); 
			break;
		case 7:
			if(baida)
				printf("\033[45;37m  7 \033[0m "); 
			else
				printf("\033[47;30m  7 \033[0m "); 
			break;
		case 8:
			if(baida)
				printf("\033[45;37m  8 \033[0m "); 
			else
				printf("\033[47;30m  8 \033[0m "); 
			break;
		case 9:
			if(baida)
				printf("\033[45;37m  9 \033[0m "); 
			else
				printf("\033[47;30m  9 \033[0m "); 
			break;
		default:
			break;
	}
}

void CardPlayer::showType(MJCard card)
{
	int value = card.value;
	int type = card.type;
	bool baida = isBaida(card);
	switch(type)
	{
		case MJTYPE_TIAO:
			if(baida)
				printf("\033[45;32m  T \033[0m "); 
			else
				printf("\033[47;32m  T \033[0m "); 
			break;
		case MJTYPE_BING:
			if(baida)
				printf("\033[45;34m  B \033[0m "); 
			else
				printf("\033[47;34m  B \033[0m "); 
			break;
		case MJTYPE_WAN:
			if(baida)
				printf("\033[45;31m  W \033[0m "); 
			else
				printf("\033[47;31m  W \033[0m "); 
			break;
		default:
			break;
	}
}

void CardPlayer::showCards()
{
	vector<	int >::iterator Iter;
	printf("\033[%dA", (40));
	printf("\033[2J");
	if(m_table != NULL)
		printf("remain: %d\n", m_table->getCardNum());
	printf("baida:\n");
	for(int count = 0 ; count < 2; count ++)
	{
		for(int i = 0; i < m_baidaVec.size(); i++)
		{
			MJCard card = m_baidaVec[i];
			if(count == 0) showValue(card);
			else showType(card);
		}
		printf("\n");
	}
	printf("\n");
	showGuid();

	for(int count = 0 ; count < 2; count ++)
	{
		for(int i = MJTYPE_BEGIN; i < MJTYPE_END; i++)
		{
			for(Iter = m_MyPAIVec[i].begin();Iter !=m_MyPAIVec[i].end(); Iter++)
			{
				MJCard card;
				card.type = i;
				if(i == MJTYPE_BAIDA)
					card.type = m_baida.type;
				card.value = *Iter;
				if(count == 0) showValue(card);
				else showType(card);
			}
		}
		if(m_tmpCard.type != -1 && m_tmpCard.value != -1)
		{
			printf(" ");
			MJCard card = m_tmpCard;
			if(count == 0) showValue(card);
			else showType(card);
		}
		printf("\n");
	}
}

void CardPlayer::dump()
{
	vector<	int >::iterator Iter;
	for(int i = MJTYPE_BEGIN; i < MJTYPE_END; i++)
	{
		for(Iter = m_MyPAIVec[i].begin();Iter !=m_MyPAIVec[i].end(); Iter++)
		{
			//printf("\033[4m");
			if(i == MJTYPE_TIAO)
				printf("  \033[40;32m%d\033[0m  ", *Iter); 
			if(i == MJTYPE_BING)
				printf("  \033[40;34m%d\033[0m  ", *Iter); 
			if(i == MJTYPE_WAN)
				printf("  \033[40;31m%d\033[0m  ", *Iter); 
			if(i == MJTYPE_BAIDA)
				printf("  \033[45;37m%d\033[0m  ", *Iter); 
		}
	}
	if(m_tmpCard.type != -1 && m_tmpCard.value != -1)
	{
		printf(" ");
		if(isBaida(m_tmpCard))
			printf("  \033[45;37m%d\033[0m  ", m_tmpCard.value); 
		else if(m_tmpCard.type == MJTYPE_TIAO)
			printf("  \033[40;32m%d\033[0m  ", m_tmpCard.value); 
		else if(m_tmpCard.type == MJTYPE_BING)
			printf("  \033[40;34m%d\033[0m  ", m_tmpCard.value); 
		else if(m_tmpCard.type == MJTYPE_WAN)
			printf("  \033[40;31m%d\033[0m  ", m_tmpCard.value); 
	}
	printf("\n");
}

/***************************************************/
// alg

CardGroup* createAX(int v, bool jiang)
{
	CardGroup* cg = new CardGroup();
	addcard(v);
	addcard(BAIDA);
	cg->jiang = true;
	cg->baida = 1;
	return cg;
}

CardGroup* createAXX(int v, bool jiang)
{
	CardGroup* cg = new CardGroup();
	addcard(v);
	addcard(BAIDA);
	addcard(BAIDA);
	cg->jiang = false || jiang;
	cg->baida = 2;
	return cg;
}

CardGroup* createAA(int v, bool jiang)
{
	CardGroup* cg = new CardGroup();
	addcard(v);
	addcard(v);
	cg->jiang = true;
	cg->baida = 0;
	return cg;
}

CardGroup* createAAX(int v, bool jiang)
{
	CardGroup* cg = new CardGroup();
	addcard(v);
	addcard(v);
	addcard(BAIDA);
	cg->jiang = false || jiang;
	cg->baida = 1;
	return cg;
}

CardGroup* createABX(int v, int u, bool jiang)
{
	CardGroup* cg = new CardGroup();
	addcard(v);
	addcard(u);
	addcard(BAIDA);
	cg->jiang = false || jiang;
	cg->baida = 1;
	return cg;
}

/*
CardGroup* createAXDXX(int v, int u, bool jiang)
{
	CardGroup* cg = new CardGroup();
	addcard(v);
	addcard(BAIDA);
	addcard(u);
	addcard(BAIDA);
	addcard(BAIDA);
	cg.jiang = true;
	cg.baida = 3;
	return cg;
}

CardGroup* createAXXDXX(int v, int u, bool jiang)
{
	CardGroup* cg = new CardGroup();
	addcard(v);
	addcard(BAIDA);
	addcard(BAIDA);
	addcard(u);
	addcard(BAIDA);
	addcard(BAIDA);
	cg->jiang = false || jiang;
	cg->baida = 4;
	return cg;
}
*/

CardGroup* createAAA(int v, bool jiang)
{
	CardGroup* cg = new CardGroup();
	addcard(v);
	addcard(v);
	addcard(v);
	cg->jiang = false || jiang;
	cg->baida = 0;
	return cg;
}

CardGroup* createABC(int v, bool jiang)
{
	CardGroup* cg = new CardGroup();
	addcard(v);
	addcard(v+1);
	addcard(v+2);
	cg->jiang = false || jiang;
	cg->baida = 0;
	return cg;
}

CardGroup* createABCBX(int v, bool jiang)
{
	CardGroup* cg = new CardGroup();
	addcard(v);
	addcard(v+1);
	addcard(v+2);

	addcard(v+1);
	addcard(BAIDA);
	cg->jiang = true;
	cg->baida = 1;
	return cg;
}

CardGroup* createABCBB(int v, bool jiang)
{
	CardGroup* cg = new CardGroup();
	addcard(v);
	addcard(v+1);
	addcard(v+2);

	addcard(v+1);
	addcard(v+1);
	cg->jiang = true;
	cg->baida = 0;
	return cg;
}

CardGroup* createABCBBX(int v, bool jiang)
{
	CardGroup* cg = new CardGroup();
	addcard(v);
	addcard(v+1);
	addcard(v+2);

	addcard(v+1);
	addcard(v+1);
	addcard(BAIDA);
	cg->jiang = false || jiang;
	cg->baida = 1;
	return cg;
}

CardGroup* createABCAXC(int v, bool jiang)
{
	CardGroup* cg = new CardGroup();
	addcard(v);
	addcard(v+1);
	addcard(v+2);

	addcard(v);
	addcard(BAIDA);
	addcard(v+2);
	cg->jiang = false || jiang;
	cg->baida = 1;
	return cg;
}

CardGroup* createABCABX(int v, bool jiang)
{
	CardGroup* cg = new CardGroup();
	addcard(v);
	addcard(v+1);
	addcard(v+2);

	addcard(v);
	addcard(v+1);
	addcard(BAIDA);
	cg->jiang = false || jiang;
	cg->baida = 1;
	return cg;
}

CardGroup* createABCXBC(int v, bool jiang)
{
	CardGroup* cg = new CardGroup();
	addcard(v);
	addcard(v+1);
	addcard(v+2);

	addcard(BAIDA);
	addcard(v+1);
	addcard(v+2);
	cg->jiang = false || jiang;
	cg->baida = 1;
	return cg;
}

CardGroup* createABCABC(int v, bool jiang)
{
	CardGroup* cg = new CardGroup();
	addcard(v);
	addcard(v+1);
	addcard(v+2);

	addcard(v);
	addcard(v+1);
	addcard(v+2);
	cg->jiang = false || jiang;
	cg->baida = 0;
	return cg;
}

CardGroup* createABCBBB(int v, bool jiang)
{
	CardGroup* cg = new CardGroup();
	addcard(v);
	addcard(v+1);
	addcard(v+2);

	addcard(v+1);
	addcard(v+1);
	addcard(v+1);
	cg->jiang = false || jiang;
	cg->baida = 0;
	return cg;
}

CardGroup* createABCBCD(int v, bool jiang)
{
	CardGroup* cg = new CardGroup();
	addcard(v);
	addcard(v+1);
	addcard(v+2);

	addcard(v+1);
	addcard(v+2);
	addcard(v+3);
	cg->jiang = false || jiang;
	cg->baida = 0;
	return cg;
}

CardGroup* createABCBCDBX(int v, bool jiang)
{
	CardGroup* cg = new CardGroup();
	addcard(v);
	addcard(v+1);
	addcard(v+2);

	addcard(v+1);
	addcard(v+2);
	addcard(v+3);

	addcard(v+1);
	addcard(BAIDA);
	cg->jiang = true;
	cg->baida = 1;
	return cg;
}

CardGroup* createABCBCDCX(int v, bool jiang)
{
	CardGroup* cg = new CardGroup();
	addcard(v);
	addcard(v+1);
	addcard(v+2);

	addcard(v+1);
	addcard(v+2);
	addcard(v+3);

	addcard(v+2);
	addcard(BAIDA);
	cg->jiang = true;
	cg->baida = 1;
	return cg;
}

CardGroup* createABCBCDBB(int v, bool jiang)
{
	CardGroup* cg = new CardGroup();
	addcard(v);
	addcard(v+1);
	addcard(v+2);

	addcard(v+1);
	addcard(v+2);
	addcard(v+3);

	addcard(v+1);
	addcard(v+1);
	cg->jiang = true;
	cg->baida = 0;
	return cg;
}

CardGroup* createABCBCDCC(int v, bool jiang)
{
	CardGroup* cg = new CardGroup();
	addcard(v);
	addcard(v+1);
	addcard(v+2);

	addcard(v+1);
	addcard(v+2);
	addcard(v+3);

	addcard(v+2);
	addcard(v+2);
	cg->jiang = true;
	cg->baida = 0;
	return cg;
}

CardGroup* createXBCBCDCDE(int v, bool jiang)
{
	CardGroup* cg = new CardGroup();
	addcard(BAIDA);
	addcard(v);
	addcard(v+1);

	addcard(v);
	addcard(v+1);
	addcard(v+2);

	addcard(v+1);
	addcard(v+2);
	addcard(v+3);
	cg->jiang = false || jiang;
	cg->baida = 1;
	return cg;
}

CardGroup* createABXBCDCDE(int v, bool jiang)
{
	CardGroup* cg = new CardGroup();
	addcard(v);
	addcard(v+1);
	addcard(BAIDA);

	addcard(v+1);
	addcard(v+2);
	addcard(v+3);

	addcard(v+2);
	addcard(v+3);
	addcard(v+4);

	cg->jiang = false || jiang;
	cg->baida = 1;
	return cg;
}

CardGroup* createABCBCDCDX(int v, bool jiang)
{
	CardGroup* cg = new CardGroup();
	addcard(v);
	addcard(v+1);
	addcard(v+2);

	addcard(v+1);
	addcard(v+2);
	addcard(v+3);

	addcard(v+2);
	addcard(v+3);
	addcard(BAIDA);

	cg->jiang = false || jiang;
	cg->baida = 1;
	return cg;
}

CardGroup* createABCBCDCDE(int v, bool jiang)
{
	CardGroup* cg = new CardGroup();
	addcard(v);
	addcard(v+1);
	addcard(v+2);

	addcard(v+1);
	addcard(v+2);
	addcard(v+3);

	addcard(v+2);
	addcard(v+3);
	addcard(v+4);
	cg->jiang = false || jiang;
	cg->baida = 0;
	return cg;
}

CardGroup* check1Card(CardGroup* group)
{
	CardGroup* cg = NULL;
	if (!group->jiang && group->baida > 0) {
		cg = createAX(v1, group->jiang);
	} else if (group->baida > 1) {
		cg = createAXX(v1, group->jiang);
	}
	return cg;
}

CardGroup* check2Cards(CardGroup* group)
{
	CardGroup* cg = NULL;
	if (v1 == v2) {
		if (!group->jiang) {
			cg = createAA(v1, group->jiang);
		} else if (group->baida > 0) {
			cg = createAAX(v1, group->jiang);
		}
	} else if (abs(v2-v1) <= 2) {
		if (group->baida > 0) {
			cg = createABX(v1, v2, group->jiang);
		}
	}
	return cg;
}

CardGroup* check3Cards(CardGroup* group)
{
	CardGroup* cg = NULL;
	//AAA
	if(v1 == v2 && v2 == v3)
	{
		cg = createAAA(v1, group->jiang);
	}
	else if(v1 == v2-1 && v2 == v3-1)
	{
		cg = createABC(v1, group->jiang);
	}
	return cg;
}

CardGroup* check4Cards(CardGroup* group)
{
	CardGroup* cg = NULL;
	if(v1 == v2-1 && v2 == v3 && v3 == v4-1)
	{
		if(!group->jiang && group->baida > 0)
		{
			// ABBCX
			cg = createABCBX(v1, group->jiang);
		}
	}
	return cg;
}

CardGroup* check5Cards(CardGroup* group)
{
	CardGroup* cg = NULL;
	if(v1 == v2-1 && v2 == v3 && v3 == v4 && v4 == v5-1)
	{
		if(!group->jiang)
		{
			// ABBBC
			cg = createABCBB(v1, group->jiang);
		} else if(group->baida > 0) {
			// ABBBCX
			cg = createABCBBX(v1, group->jiang);
		}
	}
	else if(v1 == v2 && v2 == v3-1 && v3 == v4-1 && v4 == v5)
	{
		if(group->baida > 0)
		{
			// AABCC
			cg = createABCAXC(v1, group->jiang);
		}
	}
	else if(v1 == v2 && v2 == v3-1 && v3 == v4 && v4 == v5-1)
	{
		if(group->baida > 0)
		{
			// AABBC
			cg = createABCABX(v1, group->jiang);
		}
	}
	else if(v1 == v2-1 && v2 == v3 && v3 == v4-1 && v4 == v5)
	{
		if(group->baida > 0)
		{
			//ABBCC
			cg = createABCXBC(v1, group->jiang);
		}
	}
	return cg;
}

CardGroup* check6Cards(CardGroup* group)
{
	CardGroup* cg = NULL;
	if(v1 == v2 &&
		v2 == v3-1 &&
		v3 == v4 &&
		v4 == v5-1 &&
		v5 == v6)
	{
		// AABBCC
		cg = createABCABC(v1, group->jiang);
	}
	else if(v1 == v2-1 &&
			v2 == v3 &&
			v3 == v4 &&
			v4 == v5 &&
			v5 == v6-1)
	{
		// ABBBBC
		cg = createABCBBB(v1, group->jiang);
	}
	else if(v1 == v2-1 &&
			v2 == v3 &&
			v3 == v4-1 &&
			v4 == v5 &&
			v5 == v6-1)
	{
		// ABBCCD
		cg = createABCBCD(v1, group->jiang);
	}
	return cg;
}

CardGroup* check7Cards(CardGroup* group)
{
	CardGroup* cg = NULL;
	if (!group->jiang && group->baida > 0) {
		if(v1 == v2-1 &&
			v2 == v3 &&
			v3 == v4 &&
			v4 == v5-1 &&
			v5 == v6 &&
			v6 == v7-1)
		{
			// ABBBCCD
			cg = createABCBCDBX(v1, group->jiang);
		}
		else if(v1 == v2-1 &&
			v2 == v3 &&
			v3 == v4-1 &&
			v4 == v5 &&
			v5 == v6 &&
			v6 == v7-1)
		{
			// ABBCCCD
			cg = createABCBCDCX(v1, group->jiang);
		}
	}
	return cg;
}

CardGroup* check8Cards(CardGroup* group)
{
	CardGroup* cg = NULL;
	if(v1 == v2-1 &&
		v2 == v3 &&
		v3 == v4 &&
		v4 == v5 &&
		v5 == v6-1 &&
		v6 == v7 &&
		v7 == v8-1) {
		//ABBBBCCD - ABC BCD BB
		if (!group->jiang) {
			cg = createABCBCDBB(v1, group->jiang);
		}
	} else if (v1 == v2-1 &&
			v2 == v3 &&
			v3 == v4-1 &&
			v4 == v5 &&
			v5 == v6 &&
			v6 == v7 &&
			v7 == v8-1) {
		//ABBCCCCD - ABC BCD CC
		if (!group->jiang) {
			cg = createABCBCDCC(v1, group->jiang);
		}
	} else if (group->baida > 0) {
		if(v1 == v2 && v2 == v3-1 &&
				v3 == v4 && v4 == v5 && v5 == v6-1 &&
				v6 == v7 && v7 == v8-1)
		{
			// BBCCCDDE
			cg = createXBCBCDCDE(v1, group->jiang);
		}
		else if(v1 == v2-1 &&
				v2 == v3 && v3 == v4-1 &&
				v4 == v5 && v5 == v6-1 &&
				v6 == v7 && v7 == v8-1)
		{
			// ABBCCDDE
			cg = createABXBCDCDE(v1, group->jiang);
		}
		else if(v1 == v2-1 &&
				v2 == v3 && v3 == v4-1 &&
				v4 == v5 && v5 == v6 && v6 == v7-1 &&
				v7 == v8)
		{
			// ABBCCCDD
			cg = createABCBCDCDX(v1, group->jiang);
		}
	}
	return cg;
}

CardGroup* check9Cards(CardGroup* group)
{
	//ABBCCCDDE - ABC BCD CDE
	CardGroup* cg = NULL;
	if(v1 == v2-1 &&
		v2 == v3 &&
		v3 == v4-1 &&
		v4 == v5 &&
		v5 == v6 &&
		v6 == v7-1 &&
		v7 == v8 && 
		v8 == v9-1)
	{
		cg = createABCBCDCDE(v1, group->jiang);
	}
	return cg;
}
// pattern with more than 9 cards is redundance,
// for example: 12Cards covered by 3Cards 6Cards 3Cards,
// AAAABBBBCCCC can be separated to AAA ABBBBC CCC

CardGroup* checkPattern(CardGroup* group, int idx)
{
	CardGroup* ret = NULL;
	switch(idx) {
		case 1:
			ret = check1Card(group);
			break;
		case 2:
			ret = check2Cards(group);
			break;
		case 3:
			ret = check3Cards(group);
			break;
		case 4:
			ret = check4Cards(group);
			break;
		case 5:
			ret = check5Cards(group);
			break;
		case 6:
			ret = check6Cards(group);
			break;
		case 7:
			ret = check7Cards(group);
			break;
		case 8:
			ret = check8Cards(group);
			break;
		case 9:
			ret = check9Cards(group);
			break;
		default:
			break;
	}
	return ret;
}

CardGroup* selectGroup(CardGroup* cg1, CardGroup* cg2)
{
	CardGroup* hire = NULL;
	CardGroup* fire = NULL;
	if (cg1 == NULL) {
		hire = cg2;
	} else if (cg2 == NULL) {
		hire = cg1;
	} else if (cg1->baida < cg2->baida) {
		hire = cg1;
		fire = cg2;
	} else {
		hire = cg2;
		fire = cg1;
	}
	if (fire != NULL) {
		delete fire;
	}
	return hire;
}

CardGroup* splitGroup(CardGroup* group, int idx)
{
	// invalid args
	if (group == NULL || idx > group->cards.size()) return NULL;

	CardGroup* ret = NULL;

	// check spical pattern with exactly idx cards
	if (idx == group->cards.size()) {
		return checkPattern(group, idx);
		/*
		ret = checkPattern(group, idx);
		if (ret != NULL) return ret;
		*/
	}
	//printf("check %d - %d\n", idx, group->cards.size() - idx);

	// split group into two small groups
	// first half to cg1 and second half to cg2
	CardGroup cg1;
	cg1.baida = group->baida;
	cg1.jiang = group->jiang;

	CardGroup cg2;
	cg2.baida = group->baida;
	cg2.jiang = group->jiang;
	vector< int >::iterator it;
	int i = 0;
	for(it = group->cards.begin(); it != group->cards.end(); it++)
	{
		if(i < idx) cg1.cards.push_back(*it);
		else cg2.cards.push_back(*it);
		i++;
	}

	CardGroup* tcg1 = checkGroup(&cg1);
	if(tcg1 != NULL)
	{
		cg2.jiang = tcg1->jiang;
		cg2.baida = group->baida - tcg1->baida;
		CardGroup* tcg2 =  checkGroup(&cg2);
		if(tcg2 != NULL)
		{
			// merge tcg1 and tcg2 and return it
			ret = tcg1;
			ret->cards.insert(ret->cards.end(),
					tcg2->cards.begin(), tcg2->cards.end());
			ret->jiang = tcg1->jiang | tcg2->jiang;
			ret->baida = tcg1->baida + tcg2->baida;
		} else {
			// tcg2 check failed, we should free tcg1 here
			delete tcg1;
		}
	} // nothing to be free
	return ret;
}

CardGroup* checkGroup(CardGroup* group)
{
	CardGroup* ret = NULL;
	size_t size = group->cards.size();
	int idx[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	// finding the partition using minimum baida
	for (int i = 0; i < sizeof(idx)/sizeof(idx[0]); i++) {
		if (size < idx[i]) break;
		CardGroup* cg = splitGroup(group, idx[i]);
		ret = selectGroup(ret, cg);
	}
	return ret;
}
