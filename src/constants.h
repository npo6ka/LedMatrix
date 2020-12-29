#pragma once

#define DEBUG                   (true)

// ===================== Настройки Матрицы =====================

#define DATA_PIN                (2U)                        // номер порта к которому подключены светодиоды

#define WIDTH                   (20)                        // ширина матрицы
#define HEIGHT                  (10)                        // высота матрицы
#define LEDS_CNT                (WIDTH * HEIGHT)            // общее число светодиодов в матрице
#define CURRENT_LIMIT           (1000U)                     // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит
#define COLOR_ORDER             (GRB)                       // порядок цветов на ленте. Если цвет отображается некорректно - меняйте. Начать можно с RGB

#define MATRIX_TYPE             (0U)                        // тип матрицы: 0 - зигзаг, 1 - параллельная
#define CONNECTION_ANGLE        (2U)                        // угол подключения матрицы (0-3): 0 - левый нижний, 1 - левый верхний, 2 - правый верхний, 3 - правый нижний
                                                            // 1---------2
                                                            // -----------
                                                            // -----------
                                                            // -----------
                                                            // 0---------3
#define STRIP_DIRECTION         (1U)                        // направление ленты из угла (0-1): 0 - горизонтальное (из углов влево или вправо)
                                                            // 1>>>------2  1------<<<2  1---------2  1---------2
                                                            // -----------  -----------  -----------  -----------
                                                            // -----------  -----------  -----------  -----------
                                                            // -----------  -----------  -----------  -----------
                                                            // 0---------3  0---------3  0>>>------3  0------<<<3

                                                            // 1 - Вертикальное (из углов вверх или вниз)
                                                            // 1---------2  1---------2  1---------2  1---------2
                                                            // v----------  ----------v  -----------  -----------
                                                            // v----------  ----------v  ^----------  ----------^
                                                            // -----------  -----------  ^----------  ----------^
                                                            // 0---------3  0---------3  0---------3  0---------3

// ===================== Настройки Кнопки =====================

#define BTN_PIN                 (4U)                        // пин кнопки (D2)
#define BUTTON_STEP_TIMEOUT     (100U)                      // каждые BUTTON_STEP_TIMEOUT мс будет генерироваться событие удержания кнопки (для регулировки яркости)
#define BUTTON_CLICK_TIMEOUT    (500U)                      // максимальное время между нажатиями кнопки в мс, до достижения которого считается серия последовательных нажатий

#define AUTOMOD_INTERVAL        (15000U)                     // кол-во времни между автоматическим переключением режима
#define DEFAULT_AUTOMOD         true                       // Начальное состояние автомода. true - вкл, false - выкл

// ============= Настройки Сохранения в память ================

#define MOD_LIST_OFFSET         64      // начальная точка записи адресов на режимы в ПЗУ
#define MOD_LIST_MAX_SIZE       128     // максимальное количество режимов, которое можно записать в ПЗУ
#define MOD_DATA_OFFSET         MOD_LIST_OFFSET + MOD_LIST_MAX_SIZE * 2     // 2 байта для хранения адреса на данные режима
#define MOD_DATA_SIZE           448    // размер буфера под данные режимов

// =================== Настройки Режимов ======================

#define STARTING_MOD            0