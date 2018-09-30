#include <SoftwareSerial.h>
#include <TinyGPS.h>

// Arduino port usage
const int BUZZER_PIN = 0;
const int SIM_RX = 6;
const int SIM_TX = 7;
const int BLE_RX = 8;
const int BLE_TX = 9;
const int GPS_RX = 10;
const int GPS_TX = 11;
const int SOLENOID_PIN = 12;
const int ANOLOGUE_PIN_1 = A5;
const int ANOLOGUE_PIN_2 = A4;
const int ANOLOGUE_PIN_3 = A3;

// SIM800L (Sim Module)
SoftwareSerial simSerial(SIM_RX, SIM_TX);
bool hasSentSMS = false;

// Bluetooth BLE
SoftwareSerial bleSerial(BLE_RX, BLE_TX);
const char RECEIVED_CONNECTED = 'c';
const char SEND_CONNECTED[] = "connected";
const char RECEIVED_UNLOCK = 'u';
const char SEND_UNLOCK[] = "unlock";
const char RECIEVED_SECURITY_ON = 's';
const char SEND_SECURITY_ON[] = "security_on";
const char RECIEVED_SECURITY_OFF = 'e';
const char SEND_SECURITY_OFF[] = "security_off";
const char RECIEVED_BUZZER_OFF = 'b';

char message = ' ';
bool isKeefreeSecurityOn = false;

// GPS
SoftwareSerial gpsSerial(GPS_RX, GPS_TX);
TinyGPS gps;
char gpsData = ' ';
bool hasSentGpsData = false;

// Accelerometer
unsigned int outputX = 0;
unsigned int outputY = 0;
unsigned int outputZ = 0;
unsigned int counter = 0;

// function prototypes
void handleMessage(char *message);
void handleGpsData(TinyGPS &gpsData);
bool isSimAvailable();
bool isKeefreeAvailable();
bool isGpsAvailable();
char readSim();
char readKeefree();
char readGps();
void unlockKeefree();
void sendMessage(const char *message);
void sendSMS();

void setup()
{
  // Serial monitor
  Serial.begin(9600);

  // Buzzer
  // pinMode(BUZZER_PIN, OUTPUT);

  // Solenoid
  pinMode(SOLENOID_PIN, OUTPUT);

  // SIM
  simSerial.begin(9600);
  Serial.println("SIM Serial On...");

  // BLE
  bleSerial.begin(9600); // Software serial port
  Serial.println("Bluetooth Serial On...");

  // GPS
  gpsSerial.begin(9600);
  Serial.println("GPS Serial On...");
  Serial.print("Testing TinyGPS library v. ");
  Serial.println(TinyGPS::library_version());
}

void loop()
{

  bleSerial.listen();
  // BLE Serial
  if (isKeefreeAvailable())
  {
    message = readKeefree();
    handleMessage(&message);
  }
  else
  {
    hasSentGpsData = false;
  }

  if (isKeefreeSecurityOn)
  {
    // The voltage applied to the AREF pin (0 to 5V only) is used as the reference.
    //connect 3.3v to AREF
    analogReference(EXTERNAL);

    // Accelerometer values
    outputX = analogRead(ANOLOGUE_PIN_1);
    delay(1);
    outputY = analogRead(ANOLOGUE_PIN_2);
    delay(1);
    outputZ = analogRead(ANOLOGUE_PIN_3);

    Serial.print("x: ");
    Serial.print(outputX);
    Serial.print(" y: ");
    Serial.print(outputY);
    Serial.print(" z: ");
    Serial.println(outputZ);
    delay(1000);

    if (outputX >= (outputX + 10) || outputY >= (outputY + 10) || outputY >= (outputY + 10))
    {
      Serial.print("nudge: ");
      Serial.println(counter);

      counter++;
    }

    if (counter >= 3)
    {
      Serial.println("Triggered Alarm...");

      // Set alarm off and send SMS
      tone(BUZZER_PIN, 100, 1000);
      delay(1100); // 1 second interval

      sendSMS();
    }
  }
  else
  {
    // Security turned off
    counter = 0;
  }

  // Arduino Serial
  if (Serial.available())
  {
    message = Serial.read();
    Serial.write(message);
    sendMessage(&message);
  }
}

