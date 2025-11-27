#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9,10);  // CE, CSN
const byte addressTX[6] = "00002"; // TX gửi tới RX
const byte addressRX[6] = "00001"; // TX nhận từ RX

int sendValue = 2;
int recvValue = 0;

void setup() {
  Serial.begin(115200);

  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);

  radio.openWritingPipe(addressTX);
  radio.openReadingPipe(1, addressRX);
}

void loop() {

  //--- TX GỬI ---//
  radio.stopListening();
  bool ok = radio.write(&sendValue, sizeof(sendValue));

  Serial.print("\nTX sent: ");
  Serial.print(sendValue);
  Serial.print(ok ? " (OK)" : " (FAIL)");

  //--- ĐỢI RX TRẢ LỜI ---//
  radio.startListening();
  unsigned long start = millis();
  bool received = false;

  while (millis() - start < 30) { // timeout 30ms
    if (radio.available()) {
      radio.read(&recvValue, sizeof(recvValue));
      received = true;
      break;
    }
  }

  if (received) {
    Serial.print("\nTX received: ");
    Serial.print(recvValue);
  } else {
    Serial.print("\nTX: no response");
  }

  delay(200);
}
