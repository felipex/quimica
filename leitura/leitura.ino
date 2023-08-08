#include <avr/io.h>
#include <avr/interrupt.h>

#define BTN_MODE 2
#define STATE_OFF 0b0
#define STATE_READING 0b1
#define MAX_STATE 2

#define LED_STATE 13
#define LED_ONOFF 12

#define PROP_PIN A0

unsigned char state = 0;

unsigned int reload = 0xF424; // Aproximadamente 1s;
unsigned long int us = 0;

bool readProp = 0;

ISR(TIMER1_COMPA_vect) {
  readProp = !readProp;
  us++;
  //Serial.println(readProp);
}

void setupTimer() {

  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  OCR1A = reload;
  TCCR1B = (1 << WGM12) | (1 << CS12);
  TIMSK1 = (1 << OCIE1A);
  sei();
}

void setup() {
  Serial.begin(9600);

  // Botão liga e desliga
  pinMode(BTN_MODE, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BTN_MODE), updateState, RISING);

  // LED aceso está em modo de leitura.
  pinMode(LED_ONOFF, OUTPUT);
  digitalWrite(LED_ONOFF, LOW);

  // Pisca o LED a cada leitura
  pinMode(LED_STATE, OUTPUT);
  digitalWrite(LED_STATE, LOW);

  // Configura o intervalo de quando se faz as leituras.
  setupTimer();
}

unsigned int old_state = -1;

void loop() {
  unsigned int valProp = 0;
  
  if (old_state != state) {
    //Serial.print("State changed -> ");
    //Serial.println(state);
    //Serial.println(STATE_OFF);
    //Serial.println(STATE_READING);
    old_state = state;
    if (state == 1) us = 0;

    digitalWrite(LED_STATE, state);
  }
  if (state == STATE_READING) {
    if (readProp) {
      digitalWrite(LED_ONOFF, HIGH);
      valProp = analogRead(PROP_PIN);
      enviaLeitura(us, valProp * 5./1023);
      readProp = !readProp;
      //Serial.print("PROP -> ");
      //Serial.println(valProp);
      digitalWrite(LED_ONOFF, LOW);
    }
  }
}

void updateState() {
  state++;
  state %= MAX_STATE;
}


void enviaLeitura(unsigned long int time, float valor) {
  Serial.print(time);
  Serial.print(",");
  Serial.println(String(valor, 3));
}