#include <Adafruit_TinyUSB.h> // Include Adafruit TinyUSB header
#include <Arduino.h>
#include <SPIFFS.h>

// File system
fs::FS &flashFS = SPIFFS;

// Buffer for reading and writing
uint8_t msc_buf[512];

// Constants for MSC configuration
#define MSC_BLOCK_SIZE 512     // Block size in bytes
#define MSC_BLOCK_COUNT (2048) // Total number of blocks (for 1MB file)

// Create an Adafruit TinyUSB object
Adafruit_USBSMSC usb_msc;

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

  // Set the capacity of the USB MSC
  usb_msc.setCapacity(MSC_BLOCK_COUNT, MSC_BLOCK_SIZE);

  // Start TinyUSB
  TinyUSBDevice.begin();
}

void loop() {
  // TinyUSB task to handle USB events
  usb_msc.task();
  TinyUSBDevice.poll();
}
