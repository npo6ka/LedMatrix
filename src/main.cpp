#include "myapplication.h"

MyApplication app;

// void listFiles(const char* dirname = "/") {
//     Dir dir = LittleFS.openDir(dirname);

//     Serial.println("Listing files:");
//     while (dir.next()) {
//         Serial.printf("FILE: %s\tSIZE: %d bytes\n",
//                       dir.fileName().c_str(),
//                       dir.fileSize());
//     }
// }

// void printFileIfExists(const char* path) {
//     if (!LittleFS.exists(path)) {
//         Serial.printf("File '%s' does not exist.\n", path);
//         return;
//     }

//     File file = LittleFS.open(path, "r");
//     if (!file) {
//         Serial.printf("Failed to open file '%s'\n", path);
//         return;
//     }

//     Serial.printf("Contents of '%s':\n", path);
//     while (file.available()) {
//         Serial.write(file.read());
//     }

//     file.close();
//     Serial.println(); // перенос строки после вывода
// }

// void printFileByByte(const char* path) {
//     if (!LittleFS.exists(path)) {
//         Serial.printf("File '%s' does not exist.\n", path);
//         return;
//     }

//     File file = LittleFS.open(path, "r");
//     if (!file) {
//         Serial.printf("Failed to open file '%s'\n", path);
//         return;
//     }

//     Serial.printf("Contents of '%s' by byte:\n'", path);
//     int byteCount = 1;
//     while (file.available()) {
//         uint8_t byte = 0;
//         file.read(reinterpret_cast<uint8_t*>(&byte), 1);
//         Serial.printf("%02X", byte);
//         if (byteCount % 4 == 0) {
//             Serial.printf(" ");
//         }
//         byteCount++;
//     }
//     Serial.printf("'");

//     file.close();
//     Serial.println(); // перенос строки после вывода
// }

// void printAllMods(FileEffectStorage &storage) {
//     logInfo("Current effects:\n");
//     for (size_t i = 0; i < storage.size(); i++) {
//         EffectInfo info = storage.getEffectInfo(i);
//         logInfo("Effect %zu: ID=%u, SavedIndex=%u\n", i, info.id, info.savedIndex);
//     }
// }

void setup() {
    app.onInit();
}

void loop() {
    app.onTick();
}
