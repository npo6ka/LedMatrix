# REST API LedMatrix

HTTP JSON API прошивки ESP32 для управления матрицей. Подходит для веб-интерфейса и нативных клиентов (Android, iOS и т.д.).

Авторизации нет. WebSocket нет. Команды выполняются **асинхронно**: POST сразу отвечает `{"ok":true}`, состояние меняется чуть позже — после ответа стоит запросить `GET /api/status`.

Базовый URL: `http://192.168.4.1`  
Порт: **80**  
Формат: JSON, кодировка UTF-8.

---

## Подключение

Плата работает как точка доступа (SoftAP). Клиент подключается к её Wi‑Fi и ходит на IP платы.

| Параметр | Значение |
|----------|----------|
| SSID | `LedMatrix-XXXX` (префикс + последние 2 байта MAC) |
| Пароль | из `WIFI_AP_PASSWORD` в `src/configs/matrix.h` / `line.h`; пустая строка — открытая сеть |
| IP платы | обычно `192.168.4.1` |
| mDNS | `http://ledmatrix.local` (`WIFI_MDNS_HOST`) — на Android часто не работает, лучше IP |
| Клиентов AP | максимум `WIFI_AP_MAX_CLIENTS` (по умолчанию 2) |

Настройки WiFi: `WIFI_ENABLE`, `WIFI_AP_SSID`, `WIFI_AP_PASSWORD`, `WIFI_AP_CHANNEL`, `WIFI_MDNS_HOST`.

Для веб-интерфейса нужна файловая система:

```bash
pio run -e esp32dev -t uploadfs
```

Статика (`/`, `/index.html`, `/app.js`, `/style.css`) — не часть API. Captive-portal (`/generate_204` и аналоги) редиректит на `/`, приложению не нужен.

---

## Общий контракт

### Заголовки

У POST с телом обязателен:

```
Content-Type: application/json
```

GET без тела. CORS для нативного Android не требуется. `OPTIONS` на неизвестных путях отвечает `204`.

### Успех команды (все POST)

`200 application/json`

```json
{"ok":true}
```

### Ошибка

```json
{"ok":false,"error":"<текст>"}
```

| HTTP | Когда |
|------|--------|
| 400 | битый JSON или нет обязательного поля |
| 404 | неизвестный путь `/api/...` |
| 429 | чаще чем раз в **200 мс** (общий лимит на **все** POST) |
| 500 | нет storage / переполнение JSON-буфера (512 байт) |
| 503 | очередь команд занята (`busy`) |

GET `/api/status` и `/api/effects` **не** попадают под rate limit.

Тексты ошибок: `invalid json`, `missing on`, `missing index`, `missing enabled`, `missing value`, `rate limit`, `busy`, `storage unavailable`, `not found`, `json overflow`.

---

## Сводка эндпоинтов

| Метод | Путь | Тело |
|-------|------|------|
| GET | `/api/status` | — |
| GET | `/api/effects` | — |
| POST | `/api/power` | `{"on": true}` |
| POST | `/api/mode/next` | — |
| POST | `/api/mode/prev` | — |
| POST | `/api/mode` | `{"index": 3}` |
| POST | `/api/automode` | `{"enabled": true}` |
| POST | `/api/brightness` | `{"value": 128}` |
| POST | `/api/symmetric` | `{"enabled": true}` |
| POST | `/api/reset` | — |

Примеры:

```bash
curl http://192.168.4.1/api/status
curl http://192.168.4.1/api/effects
curl -X POST http://192.168.4.1/api/power -H "Content-Type: application/json" -d "{\"on\":true}"
curl -X POST http://192.168.4.1/api/mode/next
curl -X POST http://192.168.4.1/api/mode -H "Content-Type: application/json" -d "{\"index\":3}"
curl -X POST http://192.168.4.1/api/automode -H "Content-Type: application/json" -d "{\"enabled\":true}"
curl -X POST http://192.168.4.1/api/brightness -H "Content-Type: application/json" -d "{\"value\":128}"
curl -X POST http://192.168.4.1/api/reset
```

---

## GET `/api/status`

Текущее состояние устройства.

```json
{
  "power": true,
  "autoMode": false,
  "effectIndex": 3,
  "effectId": 4,
  "effectName": "Rain",
  "fps": 42.5,
  "brightness": 128,
  "symmetric": false,
  "width": 250,
  "height": 1,
  "effectsCount": 48,
  "ip": "192.168.4.1",
  "ssid": "LedMatrix-A1B2"
}
```

