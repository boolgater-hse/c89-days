/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoLab1Sink
 * </сводка>
 *
 * <описание>
 *   Данный исходный код описывает реализацию интерфейсов CEcoLab1Sink
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2016 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#include "CEcoLab1Sink.h"
#include "IEcoConnectionPointContainer.h"

#include "RBTree.h"
#include "Tools.h"

#include <stdio.h>
#include <stdlib.h>



 /*
  *
  * <сводка>
  *   Функция QueryInterface
  * </сводка>
  *
  * <описание>
  *   Функция
  * </описание>
  *
  */
int16_t ECOCALLMETHOD CEcoLab1Sink_QueryInterface(/* in */ struct IEcoLab1Events* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    if (IsEqualUGUID(riid, &IID_IEcoLab1Events)) {
        *ppv = me;
        me->pVTbl->AddRef(me);
        return 0;
    } else if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = me;
        me->pVTbl->AddRef(me);
        return 0;
    }

    *ppv = 0;

    return -1;
}

/*
 *
 * <сводка>
 *   Функция AddRef
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1Sink_AddRef(/* in */ struct IEcoLab1Events* me) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*) me;

    if (me == 0) {
        return -1;
    }

    pCMe->m_cRef++;
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1Sink_Release(/* in */ struct IEcoLab1Events* me) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*) me;

    if (me == 0) {
        return -1;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if (pCMe->m_cRef == 0) {
        deleteCEcoLab1Sink((IEcoLab1Events*) pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Tree_Printer
 * </сводка>
 *
 * <описание>
 *   Выполняет печать двоичного дерево в 2D формате
 * </описание>
 *
 */
#define SPACE 10
void ECOCALLMETHOD Tree_Printer(/* in */ struct IEcoLab1Events* me, /* in */ struct RBNode* p_Node, /* in */ int32_t i32Space) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*) me;
    int32_t i;

    if (p_Node == 0 || pCMe->m_fnPrinter == 0) {
        return;
    }

    i32Space += SPACE;

    Tree_Printer(me, p_Node->right, i32Space);

    printf("\n");
    for (i = SPACE; i < i32Space; ++i) {
        printf(" ");
    }

    if (p_Node->color == 'b') {
        printf("%s", BLACK_NODE);
        pCMe->m_fnPrinter(p_Node->val);
        printf("%s\n", DEFAULTS);
    } else {
        printf("%s", RED_NODE);
        pCMe->m_fnPrinter(p_Node->val);
        printf("%s\n", DEFAULTS);
    }

    Tree_Printer(me, p_Node->left, i32Space);
}

/*
 *
 * <сводка>
 *   Функция Tree_Printer_With_Highlighted
 * </сводка>
 *
 * <описание>
 *   Выполняет печать двоичного дерево в 2D формате с выделенной вершиной
 *   Поддерживается выделение двумя цветами
 * </описание>
 *
 */
void ECOCALLMETHOD Tree_Printer_With_Highlighted(/* in */ struct IEcoLab1Events* me,
                                                 /* in */ struct RBNode* p_Node,
                                                 /* in */ struct RBNode* p_Highlight,
                                                 /* in */ int32_t i32Space,
                                                 /* in */ int16_t bSecondaryColor) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*) me;
    int32_t i;

    if (p_Node == 0 || p_Highlight == 0 || pCMe->m_fnPrinter == 0) {
        return;
    }

    i32Space += SPACE;

    Tree_Printer_With_Highlighted(me, p_Node->right, p_Highlight, i32Space, bSecondaryColor);

    printf("\n");
    for (i = SPACE; i < i32Space; ++i) {
        printf(" ");
    }

    if (p_Node == p_Highlight) {
        if (bSecondaryColor == 1) {
            if (p_Node->color == 'b') {
                printf("%s%s", BLACK_NODE, SUCCESS_NODE);
                pCMe->m_fnPrinter(p_Node->val);
                printf("%s\n", DEFAULTS);
            } else {
                printf("%s%s", RED_NODE, SUCCESS_NODE);
                pCMe->m_fnPrinter(p_Node->val);
                printf("%s\n", DEFAULTS);
            }
        } else {
            if (p_Node->color == 'b') {
                printf("%s%s", BLACK_NODE, HIGHLIGHTED_NODE);
                pCMe->m_fnPrinter(p_Node->val);
                printf("%s\n", DEFAULTS);
            } else {
                printf("%s%s", RED_NODE, HIGHLIGHTED_NODE);
                pCMe->m_fnPrinter(p_Node->val);
                printf("%s\n", DEFAULTS);
            }
        }
    } else if (p_Node->color == 'b') {
        printf("%s", BLACK_NODE);
        pCMe->m_fnPrinter(p_Node->val);
        printf("%s\n", DEFAULTS);
    } else {
        printf("%s", RED_NODE);
        pCMe->m_fnPrinter(p_Node->val);
        printf("%s\n", DEFAULTS);
    }

    Tree_Printer_With_Highlighted(me, p_Node->left, p_Highlight, i32Space, bSecondaryColor);
}

