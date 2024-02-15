//#include "window.h"
#include "matrix.h"

// ============= Настройки Сохранения в память ================

#define MOD_LIST_OFFSET         64      // начальная точка записи адресов на режимы в ПЗУ
#define MOD_LIST_MAX_SIZE       128     // максимальное количество режимов, которое можно записать в ПЗУ
#define MOD_DATA_OFFSET         MOD_LIST_OFFSET + MOD_LIST_MAX_SIZE * 2     // 2 байта для хранения адреса на данные режима
#define MOD_DATA_SIZE           448    // размер буфера под данные режимов

// =================== Настройки Режимов ======================

#define STARTING_MOD            0