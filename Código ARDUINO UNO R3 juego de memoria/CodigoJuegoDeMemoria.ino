#include "LiquidCrystal_I2C.h"
LiquidCrystal_I2C lcd(0x27,16,2);
#include "pitches.h"

int buttons[4] = {2, 3, 4, 5};
int leds[4]    = {8, 9, 10, 11};
boolean button[4] = {0, 0, 0, 0};

#define buzzer 6
#define levelsInGame 50

int bt_simonSaid[100];
int led_simonSaid[100];

boolean lost;
int game_play, level, stage;

// --------------------------
// Funciones de sonido
// --------------------------
void playBuzzer(int x) {
  tone(buzzer, 650 + (x * 100));
  delay(300);
  noTone(buzzer);
}

// Jingle Bells (fragmento corto, adaptado a buzzer)
void playJingleBells() {
  int notes[] = {659, 659, 659, 659, 659, 659, 659, 784, 523, 587, 659};
  int dur[]   = {200, 200, 400, 200, 200, 400, 200, 200, 200, 200, 600};
  int len = sizeof(notes)/sizeof(notes[0]);
  for (int i=0; i<len; i++) {
    tone(buzzer, notes[i], dur[i]);
    delay(dur[i] + 30);
    noTone(buzzer);
  }
}

// Super Mario — Game Over (melodía de muerte, adaptada)

void playSuperMarioLose() {
  int melody[] = { NOTE_C5, NOTE_G4, NOTE_E4,
                   NOTE_A4, NOTE_B4, NOTE_A4, NOTE_GS4, NOTE_AS4, NOTE_GS4,
                   NOTE_G4, NOTE_D4, NOTE_E4 };
  int dur[]    = {  150, 150, 150,
                     270, 270, 270, 270, 295, 300,
                     330, 150, 70 };
  int len = sizeof(melody)/sizeof(melody[0]);
  for (int i = 0; i < len; i++) {
    tone(buzzer, melody[i], dur[i]);
    delay(dur[i] + 40);
    noTone(buzzer);
  }
}

// --------------------------
// Setup
// --------------------------
void setup() {
  Serial.begin(9600);

  for (int i=0; i<=3; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
    pinMode(leds[i], OUTPUT);
  }

  pinMode(buzzer, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0); lcd.print("   Welcome To   ");
  lcd.setCursor(0,1); lcd.print("> Memory  Game <");
  delay(2000);
  lcd.clear();

  randomSeed(analogRead(0));
  stage = 0; // iniciar en pantalla de inicio
}

// --------------------------
// Loop principal
// --------------------------
void loop() {
  switch(stage) {

    // STAGE 0: pantalla inicio. Si se pulsa cualquier botón NO rojo -> Jingle Bells.
    case 0:
      lcd.setCursor(0,0); lcd.print("Press Red Button");
      lcd.setCursor(0,1); lcd.print(" for Start Game  ");

      // comprobar botones no-rojo
      for (int i=1; i<=3; i++) {
        if (digitalRead(buttons[i]) == LOW) {
          // reproducir SOLO Jingle Bells cuando se presione cualquier botón que no sea rojo
          playJingleBells();
          // esperar a que suelte
          while (digitalRead(buttons[i]) == LOW) { delay(10); }
          delay(120);
        }
      }

      // si presionan el botón rojo -> iniciar juego
      if (digitalRead(buttons[0]) == LOW) {
        while (digitalRead(buttons[0]) == LOW) { delay(10); }
        level = 1;
        stage = 1;
        game_play = 1;
        lcd.clear();
      }
    break;

    // STAGE 1: mostrar nivel y reproducir secuencia aleatoria
    case 1:
      lcd.clear();
      lcd.setCursor(4,0); lcd.print("Level: ");
      lcd.print((level/10)%10);
      lcd.print(level%10);
      lcd.setCursor(0,1); lcd.print(" -- Memorize -- ");
      delay(500);

      led_simonSaid[level] = random(8, 12); // 8..11 pins
      for (int i=1; i<=level; i++) {
        digitalWrite(led_simonSaid[i], HIGH);
        playBuzzer(led_simonSaid[i]-7);
        digitalWrite(led_simonSaid[i], LOW);
        delay(250);
      }
      delay(500);
      stage = 2;
    break;

    case 2:
      stage = 3;
      lcd.setCursor(0,1); lcd.print("   -- Play --   ");
    break;

    // STAGE 3: jugador reproduce la secuencia
    case 3:
      for (int i=0; i<=3; i++) {
        button[i] = digitalRead(buttons[i]);
        if (button[i] == LOW) {
          bt_simonSaid[game_play] = leds[i];
          digitalWrite(leds[i], HIGH);
          playBuzzer(i+1);
          while (digitalRead(buttons[i]) == LOW) { delay(10); }
          delay(50);
          digitalWrite(leds[i], LOW);
          game_play++;
          if (game_play - 1 == level) { game_play = 1; stage = 4; break; }
        }
      }
      delay(10);
    break;

    // STAGE 4: verificar
    case 4:
      lcd.setCursor(0,1); lcd.print("  Verification  ");
      delay(250);
      lost = false;
      for (int i=1; i<=level; i++) {
        if (led_simonSaid[i] != bt_simonSaid[i]) { lost = true; break; }
      }
      if (lost) stage = 5; else stage = 6;
    break;

    // STAGE 5: perdiste -> reproducir Super Mario (Game Over)
    case 5:
      lcd.setCursor(0,1); lcd.print(" !! You Lost !! ");
      playSuperMarioLose();
      tone(buzzer, 350);
      for (int i=0; i<=3; i++) digitalWrite(leds[i], HIGH);
      delay(1000);
      lcd.setCursor(0,1); lcd.print("!! GAME  OVER !!");
      noTone(buzzer);
      delay(1000);
      for (int i=0; i<=3; i++) digitalWrite(leds[i], LOW);
      level = 1; stage = 0; lost = 0;
    break;

    // STAGE 6: ganaste
    case 6:
      lcd.setCursor(0,1); lcd.print(" ** You  Win ** ");
      delay(1000);
      if (level == levelsInGame) {
        lcd.setCursor(0,0); lcd.print("Congratulation");
        lcd.setCursor(0,1); lcd.print(" Level Complete");
        delay(1000);
        lcd.clear(); level = 1;
      } else level++;
      stage = 1;
    break;

    default: break;
  }
}
