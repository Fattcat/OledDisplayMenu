#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Definícia pinov pre tlačidlá
#define BUTTON_UP    2
#define BUTTON_DOWN  3
#define BUTTON_ENTER 4

int cursorPosition = 0; // Kurzorská pozícia (0 = First, 1 = Second, 2 = Third)
String menuItems[] = {"First", "Second", "Third"};
int totalItems = sizeof(menuItems) / sizeof(menuItems[0]);

void setup() {
  // Nastavenie displeja
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  
  // Nastavenie pinov tlačidiel
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_ENTER, INPUT_PULLUP);

  // Zobrazenie počiatočného menu
  drawMenu();
}

void loop() {
  // Získať stav tlačidiel
  if (digitalRead(BUTTON_UP) == LOW) {
    cursorPosition--;
    if (cursorPosition < 0) cursorPosition = totalItems - 1;  // Cyklovanie na poslednú položku
    drawMenu();
    delay(200); // Debouncing
  }
  
  if (digitalRead(BUTTON_DOWN) == LOW) {
    cursorPosition++;
    if (cursorPosition >= totalItems) cursorPosition = 0;  // Cyklovanie na prvú položku
    drawMenu();
    delay(200); // Debouncing
  }
  
  if (digitalRead(BUTTON_ENTER) == LOW) {
    if (cursorPosition == 0 || cursorPosition == 1 || cursorPosition == 2) {
      showItemClicked(menuItems[cursorPosition]);
    }
    delay(200); // Debouncing
  }
}

void drawMenu() {
  display.clearDisplay();
  display.setCursor(0, 0);
  
  // Vykreslenie menu
  for (int i = 0; i < totalItems; i++) {
    if (i == cursorPosition) {
      display.print("-> "); // Kurzorka pred položkou
    } else {
      display.print("   ");
    }
    display.println(menuItems[i]);
  }

  display.display();
}

void showItemClicked(String item) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Clicked on: " + item);
  
  // Zobrazenie "exit" na spodnej časti obrazovky
  display.setTextSize(1);
  display.setCursor(0, 50);
  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Invertované pozadie
  display.println("[EXIT]");
  display.setTextColor(SSD1306_WHITE); // Resetovanie farby textu
  
  display.display();
  
  // Čakanie na stlačenie ENTER pre návrat do menu
  while (digitalRead(BUTTON_ENTER) == HIGH) {
    // Čaká sa na stlačenie Enter
  }
  drawMenu();
}