/*
 *
 * <сводка>
 *   Функция Tree_Inorder
 * </сводка>
 *
 * <описание>
 *   Выполняет обход двоичного дерева in-order
 *   с выделением текущей вершины
 * </описание>
 *
 */
void ECOCALLMETHOD Tree_Inorder(/* in */ struct IEcoLab1Events* me, struct RBNode* p_Root, struct RBNode* p_Traveler) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*) me;

    printf("%s%s", RESTORE_CRS_POS, CLEAR_FROM_CRS_TO_BOTTOM);
    printf("%sInorder tree traversal to get sorted sequence%s\n", BOLD, DEFAULTS);

    if (p_Traveler != 0) {
        Tree_Inorder(me, p_Root, p_Traveler->left);
        Tree_Printer_With_Highlighted(me, p_Root, p_Traveler, 0, 1);
        WAIT_FOR_NEXT;
        Tree_Inorder(me, p_Root, p_Traveler->right);
    }
}

/*
 *
 * <сводка>
 *   Функция OnTreePrintCallback
 * </сводка>
 *
 * <описание>
 *   Функция обратного вызова для печати дерева
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_OnTreePrintCallback(/* in */ struct IEcoLab1Events* me, /* in */ struct RBNode* p_Root, /* in */ int16_t i16Mode) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*) me;

    if (me == 0 || pCMe->m_fnPrinter == 0) {
        return -1;
    }

    if (p_Root == 0) {
        return;
    } else {
        printf("%sCurrent state of tree:%s\n", BOLD, DEFAULTS);

        Tree_Printer(me, p_Root, 0);
    }

    if (i16Mode == 0) {
        printf("\n");
        WAIT_FOR_NEXT;
    } else if (i16Mode == 1) {
        printf("%s%s", RESTORE_CRS_POS, CLEAR_FROM_CRS_TO_BOTTOM);
    } else if (i16Mode == 2) {
        printf("\n");
    }

    return 0;
}

/*
 *
 * <сводка>
 *   Функция OnTreePrintWithHighlightCallback
 * </сводка>
 *
 * <описание>
 *   Функция обратного вызова для печати дерева с выделенной вершиной
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_OnTreePrintWithHighlightCallback(/* in */ struct IEcoLab1Events* me,
                                                                    /* in */ struct RBNode* p_Root,
                                                                    /* in */ struct RBNode* p_Highlight,
                                                                    /* in */ int16_t bSecondaryColor,
                                                                    /* in */ void* p_Element) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*) me;

    if (me == 0 || pCMe->m_fnPrinter == 0) {
        return -1;
    }

    if (p_Root == 0) {
        return;
    } else {
        if (bSecondaryColor == 1) {
            //printf("%sPlace for %i%s", BOLD, *(int32_t*) p_Element, DEFAULTS);
            printf("%sPlace for ", BOLD);
            pCMe->m_fnPrinter(p_Element);
            printf("%s\n", DEFAULTS);

            Tree_Printer_With_Highlighted(me, p_Root, p_Highlight, 0, bSecondaryColor);

            WAIT_FOR_NEXT;
        } else {
            //printf("%sLooking for a place for %i%s\n", BOLD, *(int32_t*) p_Element, DEFAULTS);
            printf("%sPlace for ", BOLD);
            pCMe->m_fnPrinter(p_Element);
            printf("%s\n", DEFAULTS);

            Tree_Printer_With_Highlighted(me, p_Root, p_Highlight, 0, bSecondaryColor);

            WAIT_FOR_NEXT;
        }
    }

    printf("%s%s", RESTORE_CRS_POS, CLEAR_FROM_CRS_TO_BOTTOM);

    return 0;
}

/*
 *
 * <сводка>
 *   Функция OnSaveCursorPositionCallback
 * </сводка>
 *
 * <описание>
 *   Функция обратного вызова для сохранения позиции курсора
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_OnSaveCursorPositionCallback(/* in */ struct IEcoLab1Events* me) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*) me;

    if (me == 0) {
        return -1;
    }

    printf(SAVE_CRS_POS);

    return 0;
}

/*
 *
 * <сводка>
 *   Функция OnTreeInorderTraversalCallback
 * </сводка>
 *
 * <описание>
 *   Функция обратного вызова для обхода дерева in-order
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_OnTreeInorderTraversalCallback(/* in */ struct IEcoLab1Events* me, /* in */ struct RBNode* p_Root) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*) me;

    if (me == 0) {
        return -1;
    }

    Tree_Inorder(me, p_Root, p_Root);

    printf("%s%s", RESTORE_CRS_POS, CLEAR_FROM_CRS_TO_BOTTOM);

    return 0;
}

