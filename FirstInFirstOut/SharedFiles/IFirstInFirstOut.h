/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   IFirstInFirstOut
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает интерфейс IFirstInFirstOut
 * </описание>
 *
 * <ссылка>
 *
 * </ссылка>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */

#ifndef __I_FIRSTINFIRSTOUT_H__
#define __I_FIRSTINFIRSTOUT_H__

#include "IEcoBase1.h"

/* IFirstInFirstOut IID = {1333DEA1-670E-4D91-882F-E06FA00718DF} */
#ifndef __IID_IFirstInFirstOut
static const UGUID IID_IFirstInFirstOut = {0x01, 0x10, 0x13, 0x33, 0xDE, 0xA1, 0x67, 0x0E, 0x4D, 0x91, 0x88, 0x2F, 0xE0, 0x6F, 0xA0, 0x07, 0x18, 0xDF};
#endif /* __IID_IFirstInFirstOut */

typedef struct IFirstInFirstOutVTbl {

    /* IEcoUnknown */
    int16_t(ECOCALLMETHOD* QueryInterface)(/* in */ struct IFirstInFirstOut* me, /* in */ const UGUID* riid, /* out */ void** ppv);
    uint32_t(ECOCALLMETHOD* AddRef)(/* in */ struct IFirstInFirstOut* me);
    uint32_t(ECOCALLMETHOD* Release)(/* in */ struct IFirstInFirstOut* me);

    /* IFirstInFirstOut */
    int16_t(ECOCALLMETHOD* Init)(/* in */ struct IFirstInFirstOut* me, uint32_t size);
    void(ECOCALLMETHOD* Print_Header)(/* in */ struct IFirstInFirstOut* me);
    void(ECOCALLMETHOD* Print_Report_Line)(/* in */ struct IFirstInFirstOut* me, uint32_t page);
    void(ECOCALLMETHOD* Print_Final_Line)(/* in */ struct IFirstInFirstOut* me);
    int16_t(ECOCALLMETHOD* Access_Page)(/* in */ struct IFirstInFirstOut* me, uint32_t page);

} IFirstInFirstOutVTbl, *IFirstInFirstOutVTblPtr;

interface IFirstInFirstOut {
    struct IFirstInFirstOutVTbl* pVTbl;
} IFirstInFirstOut;


#endif /* __I_FIRSTINFIRSTOUT_H__ */
