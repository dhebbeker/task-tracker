#include <Arduino.h>
#include <SPIFFS.h>
#include <usb/usb_msc.h> // Native USB MSC support

// File system
fs::FS &flashFS = SPIFFS;

// Buffer for reading and writing
uint8_t msc_buf[512];

// Callback when the host reads from the USB mass storage
int32_t on_read(uint32_t lba, void *buffer, uint32_t bufsize) {
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

// Callback when the host writes to the USB mass storage
int32_t on_write(uint32_t lba, uint8_t *buffer, uint32_t bufsize) {
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

// Callback when the host wants to know the capacity of the USB mass storage
void on_get_capacity(uint32_t *block_count, uint32_t *block_size) {
  *block_count = flashFS.open("/storage.bin", FILE_READ).size() / 512;
  *block_size = 512;
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

  // Start USB mass storage
  usb_msc_init(); // Initialize the USB MSC
  usb_msc_set_callbacks(on_get_capacity, on_read, on_write); // Set callbacks
  usb_msc_start();                                           // Start MSC device
}

void loop() {
  // Let USB handle events
  delay(10);
}
