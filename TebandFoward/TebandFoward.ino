
// BY Jadson Marliere de Oliveira - Eng. Computacao e Eletrica
// Projeto Aberto - Tekband Foward v2.3
// Modulo - Estimulacao funcional - FES

//--------- include oled ---------
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//-----------library multiprocessing--------------------
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

//TaskHandle_t Task1;
TaskHandle_t Task2;

//--------- DAC ESP32 -----------
#define DAC1 25
int freq = 0;
int val = 150; //255 = 3.3V
#define DAC1 25
int deg = 0;


//----------------Start main code----------------------

void drawlcd(){
  
  freq++;
  if (freq >= 150) freq = 0; 
  // display frequencia
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Frequencia: ");
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print(freq);
  display.print(" ");
  display.setTextSize(1);
  display.print("Hz");
  
  // display intensidade
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Intensidade: ");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print("50");
  display.print(" %"); 
  
  display.display();

  delay(1000);
  
}



void setup() {
  Serial.begin(115200);


// configuracao setup para o oled
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);


// configuracao para o processamento paralelo dual core --------------------------
xTaskCreatePinnedToCore(
      secondary_task,
      "task_secondary",
      1000,
      NULL,
      1,
      &Task2,
      1);

      
}

void secondary_task(void * pvParameters){
    Serial.print("Task2 running on core ");
    Serial.println(xPortGetCoreID());
    for(;;){
      drawlcd();
    }
}
//--------------------------------------------------------------------------


// nao criar funcao para o pulso, pois o atraso causa problemas para gerar a funcao
void loop() {
  
  // 45 pontos por comprimento de onda, 8bits
  for (int deg = 0; deg < 360; deg = deg + 8){
    dacWrite(DAC1, int(128 + 40 * ( sin(deg*PI/180)))); // sine wave 1.2kHz 100mV pk-pk
  }
    
    
  
}
