//Target for the Geochallenger class project
//TX on 8, RX on 4, confirmation LED on 3;
//It would be nice to add check to validate the transmitter's id
//Instead of sending the transmitter's id, should send the target's, but then need some decent checksum

#include <VirtualWire.h>
int confLed = 3;
char iD_check[16] = {"54269236526");

uint8_t buf[16];
uint8_t buflen = 16;

void setup() {
  Serial.begin(9600);
  digitalWrite(confLed, LOW);

  vw_set_tx_pin(8);//Digital to tx's DATA
  vw_set_rx_pin(4);//rx's DATA
  vw_setup(4000);// speed of data transfer Kbps
  vw_rx_start();
}

void loop() {
  if (vw_get_message(buf, &buflen)) {
    digitalWrite(confLed, HIGH);
/*    for (int i = 0; i < buflen; i++) {
     iD_check[buflen-i] = buf[i];
    }*/
    Serial.println("Sending id check for "); //Print the transmissor's id, but not it's check
    Serial.println(*buf); //There must be a better way to do this
    send(iD_check); //send iD_check
  }
}

void send(char *message){
  vw_send((uint8_t *)message, strlen(message)); //send iD_check; maybe just use 16 on the lenght
  vw_wait_tx(); // Wait until the whole message is gone
  delay(1000); //Not sure why wait on the receiver. Maybe we avoid noise on the communications
}

