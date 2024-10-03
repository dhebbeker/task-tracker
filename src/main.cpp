#include "tusb.h" // TinyUSB header
#include <Arduino.h>
#include <SPIFFS.h>

// File system
fs::FS &flashFS = SPIFFS;

// Buffer for reading and writing
uint8_t msc_buf[512];

// Variables for USB MSC
#define MSC_BLOCK_SIZE 512     // Block size in bytes
#define MSC_BLOCK_COUNT (2048) // Total number of blocks (for 1MB file)

// Callback for reading data from the drive
int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, void *buffer,
                          uint32_t bufsize) {
  File file = flashFS.open("/storage.bin", FILE_READ);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return -1;
  }

  file.seek(lba * MSC_BLOCK_SIZE);
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

  file.seek(lba * MSC_BLOCK_SIZE);
  int32_t writtenBytes = file.write(buffer, bufsize);
  file.close();
  return writtenBytes;
}

// Callback invoked when the host requests to know the capacity of the MSC
// device
void tud_msc_capacity_cb(uint8_t lun, uint32_t *block_count,
                         uint16_t *block_size) {
  *block_size = MSC_BLOCK_SIZE;   // Block size of 512 bytes
  *block_count = MSC_BLOCK_COUNT; // 2048 blocks for 1MB file
}

// Callback to determine if the drive is writable
bool tud_msc_is_writable_cb(uint8_t lun) {
  return true; // Allow writing to the device
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
    for (int i = 0; i < MSC_BLOCK_COUNT; i++) {
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
