#include "CardPlayer.h"
#include "CardTable.h"

CardGroup CheckGroup(CardGroup group);
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
	int v1 = m_baida.value;
	int v2;
	if(v1 == 1) v2 = 9;
	else v2 = v1 - 1;
	c.value = v2;
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
	CardGroup tcg;
	cg.jiang = false;
	cg.baida = m_MyPAIVec[MJTYPE_BAIDA].size();

	depth = 0;
	if(m_MyPAIVec[MJTYPE_TIAO].size() > 0)
	{
		cg.type = MJTYPE_TIAO;
		cg.cards = m_MyPAIVec[MJTYPE_TIAO];
		tcg = CheckGroup(cg);
		//printf("tcg size TIAO: %d\n", tcg.cards.size());
		if(tcg.cards.size() <= 0)
		{
			return false;
		}
		cg.jiang |= tcg.jiang;
		cg.baida -= tcg.baida;
		tiao = tcg;
	}
	if(m_MyPAIVec[MJTYPE_BING].size() > 0)
	{
		cg.type = MJTYPE_BING;
		cg.cards = m_MyPAIVec[MJTYPE_BING];
		//debug = 1;
		tcg = CheckGroup(cg);
		//printf("tcg size BING: %d\n", tcg.cards.size());
		//debug = 0;
		if(tcg.cards.size() <= 0)
		{
			return false;
		}
		cg.jiang |= tcg.jiang;
		cg.baida -= tcg.baida;
		bing = tcg;
	}
	if(m_MyPAIVec[MJTYPE_WAN].size() > 0)
	{
		cg.type = MJTYPE_WAN;
		cg.cards = m_MyPAIVec[MJTYPE_WAN];
		tcg = CheckGroup(cg);
		//printf("tcg size WAN: %d\n", tcg.cards.size());
		if(tcg.cards.size() <= 0)
		{
			return false;
		}
		cg.jiang |= tcg.jiang;
		cg.baida -= tcg.baida;
		wan = tcg;
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
	}
	for(it = bing.cards.begin(); it != bing.cards.end(); it++)
	{
		if(*it == 0)
		{
			printf("\033[45;37m%d\033[0m ", m_MyPAIVec[MJTYPE_BAIDA][idx++]); 
		}
		else
			printf("\033[40;34m%d\033[0m ", *it); 
	}
	for(it = wan.cards.begin(); it != wan.cards.end(); it++)
	{
		if(*it == 0)
		{
			printf("\033[45;37m%d\033[0m ", m_MyPAIVec[MJTYPE_BAIDA][idx++]); 
		}
		else
			printf("\033[40;31m%d\033[0m ", *it); 
	}
	for(;idx < m_MyPAIVec[MJTYPE_BAIDA].size(); idx++)
	{
		printf("\033[45;37m%d\033[0m ", m_MyPAIVec[MJTYPE_BAIDA][idx++]); 
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

CardGroup createAX(int v1, bool jiang)
{
	CardGroup cg;
	cg.cards.push_back(v1);
	cg.cards.push_back(BAIDA);
	cg.jiang = true;
	cg.baida = 1;
	return cg;
}
CardGroup createAXX(int v1, bool jiang)
{
	CardGroup cg;
	cg.cards.push_back(v1);
	cg.cards.push_back(BAIDA);
	cg.cards.push_back(BAIDA);
	cg.jiang = false || jiang;
	cg.baida = 2;
	return cg;
}
CardGroup createAA(int v1, int v2, bool jiang)
{
	CardGroup cg;
	cg.cards.push_back(v1);
	cg.cards.push_back(v2);
	cg.jiang = true;
	cg.baida = 0;
	return cg;
}
CardGroup createAAX(int v1, int v2, bool jiang)
{
	CardGroup cg;
	cg.cards.push_back(v1);
	cg.cards.push_back(v2);
	cg.cards.push_back(BAIDA);
	cg.jiang = false || jiang;
	cg.baida = 1;
	return cg;
}

CardGroup createABX(int v1, int v2, bool jiang)
{
	return createAAX(v1, v2, jiang);
}

CardGroup createAXDXX(int v1, int v2, bool jiang)
{
	CardGroup cg;
	cg.cards.push_back(v1);
	cg.cards.push_back(BAIDA);
	cg.cards.push_back(v2);
	cg.cards.push_back(BAIDA);
	cg.cards.push_back(BAIDA);
	cg.jiang = true;
	cg.baida = 3;
	return cg;
}

CardGroup createAXXDXX(int v1, int v2, bool jiang)
{
	CardGroup cg;
	cg.cards.push_back(v1);
	cg.cards.push_back(BAIDA);
	cg.cards.push_back(BAIDA);
	cg.cards.push_back(v2);
	cg.cards.push_back(BAIDA);
	cg.cards.push_back(BAIDA);
	cg.jiang = false || jiang;
	cg.baida = 4;
	return cg;
}

CardGroup createAAA(int v1, int v2, int v3, bool jiang)
{
	CardGroup cg;
	cg.cards.push_back(v1);
	cg.cards.push_back(v2);
	cg.cards.push_back(v3);
	cg.jiang = false || jiang;
	cg.baida = 0;
	return cg;
}

CardGroup createABC(int v1, int v2, int v3, bool jiang)
{
	return createAAA(v1, v2, v3, jiang);
}

CardGroup createABBCX(int v1, int v2, int v3, int v4, bool jiang)
{
	CardGroup cg;
	cg.cards.push_back(v1);
	cg.cards.push_back(v2);
	cg.cards.push_back(v4);
	cg.cards.push_back(v3);
	cg.cards.push_back(BAIDA);
	cg.jiang = true;
	cg.baida = 1;
	return cg;
}

CardGroup createABBBC(int v1, int v2, int v3, int v4, int v5, bool jiang)
{
	CardGroup cg;
	cg.cards.push_back(v1);
	cg.cards.push_back(v2);
	cg.cards.push_back(v5);
	cg.cards.push_back(v3);
	cg.cards.push_back(v4);
	cg.jiang = true;
	cg.baida = 0;
	return cg;
}

CardGroup createABBBCX(int v1, int v2, int v3, int v4, int v5, bool jiang)
{
	CardGroup cg;
	cg.cards.push_back(v1);
	cg.cards.push_back(v2);
	cg.cards.push_back(v5);
	cg.cards.push_back(v3);
	cg.cards.push_back(v4);
	cg.cards.push_back(BAIDA);
	cg.jiang = false || jiang;
	cg.baida = 1;
	return cg;
}

CardGroup createAABXCC(int v1, int v2, int v3, int v4, int v5, bool jiang)
{
	CardGroup cg;
	cg.cards.push_back(v1);
	cg.cards.push_back(v3);
	cg.cards.push_back(v5);
	cg.cards.push_back(v2);
	cg.cards.push_back(v4);
	cg.cards.push_back(BAIDA);
	cg.jiang = false || jiang;
	cg.baida = 1;
	return cg;
}

CardGroup createAABBCX(int v1, int v2, int v3, int v4, int v5, bool jiang)
{
	CardGroup cg;
	cg.cards.push_back(v1);
	cg.cards.push_back(v2);
	cg.cards.push_back(v3);
	cg.cards.push_back(v4);
	cg.cards.push_back(v5);
	cg.cards.push_back(BAIDA);
	cg.jiang = false || jiang;
	cg.baida = 1;
	return cg;
}

CardGroup createAXBBCC(int v1, int v2, int v3, int v4, int v5, bool jiang)
{
	CardGroup cg;
	cg.cards.push_back(v1);
	cg.cards.push_back(BAIDA);
	cg.cards.push_back(v2);
	cg.cards.push_back(v3);
	cg.cards.push_back(v4);
	cg.cards.push_back(v5);
	cg.jiang = false || jiang;
	cg.baida = 1;
	return cg;
}

CardGroup createAABBCC(int v1, int v2, int v3, int v4, int v5, int v6, bool jiang)
{
	CardGroup cg;
	cg.cards.push_back(v1);
	cg.cards.push_back(v3);
	cg.cards.push_back(v5);
	cg.cards.push_back(v2);
	cg.cards.push_back(v4);
	cg.cards.push_back(v6);
	cg.jiang = false || jiang;
	cg.baida = 0;
	return cg;
}

CardGroup createABBBBC(int v1, int v2, int v3, int v4, int v5, int v6, bool jiang)
{
	CardGroup cg;
	cg.cards.push_back(v1);
	cg.cards.push_back(v2);
	cg.cards.push_back(v6);
	cg.cards.push_back(v3);
	cg.cards.push_back(v4);
	cg.cards.push_back(v5);
	cg.jiang = false || jiang;
	cg.baida = 0;
	return cg;
}

CardGroup createABBCCD(int v1, int v2, int v3, int v4, int v5, int v6, bool jiang)
{
	CardGroup cg;
	cg.cards.push_back(v1);
	cg.cards.push_back(v2);
	cg.cards.push_back(v4);
	cg.cards.push_back(v3);
	cg.cards.push_back(v5);
	cg.cards.push_back(v6);
	cg.jiang = false || jiang;
	cg.baida = 0;
	return cg;
}

CardGroup createXBBCCCDDE(int v1, bool jiang)
{
	CardGroup cg;
	cg.cards.push_back(BAIDA);
	cg.cards.push_back(v1);
	cg.cards.push_back(v1+1);

	cg.cards.push_back(v1);
	cg.cards.push_back(v1+1);
	cg.cards.push_back(v1+2);

	cg.cards.push_back(v1+1);
	cg.cards.push_back(v1+2);
	cg.cards.push_back(v1+3);
	cg.jiang = false || jiang;
	cg.baida = 1;
	return cg;
}

CardGroup createABBCCXDDE(int v1, bool jiang)
{
	CardGroup cg;
	cg.cards.push_back(v1);
	cg.cards.push_back(v1+1);
	cg.cards.push_back(v1+2);

	cg.cards.push_back(v1+1);
	cg.cards.push_back(v1+2);
	cg.cards.push_back(v1+3);

	cg.cards.push_back(BAIDA);
	cg.cards.push_back(v1+3);
	cg.cards.push_back(v1+4);
	cg.jiang = false || jiang;
	cg.baida = 1;
	return cg;
}

CardGroup createABBCCCDDX(int v1, bool jiang)
{
	CardGroup cg;
	cg.cards.push_back(v1);
	cg.cards.push_back(v1+1);
	cg.cards.push_back(v1+2);

	cg.cards.push_back(v1+1);
	cg.cards.push_back(v1+2);
	cg.cards.push_back(v1+3);

	cg.cards.push_back(v1+2);
	cg.cards.push_back(v1+3);
	cg.cards.push_back(BAIDA);
	cg.jiang = false || jiang;
	cg.baida = 1;
	return cg;
}

CardGroup createABBCCCDDE(int v1, bool jiang)
{
	CardGroup cg;
	cg.cards.push_back(v1);
	cg.cards.push_back(v1+1);
	cg.cards.push_back(v1+2);

	cg.cards.push_back(v1+1);
	cg.cards.push_back(v1+2);
	cg.cards.push_back(v1+3);

	cg.cards.push_back(v1+2);
	cg.cards.push_back(v1+3);
	cg.cards.push_back(v1+4);
	cg.jiang = false || jiang;
	cg.baida = 0;
	return cg;
}

CardGroup Check3Cards(CardGroup group)
{
	CardGroup cg;
	int v1 = group.cards[0];
	int v2 = group.cards[1];
	int v3 = group.cards[2];
	//AAA
	if(v1 == v2 && v2 == v3)
	{
		cg = createAAA(v1, v2, v3, group.jiang);
	}
	else if(v1 == v2-1 && v2 == v3-1)
	{
		cg = createABC(v1, v2, v3, group.jiang);
	}
	return cg;
}

CardGroup Check4Cards(CardGroup group)
{
	CardGroup cg;
	int v1 = group.cards[0];
	int v2 = group.cards[1];
	int v3 = group.cards[2];
	int v4 = group.cards[3];
	if(v1 == v2-1 && v2 == v3 && v3 == v4-1)
	{
		if(group.jiang == false && group.baida > 0)
		{
			cg = createABBCX(v1, v2, v3, v4, group.jiang);
		}
	}
	return cg;
}

CardGroup Check5Cards(CardGroup group)
{
	CardGroup cg;
	int v1 = group.cards[0];
	int v2 = group.cards[1];
	int v3 = group.cards[2];
	int v4 = group.cards[3];
	int v5 = group.cards[4];
	if(v1 == v2-1 && v2 == v3 && v3 == v4 && v4 == v5-1)
	{
		if(group.jiang == false)
		{
			cg = createABBBC(v1, v2, v3, v4, v5, group.jiang);
		}
		if(group.baida > 0)
		{
			cg = createABBBCX(v1, v2, v3, v4, v5, group.jiang);
		}
	}
	else if(v1 == v2 && v2 == v3-1 && v3 == v4-1 && v4 == v5)
	{
		if(group.baida > 0)
		{
			cg = createAABXCC(v1, v2, v3, v4, v5, group.jiang);
		}
	}
	else if(v1 == v2 && v2 == v3-1 && v3 == v4 && v4 == v5-1)
	{
		if(group.baida > 0)
		{
			cg = createAABBCX(v1, v2, v3, v4, v5, group.jiang);
		}
	}
	else if(v1 == v2-1 && v2 == v3 && v3 == v4-1 && v4 == v5)
	{
		if(group.baida > 0)
		{
			cg = createAXBBCC(v1, v2, v3, v4, v5, group.jiang);
		}
	}
	return cg;
}

CardGroup Check6Cards(CardGroup group)
{
	CardGroup cg;
	int v1 = group.cards[0];
	int v2 = group.cards[1];
	int v3 = group.cards[2];
	int v4 = group.cards[3];
	int v5 = group.cards[4];
	int v6 = group.cards[5];
	if(v1 == v2 && v3 == v4 && v5 == v6 &&
			v1 == v3-1 && v3 == v5-1)
	{
		cg = createAABBCC(v1, v2, v3, v4, v5, v6, group.jiang);
	}
	else if(v1 == v2-1 && v2 == v3 && v3 == v4 && v4 == v5 && v5 == v6-1)
	{
		cg = createABBBBC(v1, v2, v3, v4, v5, v6, group.jiang);
	}
	else if(v1 == v2-1 && v2 == v3 && v3 == v4-1 && v4 == v5 && v5 == v6-1)
	{
		cg = createABBCCD(v1, v2, v3, v4, v5, v6, group.jiang);
	}
	return cg;
}

CardGroup Check8Cards(CardGroup group)
{
	//ABBCCCDDE - ABC BCD CDE
	CardGroup cg;
	int v1 = group.cards[0];
	int v2 = group.cards[1];
	int v3 = group.cards[2];
	int v4 = group.cards[3];
	int v5 = group.cards[4];
	int v6 = group.cards[5];
	int v7 = group.cards[6];
	int v8 = group.cards[7];
	if(v1 != 1 && v1 == v2 && v2 == v3-1 &&
			v3 == v4 && v4 == v5 && v5 == v6-1 &&
			v6 == v7 && v7 == v8 && v7 == v8-1)
	{
		cg = createXBBCCCDDE(v1, group.jiang);
	}
	else if(v1 == v2-1 &&
			v2 == v3 && v3 == v4-1 &&
			v4 == v5 && v5 == v6-1 &&
			v6 == v7 && v7 == v8-1)
	{
		cg = createABBCCXDDE(v1, group.jiang);
	}
	else if(v1 == v2-1 &&
			v2 == v3 && v3 == v4-1 &&
			v4 == v5 && v5 == v6 && v6 == v7-1 &&
			v7 == v8 && v8 != 9)
	{
		cg = createABBCCCDDX(v1, group.jiang);
	}
	return cg;
}

CardGroup Check9Cards(CardGroup group)
{
	//ABBCCCDDE - ABC BCD CDE
	CardGroup cg;
	int v1 = group.cards[0];
	int v2 = group.cards[1];
	int v3 = group.cards[2];
	int v4 = group.cards[3];
	int v5 = group.cards[4];
	int v6 = group.cards[5];
	int v7 = group.cards[6];
	int v8 = group.cards[7];
	int v9 = group.cards[8];
	if(v1 == v2-1 &&
			v2 == v3 && v3 == v4-1 &&
			v4 == v5 && v5 == v6 && v6 == v7-1 &&
			v7 == v8 && v8 == v9-1)
	{
		cg = createABBCCCDDE(v1, group.jiang);
	}
	return cg;
}
// 12Cards covered by 3Cards 6Cards 3Cards, this func is a redundancy.
// AAAABBBBCCCC can be separated to AAA ABBBBC CCC

CardGroup UpdateCardGroup(CardGroup group1, CardGroup group2)
{
	CardGroup cg;
	int minBaida = 100;
	if(group1.cards.size() > 0 && group1.baida < minBaida)
	{
		cg = group1;
		minBaida = group1.baida;
	}
	if(group2.cards.size() > 0 && group2.baida < minBaida)
	{
		cg = group2;
		minBaida = group2.baida;
	}
	return cg;
}
vector< CardGroup >	MergeGroup(vector< CardGroup > group1, vector< CardGroup > group2)
{
	vector< CardGroup > vgroup;
	vector<	CardGroup >::iterator Iter;
	CardGroup cg;
	int minBaida = 100;
	for(Iter = group1.begin(); Iter != group1.end(); Iter++)
	{
		if(Iter->baida < minBaida)
		{
			minBaida = Iter->baida;
			cg = *Iter;
		}
	}
	for(Iter = group2.begin(); Iter != group2.end(); Iter++)
	{
		if(Iter->baida < minBaida)
		{
			minBaida = Iter->baida;
			cg = *Iter;
		}
	}
	if(minBaida < 100)
	{
		// we just save cg with minmum number of baida
		vgroup.push_back(cg);
	}
	return vgroup;
	if(0){
		vector<	CardGroup >::iterator Iter2;
		for(Iter2 = group2.begin(); Iter2 != group2.end(); Iter2++)
		{
			vector<	CardGroup >::iterator Iter1;
			bool found = false;
			for(Iter1 = group1.begin(); Iter1 != group1.end(); Iter1++)
			{
				if(Iter2->jiang == Iter1->jiang && Iter2->baida == Iter1->baida)
				{
					found = true;
				}
			}
			if(found == false)
			{
				group1.push_back(*Iter2);
			}
		}
		return group1;
	}
}

CardGroup CheckSeparateGroup(CardGroup cg, int index)
{
	CardGroup cgRet;

	CardGroup cg1;
	cg1.baida = cg.baida;
	cg1.jiang = cg.jiang;

	CardGroup cg2;
	cg2.baida = cg.baida;
	cg2.jiang = cg.jiang;
	vector< int >::iterator it;
	int i = 0;
	for(it = cg.cards.begin(); it != cg.cards.end(); it++)
	{
		// seperate cg into two small cgs
		// first half and second half
		if(i < index) cg1.cards.push_back(*it);
		else cg2.cards.push_back(*it);
		i++;
	}

	CardGroup tcg1 =  CheckGroup(cg1);
	if(tcg1.cards.size() > 0)
	{
		cg2.jiang = tcg1.jiang;
		cg2.baida = cg.baida - tcg1.baida;
		CardGroup tcg2 =  CheckGroup(cg2);
		if(tcg2.cards.size() > 0)
		{
			//cg3 is tmp value of vcg[i]+vcg2[j]
			cgRet = tcg1;
			cgRet.cards.insert(cgRet.cards.end(),
					tcg2.cards.begin(), tcg2.cards.end());
			cgRet.jiang = tcg1.jiang || tcg2.jiang;
			cgRet.baida = tcg1.baida + tcg2.baida;
		}
	}
	return cgRet;
}

CardGroup CheckGroup(CardGroup group)
{
	depth++;
	// X means baida
	CardGroup cg;
	CardGroup tcg;
	int size = group.cards.size();
	if(size == 1)
	{
		int v1 = group.cards[0];
		// AX
		if(group.baida > 0 && group.jiang == false)
		{
			cg = createAX(v1, group.jiang);
		}
		//AXX
		else if(group.baida > 1)
		{
			cg = createAXX(v1, group.jiang);
		}
		return cg;
	}
	else if(size == 2)
	{
		int v1 = group.cards[0];
		int v2 = group.cards[1];
		if(v1 == v2)
		{
			if(group.jiang == false)
			{
				//AA
				{
					cg = createAA(v1, v2, group.jiang);
				}
				//AAX
				//AAX has more baida than AA, ignore it!
				/*
				if(group.baida > 0)
				{
					vgroup.push_back(createAAX(v1, v2, group.jiang));
				}
				*/
			}
			else if(group.baida > 0)
			{
				//AAX
				cg = createAAX(v1, v2, group.jiang);
			}
		}
		else if(abs(v2 - v1) <=2)
		{
			//ABX AXB etc
			if(group.baida > 0)
			{
				cg = createABX(v1, v2, group.jiang);
			}
		}
		else
		{
			cg = CheckSeparateGroup(group, 1);
		}
		return cg;
	}
	if(size >= 9)
	{
		if(size == 9)
		{
			tcg = Check9Cards(group);
		}
		else
		{
			tcg = CheckSeparateGroup(group, 9);
		}
		cg = UpdateCardGroup(cg, tcg);
	}
	if(size >= 8)
	{
		if(size == 8)
		{
			tcg = Check8Cards(group);
		}
		else
		{
			tcg = CheckSeparateGroup(group, 8);
		}
		cg = UpdateCardGroup(cg, tcg);
	}
	if(size >= 6)
	{
		if(size == 6)
		{
			tcg = Check6Cards(group);
		}
		else
		{
			tcg = CheckSeparateGroup(group, 6);
		}
		cg = UpdateCardGroup(cg, tcg);
	}
	if(size >= 5)
	{
		if(size == 5)
		{
			tcg = Check5Cards(group);
		}
		else
		{
			tcg = CheckSeparateGroup(group, 5);
		}
		cg = UpdateCardGroup(cg, tcg);
	}
	if(size >= 4)
	{
		if(size == 4)
		{
			tcg = Check4Cards(group);
		}
		else
		{
			tcg = CheckSeparateGroup(group, 4);
			if(debug) printf("4 tcg size: %d\n", tcg.cards.size());
		}
		cg = UpdateCardGroup(cg, tcg);
		if(debug) printf("4 cg size: %d\n", cg.cards.size());
	}
	if(size >= 3)
	{
		if(size == 3)
		{
			tcg = Check3Cards(group);
			if(debug) printf("3 tcg size: %d\n", tcg.cards.size());
		}
		else
		{
			tcg = CheckSeparateGroup(group, 3);
		}
		cg = UpdateCardGroup(cg, tcg);
		if(debug) printf("3 cg size: %d\n", cg.cards.size());
	}
	tcg = CheckSeparateGroup(group, 2);
	if(debug) printf("2 tcg size: %d\n", tcg.cards.size());
	cg = UpdateCardGroup(cg, tcg);
	if(debug) printf("2 cg size: %d\n", cg.cards.size());

	tcg = CheckSeparateGroup(group, 1);
	if(debug) printf("1 tcg size: %d\n", tcg.cards.size());
	cg = UpdateCardGroup(cg, tcg);
	if(debug) printf("1 cg size: %d\n", cg.cards.size());

	return cg;
}

