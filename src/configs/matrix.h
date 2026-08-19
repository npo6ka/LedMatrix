#pragma once

#define DEBUG                   (true)

// ===================== Настройки Матрицы =====================

#define LEDS_WIDTH              (16)                        // ширина матрицы
#define LEDS_HEIGHT             (16)                        // высота матрицы
#define LEDS_MAX_POWER          (500U)                      // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит
#define LEDS_BRIGHTNRSS         (50)                        // яркость матрицы 0 - 255
#define LEDS_COLOR_ORDER        (GRB)                       // порядок цветов на ленте. Если цвет отображается некорректно - меняйте. Начать можно с RGB

#define LEDS_MATRIX_TYPE        (0U)                        // тип матрицы: 0 - зигзаг, 1 - параллельная
#define LEDS_CONNECTION_ANGLE   (3U)                        // угол подключения матрицы (0-3): 0 - левый нижний, 1 - левый верхний, 2 - правый верхний, 3 - правый нижний
                                                            // 1---------2
                                                            // -----------
                                                            // -----------
                                                            // -----------
                                                            // 0---------3
#define LEDS_STRIP_DIRECTION    (1U)                        // направление ленты из угла (0-1): 0 - горизонтальное (из углов влево или вправо)
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

// ===================== Настройки управления =====================

#define BTN_ENABLE              true                        // подключить управление кнопкой
#define BUTTON_STEP_TIMEOUT     (100U)                      // каждые BUTTON_STEP_TIMEOUT мс будет генерироваться событие удержания кнопки (для регулировки яркости)
#define BUTTON_CLICK_TIMEOUT    (500U)                      // максимальное время между нажатиями кнопки в мс, до достижения которого считается серия последовательных нажатий
#define BUTTON_HOLD_RESET_MS    (5000U)                     // удержание кнопки столько мс — сброс списка режимов на заводской

#define AUTOMOD_INTERVAL        (10000U)                    // кол-во времни между автоматическим переключением режима
#define AUTOMOD_DEF_STATE       false                       // Начальное состояние автомода. true - вкл, false - выкл

#define IR_ENABLE               false                       // подключить урпавление через ИК приёмник
#define DECODE_SAMSUNG                                      // тип ИК пульта которые даёт команды

#define RELAY_ENABLE            false                       // подключить урпавление через ИК приёмник
#define RELAY_DELAY             100

#ifdef ESP32DEV
#define MIC_ENABLE              true                        // INMP441 по I2S
#define MIC_SPECTRUM_ENABLE     true                        // FFT-разбиение на частотные полосы
#define MIC_FFT_SIZE            (256U)                      // размер окна FFT (степень двойки)
#define MIC_SPECTRUM_BANDS      (16U)                       // число логарифмических полос
#define MIC_SAMPLE_RATE         (16000)
#else
#define MIC_ENABLE              false
#endif

// ===================== Настройки WiFi (ESP32) =====================

#ifdef ESP32DEV
#define WIFI_ENABLE             true                        // веб-управление по WiFi
#define WIFI_AP_SSID            "LedMatrix"                 // префикс SSID (добавляется суффикс MAC)
#define WIFI_AP_PASSWORD        ""                  // пароль AP (мин. 8 символов)
#define WIFI_AP_CHANNEL         1
#define WIFI_AP_MAX_CLIENTS     2
#define WIFI_MDNS_HOST          "ledmatrix"                 // http://ledmatrix.local
#else
#define WIFI_ENABLE             false
#endif

// =============== Настройки Сохранения в память ==================

#define SAVE_TO_EEPROM          true                        // сохранять настройки в EEPROM
#define SAVE_TO_EEPROM_FILE     "mods.txt"                  // имя файла для сохранения настроек, если используется файловая система

// ===================== Платформозависимые настройки =====================
#ifdef ESP32DEV

#define LEDS_PIN                (21)                        // пин к которому подключены светодиоды
#define BTN_PIN                 (16)                        // пин кнопки
#define IR_RECEIVE_PIN          (24)                        // пин к которому подключен ИК приёмник
#define RELAY_PIN               (25)                        // пин управления реле
#define I2S_WS                  (25)                        // INMP441 LRCL
#define I2S_SD                  (32)                        // INMP441 DOUT
#define I2S_SCK                 (26)                        // INMP441 BCLK

#else // if DESP12F

#define LEDS_PIN                (D2)                        // пин к которому подключены светодиоды
#define BTN_PIN                 (D5)                        // пин кнопки
#define IR_RECEIVE_PIN          (D6)                        // пин к которому подключен ИК приёмник
#define RELAY_PIN               (D7)                        // пин управления реле

#endif // ESP32DEV
