#include <DHT.h>      //sensor temp et humidity

#define DHT1PIN A1     // what pin we're connected to
#define DHT2PIN A2
#define DHT1TYPE DHT11   // DHT 11 
DHT dht1(DHT1PIN, DHT1TYPE);
DHT dht2(DHT2PIN, DHT1TYPE);

#include <SimpleTimer.h>                     // load the SimpleTimer library to make timers, instead of delays & too many millis statements
#include <LCD.h>
#include <LiquidCrystal_I2C.h>                   // load the liquid crystal library
#include <SPI.h>       //data logger
#include <SD.h>       //data logger
#include <RTClib.h>

File myFile;
int pinCS = 53;   // Pin 53 on Arduino Mega
// Pin 10 on Arduino Uno
#include <OneWire.h>
#include <Wire.h>
#include <RTClib.h>  // for real time clock
RTC_DS3231 rtc;
//RTC_DS1307 RTC;

//LCD
#define I2C_ADDR    0x27
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

DateTime tc;  //variable of type DateTime
const char daysOfTheWeek[7][10] PROGMEM = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const char months[12][11] PROGMEM = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
char buffer[3]; //char buffer used for simple formatting of output, so minutes and seconds are always shown with two digits

char version[ ] = "Coop V6";  
//4.1 ajusté délais sensor de lumière
//4.2 ajusté niveau de lumière
//4.3 ajout de delais sur les switch de bypass
//Modifié conditions de lumière à la noirceur
//5.0 ajouté fonction bypass lumière (non installée)
//5.1 réajustement fonction doCoopInteriorLightDusk()
//   .2 modifié if pour doCoopInteriorLightDusk()
//5.2 ajouté delay dans stop motor pour tenter d'arrêter le movement excessif du moteur
//    alongé le delais entre les mesures de light sensor 25 minutes
//  .21 ajouté condition pour la fan : porte fermée seulement
//5.3 ajout fonction de la tour de bouffe
//5.4 ajouté un délas au twilight 10 minutes afin de rendre la fermeture moins rapide
//5.41 alongé le délais au twilight et changé le délais de sensor de lumière (retour à la normale 120000)
//5.5 ajout d'un délais dans openCoopDoorMotorB() et closeCoopDoorMotorB() pour enlever l'hésitation du moteur
//5.6 ajout RTC et log SD
//5.65 ajout dans doCoopDoorLed() pour faire flasher les led de porte si la bypass est active
//5.7 ajout du code de température d'eau
     //ajout fonction de test du RTC et carte SD
//5.8 correction de la fonction heat qui ne détectait pas la température
    //.85  ajout LCD serie
    //5.855 correction d'erreurs , menus, alarmes, sensors
//5.9 ajout une condition dans l'ouverture de la porte (pas entre 15 oct et 15avril
//5.95 ajout d'un log avec timer pour enregistrer tout sur des lignes différentes
//6 vérification fonctionnement du protoboard :
        //ajout condition dans l'annulation d'alarme, pour éviter que autre efface une alarme    
        //mise en format des données sur le LOG SD
        //retrait de la lecture de la carte sd au début
        //retrait de la fonction   doCoopHVAHumidity(); (inutile)

const boolean SerialDisplay = false;

// pins assignments
const int photocellPin = A15;                 // photocell connected to analog 15
const int photocelltourPin = A14;                 // photocell connected to analog 15

const int chipSelect = 53;                    //SD card
//digital pin 6 pour sensor temp d'eau
const int relayHeat2 = 11;                   // chauffage additionnel en dessous de l'eau ou dans l'eau
const int relayHeat = 12;      //old4               // heat lamp relay set to digital pin 5
const int relayFan = 10;       //old3               // exhaust fan relay set to digital pin 6
const int relayInteriorLight = 9;  //old45         // interior lights relay set to digital pin 45
const int enableCoopDoorMotorB = 5;          // enable motor b - pin 5
const int directionCloseCoopDoorMotorB = 22;  // direction close motor b - pin 2
const int directionOpenCoopDoorMotorB = 23;   // direction open motor b - pin 23
const int bottomSwitchPin = 35;              // bottom switch is connected to pin 34
const int topSwitchPin = 34;                 // top switch is connected to pin 35
const int coopDoorOpenLed = 40;              // led set to digital pin 40
const int coopDoorClosedLed = 41;            // led set to digital pin 41
const int bypassOpen = 32;                   // from switch bypass
const int bypassClose = 33;                  // from switch bypass
const int bypassLight = 34;                  // from switch bypass light

