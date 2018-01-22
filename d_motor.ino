// do the coop door
void doCoopDoor() {

  float t1 = dht1.readTemperature();
  float t2 = dht2.readTemperature();
  Serial.println(t1);
  Serial.println(t2);

  if ((digitalRead(bypassClose) == HIGH) || (digitalRead(bypassOpen) == HIGH)); {       //coopbypass           //si y fait froid on n'ouvre pas la porte
    doCoopDoorbypass();
  }
if (((tc.month() <= 10)&&(tc.day() <= 15)) && ((tc.month() >= 4)  && (tc.day() >= 15))){  //si on est entre le 15 avril et le 15 octobre, alors ok
  if ( bypasson == 0) {                  //on n'ouvre pas la porte si la bypass est active
    if (photocellReadingLevel  == '1') {              // if it's dark
      if (photocellReadingLevel != '2') {             // if it's not twilight
        if (photocellReadingLevel != '3') {           // if it's not light
          if (digitalRead(bottomSwitchPin) == 0) {
            Serial.println("close door dark");
            closeCoopDoorMotorB();
            if (night = 0) {
              night = 1;
              morning = 0;
              }
            }
          }
        }
      }
    }
    if (photocellReadingLevel  == '3') {              // if it's light
      if (photocellReadingLevel != '2') {             // if it's not twilight
        if (photocellReadingLevel != '1') {           // if it's not dark
          if ((digitalRead(topSwitchPin) == 0)) {
            Serial.println("open door light");
            openCoopDoorMotorB();                         // o^pen the door
            if (morning = 0) {
              night = 0;
              morning = 1;
              }
            }
          }
        }
      }
    }
  }

void doCoopDoorbypass() { //fonctionne en priorité une fois actif  TESTÉ OK
  //close
  if ((digitalRead(bypassClose) == HIGH) && (digitalRead(bottomSwitchPin) == 0)) {
    delay(100);
    if ((digitalRead(bypassClose) == HIGH) && (digitalRead(bottomSwitchPin) == 0)) {   //on répète la lecture pour éviter les erreurs
      bypasson = 1;
      Serial.println("BYPASS CLOSE");
      closeCoopDoorMotorB();                      // close the door
    }
    else if ((digitalRead(bypassClose) == HIGH) && (digitalRead(bottomSwitchPin) != 0)) {
      Serial.println("BYPASS STOPPED");
      stopCoopDoorMotorB();
    }
  }
  //open
  if ((digitalRead(bypassOpen) == HIGH) && (digitalRead(topSwitchPin) == 0)) {
    delay(100);
    if ((digitalRead(bypassOpen) == HIGH) && (digitalRead(topSwitchPin) == 0)) {
      bypasson = 1;
      Serial.println("BYPASS OPEN");
      openCoopDoorMotorB();                         // open the door
    }
    else if ((digitalRead(bypassOpen) == HIGH) && (digitalRead(topSwitchPin) != 0)) {
      Serial.println("BYPASS STOPPED");
      stopCoopDoorMotorB();
    }
  }
  if ((digitalRead(topSwitchPin) != 0) || (digitalRead(bottomSwitchPin) != 0)) {
    Serial.println("limites");
    delay(100);
    stopCoopDoorMotorB();
  }
}

void bypassoff() {
  if  ((digitalRead(bypassClose) != HIGH) && (digitalRead(bypassOpen) != HIGH)) {
    delay(100);
    if  ((digitalRead(bypassClose) != HIGH) && (digitalRead(bypassOpen) != HIGH)) {
      bypasson = 0;
      Serial.println("BYPASS OFF");
    }
  }
}



void dostopdoor() {
  Serial.print("bypasson limits : ");
  Serial.println(bypasson);
  if ( doorstate == 4) {
    if ((digitalRead(topSwitchPin) != 0) || (digitalRead(bottomSwitchPin) != 0)) {
      Serial.println("limites");
      delay(200);                //modifié était à 100ms
      stopCoopDoorMotorB();
    }
  }
}

// stop the coop door motor
void stopCoopDoorMotorB() {
  digitalWrite (directionCloseCoopDoorMotorB, LOW);      // turn off motor close direction
  digitalWrite (directionOpenCoopDoorMotorB, LOW);   // turn on motor open direction
  delay(100);
  analogWrite (enableCoopDoorMotorB, 0);                 // enable motor, 0 speed
  Serial.println(" motor stopped");
  delay(100);
}

// close the coop door motor (motor dir close = clockwise)
void closeCoopDoorMotorB() {
  Serial.println(" Door Closing");
  digitalWrite (directionCloseCoopDoorMotorB, LOW);     // turn on motor close direction
  digitalWrite (directionOpenCoopDoorMotorB, HIGH);       // turn off motor open direction
  analogWrite (enableCoopDoorMotorB, 255);               // enable motor, full speed
  delay(200);  //ajouté afin de réduire l'hésitation du moteur

  if (bottomSwitchPinVal == 1) {                         // if bottom reed switch circuit is closed
    stopCoopDoorMotorB();
    Serial.println(" Coop Door Closed - no danger");
  }
}

// open the coop door (motor dir open = counter-clockwise)
void openCoopDoorMotorB() {
  Serial.println(" Door Opening");
  digitalWrite(directionCloseCoopDoorMotorB, HIGH);       // turn off motor close direction
  digitalWrite(directionOpenCoopDoorMotorB, LOW);       // turn on motor open direction
  analogWrite(enableCoopDoorMotorB, 255);                // enable motor, full speed
  delay(200);   //ajouté afin de réduire l'hésitation du moteur

  if (topSwitchPinVal == 1) {                            // if top reed switch circuit is closed
    stopCoopDoorMotorB();
    Serial.println(" Coop Door open - danger!");
  }
}

//  coop door status: red if open, green if closed

