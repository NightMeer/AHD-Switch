// ----------- Pin-Definitionen -----------
const byte selectVideo = 5;  // D1 Outputschalter Coax | High = Rechts | Low = Links
const byte selectRadio = 14; // D5 Output zu Radio | High = Rückfahr-Modus an
const byte selectCamera = 12; // D6 Taster zur Kamera-Auswahl
const byte activateFront = 15; // D7 Signal zur Aktivierung der Frontkamera
const byte rearSignalIn = 4;   // D2 Eingangssignal vom Rückwärtsgang

// ----------- Variablen für die Logik -----------
bool lastSelectCameraState = LOW;
bool lastActivateFrontState = LOW;
bool selectVideoState = LOW;

// ----------- Timer-Variablen -----------
unsigned long radioSignalStopTime = 0;
const long nachlaufzeit = 10000; // 10 Sekunden in Millisekunden

void setup() {
  Serial.begin(9600);

  // Ausgänge definieren und auf Startzustand LOW setzen
  pinMode(selectVideo, OUTPUT);
  pinMode(selectRadio, OUTPUT);
  digitalWrite(selectVideo, LOW);
  digitalWrite(selectRadio, LOW);

  // Eingänge definieren
  pinMode(selectCamera, INPUT);
  pinMode(activateFront, INPUT);
  pinMode(rearSignalIn, INPUT);

  Serial.println("Setup abgeschlossen. Logik mit Timer-Verlängerung geladen.");
}

void loop() {
  // 1. Aktuellen Zustand aller Eingänge einlesen
  bool rearActive = digitalRead(rearSignalIn);
  bool frontActive = digitalRead(activateFront);
  bool cameraSelectPressed = digitalRead(selectCamera);

  // 2. Logik für Radio-Aktivierung (selectRadio) - Der Master-Schalter
  if (rearActive || frontActive) {
    digitalWrite(selectRadio, HIGH);
    radioSignalStopTime = millis(); // Nachlauf-Timer zurücksetzen
  }

  // 3. Logik für Video-Umschaltung und Timer-Verlängerung
  if (digitalRead(selectRadio) == HIGH) {
    
    // EREIGNIS 1: Front-Aktivierung wird gedrückt (setzt Video auf HIGH)
    if (frontActive && !lastActivateFrontState) {
      selectVideoState = HIGH;
      Serial.println("Front aktiviert -> Video auf HIGH gesetzt.");
    }
    // EREIGNIS 2: Kamera-Wahltaster wird gedrückt
    else if (cameraSelectPressed && !lastSelectCameraState) {
      // Aktion a): Video-Zustand immer umschalten (toggle)
      selectVideoState = !selectVideoState;
      Serial.print("Kamerataster gedrückt. Video-Status ist jetzt: ");
      Serial.println(selectVideoState ? "HIGH" : "LOW");
      
      // NEU - Aktion b): Timer verlängern, falls wir in der Nachlaufphase sind
      // Wir prüfen, ob die Haupt-Auslöser (rear, front) aus sind.
      if (!rearActive && !frontActive) {
        radioSignalStopTime = millis(); // Timer auf 10s zurücksetzen
        Serial.println("Nachlaufzeit durch Kamerataste neu gestartet!");
      }
    }
  }

  // 4. Finalen Zustand auf die Ausgänge schreiben
  digitalWrite(selectVideo, selectVideoState);

  // 5. Logik für die Master-Abschaltung (10s Nachlaufzeit des Radios)
  if (digitalRead(selectRadio) == HIGH && !rearActive && !frontActive) {
    if (millis() - radioSignalStopTime > nachlaufzeit) {
      Serial.println("Master-Nachlaufzeit beendet. Alles aus.");
      digitalWrite(selectRadio, LOW);
      digitalWrite(selectVideo, LOW);
      selectVideoState = LOW;
    }
  }

  // 6. Letzte Zustände für den nächsten Durchlauf speichern
  lastSelectCameraState = cameraSelectPressed;
  lastActivateFrontState = frontActive;
}