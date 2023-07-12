/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   CFirstInFirstOut
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает реализацию компонента CFirstInFirstOut
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#ifndef __C_FIRSTINFIRSTOUT_H__
#define __C_FIRSTINFIRSTOUT_H__

#include "IEcoSystem1.h"
#include "IFirstInFirstOut.h"
#include "IdEcoMemoryManager1.h"

typedef struct Page {
    uint32_t page;
    struct Page* next;
} Page;

typedef struct CFirstInFirstOut_09EF47D3 {

    /* Таблица функций интерфейса IFirstInFirstOut */
    IFirstInFirstOutVTbl* m_pVTblIFirstInFirstOut;

    /* Счетчик ссылок */
    uint32_t m_cRef;

    /* Интерфейс для работы с памятью */
    IEcoMemoryAllocator1* m_pIMem;

    /* Системный интерфейс */
    IEcoSystem1* m_pISys;

    /* Данные экземпляра */
    char_t* m_Name;

    /* FIFO переменные */
    Page* m_Pages;
    uint32_t m_PagesCounter;
    uint32_t m_Size;

    /* Переменные для статистики */
    uint32_t m_Time;
    uint32_t m_LastReference;
    uint32_t m_IsHit;
    uint32_t m_Victim;
    uint32_t m_Hits;
    uint32_t m_Faults;

} CFirstInFirstOut_09EF47D3, *CFirstInFirstOut_09EF47D3Ptr;

/* Инициализация экземпляра */
int16_t ECOCALLMETHOD initCFirstInFirstOut_09EF47D3(/*in*/ struct IFirstInFirstOut* me, /* in */ IEcoUnknown* pIUnkSystem);
/* Создание экземпляра */
int16_t ECOCALLMETHOD createCFirstInFirstOut_09EF47D3(/* in */ IEcoUnknown* pIUnkSystem, /* in */ IEcoUnknown* pIUnkOuter, /* out */ IFirstInFirstOut** ppIFirstInFirstOut);
/* Удаление */
void ECOCALLMETHOD deleteCFirstInFirstOut_09EF47D3(/* in */ IFirstInFirstOut* pIFirstInFirstOut);

#endif /* __C_FIRSTINFIRSTOUT_H__ */
