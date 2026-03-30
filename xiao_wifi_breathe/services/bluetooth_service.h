#pragma once

const char *bleAddressTypeLabel(uint8_t addressType) {
  switch (addressType) {
    case 0:
      return "public";
    case 1:
      return "random";
    case 2:
      return "public-id";
    case 3:
      return "random-id";
    default:
      return "unknown";
  }
}

void clearBleScanResultsLocked() {
  for (size_t index = 0; index < kMaxBleDevices; ++index) {
    bleResults[index].address = String("");
    bleResults[index].name = String("");
    bleResults[index].serviceUuid = String("");
    bleResults[index].rssi = 0;
    bleResults[index].hasTxPower = false;
    bleResults[index].txPower = 0;
    bleResults[index].connectable = false;
    bleResults[index].scannable = false;
    bleResults[index].legacy = false;
    bleResults[index].addressType = 0;
  }
  bleResultCount = 0;
}

void setBleScanStatus(bool scanning, const String &errorMessage) {
  if (bleScanMutex == nullptr) {
    return;
  }

  if (xSemaphoreTake(bleScanMutex, pdMS_TO_TICKS(250)) == pdTRUE) {
    bleScanInProgress = scanning;
    bleScanError = errorMessage;
    if (scanning) {
      bleScanStartedAtMs = millis();
      clearBleScanResultsLocked();
    } else {
      bleScanCompletedAtMs = millis();
    }
    xSemaphoreGive(bleScanMutex);
  }
}

void handleBleScanComplete(BLEScanResults scanResults) {
  if (bleScanMutex != nullptr && xSemaphoreTake(bleScanMutex, portMAX_DELAY) == pdTRUE) {
    clearBleScanResultsLocked();

    const int totalResults = scanResults.getCount();
    const size_t cappedResults = totalResults > static_cast<int>(kMaxBleDevices) ? kMaxBleDevices : static_cast<size_t>(totalResults);
    for (size_t index = 0; index < cappedResults; ++index) {
      BLEAdvertisedDevice device = scanResults.getDevice(index);
      BleScanResultEntry &entry = bleResults[index];
      entry.address = device.getAddress().toString();
      entry.name = device.haveName() ? device.getName() : String("");
      entry.serviceUuid = device.haveServiceUUID() ? device.getServiceUUID().toString() : String("");
      entry.rssi = device.getRSSI();
      entry.hasTxPower = device.haveTXPower();
      entry.txPower = device.haveTXPower() ? device.getTXPower() : 0;
      entry.connectable = device.isConnectable();
      entry.scannable = device.isScannable();
      entry.legacy = device.isLegacyAdvertisement();
      entry.addressType = device.getAddressType();
      ++bleResultCount;
    }

    bleScanInProgress = false;
    bleScanCompletedAtMs = millis();
    bleScanError = String("");
    xSemaphoreGive(bleScanMutex);
  }

  if (bleScan != nullptr) {
    bleScan->clearResults();
  }
}

bool startBleScan() {
  if (!bleReady || bleScan == nullptr) {
    setBleScanStatus(false, F("Bluetooth scanner unavailable."));
    return false;
  }

  if (bleScan->isScanning()) {
    return true;
  }

  setBleScanStatus(true, String(""));
  bleScan->clearResults();

  if (!bleScan->start(kBleScanDurationSeconds, handleBleScanComplete, false)) {
    setBleScanStatus(false, F("Failed to start Bluetooth scan."));
    return false;
  }

  return true;
}

void initializeBluetoothScanner() {
  bleScanMutex = xSemaphoreCreateMutex();
  if (bleScanMutex == nullptr) {
    bleScanError = F("Failed to allocate Bluetooth scan mutex.");
    return;
  }

  if (!BLEDevice::init("XIAO Device Console")) {
    bleScanError = F("BLE init failed.");
    return;
  }

  bleScan = BLEDevice::getScan();
  if (bleScan == nullptr) {
    bleScanError = F("BLE scanner unavailable.");
    return;
  }

  bleScan->setActiveScan(true);
  bleScan->setInterval(120);
  bleScan->setWindow(80);
#if defined(CONFIG_NIMBLE_ENABLED)
  bleScan->setDuplicateFilter(true);
#endif
  bleReady = true;
  bleScanError = String("");
}