//AJOUT
const int waterlevel = 30;
const int foodLed = 42;
//sensor de temperature type ds18b20
#define ONE_WIRE_BUS 6  // on pin 6 (a 4.7K resistor is necessary)
#include <DallasTemperature.h>
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//delay light hiver
const int lightON = 7 ;    // Hour when Light will turn ON (24 hr format NO zeros before single digits)
const int lightOFF = 21 ;  // Hour when Light will turn OFF (24 hr format NO zeros before single digits)
unsigned long lasttimelightON  = 0;
unsigned long lasttimelightOFF = 0;

// variables

float t1 = dht1.readTemperature();   // in
float t2 = dht2.readTemperature();    // out
float h1 = dht1.readHumidity();
float h2 = dht2.readHumidity();
// photocell
int photocellReading;                // analog reading of the photocel
int photocellReadingLevel;           // photocel reading levels (dark, twilight, light)
// photocelltour
int photocellReadingtour;            // analog reading of the photoceltour
int photocellReadingLeveltour;       // photocel reading levels (full,empty)
float morning;         //statut du matin, permet de prendre en note l'heure du lever du soleil (au moment que le systeme ouvre la porte par le fait même)
float night;           //statut du soir, permet de prendre en note l'heure du coucher du soleil
// reed switches top and bottom of coop door
int doorstate;         //status de la porte, ouverte, fermee, en mouvement, en probleme
int alarmstate;       //code d'alarme
int alarmcount;
// bypass val
int bypassOpenVal;
int bypassCloseVal;
float bypasson;        //statut actif de la bypass de porte

// bypass light val
int bypassLightVal;
float bypasslight;

// top switch
int topSwitchPinVal;                   // top switch var for reading the pin status
int topSwitchPinVal2;                  // top switch var for reading the pin delay/debounce status
int topSwitchState;                    // top switch var for to hold the switch state

// bottom switch
int bottomSwitchPinVal;                // bottom switch var for reading the pin status
int bottomSwitchPinVal2;               // bottom switch var for reading the pin delay/debounce status
int bottomSwitchState;                 // bottom switch var for to hold the switch state

// SimpleTimer objects
SimpleTimer coopPhotoCellTimer;
SimpleTimer coopPhotoCellTimerTour;
SimpleTimer coopLogOnSDtimer;
SimpleTimer coopHEATtimer;
SimpleTimer coopCOOLtimer;

// temperature check delay
long lastTempCheckTime = 0;
//long TempCheckDelay = 10000;           // valeur test
long TempCheckDelay = 600000;           // 10 minutes

// humidity check delay
long lastHumCheckTime = 0;
//long HumCheckDelay = 10000;           // valeur test
long HumCheckDelay = 600000;           // 10 minutes

// lcd
LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

// ************************************** the setup **************************************


