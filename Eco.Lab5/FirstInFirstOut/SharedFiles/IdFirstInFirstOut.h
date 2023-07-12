/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   IdFirstInFirstOut
 * </сводка>
 *
 * <описание>
 *   Данный заголовок описывает интерфейс IdFirstInFirstOut
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

#ifndef __ID_FIRSTINFIRSTOUT_H__
#define __ID_FIRSTINFIRSTOUT_H__

#include "IEcoBase1.h"
#include "IFirstInFirstOut.h"

/* FirstInFirstOut CID = {D8099099-66BA-4E10-9B52-C6F209EF47D3} */
#ifndef __CID_FirstInFirstOut
static const UGUID CID_FirstInFirstOut = {0x01, 0x10, 0xD8, 0x09, 0x90, 0x99, 0x66, 0xBA, 0x4E, 0x10, 0x9B, 0x52, 0xC6, 0xF2, 0x09, 0xEF, 0x47, 0xD3};
#endif /* __CID_FirstInFirstOut */

/* Фабрика компонента для динамической и статической компановки */
#ifdef ECO_DLL
ECO_EXPORT IEcoComponentFactory* ECOCALLMETHOD GetIEcoComponentFactoryPtr();
#elif ECO_LIB
extern IEcoComponentFactory* GetIEcoComponentFactoryPtr_D809909966BA4E109B52C6F209EF47D3;
#endif

#endif /* __ID_FIRSTINFIRSTOUT_H__ */