| Поле | Тип | Смысл |
|------|-----|--------|
| `power` | bool | лента вкл/выкл (в JSON **не** `powerOn`) |
| `autoMode` | bool | автосмена режимов |
| `effectIndex` | number | индекс в **текущем списке** — его слать в `POST /api/mode` |
| `effectId` | number | стабильный id эффекта на плате (`EffectId`) |
| `effectName` | string | имя эффекта |
| `fps` | number | текущий FPS (float) |
| `brightness` | number | яркость 0–255 |
| `symmetric` | bool | симметричное отображение |
| `width` | number | текущая логическая ширина (в симметричном режиме вдвое меньше физической) |
| `height` | number | текущая логическая высота |
| `effectsCount` | number | длина списка режимов |
| `ip` | string | IP SoftAP |
| `ssid` | string | SSID SoftAP |

Поллинг раз в 2–3 с достаточно (веб-UI использует 3000 мс).

---

## GET `/api/effects`

Список режимов **в порядке переключения**. Ответ — JSON-массив, не объект.

```json
[
  {"index": 0, "id": 1, "name": "SlowRandom"},
  {"index": 1, "id": 2, "name": "SimpleRainbow"}
]
```

| Поле | Тип | Смысл |
|------|-----|--------|
| `index` | number | позиция в списке; аргумент `POST /api/mode` |
| `id` | number | `EffectId`; один и тот же id может повторяться |
| `name` | string | человекочитаемое имя |

После `POST /api/reset` список нужно загрузить заново.

---

## POST `/api/power`

Включить или выключить ленту.

```json
{"on": true}
```

`on` — строго boolean. Отдельного toggle на сервере нет: клиент шлёт `on: !status.power`.

---

## POST `/api/mode/next` и `/api/mode/prev`

Следующий / предыдущий режим в списке. Тело не нужно.

На клиенте лучше блокировать кнопки до ответа (очередь команд ограничена, плюс rate limit 200 мс).

---

## POST `/api/mode`

Выбрать режим по индексу из `/api/effects`, **не** по `effectId`.

```json
{"index": 3}
```

`index` — целое число.

---

## POST `/api/automode`

```json
{"enabled": true}
```

`enabled` — строго boolean.

---

## POST `/api/brightness`

```json
{"value": 128}
```

`value` — целое. Плата обрезает в диапазон **0–255**. На слайдере имеет смысл debounce ≥ 200 мс, чтобы не ловить `429`.

---

## POST `/api/symmetric`

```json
{"enabled": true}
```

Включает симметричное отображение: логическая матрица уменьшается вдвое по длинной стороне, а нарисованная половина зеркалится на вторую. Логическая координата 0 оказывается на обоих краях ленты, последняя — в центре.

Текущий эффект перезапускается, значение сохраняется в LittleFS (`symmetric.bin`) и переживает перезагрузку. Новые размеры видны в `width` / `height` из `GET /api/status`.

---

## POST `/api/reset`

Сброс списка эффектов на заводской (`DefaultEffectList`). Тела нет.

После успеха: снова `GET /api/effects` и `GET /api/status`.

---

## Чего в API нет

Настройка параметров отдельного эффекта, цвет, Wi‑Fi STA, OTA, микрофон, авторизация.

---

## Android

1. **Обнаружение:** scan Wi‑Fi на SSID `LedMatrix-*`, подключение, запросы на `http://192.168.4.1`. На mDNS не опираться.
2. **Маршрутизация:** на Android 10+ привязать процесс к этой Wi‑Fi сети (`WifiNetworkSpecifier` / `bindProcessToNetwork`), иначе трафик уйдёт в мобильный интернет.
3. **Разрешения:** `INTERNET`; для скана — `ACCESS_FINE_LOCATION` и/или `NEARBY_WIFI_DEVICES`.
4. **HTTP без TLS:** `android:usesCleartextTraffic="true"` или Network Security Config для `192.168.4.1`.
5. **Таймауты:** connect ~3 с, read ~5 с.
6. **Старт UI:** `GET /api/effects` → `GET /api/status` → поллинг status. После POST сразу обновить status.

Минимальный пример (OkHttp + org.json):

```kotlin
val client = OkHttpClient.Builder()
    .connectTimeout(3, TimeUnit.SECONDS)
    .readTimeout(5, TimeUnit.SECONDS)
    .build()

fun getStatus(): JSONObject {
    val req = Request.Builder().url("http://192.168.4.1/api/status").build()
    return client.newCall(req).execute().use { JSONObject(it.body!!.string()) }
}

fun setPower(on: Boolean) {
    val body = JSONObject().put("on", on).toString()
        .toRequestBody("application/json".toMediaType())
    val req = Request.Builder()
        .url("http://192.168.4.1/api/power")
        .post(body)
        .build()
    client.newCall(req).execute().close()
}
```
