#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <SPI.h> // SPI
#include <MFRC522.h> // RFID
#include <ESP32Servo.h>
#include <DHT.h>
#include "ThingSpeak.h"

#endif
//////////DHT11 PART
char ssid[] = "TOPNET_CD70"; // your network SSID (name)
char pass[] = "fwh29fy4eo"; // your network password
WiFiClient client;
unsigned long myChannelNumber = 1695428;
const char * myWriteAPIKey = "KVCIPAW27UTCFOBM";
DHT dht(5, DHT11);
float temp = 0; // setup a int to simulate a temp
float hum = 0;
///////////////////////////

//Provide the token generation process info.
#include <addons/TokenHelper.h>
//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "TOPNET_CD70"
#define WIFI_PASSWORD "fwh29fy4eo"
#define SS_PIN 21
#define RST_PIN 22
// Déclaration 
MFRC522 rfid(SS_PIN, RST_PIN); 

// Tableau contentent l'ID
byte nuidPICC[4];

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position


//For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "6HKYT31Xeh3K7XxAjzfg5x2F3WVEEWMOY1XFy8tp"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://project-iot-613d3-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app



//Define Firebase Data object
FirebaseData fbdo;
FirebaseData fbdo2;
FirebaseAuth auth;
FirebaseConfig config;
String main="";
String ch="";
String ch1,ch2;
String chtry;
String a,a1;
String tryy="valeur";
String tryy4="name";
int tryy2=1;
int verif=0;
int  b, y;
int compt=0;
int capteurdeflamme = 13; 
void setup()
{
   // Init SPI bus
  SPI.begin(); 

  // Init MFRC522 
  rfid.PCD_Init(); 
  Serial.begin(115200);
  
delay(2000);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  /* Assign the api key (required) */
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  //////////////////////////////////////////////////////////////////////////////////////////////
  //Please make sure the device free Heap is not lower than 80 k for ESP32 and 10 k for ESP8266,
  //otherwise the SSL connection will fail.
  //////////////////////////////////////////////////////////////////////////////////////////////
  Firebase.begin(DATABASE_URL, API_KEY);
  //Comment or pass false value when WiFi reconnection will control by your code or third party library
 // Firebase.reconnectWiFi(true);
  Firebase.setDoubleDigits(5);
    delay(3000);
WiFi.begin(ssid, pass);
ThingSpeak.begin(client);
dht.begin();
////flamee
  pinMode (capteurdeflamme, INPUT); 


    myservo.attach(4);  // attaches the servo on pin 13 to the servo object
}
void loop()
{
  ////////////FLAMME PART
  
  
  
 int etatducapteur = digitalRead (capteurdeflamme);
  if (etatducapteur == 0){
    ThingSpeak.writeField(myChannelNumber, 3, 100, myWriteAPIKey);
    Serial.println(" IL YA FLAMME");
  }
  else
  {
       ThingSpeak.writeField(myChannelNumber, 3, 0, myWriteAPIKey);
  Serial.println("NOO FLAMME ");
  } 
  
  
  
  
  
  ///////////////////DHT11 PART
  
delay(1000);
temp = dht.readTemperature();
hum = dht.readHumidity();

Serial.print("H= ");
Serial.println(hum);
Serial.print("T= ");
Serial.println(temp);
ThingSpeak.writeField(myChannelNumber, 1, temp, myWriteAPIKey);
ThingSpeak.writeField(myChannelNumber, 2, hum, myWriteAPIKey);
 // Initialisé la boucle si aucun badge n'est présent 
  if ( !rfid.PICC_IsNewCardPresent())
  {
    return;
  }
  // Vérifier la présence d'un nouveau badge 
  if ( !rfid.PICC_ReadCardSerial())
  {
    return;
  }
  // Enregistrer l'ID du badge (4 octets) 
  for (byte i = 0; i < 4; i++) 
  {
    nuidPICC[i] = rfid.uid.uidByte[i];
  }


  // Affichage de l'ID 
  Serial.println("Un badge est détecté");
  Serial.println(" L'UID du tag est:");
  for (byte i = 0; i < 4; i++) 
  {
    Serial.print(nuidPICC[i], HEX);
    ch=ch+" "+nuidPICC[i];
    Serial.print(" ");
  
  }
  Serial.println();
  /// this code makes the rfid read the carte once (or you'll get in a loop when the card stay in front of rfid 
  // Re-Init RFID
  rfid.PICC_HaltA(); // Halt PICC
  rfid.PCD_StopCrypto1(); // Stop encryption on PCD


  
  verif=0;
  if (Firebase.ready()) 
  {
    for(byte i=1;i<5;i++)
    {
        tryy2=i;
        ch1="/table/"+tryy+tryy2;
        ch2="/names/"+tryy4+tryy2;
        Firebase.getString(fbdo, ch1);
        Firebase.getString(fbdo2, ch2);
        a=(fbdo.to<String>());
         a1=(fbdo2.to<String>());
         if (ch==a)
         {
          verif=1;
          compt++;
          
          a1=ch+"  "+a1;
          if (compt==1)
          Firebase.setString(fbdo,"/test/verif", a1);
          else if (compt==2)
        Firebase.setString(fbdo,"/test/verif1", a1);
        else if (compt==3)
        Firebase.setString(fbdo,"/test/verif2", a1);
         else if (compt==4)
          Firebase.setString(fbdo,"/test/verif3", a1);
               else if (compt==5)
               {
                         Firebase.setString(fbdo,"/test/verif4", a1);
                compt=0;
               }

          }
    }
    
   ch="";
    delay(200);

if(verif==1)
{


  
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

  Serial.println();  
  Serial.print("a:");
  Serial.print(a);

}
  Serial.println();
  Serial.println("------------------");
  Serial.println();
  delay(2500);
  }
}
