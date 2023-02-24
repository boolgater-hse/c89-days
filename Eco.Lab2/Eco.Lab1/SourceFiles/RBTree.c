#include "RBTree.h"

/*
 *
 * <описание>
 *   Macro implementing functionality of memcpy.
 * </описание>
 *
 */
#define MOVE(to, from, size)           \
do {                                   \
    size_t __size = (size);            \
    char_t *__a = (to), *__b = (from); \
    do {                               \
        *__a++ = *__b++;               \
    } while (--__size > 0);            \
} while (0)

 /*
  *
  * <сводка>
  *   Insert function
  * </сводка>
  *
  * <описание>
  *   Non-recursion version of function Insert into Red-black tree.
  * </описание>
  *
  */
struct RBNode* Insert(struct IEcoLab1* me, struct RBNode** p_Root, void* val, size_t u32Size, int (ECOCDECLMETHOD* fnCompare)(const void*, const void*)) {
    struct RBNode* current = *p_Root;
    struct RBNode* inserted;

    if (*p_Root == 0) {
        return Make_New(me, 0, val, u32Size);
    }
    while (current != 0) {
        if ((*fnCompare) (val, current->val) <= 0) {
            if (current->left != 0) {
                current = current->left;
            }
            else {
                inserted = Make_New(me, current, val, u32Size);
                current->left = inserted;
                current = inserted;
                break;
            }
        }
        else {
            if (current->right != 0) {
                current = current->right;
            }
            else {
                inserted = Make_New(me, current, val, u32Size);
                current->right = inserted;
                current = inserted;

                break;
            }
        }
    }

    while (current != *p_Root) {
        Balance(current);
        if (current->parent == 0) {
            *p_Root = current;
            break;
        }
        current = current->parent;
        if (current == *p_Root) {
            current->color = 0;
        }
    }

    return *p_Root;
}

/*
 *
 * <сводка>
 *   Balance function
 * </сводка>
 *
 * <описание>
 *   Helper function for Insert. Implements rotation of binary tree.
 * </описание>
 *
 */
void Balance(struct RBNode* node) {
    struct RBNode* child;
    struct RBNode* parent;
    struct RBNode* grand_parent;
    struct RBNode* great_grand_parent;

    if (node == 0 || node->parent == 0) {
        return;
    }

    child = node;
    if (node->color == 0 || (node->parent)->color == 0) {
        return;
    }

    parent = node->parent;
    grand_parent = parent->parent;

    if (grand_parent == 0) {
        parent->color = 0;
        return;
    }

    if (grand_parent->right != 0 && (grand_parent->right)->color == 1 &&
        grand_parent->left != 0 && (grand_parent->left)->color == 1) {
        (grand_parent->right)->color = 0;
        (grand_parent->left)->color = 0;
        grand_parent->color = 1;
        return;
    }
    else {
        great_grand_parent = grand_parent->parent;
        if (grand_parent->right == parent) {
            if (parent->right == node) {
                grand_parent->right = parent->left;
                if (parent->left != 0) {
                    (parent->left)->parent = grand_parent;
                }
                parent->left = grand_parent;
                grand_parent->parent = parent;

                parent->parent = great_grand_parent;
                if (great_grand_parent != 0) {
                    if (great_grand_parent->left != 0 &&
                        great_grand_parent->left == grand_parent) {
                        great_grand_parent->left = parent;
                    }
                    else {
                        great_grand_parent->right = parent;
                    }
                }

                parent->color = 0;
                grand_parent->color = 1;
            }
            else {
                parent->left = child->right;
                if (child->right != 0) {
                    (child->right)->parent = parent;
                }
                child->right = parent;
                parent->parent = child;

                grand_parent->right = child->left;
                if (child->left != 0) {
                    (child->left)->parent = grand_parent;
                }
                child->left = grand_parent;
                grand_parent->parent = child;

                child->parent = great_grand_parent;
                if (great_grand_parent != 0) {
                    if (great_grand_parent->left != 0 &&
                        great_grand_parent->left == grand_parent) {
                        great_grand_parent->left = child;
                    }
                    else {
                        great_grand_parent->right = child;
                    }
                }

                child->color = 0;
                grand_parent->color = 1;
            }
        }
        else {
            if (parent->left == node) {
                grand_parent->left = parent->right;
                if (parent->right != 0) {
                    (parent->right)->parent = grand_parent;
                }
                parent->right = grand_parent;
                grand_parent->parent = parent;

                parent->parent = great_grand_parent;
                if (great_grand_parent != 0) {
                    if (great_grand_parent->left != 0 &&
                        great_grand_parent->left == grand_parent) {
                        great_grand_parent->left = parent;
                    }
                    else {
                        great_grand_parent->right = parent;
                    }
                }

                parent->color = 0;
                grand_parent->color = 1;
            }
            else {
                parent->right = child->left;
                if (child->left != 0) {
                    (child->left)->parent = parent;
                }
                child->left = parent;
                parent->parent = child;

                grand_parent->left = child->right;
                if (child->right != 0) {
                    (child->right)->parent = grand_parent;
                }
                child->right = grand_parent;
                grand_parent->parent = child;

                child->parent = great_grand_parent;
                if (great_grand_parent != 0) {
                    if (great_grand_parent->left != 0 &&
                        great_grand_parent->left == grand_parent) {
                        great_grand_parent->left = child;
                    }
                    else {
                        great_grand_parent->right = child;
                    }
                }

                child->color = 0;
                grand_parent->color = 1;
            }
        }
    }
}