void setup() {
  sensors.begin();  //pour le sensor de température dans l'eau
  lcd.begin (16,2); //  start le LCD (nombre de (colonne,ligne))
  //setup RTC
//  if (! rtc.begin()) {
//    Serial.println( F("Couldn't find RTC") );  // using F() saves RAM - if text is longer than 4 characters
//    while (1);  }  // sketch halts in an endless loop
//  if (! rtc.isrunning()) {
//    Serial.println( F("RTC is NOT running!") );
   //set the RTC date & time - to the time this sketch was compiled
//   rtc.adjust(DateTime(2018, 12, 12, 17, 9, 0));//}
    rtc.adjust(DateTime(__DATE__, __TIME__)); // }
  //setup RTC END

  // Switch on the backlight
lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
lcd.setBacklight(HIGH);
//lcd.home (); // go home
//lcd.print("SainSmartI2C16x2");  
//lcd.setBacklight(HIGH);     // Backlight on

  //AJOUT
  pinMode(foodLed, OUTPUT);
  digitalWrite(foodLed, HIGH);
  
  pinMode(relayHeat, OUTPUT);   //set heat lamp relay output
  pinMode(relayHeat2, OUTPUT);   //set heat lamp relay output
  pinMode(relayFan, OUTPUT);    //set exhaust fan relay output
  digitalWrite(relayHeat, HIGH);
  digitalWrite(relayHeat2, HIGH);  
  digitalWrite(relayFan, HIGH);
  pinMode (enableCoopDoorMotorB, OUTPUT);           // enable motor pin = output
  pinMode (directionCloseCoopDoorMotorB, OUTPUT);   // motor close direction pin = output
  pinMode (directionOpenCoopDoorMotorB, OUTPUT);    // motor open direction pin = output
  pinMode (coopDoorOpenLed, OUTPUT);                // enable coopDoorOpenLed = output
  pinMode (coopDoorClosedLed, OUTPUT);              // enable coopDoorClosedLed = output
  digitalWrite(coopDoorOpenLed, HIGH);
  digitalWrite(coopDoorClosedLed, HIGH);
  pinMode(bottomSwitchPin, INPUT);                  // set bottom switch pin as input
  pinMode(topSwitchPin, INPUT);                     // set top switch pin as input

  // interior lights relay
  pinMode(relayInteriorLight, OUTPUT);
  digitalWrite(relayInteriorLight, HIGH);

  // timed actions setup
  coopPhotoCellTimer.setInterval(1100000, readPhotoCell);   // read the photocell every 20 minutes
  coopPhotoCellTimerTour.setInterval(4000, readPhotoCellTour);   // read the photocell every 4 secondes TEST
  coopLogOnSDtimer.setInterval(120000, checkRTCsd); // check sd card 120000 = 2 minutes
  coopHEATtimer.setInterval(360000, doCoopHVACHeat); // 360000 = 6 minutes
  coopCOOLtimer.setInterval(360000, doCoopHVACCool); // 360000 = 6 minutes
//  coopHEATtimer.setInterval(4000, doCoopHVACHeat); // 360000 = 6 minutes
//  coopCOOLtimer.setInterval(4000, doCoopHVACCool); // 360000 = 6 minutes  

  Serial.begin(9600); // initialize serial port hardware
  Serial.println("Version : ");
  Serial.print(version);
  Serial.println(" Processes running:");
  Serial.print("Initializing SD card...");

  //SD Card
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;  }
  Serial.println("card initialized.");

/*myFile = SD.open("test.txt", FILE_WRITE);
   if the file opened okay, write to it:
  if (myFile) {
  Serial.println("Writing to file...");
   Write to file
  myFile.println("Testing text 1, 2 ,4...");
  myFile.close(); // close the file
  Serial.println("Done.");
  }
   if the file didn't open, print an error:
  else {
    Serial.println("error opening test.txt");
  }
   Reading the file

  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("Read:");
    // Reading the whole file
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close();  }
  else {  Serial.println("error opening test.txt");  }
*/

  dht1.begin();
  dht2.begin();
  //    lcd.begin(16, 2);
  //   clearPrintTitle();
  bypasson = 0;
  bypasslight = 0; 
  
 
}

// ************************************** functions **************************************

void doCoopDoorLed() {
  if (bypasson = 1) {
    if (digitalRead(coopDoorClosedLed) == LOW) {
      delay(500);
      digitalWrite(coopDoorClosedLed, HIGH);     }
    else if (digitalRead(coopDoorClosedLed) == HIGH) {
      delay(500);
      digitalWrite(coopDoorClosedLed, LOW);    }
    else if (digitalRead(coopDoorOpenLed) == LOW) {
      delay(500);
      digitalWrite(coopDoorOpenLed, HIGH);    }
    else if (digitalRead(coopDoorOpenLed) == HIGH) {
      delay(500);
      digitalWrite(coopDoorOpenLed, LOW);    } }
 
  if ((digitalRead(bottomSwitchPin) != 0) && (digitalRead(topSwitchPin) == 0)) {                         // if bottom reed switch circuit is closed
    digitalWrite (coopDoorClosedLed, LOW);              // turns on coopDoorClosedLed (green)
    digitalWrite (coopDoorOpenLed, HIGH );                 // turns off coopDoorOpenLed (red)
    doorstate = 1;  } //Closed
  else if ((digitalRead(topSwitchPin) != 0) && (digitalRead(bottomSwitchPin) == 0)) {                       // if top reed switch circuit is closed
    digitalWrite (coopDoorClosedLed, HIGH);               // turns off coopDoorClosedLed (green)
    digitalWrite (coopDoorOpenLed, LOW);               // turns on coopDoorOpenLed (red)
    doorstate = 2;   } // Open
  else if ((digitalRead(topSwitchPin) != 0) && (digitalRead(bottomSwitchPin) != 0)) {
    digitalWrite (coopDoorClosedLed, LOW);              // turns off coopDoorClosedLed (green)
    digitalWrite (coopDoorOpenLed, LOW);                // turns off coopDoorOpenLed (red)
    doorstate = 3;   } //problem
  else if ((digitalRead(topSwitchPin) == 0) && (digitalRead(bottomSwitchPin) == 0)) {
    digitalWrite (coopDoorClosedLed, HIGH);              // turns on coopDoorClosedLed (green)
    digitalWrite (coopDoorOpenLed, HIGH);                // turns on coopDoorOpenLed (red)
    doorstate = 4;   }  //moving
}

