//Transmitter for the Geochallenger class project
//TX on 8, RX on 4, confirmation LED on 3, button on 7
//Needs to add a check to the transmitter's id
//Instead of receiving it's own id, it should receive the target's, but then would need some checksum

#include <VirtualWire.h>
char iD[16] = "123456789ABCDEF";
char iD_check[16] = "FEDCBA987654321";
int sendButton = 7;
int valButton = 1; //check button's state(HIGH or LOW)
int confLed = 3; //Confirmation bar. Only 1 LED for now

void setup() {
  Serial.begin(9600);
  pinMode(sendButton, INPUT_PULLUP);
  pinMode(confLed, OUTPUT);
  digitalWrite(confLed, LOW);
  
  vw_set_tx_pin(8);//Digital to tx's DATA
  vw_set_rx_pin(4);//rx' DATA
  vw_setup(4000);// speed of data transfer Kbps
  Serial.println("Started");
}

void loop() { //If button is pressed send iD then wait 2 seconds for confirmation
  valButton = digitalRead(sendButton);
  Serial.println(valButton);
  if (valButton == LOW) { 
    Serial.println("button pressed");
    send(iD);
    //
    confirmation(iD_check); //wait confirmation
    delay(1000); //Wait 1 second to avert missclicks, unless the whole proccess would take more than a second
  }
}

void send(char *message) {
  vw_send((uint8_t *)message, strlen(message)); //send iD; maybe change the string lenght  to 16
  vw_wait_tx(); // Wait until the whole message is gone
  Serial.println("send ok"); 
//  delay(2000);
}

void confirmation(char *iD_check) {
  uint8_t buf[16];
  uint8_t buflen = 16; //strlen(iD_check)
  
  vw_rx_start(); //save battery:receive only when expecting a message
  if (vw_wait_rx_max(5000) && vw_get_message(buf, &buflen)) { //wait 2 seconds to receive
    digitalWrite(confLed, HIGH); //When we get the LED strip, we call it's updating function here
    for (int i = 0; i < buflen; i++) { //If confirmation sequence is wrong, exit
      if (buf[i] != iD_check[i]) {
        Serial.println("Wrong confirmation id");
        delay(1000);
        vw_rx_stop();
        break;//exit(0);//Need to stop confirmation and go back to loop. Better ways?
      }else Serial.println("Good check!");
    }
  } else Serial.println("timeout");
  vw_rx_stop();
  
  
}