void handleMessage(char *message)
{
  switch (*message)
  {
  case RECEIVED_CONNECTED:
    Serial.println("RECEIVED CONNECTED");

    //Send GPS data when keefree is connected
    while (!hasSentGpsData)
    {
      gpsSerial.listen();
      if (isGpsAvailable())
      {
        gpsData = readGps();
        if (gps.encode(gpsData))
        {
          handleGpsData(gps);
          hasSentGpsData = true;
        }
      }
    }

    //sendMessage(SEND_CONNECTED);
    break;

  case RECEIVED_UNLOCK:
    Serial.println("RECEIVED UNLOCK");
    unlockKeefree();
    sendMessage(SEND_UNLOCK);
    break;

  case RECIEVED_SECURITY_ON:
    Serial.println("RECEIVED SECUIRTY ON");
    isKeefreeSecurityOn = true;
    sendMessage(SEND_SECURITY_ON);
    break;

  case RECIEVED_SECURITY_OFF:
    Serial.println("RECEIVED SECURITY OFF");
    isKeefreeSecurityOn = false;
    sendMessage(SEND_SECURITY_OFF);
    break;

  case RECIEVED_BUZZER_OFF:
    noTone(BUZZER_PIN);
    counter = 0;
    hasSentSMS = false; // reset SMS once Buzzer has been turned off
    break;

  default:
    Serial.print("Error: wrong message received: ");
    Serial.println(message);
  }
}

void handleGpsData(TinyGPS &gpsData)
{
  float f_latitude, f_longitude, f_speed, f_altitude;
  unsigned long ul_age;

  gpsData.f_get_position(&f_latitude, &f_longitude, &ul_age);
  f_speed = gpsData.f_speed_kmph();
  f_altitude = gpsData.f_altitude();

  // Latitude and Longitude
  if (f_latitude >= -90 && f_latitude <= 90 && f_longitude >= -180 && f_longitude <= 180)
  {
    String lat = String(f_latitude);
    String lon = String(f_longitude);
    String latLon = String(lat + "," + lon);
    sendMessage(latLon.c_str());
    delay(2000); // Adding the delays prevents from sending messages together
  }

  // Speed
  if (f_speed > 0)
  {
    String s = String(f_speed);
    String speed = String(s + "kmph");
    sendMessage(speed.c_str());
    delay(2000);
  }

  // Altitude
  String a = String(f_altitude);
  String altitude = String(a + "m");
  sendMessage(altitude.c_str());

  Serial.println("Finishing sending GPS data...");
}

bool isSimAvailable()
{
  return simSerial.available() && simSerial.isListening();
}

bool isKeefreeAvailable()
{
  return bleSerial.available() && bleSerial.isListening();
}

bool isGpsAvailable()
{
  return gpsSerial.available() && gpsSerial.isListening();
}

char readSim()
{
  return simSerial.read();
}

char readKeefree()
{
  return bleSerial.read();
}

char readGps()
{
  return gpsSerial.read();
}

void unlockKeefree()
{
  digitalWrite(SOLENOID_PIN, HIGH);
  delay(5000); // unlock for 5 seconds
  digitalWrite(SOLENOID_PIN, LOW);
}

void sendMessage(const char *message)
{
  Serial.print("SENDING: '");
  Serial.print(message);
  Serial.println("'");

  bleSerial.write(message);
}

void sendSMS()
{
  //Send GPS data when keefree is connected
  while (!hasSentSMS)
  {
    simSerial.listen();

    if (isSimAvailable())
    {
      Serial.println("Preparing SMS...");

      //Set SMS format to ASCII
      simSerial.write("AT+CMGF=1\r\n");
      delay(1000);

      //Send new SMS command and message number
      simSerial.write("AT+CMGS=\"0410415185\"\r\n"); // Paulo Dacaya's number
      delay(1000);

      //Send SMS content
      simSerial.write("!!KEEFREE WARNING!! Your bicycle may be in risk.");
      delay(1000);

      //Send Ctrl+Z / ESC to denote SMS message is complete
      simSerial.write((char)26);
      delay(1000);

      Serial.println("SMS has been sent...");

      hasSentSMS = true;
    }
  }
}

// Can't use analogWrite() on pin 3 and 11 at the same time you are using the tone() function.
// tone() cannot be used at the same time on two seprate pins. need to use noTone(#pin) stop previous one.
//tone()

// digitalWrite(BUZZER_PIN, HIGH);
// delay(1000);
// digitalWrite(BUZZER_PIN, LOW);
// delay(1000);
