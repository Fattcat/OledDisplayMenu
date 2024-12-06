#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BUTTON_UP    2
#define BUTTON_DOWN  3
#define BUTTON_ENTER 4

char scrollText[] = "DonkToolV1"; // Posúvaný text
int textX = 0;             // X pozícia textu
int direction = 1;         // 1 = doprava, -1 = doľava

char staticText1[] = "_____________________"; // Statický text

int cursorPosition = 0; // Aktuálna poloha kurzora
char menuItems[][10] = {"First", "Second", "Third", "Forth"};
int totalItems = sizeof(menuItems) / sizeof(menuItems[0]);

void setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_ENTER, INPUT_PULLUP);

  textX = SCREEN_WIDTH / 2 - (strlen(scrollText) * 6) / 2; // Nastavíme na stred
}

void loop() {
  // Krok 1: Vymažeme len časť displeja
  display.clearDisplay();

  // Krok 2: Posúvame text
  textX += direction;
  if (textX <= 0 || textX >= SCREEN_WIDTH - strlen(scrollText) * 6) {
    direction *= -1; // Zmena smeru
  }
  display.setCursor(textX, 0);
  display.print(scrollText);

  // Krok 3: Zobrazíme statický text
  display.setCursor(0, 3);
  display.print(staticText1);

  // Krok 4: Zobrazíme menu
  drawMenu();

  // Krok 5: Spracujeme tlačidlá
  if (digitalRead(BUTTON_UP) == LOW) {
    delay(50);
    cursorPosition--;
    if (cursorPosition < 0) cursorPosition = totalItems - 1;
    delay(200);
  }

  if (digitalRead(BUTTON_DOWN) == LOW) {
    delay(50);
    cursorPosition++;
    if (cursorPosition >= totalItems) cursorPosition = 0;
    delay(200);
  }

  if (digitalRead(BUTTON_ENTER) == LOW) {
    delay(50);
    showItemClicked(menuItems[cursorPosition]);
    delay(200);
  }

  // Krok 6: Aktualizácia displeja
  display.display();
}

void drawMenu() {
  // Kreslí menu od riadku 20 nižšie
  display.setCursor(0, 12);
  for (int i = 0; i < totalItems; i++) {
    if (i == cursorPosition) {
      display.print("-> ");
    } else {
      display.print("   ");
    }
    display.println(menuItems[i]);
  }
}

void showItemClicked(const char* item) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Clicked on: ");
  display.println(item);

  display.setCursor(25, 50);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  display.println("[EXIT]");
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.display();

  while (digitalRead(BUTTON_ENTER) == HIGH) {
    display.display();
    delay(10);
  }
}
