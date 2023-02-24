/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CEcoLab1
 * </сводка>
 *
 * <описание>
 *   Данный исходный код описывает реализацию интерфейсов CEcoLab1
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#include "CEcoLab1.h"

#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "IEcoSystem1.h"

#include "RBTree.h"

#include "IdEcoCalculatorA.h"
#include "IdEcoCalculatorB.h"
#include "IdEcoCalculatorD.h"
#include "IdEcoCalculatorE.h"

 /*
  *
  * <сводка>
  *   Функция QueryInterface
  * </сводка>
  *
  * <описание>
  *   Функция QueryInterface для интерфейса IEcoLab1
  * </описание>
  *
  */
int16_t ECOCALLMETHOD CEcoLab1_QueryInterface(/* in */ struct IEcoLab1* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab1* pCMe = (CEcoLab1*) me;

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return -1;
    }

    /* Проверка и получение запрошенного интерфейса */
    if (IsEqualUGUID(riid, &IID_IEcoLab1)) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*) pCMe);
    } else if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*) pCMe);
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX) && pCMe->m_pInnerUnknown != 0) {
        return pCMe->m_pInnerUnknown->pVTbl->QueryInterface(pCMe->m_pInnerUnknown, riid, ppv);
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = &pCMe->m_pVTblIEcoCalculatorX;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*) pCMe);
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &pCMe->m_pVTblIEcoCalculatorY;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*) pCMe);
    } else {
        *ppv = 0;
        return -1;
    }
    return 0;
}

/*
 *
 * <сводка>
 *   Функция AddRef
 * </сводка>
 *
 * <описание>
 *   Функция AddRef для интерфейса IEcoLab1
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1_AddRef(/* in */ struct IEcoLab1* me) {
    CEcoLab1* pCMe = (CEcoLab1*) me;

    /* Проверка указателя */
    if (me == 0) {
        return -1;
    }

    return ++pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoLab1
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CEcoLab1_Release(/* in */ struct IEcoLab1* me) {
    CEcoLab1* pCMe = (CEcoLab1*) me;

    /* Проверка указателя */
    if (me == 0) {
        return -1;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if (pCMe->m_cRef == 0) {
        deleteCEcoLab1((IEcoLab1*) pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция TreeSort
 * </сводка>
 *
 * <описание>
 *   Performs Tree sort on array p_Base of size u32Num with one element byte size
 *   u32Size. Elements are comparing by fnCompare comparator.
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CEcoLab1_TreeSort(/* in */ struct IEcoLab1* me,
                                        /* in */ void* p_Base,
                                        /* in */ size_t u32Num,
                                        /* in */ size_t u32Size,
                                        /* in */ int (ECOCDECLMETHOD* fnCompare)(const void*, const void*)) {
    CEcoLab1* pCMe = (CEcoLab1*)me;
    struct RBNode* p_bst = 0;
    size_t u32Index = 0;
    int i = 0;

    /* Проверка указателей */
    if (me == 0 || p_Base == 0 || fnCompare == 0 || u32Num < 0 || u32Size < 0) {
        return -1;
    }

    for (i = 0; i < u32Num; ++i) {
        p_bst = Insert(me, &p_bst, (char_t*)p_Base + (u32Size * i), u32Size, fnCompare);
    }

    Write_Inorder(p_bst, p_Base, u32Size, &u32Index);

    Destroy(me, p_bst);

    return 0;
}

int16_t CEcoLab1_IEcoCalculatorX_QueryInterface(/* in */ struct IEcoCalculatorX* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab1* pCMe = (CEcoLab1*) ((uint64_t) me - sizeof(struct IEcoLab1*));

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return -1;
    }

    /* Проверка и получение запрошенного интерфейса */
    if (IsEqualUGUID(riid, &IID_IEcoLab1)) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*) pCMe);
    } else if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*) pCMe);
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX) && pCMe->m_pInnerUnknown != 0) {
        return pCMe->m_pInnerUnknown->pVTbl->QueryInterface(pCMe->m_pInnerUnknown, riid, ppv);
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = &pCMe->m_pVTblIEcoCalculatorX;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*) pCMe);
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &pCMe->m_pVTblIEcoCalculatorY;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*) pCMe);
    } else {
        *ppv = 0;
        return -1;
    }
    return 0;
}



