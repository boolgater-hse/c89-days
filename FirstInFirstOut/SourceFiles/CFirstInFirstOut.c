/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CFirstInFirstOut_09EF47D3
 * </сводка>
 *
 * <описание>
 *   Данный исходный код описывает реализацию интерфейсов CFirstInFirstOut_09EF47D3
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#include "CFirstInFirstOut.h"
#include "IEcoInterfaceBus1.h"
#include "IEcoInterfaceBus1MemExt.h"
#include "IEcoSystem1.h"

/*
 *
 * <сводка>
 *   Функция QueryInterface
 * </сводка>
 *
 * <описание>
 *   Функция QueryInterface для интерфейса IFirstInFirstOut
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CFirstInFirstOut_09EF47D3_QueryInterface(/* in */ struct IFirstInFirstOut* me, /* in */ const UGUID* riid, /* out */ void** ppv) {
    CFirstInFirstOut_09EF47D3* pCMe = (CFirstInFirstOut_09EF47D3*) me;

    /* Проверка указателей */
    if (me == 0 || ppv == 0) {
        return -1;
    }

    /* Проверка и получение запрошенного интерфейса */
    if (IsEqualUGUID(riid, &IID_IFirstInFirstOut)) {
        *ppv = &pCMe->m_pVTblIFirstInFirstOut;
        pCMe->m_pVTblIFirstInFirstOut->AddRef((IFirstInFirstOut*) pCMe);
    } else if (IsEqualUGUID(riid, &IID_IEcoUnknown)) {
        *ppv = &pCMe->m_pVTblIFirstInFirstOut;
        pCMe->m_pVTblIFirstInFirstOut->AddRef((IFirstInFirstOut*) pCMe);
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
 *   Функция AddRef для интерфейса IFirstInFirstOut
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CFirstInFirstOut_09EF47D3_AddRef(/* in */ struct IFirstInFirstOut* me) {
    CFirstInFirstOut_09EF47D3* pCMe = (CFirstInFirstOut_09EF47D3*) me;

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
 *   Функция Release для интерфейса IFirstInFirstOut
 * </описание>
 *
 */
uint32_t ECOCALLMETHOD CFirstInFirstOut_09EF47D3_Release(/* in */ struct IFirstInFirstOut* me) {
    CFirstInFirstOut_09EF47D3* pCMe = (CFirstInFirstOut_09EF47D3*) me;

    /* Проверка указателя */
    if (me == 0) {
        return -1;
    }

    /* Уменьшение счетчика ссылок на компонент */
    --pCMe->m_cRef;

    /* В случае обнуления счетчика, освобождение данных экземпляра */
    if (pCMe->m_cRef == 0) {
        deleteCFirstInFirstOut_09EF47D3((IFirstInFirstOut*) pCMe);
        return 0;
    }
    return pCMe->m_cRef;
}

/*
 *
 * <сводка>
 *   Функция Init
 * </сводка>
 *
 * <описание>
 *   Инициализация FIFO алгоритма
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CFirstInFirstOut_09EF47D3_Init(/* in */ struct IFirstInFirstOut* me, uint32_t size) {
    CFirstInFirstOut_09EF47D3* pCMe = (CFirstInFirstOut_09EF47D3*) me;

    /* Проверка указателей */
    if (me == 0) {
        return -1;
    }

    pCMe->m_Pages = (Page*) pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, sizeof(Page));
    pCMe->m_Pages->next = 0;
    pCMe->m_PagesCounter = 0;
    pCMe->m_Size = size;

    pCMe->m_Time = 0;
    pCMe->m_LastReference = 0;
    pCMe->m_IsHit = 0;
    pCMe->m_Victim = 0;
    pCMe->m_Hits = 0;
    pCMe->m_Faults = 0;

    return 0;
}

/*
 *
 * <сводка>
 *   Функция Print_Header
 * </сводка>
 *
 * <описание>
 *   Распечатать названия столбцов для таблицы отчета
 * </описание>
 *
 */
void ECOCALLMETHOD CFirstInFirstOut_09EF47D3_Print_Header(/* in */ struct IFirstInFirstOut* me) {
    CFirstInFirstOut_09EF47D3* pCMe = (CFirstInFirstOut_09EF47D3*) me;
    uint32_t i;

    /* Проверка указателей */
    if (me == 0 || pCMe->m_Pages == 0) {
        return;
    }

    printf("%3s %3s ", "t", "ref");
    for (i = 0; i < pCMe->m_Size; ++i) {
        printf("%3s ", "f");
    }
    printf("%3s %s\n", "hit", "victim");
}

/*
 *
 * <сводка>
 *   Функция Print_Report_Line
 * </сводка>
 *
 * <описание>
 *   Распечатать отчет последнего доступа к странице
 * </описание>
 *
 */
void ECOCALLMETHOD CFirstInFirstOut_09EF47D3_Print_Report_Line(/* in */ struct IFirstInFirstOut* me, uint32_t page) {
    CFirstInFirstOut_09EF47D3* pCMe = (CFirstInFirstOut_09EF47D3*) me;
    uint32_t i;
    Page* current;

    /* Проверка указателей */
    if (me == 0 || pCMe->m_Pages == 0) {
        return;
    }

    current = pCMe->m_Pages;

    if (pCMe->m_Time == 0) {
        printf("%3u\n", pCMe->m_Time);
    } else {
        printf("%3u ", pCMe->m_Time);
        printf("%3u ", pCMe->m_LastReference);
        while (current != 0) {
            if (page == current->page) {
                printf("%s", (pCMe->m_IsHit == 0) ? "\x1b[30;41m" : "\x1b[30;42m");
                printf("%3u", current->page);
                printf("%s ", "\x1b[0m");
            } else {
                printf("%3u ", current->page);
            }

            current = current->next;
        }
        if (pCMe->m_PagesCounter < pCMe->m_Size) {
            for (i = 0; i < pCMe->m_Size - pCMe->m_PagesCounter; ++i) {
                printf("    ");
            }
        }
        printf("%s", (pCMe->m_IsHit == 0) ? "\x1b[30;41m" : "\x1b[30;42m");
        printf("%3s", (pCMe->m_IsHit == 0) ? "NO" : "YES");
        printf("%s ", "\x1b[0m");
        if (pCMe->m_Victim != 0) {
            printf("%3u", pCMe->m_Victim);
        }
        printf("\n");
    }
}

/*
 *
 * <сводка>
 *   Функция Print_Final_Lin
 * </сводка>
 *
 * <описание>
 *   Распечатать общую статистику по замещению страниц
 * </описание>
 *
 */
void ECOCALLMETHOD CFirstInFirstOut_09EF47D3_Print_Final_Line(/* in */ struct IFirstInFirstOut* me) {
    CFirstInFirstOut_09EF47D3* pCMe = (CFirstInFirstOut_09EF47D3*) me;

    /* Проверка указателей */
    if (me == 0 || pCMe->m_Pages == 0) {
        return;
    }

    printf("Total references: %u\n", pCMe->m_Faults + pCMe->m_Hits);
    printf("Hits: %u\n", pCMe->m_Hits);
    printf("Faults: %u\n", pCMe->m_Faults);
    printf("Hit rate: %.3f%%\n", (double_t) pCMe->m_Hits * 100 / (pCMe->m_Faults + pCMe->m_Hits));
    printf("Faults rate: %.3f%%", (double_t) pCMe->m_Faults * 100 / (pCMe->m_Faults + pCMe->m_Hits));
}

int16_t find_page(/* in */ struct IFirstInFirstOut* me, uint32_t page){
    CFirstInFirstOut_09EF47D3* pCMe = (CFirstInFirstOut_09EF47D3*) me;

    Page* current = pCMe->m_Pages;

    while (current != 0) {
        if (current->page == page) {
            return 1;
        }

        current = current->next;
    }

    return 0;
}

/*
 *
 * <сводка>
 *   Функция Access_Page
 * </сводка>
 *
 * <описание>
 *   Получить доступ к странице
 * </описание>
 *
 */
int16_t ECOCALLMETHOD CFirstInFirstOut_09EF47D3_Access_Page(/* in */ struct IFirstInFirstOut* me, uint32_t page) {
    CFirstInFirstOut_09EF47D3* pCMe = (CFirstInFirstOut_09EF47D3*) me;
    Page* new_page;
    Page* temp;

    /* Проверка указателей */
    if (me == 0 || pCMe->m_Pages == 0 || page == 0) {
        return -1;
    }

    if (find_page(me, page) == 1) {
        pCMe->m_Time++;
        pCMe->m_LastReference = page;
        pCMe->m_IsHit = 1;
        pCMe->m_Victim = 0;
        pCMe->m_Hits++;

        return 0;
    }

    if (pCMe->m_PagesCounter == 0) {
        pCMe->m_Pages->page = page;

        pCMe->m_Time++;
        pCMe->m_LastReference = page;
        pCMe->m_IsHit = 0;
        pCMe->m_Victim = 0;
        pCMe->m_Faults++;

        pCMe->m_PagesCounter++;

        return 1;
    } else if (pCMe->m_PagesCounter < pCMe->m_Size) {
        new_page = (Page*) pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, sizeof(Page));

        new_page->page = page;
        new_page->next = pCMe->m_Pages;
        pCMe->m_Pages = new_page;

        pCMe->m_Time++;
        pCMe->m_LastReference = page;
        pCMe->m_IsHit = 0;
        pCMe->m_Victim = 0;
        pCMe->m_Faults++;

        pCMe->m_PagesCounter++;

        return 1;
    } else {
        new_page = (Page*) pCMe->m_pIMem->pVTbl->Alloc(pCMe->m_pIMem, sizeof(Page));

        new_page->page = page;
        new_page->next = pCMe->m_Pages;

        pCMe->m_Pages = new_page;

        temp = pCMe->m_Pages;
        while (temp->next->next != 0) {
            temp = temp->next;
        }

        pCMe->m_Time++;
        pCMe->m_LastReference = page;
        pCMe->m_IsHit = 0;
        pCMe->m_Victim = temp->next->page;
        pCMe->m_Faults++;

        pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, temp->next);
        temp->next = 0;

        return 1;
    }

    return 0;
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
int16_t ECOCALLMETHOD initCFirstInFirstOut_09EF47D3(/*in*/ struct IFirstInFirstOut* me, /* in */ struct IEcoUnknown* pIUnkSystem) {
    CFirstInFirstOut_09EF47D3* pCMe = (CFirstInFirstOut_09EF47D3*) me;
    IEcoInterfaceBus1* pIBus = 0;
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

    /* Освобождение */
    pIBus->pVTbl->Release(pIBus);

    return result;
}

