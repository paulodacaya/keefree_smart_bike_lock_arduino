
// Bluetooth BLE
# include <SoftwareSerial.h>
SoftwareSerial bleSerial( 8, 9 ); // RX = 8, TX = 9
char serialValue = ' ';

void setup() {

  Serial.begin( 9600 );    // Bluetooth BLE hardware serial port
  bleSerial.begin( 9600 ); // Software serial port
  Serial.println( "Connection Established." );
  
}


void loop() {
  
  // BLE Serial
  if( bleSerial.available() ) 
  {
    serialValue = bleSerial.read();
    Serial.write( serialValue ); // Read data from BLE module, print to Serial Monitor
  }

  // Arduino Serial
  if( Serial.available() ) 
  {
    serialValue = Serial.read();
    bleSerial.write( serialValue ); // Write value from Serial Monitor to BLE
  }
  
}