/*
 *
 * <сводка>
 *   Функция AddRef
 * </сводка>
 *
 * <описание>
 *   Функция AddRef для интерфейса IEcoCalculatorX
 * </описание>
 *
 */
uint32_t CEcoLab1_IEcoCalculatorX_AddRef(/* in */ struct IEcoCalculatorX* me) {
    CEcoLab1* pCMe = (CEcoLab1*) ((uint64_t) me - sizeof(struct IEcoLab1*));

    /* Проверка указателя */
    if (me == 0) {
        return -1;
    }

    return ++pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoCalculatorY
 * </описание>
 *
 */
uint32_t CEcoLab1_IEcoCalculatorX_Release(/* in */ struct IEcoCalculatorX* me) {
    CEcoLab1* pCMe = (CEcoLab1*) ((uint64_t) me - sizeof(struct IEcoLab1*));

    /* Проверка указателя */
    if (me == 0) {
        return -1;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if (pCMe->m_cRef == 0) {
        deleteCEcoLab1((IEcoLab1*) pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}


/*
 *
 * <сводка>
 *   Функция Addition
 * </сводка>
 *
 * <описание>
 *   Функция сложения
 * </описание>
 *
 */
int32_t CEcoLab1_Addition(/* in */ struct IEcoCalculatorX* me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab1* pCMe = (CEcoLab1*) ((uint64_t) me - sizeof(struct IEcoLab1*));

    /* Проверка указателей */
    if (me == 0 || pCMe->m_pIEcoCalculatorX == 0) {
        return -1;
    }

    return pCMe->m_pIEcoCalculatorX->pVTbl->Addition(pCMe->m_pIEcoCalculatorX, a, b);
}


/*
 *
 * <сводка>
 *   Функция Subtraction
 * </сводка>
 *
 * <описание>
 *   Функция вычитания
 * </описание>
 *
 */
int16_t CEcoLab1_Subtraction(/* in */ struct IEcoCalculatorX* me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab1* pCMe = (CEcoLab1*) ((uint64_t) me - sizeof(struct IEcoLab1*));

    /* Проверка указателей */
    if (me == 0 || pCMe->m_pIEcoCalculatorX == 0) {
        return -1;
    }

    return pCMe->m_pIEcoCalculatorX->pVTbl->Subtraction(pCMe->m_pIEcoCalculatorX, a, b);
}


int16_t CEcoLab1_IEcoCalculatorY_QueryInterface(/* in */ struct IEcoCalculatorY* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CEcoLab1* pCMe = (CEcoLab1*) ((uint64_t) me - sizeof(struct IEcoLab1*) * 2);

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return -1;
    }

    /* Проверка и получение запрошенного интерфейса */
    if (IsEqualUGUID(riid, &IID_IEcoLab1)) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*) pCMe);
    } else if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &pCMe->m_pVTblIEcoLab1;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*) pCMe);
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX) && pCMe->m_pInnerUnknown != 0) {
        return pCMe->m_pInnerUnknown->pVTbl->QueryInterface(pCMe->m_pInnerUnknown, riid, ppv);
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorX)) {
        *ppv = &pCMe->m_pVTblIEcoCalculatorX;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*) pCMe);
    } else if (IsEqualUGUID(riid, &IID_IEcoCalculatorY)) {
        *ppv = &pCMe->m_pVTblIEcoCalculatorY;
        pCMe->m_pVTblIEcoLab1->AddRef((IEcoLab1*) pCMe);
    } else {
        *ppv = 0;
        return -1;
    }

    return 0;
}

/*
 *
 * <сводка>
 *   Функция AddRef
 * </сводка>
 *
 * <описание>
 *   Функция AddRef для интерфейса IEcoCalculatorY
 * </описание>
 *
 */
