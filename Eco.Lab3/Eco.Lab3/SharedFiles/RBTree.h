#ifndef __RBTREE_H__
#define __RBTREE_H__

#include "RBTree.h"
#include "CEcoLab1.h"
#include "IEcoLab1Events.h"

struct RBNode {
    void* val;
    struct RBNode* left;
    struct RBNode* right;
    struct RBNode* parent;
    char_t color;
};

int16_t ECOCALLMETHOD Fire_OnTreePrintCallback(/* in */ struct IEcoLab1* me, /* in */ struct RBNode* p_Root, /* in */ int16_t bClear);
int16_t ECOCALLMETHOD Fire_OnTreePrintWithHighlightCallback(/* in */ struct IEcoLab1* me,
                                                                            /* in */ struct RBNode* p_Root,
                                                                            /* in */ struct RBNode* p_Highlight,
                                                                            /* in */ int16_t bSecondaryColor,
                                                                            /* in */ void* p_Element);

struct RBNode* Insert(struct IEcoLab1* me, struct RBNode** p_Root, void* val, size_t u32Size, int (ECOCDECLMETHOD* fnCompare)(const void*, const void*));

struct RBNode* Make_New(struct IEcoLab1* me, struct RBNode* p_Parent, void* val, size_t u32Size);

void Balance(struct RBNode* node);

void Write_Inorder(struct IEcoLab1* me, struct RBNode* p_Root, void* p_Base, size_t u32Size, size_t* pi_u32Index);

void Destroy(struct IEcoLab1* me, struct RBNode* p_Root);

#endif /* __RBTREE_H__ */
