#include <CountUpDownTimer.h> //This library contains the timer functions I'm using


//Inititalization of individual timer objects and logical variables to exit loops
CountUpDownTimer Main(DOWN);
CountUpDownTimer BurnTime(DOWN);
const int IRIDIUM_0 = 8;
const int IRIDIUM_1 = 6;
const int IRIDIUM_2 = 4;
const int NICHROME = 12;
//boolean burnComplete = false;
boolean cutdownComplete = false;

const int TPin = 16;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  Serial.print("Systems ready\n");
  pinMode(NICHROME, OUTPUT);
  pinMode(IRIDIUM_0, INPUT);
  pinMode(IRIDIUM_1, INPUT);
  pinMode(IRIDIUM_2, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Main.SetTimer(0,5,0);    //Initialize timer periods
  //BurnTime.SetTimer(0,0,10);
  //Main.SetTimer(0, 0, 10);
  BurnTime.SetTimer(0, 0, 10);
  Main.StartTimer();
  BurnTime.StartTimer();
}

void loop() {
  /* 
  This while loop is here to prevent any accidental
  misfires after the cutdown has finished.
  This way, after the cutdown finishes, the arduino
  won't be able to power the nichrome again
  */
  while (cutdownComplete == false) {

    // This section reads in the pin states from the iridium modem
    int p1 = digitalRead(IRIDIUM_0);
    int p2 = digitalRead(IRIDIUM_1);
    int p3 = digitalRead(IRIDIUM_2);

    Serial.print(Main.ShowHours());
    Serial.print(":");
    Serial.print(Main.ShowMinutes());
    Serial.print(":");
    Serial.print(Main.ShowSeconds());
    Serial.print("\n");
    Main.Timer();
    delay(1000);

    if(p1 == 1 && p2 == 0 && p3 == 0){
      cutdownComplete = burn();
      Main.StopTimer();
    }
    else if (Main.TimeCheck(0, 0, 0)) {
      Serial.print("Beginning Cutdown\n");
      cutdownComplete = burn();
    }
    /*
    Once the balloon has been in the air for a given amount
    of time (currently 1.5 hours), the arduino will output
    5v to the cutdown circuit, powering a length of
    nichrome wire wrapped around the rope, melting the rope
    in the process
    */
  }
}

bool burn(){
  Serial.print(BurnTime.ShowMinutes());
  Serial.print(":");
  Serial.print(BurnTime.ShowSeconds());
  Serial.print("\n");
  BurnTime.Timer();
  //digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(NICHROME, HIGH);
  delay(1000);

  /*
  Once the arduino has been powering the circuit for a
  period of time, it cuts power and exits all loops,
  allowing a safe descent and reducing fire hazard
  substantially
  */
  if (BurnTime.TimeCheck(0, 0, 0)) {
  Serial.print("Cutdown Complete\n");
  //burnComplete = true;
  cutdownComplete = true;
  digitalWrite(NICHROME, LOW);
  //digitalWrite(LED_BUILTIN, LOW);
  }
  return cutdownComplete;
}