uint32_t CEcoLab1_IEcoCalculatorY_AddRef(/* in */ struct IEcoCalculatorY* me) {
    CEcoLab1* pCMe = (CEcoLab1*) ((uint64_t) me - sizeof(struct IEcoLab1*) * 2);

    /* Проверка указателя */
    if (me == 0) {
        return -1;
    }

    return ++pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Release
 * </сводка>
 *
 * <описание>
 *   Функция Release для интерфейса IEcoCalculatorY
 * </описание>
 *
 */
uint32_t CEcoLab1_IEcoCalculatorY_Release(/* in */ struct IEcoCalculatorY* me) {
    CEcoLab1* pCMe = (CEcoLab1*) ((uint64_t) me - sizeof(struct IEcoLab1*) * 2);

    /* Проверка указателя */
    if (me == 0) {
        return -1;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if (pCMe->m_cRef == 0) {
        deleteCEcoLab1((IEcoLab1*) pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}


/*
 *
 * <сводка>
 *   Функция Multiplication
 * </сводка>
 *
 * <описание>
 *   Функция умножения
 * </описание>
 *
 */
int32_t CEcoLab1_Multiplication(/* in */ struct IEcoCalculatorY* me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab1* pCMe = (CEcoLab1*) ((uint64_t) me - sizeof(struct IEcoLab1*) * 2);

    /* Проверка указателей */
    if (me == 0 || pCMe->m_pIEcoCalculatorY == 0) {
        return -1;
    }

    return pCMe->m_pIEcoCalculatorY->pVTbl->Multiplication(pCMe->m_pIEcoCalculatorY, a, b);
}


/*
 *
 * <сводка>
 *   Функция Division
 * </сводка>
 *
 * <описание>
 *   Функция деления
 * </описание>
 *
 */
int16_t CEcoLab1_Division(/* in */ struct IEcoCalculatorY* me, /* in */ int16_t a, /* in */ int16_t b) {
    CEcoLab1* pCMe = (CEcoLab1*) ((uint64_t) me - sizeof(struct IEcoLab1*) * 2);

    /* Проверка указателей */
    if (me == 0 || pCMe->m_pIEcoCalculatorY == 0) {
        return -1;
    }

    return pCMe->m_pIEcoCalculatorY->pVTbl->Division(pCMe->m_pIEcoCalculatorY, a, b);
}


/*
 *
 * <сводка>
 *   Функция Init
 * </сводка>
 *
 * <описание>
 *   Функция инициализации экземпляра
 * </описание>
 *
 */
int16_t ECOCALLMETHOD initCEcoLab1(/*in*/ struct IEcoLab1* me, /* in */ struct IEcoUnknown* pIUnkSystem) {
    CEcoLab1* pCMe = (CEcoLab1*) me;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoUnknown* pOuterUnknown = (IEcoUnknown*) me;
    int16_t result = -1;

    /* Проверка указателей */
    if (me == 0) {
        return result;
    }

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = (IEcoSystem1*) pIUnkSystem;

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pCMe->m_pISys->pVTbl->QueryInterface(pCMe->m_pISys, &IID_IEcoInterfaceBus1, (void**) &pIBus);

    /* Проверка указателей */
    if (me == 0) {
        return result;
    }

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = (IEcoSystem1*) pIUnkSystem;

    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorE, 0, &IID_IEcoCalculatorY, (void**) &pCMe->m_pIEcoCalculatorY);
    if (result != 0 || pCMe->m_pIEcoCalculatorY == 0) {
        result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorD, 0, &IID_IEcoCalculatorY, (void**) &pCMe->m_pIEcoCalculatorY);
    }
    if (result != 0) {
        goto release;
    }

    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorB, pOuterUnknown, &IID_IEcoUnknown, (void**) &pCMe->m_pInnerUnknown);
    if (result != 0 || pCMe->m_pInnerUnknown == 0) {
        result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoCalculatorA, 0, &IID_IEcoCalculatorX, (void**) &pCMe->m_pIEcoCalculatorX);
    }

release:

    /* Освобождение */
    pIBus->pVTbl->Release(pIBus);

    return result;
}

/* Create Virtual Table IEcoLab1 */
IEcoLab1VTbl g_x277FC00C35624096AFCFC125B94EEC90VTbl = {
    CEcoLab1_QueryInterface,
    CEcoLab1_AddRef,
    CEcoLab1_Release,
    CEcoLab1_TreeSort
};

