
void  sensorwatertemp() {    //testé fonctionne

  sensors.requestTemperatures(); // Send the command to get temperature readings
  Serial.print("Temperature is: ");
  Serial.println(sensors.getTempCByIndex(0));
  if (sensors.getTempCByIndex(0) <= 1.05) {
    Serial.println("too cool! ");
    alarmstate =1;
  }
  else if (sensors.getTempCByIndex(0) > 1.05) {
    Serial.println("perfect! ");
    if (alarmstate == 1) { 
    alarmstate =0;
    }
} }


//même alarme que l'Eau
void readPhotoCellTour() { // function to be called repeatedly - per coopPhotoCellTimer set in setup
  digitalWrite (foodLed, HIGH); //pour allumer la led dans la tour
  delay(200);

  photocellReadingtour = analogRead(photocelltourPin);
  Serial.print(" photocell tour Analog Reading = ");
  Serial.println(photocellReadingtour);

  //  set photocel threshholds
  if (photocellReadingtour >= 0 && photocellReadingtour <= 10) {
    photocellReadingLeveltour = '1';
    Serial.print(" Full");
        if (alarmstate == 2) { 
    alarmstate =0;
    digitalWrite (foodLed, LOW); // pour éteindre la led
  }}
  else if (photocellReading  >= 11 && photocellReading <= 99) {
    photocellReadingLeveltour = '2';
    Serial.print(" Photocel Reading Level:");
    Serial.println(" - Empty");
    digitalWrite (foodLed, LOW); // pour éteindre la led
    alarmstate =2;
  }
}

// photocel to read levels of exterior light
void readPhotoCell() { // function to be called repeatedly - per coopPhotoCellTimer set in setup
  photocellReading = analogRead(photocellPin);
  Serial.print(" Photocel Analog Reading = ");
  Serial.println(photocellReading);
  //  set photocel threshholds
  if (photocellReading >= 0 && photocellReading <= 0) {
    photocellReadingLevel = '1';
    Serial.print(" Photocel Reading Level:");
    Serial.println(" - Dark");   }
  else if (photocellReading  >= 4 && photocellReading <= 99) {
    photocellReadingLevel = '2';
    Serial.print(" Photocel Reading Level:");
    Serial.println(" - Twilight");  }
  else if (photocellReading  >= 100 ) {
    photocellReadingLevel = '3';
    Serial.print(" Photocel Reading Level:");
    Serial.println(" - Light");
} }

// credit: bildr.org/2011/07/ds18b20-arduino/
// DS18S20 digital thermometer stuff

float getTemp() {                //returns the temperature from one DS18S20 in DEG Celsius
  float t1 = dht1.readTemperature();
  float t2 = dht2.readTemperature();
  float h1 = dht1.readHumidity();
  float h2 = dht2.readHumidity();
    Serial.print("Temperature IN= ");
    Serial.println(t1);
    Serial.print("Humidity IN= ");
    Serial.println(h1);
    Serial.print("Temperature OUT = ");
    Serial.println(t2);
    Serial.print("Humidity OUT= ");
    Serial.println(h2);
    //delay(1000);
  return t1;
  return h1;
  return t2;
  return h2;
}


void checkwater() {
  if  (digitalRead(waterlevel) != 1) {   //les états peuvent différer
    Serial.print(" Verification du Niveau d'eau");
    Serial.println(" - Plein");
  if (alarmstate == 2) { 
    alarmstate =0;
  }}
  else if (digitalRead(waterlevel) == 1) {
    Serial.print(" Verification du Niveau d'eau");
    Serial.println(" - Vide");
    alarmstate = 2;
    digitalWrite (coopDoorClosedLed, LOW);              // turns on coopDoorClosedLed (green)
    digitalWrite (coopDoorOpenLed, LOW );
    delay(500);
    digitalWrite (coopDoorClosedLed, HIGH);              // turns on coopDoorClosedLed (green)
    digitalWrite (coopDoorOpenLed, HIGH );
    delay(500);
    digitalWrite (coopDoorClosedLed, LOW);              // turns on coopDoorClosedLed (green)
    digitalWrite (coopDoorOpenLed, LOW );
    delay(500);
    digitalWrite (coopDoorClosedLed, HIGH);              // turns on coopDoorClosedLed (green)
    digitalWrite (coopDoorOpenLed, HIGH );
    delay(500);    
  }
}
