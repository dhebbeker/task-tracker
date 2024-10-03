#include "tusb.h" // TinyUSB header
#include <Arduino.h>
#include <SPIFFS.h>

// File system
fs::FS &flashFS = SPIFFS;

// Buffer for reading and writing
uint8_t msc_buf[512];

// Callback for getting the disk capacity
bool tud_msc_capacity_cb(uint8_t lun, uint32_t *block_count,
                         uint16_t *block_size) {
  *block_size = 512; // Each block is 512 bytes
  *block_count = flashFS.open("/storage.bin", FILE_READ).size() / 512;
  return true;
}

// Callback for reading data from the drive
int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, void *buffer,
                          uint32_t bufsize) {
  File file = flashFS.open("/storage.bin", FILE_READ);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return -1;
  }

  file.seek(lba * 512);
  int32_t readBytes = file.read((uint8_t *)buffer, bufsize);
  file.close();
  return readBytes;
}

// Callback for writing data to the drive
int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint8_t *buffer,
                           uint32_t bufsize) {
  File file = flashFS.open("/storage.bin", FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return -1;
  }

  file.seek(lba * 512);
  int32_t writtenBytes = file.write(buffer, bufsize);
  file.close();
  return writtenBytes;
}

// Callback to check if the drive is writable
bool tud_msc_is_writable_cb(uint8_t lun) {
  return true; // Allow write operations
}

void setup() {
  // Initialize serial for debugging
  Serial.begin(115200);

  // Initialize the file system
  if (!SPIFFS.begin(true)) {
    Serial.println("Failed to mount SPIFFS");
    return;
  }

  // Prepare a file in flash memory for USB storage
  if (!flashFS.exists("/storage.bin")) {
    File file = flashFS.open("/storage.bin", FILE_WRITE);
    if (!file) {
      Serial.println("Failed to create storage.bin");
      return;
    }

    // Initialize a 1MB file (2048 sectors of 512 bytes)
    for (int i = 0; i < 2048; i++) {
      file.write(msc_buf, sizeof(msc_buf));
    }
    file.close();
  }

  // Start TinyUSB
  tusb_init();
}

void loop() {
  // TinyUSB task to handle USB events
  tud_task();
}
