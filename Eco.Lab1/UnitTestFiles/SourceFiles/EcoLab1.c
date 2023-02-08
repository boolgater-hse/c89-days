/*
 * <кодировка символов>
 *   Cyrillic (UTF-8 with signature) - Codepage 65001
 * </кодировка символов>
 *
 * <сводка>
 *   EcoLab1
 * </сводка>
 *
 * <описание>
 *   Данный исходный файл является точкой входа
 * </описание>
 *
 * <автор>
 *   Copyright (c) 2018 Vladimir Bashev. All rights reserved.
 * </автор>
 *
 */


 /* Eco OS */
#include "IEcoSystem1.h"
#include "IdEcoMemoryManager1.h"
#include "IdEcoInterfaceBus1.h"
#include "IdEcoFileSystemManagement1.h"
#include "IdEcoLab1.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 *
 * <сводка>
 *   Get_Random_In_Range_int32 function
 * </сводка>
 *
 * <описание>
 *   Returns random number of type int32_t in range [lower, upper].
 * </описание>
 *
 */
int32_t Get_Random_In_Range_int32(/* in */ int32_t lower, /* in */ int32_t upper) {
    return (rand() % (upper + 1 - lower) + lower);
}

/*
 *
 * <сводка>
 *   Get_Random_In_Range_double function
 * </сводка>
 *
 * <описание>
 *   Returns random number of type double_t in range [lower, upper].
 * </описание>
 *
 */
double_t Get_Random_In_Range_double(/* in */ double_t lower, /* in */ double_t upper) {
    double_t range = (upper - lower);
    double_t div = RAND_MAX / range;
    return lower + (rand() / div);
}

/*
 *
 * <сводка>
 *   Check_Is_Increasing function
 * </сводка>
 *
 * <описание>
 *   Returns 0 if sequence arr satisfies comparator function fnCompare.
 *   Returns -1 if sequence arr not satisfies.
 * </описание>
 *
 */
int16_t ECOCDECLMETHOD Check_Is_Increasing(/* in */ void* arr,
                                          /* in */ size_t num,
                                          /* in */ size_t size,
                                          /* in */ int (ECOCDECLMETHOD* fnCompare)(const void*, const void*)) {
    size_t i = 0;
    for (i = 0; i < num - 1; ++i) {
        if ((*fnCompare) ((char_t*) arr + (size * i), (char_t*) arr + (size * (i + 1))) > 0) {
            return -1;
        }
    }

    return 0;
}

/*
 *
 * <сводка>
 *   comp_int32 function
 * </сводка>
 *
 * <описание>
 *   Comparator for int32_t
 * </описание>
 *
 */
int16_t ECOCDECLMETHOD comp_int32(const int32_t* i, const int32_t* j) {
    if (*i > *j) {
        return 1;
    } else if (*i < *j) {
        return -1;
    }
    return 0;
}

/*
 *
 * <сводка>
 *   comp_double function
 * </сводка>
 *
 * <описание>
 *   Comparator for double_t
 * </описание>
 *
 */
int16_t ECOCDECLMETHOD comp_double(const double_t* i, const double_t* j) {
    double_t EPS = DBL_EPSILON;
    double_t diff = *i - *j;
    if (diff < -EPS) {
        return -1;
    } else if (diff > EPS) {
        return 1;
    }
    return 0;
}

/*
 *
 * <сводка>
 *   comp_char_p function
 * </сводка>
 *
 * <описание>
 *   Comparator for char_t**
 * </описание>
 *
 */
int16_t ECOCDECLMETHOD comp_char_p(const char_t** a, const char_t** b) {
    int32_t i = 0;
    char_t* _a = *a;
    char_t* _b = *b;
    for (i = 0; _a[i] != '\0' || _b[i] != '\0'; ++i) {
        if (_a[i] > _b[i]) {
            return 1;
        } else if (_b[i] > _a[i]) {
            return -1;
        }
    }
    if (_a[i] != '\0') {
        return 1;
    } else if (_b[i] != '\0') {
        return -1;
    } else {
        return 0;
    }
}

#define PRINTED_WORD_SIZE 4
#define WORD_SIZE 6