/*
 *
 * <сводка>
 *   Функция Advise
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_Advise(/* in */ struct CEcoLab1Sink* me, /* in */IEcoLab1* pIEcoLab1) {
    IEcoConnectionPointContainer* pCPC = 0;
    IEcoConnectionPoint* pCP = 0;
    int16_t result = 0;

    result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoConnectionPointContainer, (void**) &pCPC);

    if (result == 0 && pCPC != 0) {
        result = pCPC->pVTbl->FindConnectionPoint(pCPC, &IID_IEcoLab1Events, &pCP);
        pCPC->pVTbl->Release(pCPC);
        pCPC = 0;
        if (result == 0 && pCP != 0) {

            result = pCP->pVTbl->Advise(pCP, (IEcoUnknown*) me, &me->m_cCookie);
            pCP->pVTbl->Release(pCP);
            pCP = 0;
        }
    }

    return result;
}

/*
 *
 * <сводка>
 *   Функция Unadvise
 * </сводка>
 *
 * <описание>
 *   Функция
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1Sink_Unadvise(/* in */ struct CEcoLab1Sink* me, /* in */IEcoLab1* pIEcoLab1) {
    IEcoConnectionPointContainer* pCPC = 0;
    IEcoConnectionPoint* pCP = 0;
    int16_t result = 0;

    if (me->m_cCookie) {
        result = pIEcoLab1->pVTbl->QueryInterface(pIEcoLab1, &IID_IEcoConnectionPointContainer, (void**) &pCPC);
        if (result == 0) {
            result = pCPC->pVTbl->FindConnectionPoint(pCPC, &IID_IEcoLab1Events, &pCP);
            pCPC->pVTbl->Release(pCPC);
            pCPC = 0;
            if (result == 0) {
                result = pCP->pVTbl->Unadvise(pCP, me->m_cCookie);
                pCP->pVTbl->Release(pCP);
                pCP = 0;
            }
        }
    }
    return result;
}

int16_t ECOCALLMETHOD CEcoLab1Sink_SetPrintFunction(/* in */ struct CEcoLab1Sink* me, /* in */ void (ECOCDECLMETHOD* fnPrinter)(const void*)) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*) me;

    if (me == 0) {
        return -1;
    }

    pCMe->m_fnPrinter = fnPrinter;
}

/* Create Virtual Table IEcoLab1VTblEvents */
IEcoLab1VTblEvents g_x2D2E3B9214F248A6A09ECB494B59C795VTblEvents = {
    CEcoLab1Sink_QueryInterface,
    CEcoLab1Sink_AddRef,
    CEcoLab1Sink_Release,
    CEcoLab1Sink_OnTreePrintCallback,
    CEcoLab1Sink_OnTreePrintWithHighlightCallback,
    CEcoLab1Sink_OnSaveCursorPositionCallback,
    CEcoLab1Sink_OnTreeInorderTraversalCallback
};

/*
 *
 * <сводка>
 *   Функция Create
 * </сводка>
 *
 * <описание>
 *   Функция создания экземпляра
 * </описание>
 *
 */
int16_t ECOCALLMETHOD createCEcoLab1Sink(/* in */ IEcoMemoryAllocator1* pIMem, /* out */ IEcoLab1Events** ppIEcoLab1Events) {
    int16_t result = -1;
    CEcoLab1Sink* pCMe = 0;

    /* Проверка указателей */
    if (ppIEcoLab1Events == 0 || pIMem == 0) {
        return result;
    }

    /* Выделение памяти для данных экземпляра */
    pCMe = (CEcoLab1Sink*) pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoLab1Sink));

    /* Сохранение указателя на интерфейс для работы с памятью */
    pCMe->m_pIMem = pIMem;
    pCMe->m_pIMem->pVTbl->AddRef(pCMe->m_pIMem);

    /* Установка счетчика ссылок на компонент */
    pCMe->m_cRef = 1;

    /* Создание таблицы функций интерфейса IEcoP2PEvents */
    pCMe->m_pVTblIEcoLab1Events = &g_x2D2E3B9214F248A6A09ECB494B59C795VTblEvents;
    pCMe->SetPrintFunction = CEcoLab1Sink_SetPrintFunction;

    *ppIEcoLab1Events = (IEcoLab1Events*) pCMe;

    return 0;
};

/*
 *
 * <сводка>
 *   Функция Delete
 * </сводка>
 *
 * <описание>
 *   Функция освобождения экземпляра
 * </описание>
 *
 */
void ECOCALLMETHOD deleteCEcoLab1Sink(IEcoLab1Events* pIEcoLab1Events) {
    CEcoLab1Sink* pCMe = (CEcoLab1Sink*) pIEcoLab1Events;
    IEcoMemoryAllocator1* pIMem = 0;

    if (pIEcoLab1Events != 0) {
        pIMem = pCMe->m_pIMem;
        /* Освобождение */
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
};
