#include "myapplication.h"

MyApplication app;

void setup() {
    app.onInit();
}

void loop() {
    app.onTick();
}

//#include "LittleFS.h"

    // if (!LittleFS.begin()) {
    //     Serial.println("An Error has occurred while mounting LittleFS");
    //     return;
    // }

    /*FSInfo fsi;
    LittleFS.info(fsi);
    printf("\ttotalBytes: %d\n\t usedBytes: %d\n\t blockSize: %d\n\t pageSize: \
    %d\n\t maxOpenFiles: %d\n\t maxPathLength: %d\n", fsi.totalBytes,
    fsi.usedBytes, fsi.blockSize, fsi.pageSize, fsi.maxOpenFiles, fsi.maxPathLength);*/

    // Dir dir = LittleFS.openDir("/");
    // while (dir.next()) {
    //     Serial.println(dir.fileName());
    //     if(dir.fileSize()) {
    //         File f = dir.openFile("r");
    //         Serial.println(f.size());
    //     }
    // }



    // File fp = LittleFS.open("/2.txt", "r+");
    // if (!fp) {
    //     Serial.println("Failed to open file for reading");
    //     return;
    // }

    // fp.seek(0, SeekSet);
    // uint16_t vals[] = {0x12, 0x15, 0x22, 0x34, 0x38, 0x55, 0x5b, 0x60, 0x65, 0x68, 0x70, 0x72, 0x74, 0x76, 0x78, 0x80, 0x82, 0x84, 0x86, 0x88, 0x90, 0x92, 0x94, 0x96, 0x98, 0x100};
    // uint8_t size = sizeof(vals) / sizeof(vals[0]);
    // Serial.printf("size: %d\n", size);

    // fp.seek(0, SeekSet);
    // for (uint16_t i = 0; i < vals[size - 1]; ++i) {
    //     fp.write(0);
    // }

    // fp.seek(0, SeekSet);
    // for (uint16_t i = 0; i < size; ++i) {
    //     fp.write(vals[i]);
    //     fp.seek(vals[i], SeekSet);
    // }
    // fp.write("e");

    // fp.seek(0, SeekSet);
    // Serial.println("File Content:");
    // while (fp.available()) {
    //     Serial.printf("%d ", fp.read());
    // }

    // unsigned long myTime1, myTime2;
    // myTime1 = micros();

    // fp.seek(0, SeekSet);
    // uint8_t val;
    // for (int i = 0; i < 26; ++i) {
    //     val = fp.read();
    //     // Serial.printf("%x ", val);
    //     fp.seek(val, SeekSet);
    // }
    // myTime2 = micros();
    // Serial.printf("time diff1: %lu\n", myTime2 - myTime1);


    // uint16_t vals[] = {0x12, 0x15, 0x22, 0x34, 0x38, 0x55, 0x5b, 0x60, 0x65, 0x68, 0x70, 0x72, 0x74, 0x76, 0x78, 0x80, 0x82, 0x84, 0x86, 0x88, 0x90, 0x92, 0x94, 0x96, 0x98, 0x100};
    // uint16_t vals2[] = {0x12, 0x15, 0x22, 0x34, 0x38, 0x55, 0x5b, 0x60, 0x65, 0x68, 0x70, 0x72, 0x74, 0x76, 0x78, 0x80, 0x82, 0x84, 0x86, 0x88, 0x90, 0x92, 0x94, 0x96, 0x98, 0x100};
    // myTime1 = micros();
    // for (int i = 0; i < 26; ++i) {
    //     val = vals[i];
    //     vals2[i] = val;
    // }
    // vals2[0]++;
    // myTime2 = micros();
    // Serial.printf("time diff2: %lu\n", myTime2 - myTime1);

    // fp.close();