IEcoCalculatorXVTbl g_x9322111622484742AE0682819447843DVTblD = {
    CEcoLab1_IEcoCalculatorX_QueryInterface,
    CEcoLab1_IEcoCalculatorX_AddRef,
    CEcoLab1_IEcoCalculatorX_Release,
    CEcoLab1_Addition,
    CEcoLab1_Subtraction
};

IEcoCalculatorYVTbl g_xBD6414C29096423EA90C04D77AFD1CADVTblD = {
    CEcoLab1_IEcoCalculatorY_QueryInterface,
    CEcoLab1_IEcoCalculatorY_AddRef,
    CEcoLab1_IEcoCalculatorY_Release,
    CEcoLab1_Multiplication,
    CEcoLab1_Division
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
int16_t ECOCALLMETHOD createCEcoLab1(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IEcoLab1** ppIEcoLab1) {
    int16_t result = -1;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    CEcoLab1* pCMe = 0;
    UGUID* rcid = (UGUID*) &CID_EcoMemoryManager1;

    /* Проверка указателей */
    if (ppIEcoLab1 == 0 || pIUnkSystem == 0) {
        return result;
    }

    /* Получение системного интерфейса приложения */
    result = pIUnkSystem->pVTbl->QueryInterface(pIUnkSystem, &GID_IEcoSystem1, (void**) &pISys);

    /* Проверка */
    if (result != 0 && pISys == 0) {
        return result;
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void**) &pIBus);

    /* Получение идентификатора компонента для работы с памятью */
    result = pIBus->pVTbl->QueryInterface(pIBus, &IID_IEcoInterfaceBus1MemExt, (void**) &pIMemExt);
    if (result == 0 && pIMemExt != 0) {
        rcid = (UGUID*) pIMemExt->pVTbl->get_Manager(pIMemExt);
        pIMemExt->pVTbl->Release(pIMemExt);
    }

    /* Получение интерфейса распределителя памяти */
    pIBus->pVTbl->QueryComponent(pIBus, rcid, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Проверка */
    if (result != 0 && pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        pISys->pVTbl->Release(pISys);
        return result;
    }

    /* Выделение памяти для данных экземпляра */
    pCMe = (CEcoLab1*) pIMem->pVTbl->Alloc(pIMem, sizeof(CEcoLab1));

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = pISys;

    /* Сохранение указателя на интерфейс для работы с памятью */
    pCMe->m_pIMem = pIMem;

    /* Установка счетчика ссылок на компонент */
    pCMe->m_cRef = 1;

    /* Создание таблицы функций интерфейса IEcoLab1 */
    pCMe->m_pVTblIEcoLab1 = &g_x277FC00C35624096AFCFC125B94EEC90VTbl;

    pCMe->m_pVTblIEcoCalculatorX = &g_x9322111622484742AE0682819447843DVTblD;
    pCMe->m_pVTblIEcoCalculatorY = &g_xBD6414C29096423EA90C04D77AFD1CADVTblD;

    /* Инициализация данных */
    pCMe->m_Name = 0;

    /* Возврат указателя на интерфейс */
    *ppIEcoLab1 = (IEcoLab1*) pCMe;

    /* Освобождение */
    pIBus->pVTbl->Release(pIBus);

    return 0;
}

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
void ECOCALLMETHOD deleteCEcoLab1(/* in */ IEcoLab1* pIEcoLab1) {
    CEcoLab1* pCMe = (CEcoLab1*) pIEcoLab1;
    IEcoMemoryAllocator1* pIMem = 0;

    if (pIEcoLab1 != 0) {
        pIMem = pCMe->m_pIMem;
        /* Освобождение */
        if (pCMe->m_Name != 0) {
            pIMem->pVTbl->Free(pIMem, pCMe->m_Name);
        }
        if (pCMe->m_pISys != 0) {
            pCMe->m_pISys->pVTbl->Release(pCMe->m_pISys);
        }
        pIMem->pVTbl->Free(pIMem, pCMe);
        pIMem->pVTbl->Release(pIMem);
    }
}