/* Real words sizes are plus one ahead of original sizes because of \0 */
#define REAL_PRINTED_WORD_SIZE (PRINTED_WORD_SIZE + 1)
#define REAL_WORD_SIZE (WORD_SIZE + 1)

#define PRINTED_TEST_SIZE 10
#define TEST_SIZE 250000
#define BIG_TEST_SIZE 500000

/* Limits for a random number */
#define LOWER -1000
#define UPPER 1000

/*
 *
 * <сводка>
 *   Функция EcoMain
 * </сводка>
 *
 * <описание>
 *   Функция EcoMain - точка входа
 * </описание>
 *
 */
int16_t EcoMain(IEcoUnknown* pIUnk) {
    int16_t result = -1;
    /* Указатель на системный интерфейс */
    IEcoSystem1* pISys = 0;
    /* Указатель на интерфейс работы с системной интерфейсной шиной */
    IEcoInterfaceBus1* pIBus = 0;
    /* Указатель на интерфейс работы с памятью */
    IEcoMemoryAllocator1* pIMem = 0;
    /* Указатель на тестируемый интерфейс */
    IEcoLab1* pIEcoLab1 = 0;

    int32_t* p_i32Arr = 0;
    double_t* p_dbArr = 0;
    char_t** p_chArr = 0;

    int (ECOCDECLMETHOD* comp)(const void*, const void*);

    uint32_t i = 0;
    uint32_t j = 0;

    clock_t begin;
    clock_t end;

    srand(time(0));

    /* Проверка и создание системного интрефейса */
    if (pISys == 0) {
        result = pIUnk->pVTbl->QueryInterface(pIUnk, &GID_IEcoSystem1, (void**) &pISys);
        if (result != 0 && pISys == 0) {
            /* Освобождение системного интерфейса в случае ошибки */
            goto Release;
        }
    }

    /* Получение интерфейса для работы с интерфейсной шиной */
    result = pISys->pVTbl->QueryInterface(pISys, &IID_IEcoInterfaceBus1, (void**) &pIBus);
    if (result != 0 || pIBus == 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
#ifdef ECO_LIB
    /* Регистрация статического компонента для работы со списком */
    result = pIBus->pVTbl->RegisterComponent(pIBus, &CID_EcoLab1, (IEcoUnknown*) GetIEcoComponentFactoryPtr_1F5DF16EE1BF43B999A434ED38FE8F3A);
    if (result != 0) {
        /* Освобождение в случае ошибки */
        goto Release;
    }
#endif
    /* Получение интерфейса управления памятью */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoMemoryManager1, 0, &IID_IEcoMemoryAllocator1, (void**) &pIMem);

    /* Проверка */
    if (result != 0 || pIMem == 0) {
        /* Освобождение системного интерфейса в случае ошибки */
        goto Release;
    }

    /* Получение тестируемого интерфейса */
    result = pIBus->pVTbl->QueryComponent(pIBus, &CID_EcoLab1, 0, &IID_IEcoLab1, (void**) &pIEcoLab1);
    if (result != 0 || pIEcoLab1 == 0) {
        /* Освобождение интерфейсов в случае ошибки */
        goto Release;
    }

    comp = comp_int32;
    printf("/// int32_t test ///\n");
    {
        {
            printf("Test #1: ");
            p_i32Arr = (int32_t*) pIMem->pVTbl->Alloc(pIMem, sizeof(int32_t) * PRINTED_TEST_SIZE);

            for (i = 0; i < PRINTED_TEST_SIZE; ++i) {
                ((int32_t*) p_i32Arr)[i] = Get_Random_In_Range_int32(LOWER, UPPER);
            }

            for (i = 0; i < PRINTED_TEST_SIZE; ++i) {
                printf("%i ", ((int32_t*) p_i32Arr)[i]);
            }
            printf("\n");
            printf("Sorting started\n");
            result = pIEcoLab1->pVTbl->TreeSort(pIEcoLab1, p_i32Arr, PRINTED_TEST_SIZE, sizeof(int32_t), comp);
            printf("Result #1:\n");
            for (i = 0; i < PRINTED_TEST_SIZE; ++i) {
                printf("%i ", ((int32_t*) p_i32Arr)[i]);
            }
            printf("\n");
            printf("Increasing?: %s", (Check_Is_Increasing(p_i32Arr, PRINTED_TEST_SIZE, sizeof(int32_t), comp) == 0) ? "Yes" : "No");

            pIMem->pVTbl->Free(pIMem, p_i32Arr);

            printf("\n\n");
        }

        {
            printf("Test #2 (One element): ");
            p_i32Arr = (int32_t*) pIMem->pVTbl->Alloc(pIMem, sizeof(int32_t) * 1);

            for (i = 0; i < 1; ++i) {
                ((int32_t*) p_i32Arr)[i] = Get_Random_In_Range_int32(LOWER, UPPER);
            }

            for (i = 0; i < 1; ++i) {
                printf("%i ", ((int32_t*) p_i32Arr)[i]);
            }
            printf("\n");
            printf("Sorting started\n");
            result = pIEcoLab1->pVTbl->TreeSort(pIEcoLab1, p_i32Arr, 1, sizeof(int32_t), comp);
            printf("Result #2:\n");
            for (i = 0; i < 1; ++i) {
                printf("%i ", ((int32_t*) p_i32Arr)[i]);
            }
            printf("\n");
            printf("Increasing?: %s", (Check_Is_Increasing(p_i32Arr, 1, sizeof(int32_t), comp) == 0) ? "Yes" : "No");

            pIMem->pVTbl->Free(pIMem, p_i32Arr);

            printf("\n\n");
        }

        {
            printf("Test #3 (%d elements/Random): Allocating & filling", TEST_SIZE);
            p_i32Arr = (int32_t*) pIMem->pVTbl->Alloc(pIMem, sizeof(int32_t) * TEST_SIZE);

            for (i = 0; i < TEST_SIZE; ++i) {
                ((int32_t*) p_i32Arr)[i] = Get_Random_In_Range_int32(LOWER, UPPER);
            }

            printf("\n");

            printf("Sorting started\n");
            begin = clock();
            result = pIEcoLab1->pVTbl->TreeSort(pIEcoLab1, p_i32Arr, TEST_SIZE, sizeof(int32_t), comp);
            end = clock();
            printf("Test took %f ms\n", (double) (end - begin) * 1000.0 / CLOCKS_PER_SEC);

            printf("Result #3:\n");
            printf("Increasing?: %s", (Check_Is_Increasing(p_i32Arr, TEST_SIZE, sizeof(int32_t), comp) == 0) ? "Yes" : "No");

            pIMem->pVTbl->Free(pIMem, p_i32Arr);

            printf("\n\n");
        }

        {
            printf("Test #4 (%d elements/Random): Allocating & filling", BIG_TEST_SIZE);
            p_i32Arr = (int32_t*) pIMem->pVTbl->Alloc(pIMem, sizeof(int32_t) * BIG_TEST_SIZE);

            for (i = 0; i < BIG_TEST_SIZE; ++i) {
                ((int32_t*) p_i32Arr)[i] = Get_Random_In_Range_int32(-100, 100);
            }

            printf("\n");

            printf("Sorting started\n");
            begin = clock();
            result = pIEcoLab1->pVTbl->TreeSort(pIEcoLab1, p_i32Arr, BIG_TEST_SIZE, sizeof(int32_t), comp);
            end = clock();
            printf("Test took %f ms\n", (double) (end - begin) * 1000.0 / CLOCKS_PER_SEC);

            printf("Result #4:\n");
            printf("Increasing?: %s", (Check_Is_Increasing(p_i32Arr, BIG_TEST_SIZE, sizeof(int32_t), comp) == 0) ? "Yes" : "No");

            pIMem->pVTbl->Free(pIMem, p_i32Arr);

            printf("\n\n");
        }

        {
            printf("Test #5 (%d elements/Increasing): Allocating & filling", TEST_SIZE);
            p_i32Arr = (int32_t*) pIMem->pVTbl->Alloc(pIMem, sizeof(int32_t) * TEST_SIZE);

            for (i = 0; i < TEST_SIZE; ++i) {
                ((int32_t*) p_i32Arr)[i] = i + 1;
            }

            printf("\n");

            printf("Sorting started\n");
            begin = clock();
            result = pIEcoLab1->pVTbl->TreeSort(pIEcoLab1, p_i32Arr, TEST_SIZE, sizeof(int32_t), comp);
            end = clock();
            printf("Test took %f ms\n", (double) (end - begin) * 1000.0 / CLOCKS_PER_SEC);

            printf("Result #5:\n");
            printf("Increasing?: %s", (Check_Is_Increasing(p_i32Arr, TEST_SIZE, sizeof(int32_t), comp) == 0) ? "Yes" : "No");

            pIMem->pVTbl->Free(pIMem, p_i32Arr);

            printf("\n\n");
        }

        {
            printf("Test #6 (%d elements/Increasing): Allocating & filling", BIG_TEST_SIZE);
            p_i32Arr = (int32_t*) pIMem->pVTbl->Alloc(pIMem, sizeof(int32_t) * BIG_TEST_SIZE);

            for (i = 0; i < BIG_TEST_SIZE; ++i) {
                ((int32_t*) p_i32Arr)[i] = i + 1;
            }

            printf("\n");

            printf("Sorting started\n");
            begin = clock();
            result = pIEcoLab1->pVTbl->TreeSort(pIEcoLab1, p_i32Arr, BIG_TEST_SIZE, sizeof(int32_t), comp);
            end = clock();
            printf("Test took %f ms\n", (double) (end - begin) * 1000.0 / CLOCKS_PER_SEC);

            printf("Result #6:\n");
            printf("Increasing?: %s", (Check_Is_Increasing(p_i32Arr, BIG_TEST_SIZE, sizeof(int32_t), comp) == 0) ? "Yes" : "No");

            pIMem->pVTbl->Free(pIMem, p_i32Arr);

            printf("\n\n");
        }
    }
    printf("\n\n");

    comp = comp_double;
    printf("/// double_t tests ///\n");
    {
        {
            printf("Test #1: ");
            p_dbArr = (double_t*) pIMem->pVTbl->Alloc(pIMem, sizeof(double_t) * PRINTED_TEST_SIZE);

            for (i = 0; i < PRINTED_TEST_SIZE; ++i) {
                ((double_t*) p_dbArr)[i] = Get_Random_In_Range_double((double_t) LOWER, (double_t) UPPER);
            }

            for (i = 0; i < PRINTED_TEST_SIZE; ++i) {
                printf("%f ", ((double_t*) p_dbArr)[i]);
            }
            printf("\n");
            printf("Sorting started\n");
            result = pIEcoLab1->pVTbl->TreeSort(pIEcoLab1, p_dbArr, PRINTED_TEST_SIZE, sizeof(double_t), comp);
            printf("Result #1:\n");
            for (i = 0; i < PRINTED_TEST_SIZE; ++i) {
                printf("%f ", ((double_t*) p_dbArr)[i]);
            }
            printf("\n");
            printf("Increasing?: %s", (Check_Is_Increasing(p_dbArr, PRINTED_TEST_SIZE, sizeof(double_t), comp) == 0) ? "Yes" : "No");

            pIMem->pVTbl->Free(pIMem, p_dbArr);

            printf("\n\n");
        }

        {
            printf("Test #2 (One element): ");
            p_dbArr = (double_t*) pIMem->pVTbl->Alloc(pIMem, sizeof(double_t) * 1);

            for (i = 0; i < 1; ++i) {
                ((double_t*) p_dbArr)[i] = Get_Random_In_Range_int32((double_t) LOWER, (double_t) UPPER);
            }

            for (i = 0; i < 1; ++i) {
                printf("%5f ", ((double_t*) p_dbArr)[i]);
            }
            printf("\n");
            printf("Sorting started\n");
            result = pIEcoLab1->pVTbl->TreeSort(pIEcoLab1, p_dbArr, 1, sizeof(double_t), comp);
            printf("Result #2:\n");
            for (i = 0; i < 1; ++i) {
                printf("%5f ", ((double_t*) p_dbArr)[i]);
            }
            printf("\n");
            printf("Increasing?: %s", (Check_Is_Increasing(p_dbArr, 1, sizeof(double_t), comp) == 0) ? "Yes" : "No");

            pIMem->pVTbl->Free(pIMem, p_dbArr);

            printf("\n\n");
        }

        {
            printf("Test #3 (%d elements/Random): Allocating & filling", TEST_SIZE);
            p_dbArr = (double_t*) pIMem->pVTbl->Alloc(pIMem, sizeof(double_t) * TEST_SIZE);

            for (i = 0; i < TEST_SIZE; ++i) {
                ((double_t*) p_dbArr)[i] = Get_Random_In_Range_int32((double_t) LOWER, (double_t) UPPER);
            }

            printf("\n");

            printf("Sorting started\n");
            begin = clock();
            result = pIEcoLab1->pVTbl->TreeSort(pIEcoLab1, p_dbArr, TEST_SIZE, sizeof(double_t), comp);
            end = clock();
            printf("Test took %f ms\n", (double) (end - begin) * 1000.0 / CLOCKS_PER_SEC);

            printf("Result #3:\n");
            printf("Increasing?: %s", (Check_Is_Increasing(p_dbArr, TEST_SIZE, sizeof(double_t), comp) == 0) ? "Yes" : "No");

            pIMem->pVTbl->Free(pIMem, p_dbArr);

            printf("\n\n");
        }

        {
            printf("Test #4 (%d elements/Random): Allocating & filling", BIG_TEST_SIZE);
            p_dbArr = (double_t*) pIMem->pVTbl->Alloc(pIMem, sizeof(double_t) * BIG_TEST_SIZE);

            for (i = 0; i < BIG_TEST_SIZE; ++i) {
                ((double_t*) p_dbArr)[i] = Get_Random_In_Range_double((double_t) LOWER, (double_t) UPPER);
            }

            printf("\n");

            printf("Sorting started\n");
            begin = clock();
            result = pIEcoLab1->pVTbl->TreeSort(pIEcoLab1, p_dbArr, BIG_TEST_SIZE, sizeof(double_t), comp);
            end = clock();
            printf("Test took %f ms\n", (double) (end - begin) * 1000.0 / CLOCKS_PER_SEC);

            printf("Result #4:\n");
            printf("Increasing?: %s", (Check_Is_Increasing(p_dbArr, BIG_TEST_SIZE, sizeof(double_t), comp) == 0) ? "Yes" : "No");

            pIMem->pVTbl->Free(pIMem, p_dbArr);

            printf("\n\n");
        }

        {
            printf("Test #5 (%d elements/Increasing): Allocating & filling", TEST_SIZE);
            p_dbArr = (double_t*) pIMem->pVTbl->Alloc(pIMem, sizeof(double_t) * TEST_SIZE);

            for (i = 0; i < TEST_SIZE; ++i) {
                ((double_t*) p_dbArr)[i] = (double) i + 1;
            }

            printf("\n");

            printf("Sorting started\n");
            begin = clock();
            result = pIEcoLab1->pVTbl->TreeSort(pIEcoLab1, p_dbArr, TEST_SIZE, sizeof(double_t), comp);
            end = clock();
            printf("Test took %f ms\n", (double) (end - begin) * 1000.0 / CLOCKS_PER_SEC);

            printf("Result #5:\n");
            printf("Increasing?: %s", (Check_Is_Increasing(p_dbArr, TEST_SIZE, sizeof(double_t), comp) == 0) ? "Yes" : "No");

            pIMem->pVTbl->Free(pIMem, p_dbArr);

            printf("\n\n");
        }

        {
            printf("Test #6 (%d elements/Increasing): Allocating & filling", BIG_TEST_SIZE);
            p_dbArr = (double_t*) pIMem->pVTbl->Alloc(pIMem, sizeof(double_t) * BIG_TEST_SIZE);

            for (i = 0; i < BIG_TEST_SIZE; ++i) {
                ((double_t*) p_dbArr)[i] = (double) i + 1;
            }

            printf("\n");

            printf("Sorting started\n");
            begin = clock();
            result = pIEcoLab1->pVTbl->TreeSort(pIEcoLab1, p_dbArr, BIG_TEST_SIZE, sizeof(double_t), comp);
            end = clock();
            printf("Test took %f ms\n", (double) (end - begin) * 1000.0 / CLOCKS_PER_SEC);

            printf("Result #6:\n");
            printf("Increasing?: %s", (Check_Is_Increasing(p_dbArr, BIG_TEST_SIZE, sizeof(double_t), comp) == 0) ? "Yes" : "No");

            pIMem->pVTbl->Free(pIMem, p_dbArr);

            printf("\n\n");
        }
    }
    printf("\n\n");

    comp = comp_char_p;
    printf("/// word array tests ///\n");
    {
        {
            printf("Test #1: ");
            p_chArr = (char_t**) pIMem->pVTbl->Alloc(pIMem, PRINTED_TEST_SIZE * sizeof(char_t*));

            for (i = 0; i < PRINTED_TEST_SIZE; ++i) {
                p_chArr[i] = (char_t*) pIMem->pVTbl->Alloc(pIMem, REAL_PRINTED_WORD_SIZE);
            }

            for (i = 0; i < PRINTED_TEST_SIZE; ++i) {
                for (j = 0; j < REAL_PRINTED_WORD_SIZE - 1; ++j) {
                    p_chArr[i][j] = 'a' + (rand() % 26);
                }
                p_chArr[i][j] = '\0';
            }

            for (i = 0; i < PRINTED_TEST_SIZE; ++i) {
                printf("%s ", p_chArr[i]);
            }
            printf("\n");
            printf("Sorting started\n");
            result = pIEcoLab1->pVTbl->TreeSort(pIEcoLab1, p_chArr, PRINTED_TEST_SIZE, sizeof(char_t**), comp);
            printf("Result #1:\n");
            for (i = 0; i < PRINTED_TEST_SIZE; ++i) {
                printf("%s ", p_chArr[i]);
            }
            printf("\n");
            printf("Increasing?: %s", (Check_Is_Increasing(p_chArr, PRINTED_TEST_SIZE, sizeof(char_t**), comp) == 0) ? "Yes" : "No");

            for (i = 0; i < PRINTED_TEST_SIZE; ++i) {
                char* ttt = p_chArr[i];
                pIMem->pVTbl->Free(pIMem, p_chArr[i]);
            }
            pIMem->pVTbl->Free(pIMem, p_chArr);

            printf("\n\n");
        }

        {
            printf("Test #2 (%d elements/Word size %d/Random): Allocating & filling: ", TEST_SIZE, WORD_SIZE);
            p_chArr = (char_t**) pIMem->pVTbl->Alloc(pIMem, TEST_SIZE * sizeof(char_t*));

            for (i = 0; i < TEST_SIZE; ++i) {
                p_chArr[i] = (char_t*) pIMem->pVTbl->Alloc(pIMem, REAL_WORD_SIZE);
            }

            for (i = 0; i < TEST_SIZE; ++i) {
                for (j = 0; j < REAL_WORD_SIZE - 1; ++j) {
                    p_chArr[i][j] = 'a' + (rand() % 26);
                }
                p_chArr[i][j] = '\0';
            }

            printf("\n");

            printf("Sorting started\n");
            begin = clock();
            result = pIEcoLab1->pVTbl->TreeSort(pIEcoLab1, p_chArr, TEST_SIZE, sizeof(char_t**), comp);
            end = clock();
            printf("Test took %f ms\n", (double) (end - begin) * 1000.0 / CLOCKS_PER_SEC);

            printf("Result #2:\n");
            printf("Increasing?: %s", (Check_Is_Increasing(p_chArr, TEST_SIZE, sizeof(char_t**), comp) == 0) ? "Yes" : "No");

            for (i = 0; i < TEST_SIZE; ++i) {
                pIMem->pVTbl->Free(pIMem, p_chArr[i]);
            }
            pIMem->pVTbl->Free(pIMem, p_chArr);

            printf("\n\n");
        }
    }
    printf("\n\n");

Release:

    /* Освобождение интерфейса для работы с интерфейсной шиной */
    if (pIBus != 0) {
        pIBus->pVTbl->Release(pIBus);
    }

    /* Освобождение интерфейса работы с памятью */
    if (pIMem != 0) {
        pIMem->pVTbl->Release(pIMem);
    }

    /* Освобождение тестируемого интерфейса */
    if (pIEcoLab1 != 0) {
        pIEcoLab1->pVTbl->Release(pIEcoLab1);
    }

    /* Освобождение системного интерфейса */
    if (pISys != 0) {
        pISys->pVTbl->Release(pISys);
    }

    return result;
}
