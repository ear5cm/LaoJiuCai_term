#include "CardTable.h"
	
CardTable::CardTable()
{
	m_huangNum = 0;
}

CardTable::~CardTable()
{
	
}

void CardTable::init(int huangNum, unsigned int seed)
{
	m_huangNum = huangNum;
	m_mjVec.clear();
	for(int i = 1 ; i <= 9 ; i++)
	{
		MJCard card;
		card.value = i;
		card.type = MJTYPE_TIAO;
		m_mjVec.push_back(card);
		m_mjVec.push_back(card);
		m_mjVec.push_back(card);
		m_mjVec.push_back(card);
		card.type = MJTYPE_BING;
		m_mjVec.push_back(card);
		m_mjVec.push_back(card);
		m_mjVec.push_back(card);
		m_mjVec.push_back(card);
		card.type = MJTYPE_WAN;
		m_mjVec.push_back(card);
		m_mjVec.push_back(card);
		m_mjVec.push_back(card);
		m_mjVec.push_back(card);
	}
	shuffle(seed);
}

void CardTable::shuffle(unsigned int s)
{
	srand( s );
	random_shuffle(m_mjVec.begin(),m_mjVec.end());
}

int CardTable::getCardNum()
{
	return m_mjVec.size();
}
	
MJCard CardTable::getCard()
{
		MJCard card;
		card.type = MJTYPE_UNDEFINE;
		card.value = -1;
		if(m_mjVec.size()-1 > m_huangNum)
		{
				card.type = m_mjVec.back().type;
				card.value = m_mjVec.back().value;
				m_mjVec.pop_back();
		}
		return card;
}

