#include "USB.h" // Arduino USB header
#include <Arduino.h>
#include <SPIFFS.h>

// File system
fs::FS &flashFS = SPIFFS;

// Buffer for reading and writing
uint8_t msc_buf[512];

// USB MSC instance
USBMSC usb_msc;

// Variables for USB MSC
#define MSC_BLOCK_SIZE 512     // Block size in bytes
#define MSC_BLOCK_COUNT (2048) // Total number of blocks (for 1MB file)

// Callback for reading data from the drive
int32_t on_read(uint32_t lba, void *buffer, uint32_t bufsize) {
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
int32_t on_write(uint32_t lba, uint8_t *buffer, uint32_t bufsize) {
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

// Setup function
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

  // Setup USB MSC with block count and block size
  usb_msc.setCapacity(MSC_BLOCK_COUNT, MSC_BLOCK_SIZE);

  // Set read/write callbacks
  usb_msc.setReadWriteCallback(on_read, on_write);

  // Start USB MSC
  usb_msc.begin();
}

void loop() {
  // No special loop needed for USB MSC
}
