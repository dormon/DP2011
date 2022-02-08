#ifndef _NTREE_H_
#define _NTREE_H_

#include"adtfce.h"
#include"standard.h"

/**
 * Zakladni funkce pro oktalovy strom
 */
extern const SADTFCE NTREE_ADTFCE;

/**
 * @brief Reprezentuje octalovy strom
 */
struct SNTreeItem{
	struct SNTreeItem**Next;/// podstromy
	struct SNTreeItem*Parent;/// rodic
	unsigned Id;/// cislo potomka
	unsigned NumNext;///pocet nasledniku
	void*Data;/// data
};

typedef struct{
	struct SNTreeItem*Root;/// koren
	SADTFCE*F;/// funkce pro praci
}SNTree;

/**
 * @brief Inicialiuje strom
 *
 * @param T strom
 * @param F funkce pro praci
 */
void ntree_Init(SNTree*T,SADTFCE*F);

/**
 * @brief Uvolni strom
 *
 * @param T strom
 */
void ntree_Free(void*T);

/**
 * @brief Uvolni prvek stromu
 *
 * @param A prvek
 * @param F uvolnovaci funkce stromu SNTree->F.Free
 */
void ntree_ItemFree(struct SNTreeItem*A,SADTFCE*F);

/**
 * @brief Vytvori prvek
 *
 * @param A prvek
 * @param Parent rodic
 * @param Id cislo potomka
 * @param NumNext pocet nasledniku
 */
void ntree_ItemCreate(struct SNTreeItem**A,struct SNTreeItem*Parent,
		unsigned Id,unsigned NumNext);

#endif//_NTREE_H_
