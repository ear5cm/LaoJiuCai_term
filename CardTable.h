#ifndef _CARDTABLE_H
#define _CARDTABLE_H
#include "Card.h"

class CardTable
{
	vector< MJCard >	m_mjVec;
	int				m_huangNum;
public:

	CardTable();
	~CardTable();
	void	init(int huangNum, unsigned int seed);
	MJCard	getCard();
	int		getCardNum();
private:
	void	shuffle(unsigned int s);
};
#endif
