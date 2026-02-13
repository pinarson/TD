//
// Parse incoming messages consisting of 2 decimal values followed by a carriage return
//  Exemple : msg = (str(channel.index) +' '+ str(val) )
//  In TouchDesigner:    op('serial1').send( msg, terminator='\n') 
//
#include <Servo.h>

static const uint8_t N = 4;
static const uint8_t pins[N] = {3, 5, 6, 9}; // à adapter

Servo servos[N];

float target[N]   = {90, 90, 90, 90};
float filtered[N] = {90, 90, 90, 90};

const float alpha = 0.25f;          // lissage (0 = figé, 1 = brut)
const uint32_t failsafeMs = 500;    // si plus de data -> position safe
uint32_t lastRx = 0;

String rx; // OK pour petites trames; sinon buffer char[]

void setup() {
  Serial.begin(115200);
  for (uint8_t i = 0; i < N; i++) {
    servos[i].attach(pins[i]);
    servos[i].write((int)filtered[i]);
  }
  lastRx = millis();
}

bool parseFrame(const String& s) {
  // attend "<a,b,c,d>"
  if (s.length() < 3) return false;
  if (s[0] != '<' || s[s.length()-1] != '>') return false;

  String payload = s.substring(1, s.length()-1);

  int values[N];
  uint8_t idx = 0;

  int start = 0;
  while (idx < N) {
    int comma = payload.indexOf(',', start);
    String token = (comma == -1) ? payload.substring(start) : payload.substring(start, comma);
    token.trim();
    if (token.length() == 0) return false;

    values[idx] = token.toInt();
    idx++;

    if (comma == -1) break;
    start = comma + 1;
  }

  if (idx != N) return false;

  for (uint8_t i = 0; i < N; i++) {
    target[i] = constrain(values[i], 0, 180);
  }

  lastRx = millis();
  return true;
}

void loop() {
  // Lecture non bloquante
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    if (c == '\n' || c == '\r') continue;

    if (c == '<') rx = "<";                 // reset frame
    else if (rx.length() > 0) rx += c;      // on accumule uniquement si on est “dans” une frame

    if (c == '>' && rx.length() > 0) {
      parseFrame(rx);
      rx = "";
    }

    // Anti débordement si trame pourrie
    if (rx.length() > 64) rx = "";
  }

  // Failsafe
  if (millis() - lastRx > failsafeMs) {
    for (uint8_t i = 0; i < N; i++) target[i] = 90;
  }

  // Update servos + lissage (cadence libre)
  for (uint8_t i = 0; i < N; i++) {
    filtered[i] = filtered[i] + alpha * (target[i] - filtered[i]);
    servos[i].write((int)(filtered[i] + 0.5f));
  }
}