/* Create Virtual Table IFirstInFirstOut */
IFirstInFirstOutVTbl g_x1333DEA1670E4D91882FE06FA00718DFVTbl_09EF47D3 = {
        CFirstInFirstOut_09EF47D3_QueryInterface,
        CFirstInFirstOut_09EF47D3_AddRef,
        CFirstInFirstOut_09EF47D3_Release,
        CFirstInFirstOut_09EF47D3_Init,
        CFirstInFirstOut_09EF47D3_Print_Header,
        CFirstInFirstOut_09EF47D3_Print_Report_Line,
        CFirstInFirstOut_09EF47D3_Print_Final_Line,
        CFirstInFirstOut_09EF47D3_Access_Page
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
int16_t ECOCALLMETHOD createCFirstInFirstOut_09EF47D3(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IFirstInFirstOut** ppIFirstInFirstOut) {
    int16_t result = -1;
    IEcoSystem1* pISys = 0;
    IEcoInterfaceBus1* pIBus = 0;
    IEcoInterfaceBus1MemExt* pIMemExt = 0;
    IEcoMemoryAllocator1* pIMem = 0;
    CFirstInFirstOut_09EF47D3* pCMe = 0;
    UGUID* rcid = (UGUID*) &CID_EcoMemoryManager1;

    /* Проверка указателей */
    if (ppIFirstInFirstOut == 0 || pIUnkSystem == 0) {
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
    pCMe = (CFirstInFirstOut_09EF47D3*) pIMem->pVTbl->Alloc(pIMem, sizeof(CFirstInFirstOut_09EF47D3));

    /* Сохранение указателя на системный интерфейс */
    pCMe->m_pISys = pISys;

    /* Сохранение указателя на интерфейс для работы с памятью */
    pCMe->m_pIMem = pIMem;

    /* Установка счетчика ссылок на компонент */
    pCMe->m_cRef = 1;

    /* Создание таблицы функций интерфейса IFirstInFirstOut */
    pCMe->m_pVTblIFirstInFirstOut = &g_x1333DEA1670E4D91882FE06FA00718DFVTbl_09EF47D3;

    /* Инициализация данных */
    pCMe->m_Name = 0;

    /* Возврат указателя на интерфейс */
    *ppIFirstInFirstOut = (IFirstInFirstOut*) pCMe;

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
void ECOCALLMETHOD deleteCFirstInFirstOut_09EF47D3(/* in */ IFirstInFirstOut* pIFirstInFirstOut) {
    CFirstInFirstOut_09EF47D3* pCMe = (CFirstInFirstOut_09EF47D3*) pIFirstInFirstOut;
    IEcoMemoryAllocator1* pIMem = 0;
    Page* current = pCMe->m_Pages;
    Page* temp;

    if (pIFirstInFirstOut != 0) {
        pIMem = pCMe->m_pIMem;
        /* Освобождение */
        if (pCMe->m_Pages != 0) {
            while (current != 0) {
                temp = current;
                current = current->next;
                pCMe->m_pIMem->pVTbl->Free(pCMe->m_pIMem, temp);
            }
        }
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
