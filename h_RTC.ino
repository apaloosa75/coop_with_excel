void TestRTC(){
  if (! rtc.isrunning()) {
    Serial.println( F("RTC is NOT running!") );
}}

void logeverything() {//ajouter un led qui indique quand la carte est en écriture pour viter de scraper des cartes sd
TestRTC();
   myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {
    myFile.println("  ");
    myFile.print(tc.year(), DEC);
    myFile.print(" / ");
    myFile.print(tc.month(), DEC);
    myFile.print(" / ");
    sprintf(buffer, "%02d", tc.day());
    myFile.print(buffer);
    myFile.print("   ,");   //ajout du ,
    myFile.print(tc.hour(), DEC);
    myFile.print(" : ");
    sprintf(buffer, "%02d", tc.minute());
    myFile.print(buffer);
    myFile.print(" : ");
    sprintf(buffer, "%02d", tc.second());
    myFile.print(buffer);
      photocellReading = analogRead(photocellPin);
      myFile.print(",");
   myFile.print(photocellReading);     
  sensors.requestTemperatures(); // Send the command to get temperature readings
  myFile.print(",");
  myFile.print(sensors.getTempCByIndex(0));


  float t1 = dht1.readTemperature();
  float t2 = dht2.readTemperature();
  float h1 = dht1.readHumidity();
  float h2 = dht2.readHumidity();
    myFile.print(",");
    myFile.print(t1);
    myFile.print(",");
    myFile.print(h1);
    myFile.print(",");
    myFile.print(t2);
    myFile.print(",");
    myFile.print(h2);
   myFile.print(",");
  if ((digitalRead(bottomSwitchPin) != 0) && (digitalRead(topSwitchPin) == 0)) { 
    myFile.print("porte fermee");  } //Closed
  else if ((digitalRead(topSwitchPin) != 0) && (digitalRead(bottomSwitchPin) == 0)) { 
    myFile.print("porte ouverte");   } // Open
  else if ((digitalRead(topSwitchPin) != 0) && (digitalRead(bottomSwitchPin) != 0)) {             
    myFile.println("porte en probleme");   } //problem
  else if ((digitalRead(topSwitchPin) == 0) && (digitalRead(bottomSwitchPin) == 0)) {               
    myFile.print("porte en mouvement");   }  //moving
  myFile.print(",");
   if (digitalRead(relayInteriorLight) == 0) { //pourrait être inversé
    myFile.print("lumiere allumee");}
   else if (digitalRead(relayInteriorLight) != 0){
    myFile.print("lumiere fermee");}
  myFile.print(",");     
  if (alarmstate == 0) {        
    myFile.print("Aucune alarme");  }                
  else if (alarmstate == 1){     
    myFile.print("Temperature eau basse");  }         
  else if (alarmstate == 2) {    
   myFile.print("Ajout d'eau");  }    
  else if (alarmstate == 3) {     
    myFile.print("RTC"); } 
  else if (alarmstate == 4) {    // if coop door bottom switch is ope 
    myFile.print("SD Card");}
  myFile.print(",");    
  if (digitalRead(relayHeat) == 0) { 
    myFile.print("Heat ON");  } 
    else if (digitalRead(relayHeat) != 0){
    myFile.print("Heat OFF");  }
      myFile.print(",");
  if (digitalRead(relayHeat2) == 0) { 
    myFile.print("Heat2 ON");  }
    else if (digitalRead(relayHeat2) != 0){
    myFile.print("Heat2 OFF");  }
      myFile.print(",");
   if (digitalRead(relayFan) == 0) { //pourrait être inversé
    myFile.print("Fan allumee");}
   else if (digitalRead(relayFan) != 0){
    myFile.print("Fan fermee");}
 
    Serial.println(" file mis a jour");
    myFile.close(); // close the file
delay (100);}
}


