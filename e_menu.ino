void menu(){
//   Serial.println(tc.second());
   if ((tc.second() >0) && ( tc.second() <= 59)){
   
      if ((tc.second() > 0)&& ( tc.second() <= 9)){
      menu1();  }
      else if((tc.second() > 9)&& ( tc.second() <= 18)) {
      menu2();  } 
      else if((tc.second() > 18)&& ( tc.second() <= 27)) {
      menu3();  } 
      else if((tc.second() > 27)&& ( tc.second() <= 36)) {
      menu4();  } 
      else if((tc.second() > 36)&& ( tc.second() <= 45)) {
      menu5();  }
      else if((tc.second() > 45)&& ( tc.second() <= 54)) {
      menu6();  } 
      else if((tc.second() > 54)&& ( tc.second() <= 59)) {
      menu7();  //la où la carte sd va tout écrire
      }
    }
  if ( tc.second() > 59){
    mennuerreur();
  }
}

void mennuerreur() {
  lcd.setCursor (0,0);
 lcd.print(tc.month(),DEC);
 lcd.print("/");  
 lcd.print(tc.day(),DEC);
 lcd.print("      ");
 lcd.print(tc.hour(),DEC);
 lcd.print(":");
 if ( tc.minute() < 10){
   lcd.print("0"); }
 lcd.print(tc.minute(),DEC);
 
 lcd.setCursor (0,1);
 lcd.print("erreur temps");
}


void menu1()  {    //date et heure + état de la porte
  lcd.setCursor (0,0);
//date et heure
 if ( tc.month() < 10){
   lcd.print("0"); }
 lcd.print(tc.month(),DEC);
 lcd.print("/");  
  if ( tc.day() < 10){
   lcd.print("0"); }
 lcd.print(tc.day(),DEC);
 lcd.print("      ");
 lcd.print(tc.hour(),DEC);
 lcd.print(":"); 
 if ( tc.minute() < 10){
   lcd.print("0"); }
 lcd.print(tc.minute(),DEC);
 
//état de la porte
    lcd.setCursor (0,1);
  if (doorstate == 1) {         // if coop door bottom switch is closed
    lcd.print("Closed");  }                // display "Closed"
  else if (doorstate == 2){     // if coop door bottom switch is open
    lcd.print("Open");  }       // display "Open"  
  else if (doorstate == 3) {    // if coop door bottom switch is open
    lcd.print("PROBLEM");  }    // display "Problem"
  else if (doorstate == 4) {     // if coop door bottom switch is open
    lcd.print("Moving");  // display "Moving"
  delay (100);
}}

void menu2(){   //température intérieure  +  //humidité intérieure
  lcd.clear();
  lcd.setCursor (0,0);
//température intérieure
  float t1 = dht1.readTemperature();
  delay(500);
  lcd.print (t1) ;                    // show temperature of interior coop
  lcd.print("C");                        // show "C" 
  lcd.print ("   INT");
  
  lcd.setCursor (0,1);  
//humidité intérieure
  float h1 = dht1.readHumidity();
  delay(500);
 lcd.print (h1) ;                    // show temperature of interior coop
  lcd.print("%");    
}

void menu3(){   //température extérieure + humidité extérieure
  lcd.clear();
  lcd.setCursor (0,0);
//température extérieure
  float t2 = dht2.readTemperature();
    delay(500);
  lcd.print (t2) ;                    // show temperature of extérieure coop
  lcd.print("C");                        // show "C" 
  lcd.print ("   EXT");
  
  lcd.setCursor (0,1);  
//humidité extérieure
  float h2 = dht2.readHumidity();
    delay(500);
 lcd.print (h2) ;                    // show temperature of extérieure coop
  lcd.print("%");    
}

void menu4(){  //température de l'eau  +  dernière alarme d'eau (en heures)
//température de l'eau
//  digitalWrite(relayHeat, HIGH);
  lcd.clear();
  sensors.requestTemperatures();
    delay(500);
  lcd.print("Temp. eau : ");
  lcd.print(sensors.getTempCByIndex(0));
//dernière alarme d'eau (en heures)
//  digitalWrite(relayHeat, LOW);
}

void menu5(){  //dernière alarme de nourriture en (heure)  +  niveau numérique de lumière ext
  lcd.clear();
//dernière alarme de nourriture en (heure)
//niveau numérique de lumière ext
  photocellReading = analogRead(photocellPin);
   lcd.print("Photocell = ");
   lcd.print(photocellReading);
  delay (100);
}

void menu6(){   //version du logiciel  + nombres d'heure depuis dernier reset
  lcd.clear();
  lcd.setCursor (0,0);
//version du logiciel
 // lcd.print ("Version : "); 
    lcd.print(version);

//alarme présente
    lcd.setCursor (0,1);
  if (alarmstate == 0) {        
    lcd.print("Aucune");  }                
  else if (alarmstate == 1){     
    lcd.print("Temperature eau");  }         
  else if (alarmstate == 2) {    
    lcd.print("Ajout d'eau");  }    
  else if (alarmstate == 3) {     
    lcd.print("RTC"); } 
  else if (alarmstate == 4) {    // if coop door bottom switch is ope 
    lcd.print("SD Card");} 
    delay (100);
}

void menu7(){
  lcd.clear();

}