/*
 *
 * <сводка>
 *   Make_New function
 * </сводка>
 *
 * <описание>
 *   Helper function for Insert. Creates and allocates new RBNode and returns pointer to it.
 * </описание>
 *
 */
struct RBNode* Make_New(struct IEcoLab1* me, struct RBNode* p_Parent, void* p_val, size_t u32Size) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    struct RBNode* node = (struct RBNode*)pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, sizeof(struct RBNode));

    if (node == 0) {
        return 0;
    }
    else {
        node->val = pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, u32Size);
        if (node->val == 0) return 0;
        MOVE(node->val, p_val, u32Size);
        node->left = 0;
        node->right = 0;
        node->parent = p_Parent;
        node->color = 1;
    }

    return node;
}

/*
 *
 * <сводка>
 *   Write_Inorder function
 * </сводка>
 *
 * <описание>
 *   Non-recursive function that performs an in-order tree traversal and writes the result to an array passed for sorting.
 * </описание>
 *
 */
void Write_Inorder(struct RBNode* p_Root,
    void* p_Base,
    size_t u32Size,
    size_t* pi_u32Index) {
    struct RBNode* current = p_Root;
    struct RBNode* prev;

    while (current != 0) {
        if (current->left == 0) {
            MOVE((char_t*)p_Base + (u32Size * (*pi_u32Index)++), current->val, u32Size);
            current = current->right;
        }
        else {
            prev = current->left;
            while (prev->right != 0 && prev->right != current) {
                prev = prev->right;
            }
            if (prev->right == 0) {
                prev->right = current;
                current = current->left;
            }
            else {
                prev->right = 0;
                MOVE((char_t*)p_Base + (u32Size * (*pi_u32Index)++), current->val, u32Size);
                current = current->right;
            }
        }
    }
}

/*
 *
 * <сводка>
 *   Destroy function
 * </сводка>
 *
 * <описание>
 *   Traverses the tree using a non-recursive post-order tree traversal and frees up memory allocated for nodes and data in them.
 * </описание>
 *
 */
void Destroy(struct IEcoLab1* me, struct RBNode* p_Root) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    struct RBNode* current = p_Root;
    struct RBNode* up = 0;

    struct RBNode* left;
    struct RBNode* right;

    while (current != 0) {
        if (current->left != 0) {
            left = current->left;
            current->left = up;
            up = current;
            current = left;
        }
        else if (current->right != 0) {
            right = current->right;
            current->left = up;
            current->right = 0;
            up = current;
            current = right;
        }
        else {
            if (up == 0) {
                pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, current->val);
                pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, current);
                current = 0;
            }
            while (up != 0) {
                pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, current->val);
                pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, current);
                if (up->right != 0) {
                    current = up->right;
                    up->right = 0;
                    break;
                }
                else {
                    current = up;
                    up = up->left;
                }
            }
        }
    }
}
