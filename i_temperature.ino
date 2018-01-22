// coop heat evac
void doCoopHVACHeat() { //remplace la fonction d'origine
  sensors.requestTemperatures(); // Send the command to get temperature readings
  Serial.print("Temperature is: ");
  Serial.println(sensors.getTempCByIndex(0));
  delay(1000);
  if (sensors.getTempCByIndex(0) <= 5.00) {
      digitalWrite(relayHeat2, LOW);
      Serial.println(" Coop heat2 ON: by EAU"); 
  }
    else if (sensors.getTempCByIndex(0) > 5.00) {
      Serial.println("perfect! ");
      digitalWrite(relayHeat2, HIGH);
      Serial.println(" Coop heat2 OFF: by EAU");      
  }
   float t1 = dht1.readTemperature();
   delay(300);
    // if cold, turn on heat lamps
   if (t1 <= 9.00) {                                      // if temp drops below 40F turn on heat lamp(s) relay
      digitalWrite(relayHeat, LOW);
      Serial.println(" Coop heat ON: by T1");             // print out coop temperature
    }
    else if (t1 > 9.00) {
      digitalWrite(relayHeat, HIGH);                        // if temp remains above 40F turn off heat lamp(s) relay
      Serial.println(" Coop heat OFF: by T1"); 
}  }

// if too hot, turn on cooling fans
void doCoopHVACCool() {  //trop chaud
    Serial.print(" Coop Temperature:");             // print out coop temperature
    float t1 = dht1.readTemperature();
    delay(300);
    Serial.println(t1);                          // print out the temperature
    if ( doorstate == 1) {   //vérifie que la porte n'est pas ouverte
      if (t1 >= 20) {                                      // if temp rises above 85F turn on cooling fan(s) relay
        digitalWrite(relayFan, LOW);
        Serial.println(" Coop vent ON: too hot");}
      else if (t1 < 20) {
        digitalWrite(relayFan, HIGH);
        Serial.println(" Coop vent OFF: pas trop chaud");
  }  }
    else if ( doorstate != 1){
      Serial.println(" Coop Door open, pas besoin");
      digitalWrite(relayFan, HIGH);
      }
}

