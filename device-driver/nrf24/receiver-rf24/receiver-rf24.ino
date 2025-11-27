#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(48,53);  // CE, CSN (Arduino Mega)
const byte addressRX[6] = "00001"; // RX gửi về TX
const byte addressTX[6] = "00002"; // RX nhận từ TX

int receivedValue = 0;
int sendBackValue = 3;

void setup() {
  Serial.begin(115200);

  pinMode(4, INPUT);

  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);

  radio.openWritingPipe(addressRX);
  radio.openReadingPipe(1, addressTX);

  radio.startListening();
}

void loop() {

  if (radio.available()) {
    //--- NHẬN TỪ TX ---//
    radio.read(&receivedValue, sizeof(receivedValue));
    Serial.print("\nRX received: ");
    Serial.print(receivedValue);

    //--- GỬI PHẢN HỒI NGAY ---//
    radio.stopListening();
    delayMicroseconds(200);    // quan trọng!

    bool ok = radio.write(&sendBackValue, sizeof(sendBackValue));

    if (!ok) Serial.print("\nFail to send back");
    else     Serial.print("\nRX sent: 3 OK");

    radio.startListening();
  }
}
