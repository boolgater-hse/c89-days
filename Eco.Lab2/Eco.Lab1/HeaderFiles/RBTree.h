#ifndef __RBTREE_H__
#define __RBTREE_H__

#include "RBTree.h"
#include "CEcoLab1.h"

struct RBNode {
    void* val;
    struct RBNode* left;
    struct RBNode* right;
    struct RBNode* parent;
    int color;
};

struct RBNode* Insert(struct IEcoLab1* me, struct RBNode** p_Root, void* val, size_t u32Size, int (ECOCDECLMETHOD* fnCompare)(const void*, const void*));

struct RBNode* Make_New(struct IEcoLab1* me, struct RBNode* p_Parent, void* val, size_t u32Size);

void Balance(struct RBNode* node);

void Write_Inorder(struct RBNode* p_Root, void* p_Base, size_t u32Size, size_t* pi_u32Index);

void Destroy(struct IEcoLab1* me, struct RBNode* p_Root);

#endif /* __RBTREE_H__ */
