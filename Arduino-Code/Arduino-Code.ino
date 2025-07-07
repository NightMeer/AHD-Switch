const byte outputOnePin = 5; //D1

const byte outputTwoPin = 14; //D5 Temp Vorne
const byte outputThreePin = 16; //D0 Temp Hinten

const byte inputOnePin = 12; //D6 Knopf
const byte inputTwoPin = 13; //D7 Knopf
const byte inputThreePin = 4; //D2 Rückfahr

const unsigned long multi = 1000;
const unsigned long seconds  = 10;

unsigned long duration = multi * seconds;
unsigned long startMillis = 0;

bool functionActive = false;
bool buttonPressed = false;
bool camera = false; //Vorne = True, Heck = Fals

void setup() {
  Serial.begin(9600);
  pinMode(outputOnePin, OUTPUT);
  pinMode(outputTwoPin, OUTPUT);
  pinMode(outputThreePin, OUTPUT);

  pinMode(inputOnePin, INPUT);
  pinMode(inputTwoPin, INPUT);
  pinMode(inputThreePin, INPUT);

  selectCam();
}

void loop() {
  if (digitalRead(inputOnePin) == HIGH || digitalRead(inputThreePin) == HIGH) {
    if (!functionActive) {
      Serial.println("Taster gedrückt");
      if(digitalRead(inputOnePin) == HIGH){
        camera = true; //Vorne = True, Heck = Fals
      }else if (digitalRead(inputThreePin) == HIGH){
        camera = false; //Vorne = True, Heck = Fals
      }
    }
    functionActive = true;
    startMillis = millis();
  }




  if (functionActive && (millis() - startMillis >= duration)) {
    functionActive = false; // Funktion beenden
    digitalWrite(outputOnePin, LOW);
    camera = false; //Vorne = True, Heck = Fals
    Serial.println("Funktion gestoppt.");
    // Hier können Sie zusätzlichen Code einfügen, der einmal beim Beenden der Funktion ausgeführt werden soll
  }

  if (functionActive) {

    if (digitalRead(inputTwoPin) == HIGH && !buttonPressed){
      buttonPressed = true;
      if (camera){
        camera = false; //Vorne = True, Heck = Fals
      }else{
        camera = true; //Vorne = True, Heck = Fals
      }

    }else if (digitalRead(inputTwoPin == LOW)){
      buttonPressed = false;
    }

    digitalWrite(outputOnePin, HIGH); // LED einschalten (oder andere Aktionen)
    selectCam();
    delay(100);


  }

}


void selectCam(){
    if (camera){
      digitalWrite(outputTwoPin, HIGH);
      digitalWrite(outputThreePin, LOW);
    }else{
      digitalWrite(outputTwoPin, LOW);
      digitalWrite(outputThreePin, HIGH);
    }
}