void doInteriorLight() {
  if (digitalRead(bypassLight) == 0) {   //pourrait être inversé... a tester
    delay(200);
    if (digitalRead(bypassLight) == 0) {
      digitalWrite (relayInteriorLight, LOW);  //light on sur LOW
      Serial.println("Interior Light On switch");
      bypasslight = 1;  //switch on
}  }
  else if (digitalRead(bypassLight) != 0) {       //pourrait être inversé... a tester
    delay(200);
    if (digitalRead(bypassLight) != 0) {
      digitalWrite (relayInteriorLight, HIGH ); //light off sur HIGH
      bypasslight = 0;  //switch off
}  }}

void doCoopInteriorLightDusk() {
  Serial.println("doCoopInteriorLightDusk ");
  Serial.println("doorstate : ");
  Serial.println(doorstate);
  if ((bypasslight != HIGH) && (doorstate = 2) && (photocellReadingLevel == '2') ) {    //verifie que la switch n'est pas active  & la porte est ouverte
    digitalWrite (relayInteriorLight, LOW);     //light on sur LOW
    Serial.println(" Interior Light: On : twilight");
    Serial.println(" delay 20 minutes");
    delay(1700000);
  }
  else if ((bypasslight != HIGH) && (doorstate = 1)) {    //verifie que la switch n'est pas active  & la porte est fermée
    digitalWrite (relayInteriorLight, HIGH);      //light off sur HIGH
    Serial.println(" Interior Light: Off");
} }

void lightupwinter() {
  tc = rtc.now(); // read the time from the DS3231 RTC chip
 // Serial.print("winter light on");
 // Serial.println(lightON);
 // Serial.print("winter light off");
 // Serial.println(lightOFF);
 // Serial.print("tc.hour()");
 // Serial.println(tc.hour());
  if (lightOFF > lightON) {   // Same Day
    if (tc.hour() >= lightON && (tc.hour ()) <= lightOFF - 1)  {
      // if current hour is equal or greater than ON time AND equal to or less than OFF time minus 1 then turn the   lights ON (by turning pins LOW because of using LOW level Relays)   Minus 1 from OFF time because we are not using Minutes here   For Example turn off time is "16" (4 PM) the hour number will be "16" from   16:00 till 16:59 (4:00 till 4:59)
      digitalWrite(relayInteriorLight, LOW);
      Serial.println("winter light on");
      //   Alarm.delay (1000);
      ////     LightStatus = 1 ; 
      }
    else {
      digitalWrite(relayInteriorLight, HIGH);
      Serial.println("winter light off");
      //   LightStatus = 0 ;
} } }

// ************************************** the loop **************************************

void loop() {
  delay(1000);
  menu();
  coopPhotoCellTimer.run();           // timer for readPhotoCell
  // coopPhotoCellTimerTour.run();    // timer for readPhotoCellTour
  coopLogOnSDtimer.run();             //check RTC
  coopHEATtimer.run();                //check si besoin de chauffage
  coopCOOLtimer.run();                //check si besoin de ventillation

  dostopdoor();
  bypassoff();
  doCoopDoorLed();
  doCoopDoor();

  //AJOUT
  checkwater(); //check le niveau d'eau
  sensorwatertemp();
  //  doInteriorLight();  //la bypass
  //  doCoopInteriorLightDusk();   //for summer
  lightupwinter();  //winter
  alarmstateled();

}


