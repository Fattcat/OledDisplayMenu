#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Definícia šírky a výšky OLED displeja
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64  // Zmena na 64px výšku

// Nastavenie SSD1306 napájania
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Text, ktorý chceme scrollovať
String scrollText = "DonkToolV1";
int textX; // Pozícia textu na osi X
int direction = 1; // 1 znamená pohyb doprava, -1 znamená pohyb doľava

// Statické texty, ktoré nebudú scrollovať
String staticText1 = "______________________";
String staticText2 = "Menu";
String staticText3 = "Diodes :";
String staticText4 = "Switch :";

void setup() {
  // Inicializácia displeja
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("OLED displej sa nepodarilo inicializovať"));
    for (;;);
  }

  // Vyčistenie displeja
  display.clearDisplay();
  display.setTextSize(1);  // Nastavenie veľkosti textu
  display.setTextColor(SSD1306_WHITE); // Nastavenie farby textu
  
  // Nastavenie počiatočnej pozície textu na stred prvého riadku
  textX = SCREEN_WIDTH / 2 - (scrollText.length() * 6) / 2; // Stred displeja
}

void loop() {
  display.clearDisplay(); // Vyčistenie displeja

  // Zobrazíme text na prvej línii (posúvaný text)
  display.setCursor(textX, 0); 
  display.print(scrollText); // Vypísanie posúvaného textu na displej

  // Statický text na druhom riadku
  display.setCursor(0, 4); // Pozícia druhého riadku
  display.print(staticText1);
  
  display.setCursor(0, 16);
  display.print(staticText2);
  
  display.setCursor(0, 26);
  display.print(staticText3);
  
  display.setCursor(0, 36);
  display.print(staticText4);

  // Aktualizácia displeja
  display.display(); 

  // Posúvame text
  textX += direction; // Posúvame pozíciu podľa smeru

  // Ak text narazí na ľavú stranu (x = 0), začneme posúvať doprava
  if (textX <= 0) {
    direction = 1; // Posúvame doprava
  }

  // Ak text narazí na pravú stranu (x = 128), začneme posúvať doľava
  if (textX >= SCREEN_WIDTH - scrollText.length() * 6) {
    direction = -1; // Posúvame doľava
  }

  delay(50); // Nastavenie rýchlosti posunu
}
