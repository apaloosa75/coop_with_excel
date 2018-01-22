//int alarmstate;      code d'alarme
//int alarmcount;      compte le nombre de fois que les leds flashent
// 0 = none
// 1 = temperature eau
// 2 = niveau d'eau
// 3 = RTC
// 4 = SD card

void flashleds(){
        digitalWrite (coopDoorClosedLed, LOW);              // turns on coopDoorClosedLed (green)
        digitalWrite (coopDoorOpenLed, LOW );
        delay(500);    
        digitalWrite (coopDoorClosedLed, HIGH);              // turns on coopDoorClosedLed (green)
        digitalWrite (coopDoorOpenLed, HIGH );
        alarmcount = (alarmcount+1); 
        }

void alarmstateled(){
   if (alarmstate == 1){     
      if (alarmcount < 1){
        flashleds();      }
      else if (alarmcount == 1){
        delay(1000);
        alarmcount = 0;}   } 
  else if (alarmstate == 2) {   
      if (alarmcount < 2){
        flashleds();     }
      else if (alarmcount == 2){
        delay(1000);
        alarmcount = 0; }   } 
  else if (alarmstate == 3) {     
      if (alarmcount < 3){
        flashleds();        }
      else if (alarmcount == 3){
        delay(1000);
        alarmcount = 0; }      }
  else if (alarmstate == 4) {     
      if (alarmcount < 4){
        flashleds();      }
      else if (alarmcount == 4){
        delay(1000);
        alarmcount = 0;  }     }
}

void checkRTCsd() {
  //function to be called sometimes to verifie stuff : RTC, SD card
    logeverything();
}

