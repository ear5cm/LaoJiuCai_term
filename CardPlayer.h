#ifndef _CARDPLAYER_H
#define _CARDPLAYER_H
/*============================================
 *Author:baili_feng
 *Date:2015/01/02
 *============================================
 */
#include "Card.h"
#include "CardTable.h"

class CardPlayer
{
public:
	vector<	int >		m_MyPAIVec[6];		
	vector<	int >		m_ChiPAIVec[6];		
	vector<	int >		m_PengPAIVec[6];	
	vector<	int >		m_GangPAIVec[6];	

	MJCard				m_baida;
	vector< MJCard >	m_baidaVec;
	MJCard				m_tmpCard;
	CardGroup			m_tiao;
	CardGroup			m_bing;
	CardGroup			m_wan;
	CardTable*			m_table;

public:

	CardPlayer();
	~CardPlayer();
	void			setTable(CardTable* table);
	void			addCard(MJCard card);
	bool			rmCard(MJCard card);
	bool			rmCard(int index);
	bool			checkHu();
	bool			checkTing();
	void			showGuid();
	void			showHu();
	void			showCards();
	void			showValue(MJCard card);
	void			showType(MJCard card);
	void			dump();
	void			setBaida(MJCard card);
	bool			isBaida(MJCard);
	bool			addTmpCard(MJCard card);
	bool			rmTmpCard();
	void			reset();
};

#endif
