/* Sparkfun Pro Micro & DFRobot Huskylens
   Classification - Learn and Inference
   by: Alexandre Dumont
   date: January, 2021
*/

#include <Bounce2.h>

#include "HUSKYLENS.h"

HUSKYLENS huskylens;

// Pin definitions
#define BOTON_BLANCO   10
#define BOTON_VERDE    19 // A1
#define BOTON_AMARILLO 18 // A0
#define BOTON_ROJO     16

#define LED_BLANCO     21 // A3
#define LED_VERDE      20 // A2
#define LED_AMARILLO   15
#define LED_ROJO       14


Bounce boton_blanco   = Bounce();
Bounce boton_verde    = Bounce();
Bounce boton_amarillo = Bounce();
Bounce boton_rojo     = Bounce();

void setup()
{
  pinMode(LED_BLANCO,     OUTPUT);
  pinMode(LED_VERDE,      OUTPUT);
  pinMode(LED_AMARILLO,   OUTPUT);
  pinMode(LED_ROJO,       OUTPUT);

  switchLeds(1,1,1,1);

  boton_blanco.attach(BOTON_BLANCO, INPUT_PULLUP); // Attach the debouncer to a pin with INPUT_PULLUP mode
  boton_verde.attach(BOTON_VERDE, INPUT_PULLUP);
  boton_amarillo.attach(BOTON_AMARILLO, INPUT_PULLUP);
  boton_rojo.attach(BOTON_ROJO, INPUT_PULLUP);

  boton_blanco.interval(50); // Use a debounce interval of 25 milliseconds
  boton_verde.interval(50);
  boton_amarillo.interval(50);
  boton_rojo.interval(50);

  Serial.begin(115200);

  Wire.begin();
  while (!huskylens.begin(Wire))
  {
      Serial.println(F("Huskylens Begin failed!"));
      delay(100);
  }

  while (!huskylens.writeAlgorithm(ALGORITHM_OBJECT_CLASSIFICATION))
  {
      Serial.println(F("Huskylens failed to switch Algorithm!"));
      delay(100);
  }

  while (!huskylens.setCustomName("White",1))
  {
    Serial.println(F("ID1 customname failed!")); 
    delay(100);
  }  

  while (!huskylens.setCustomName("Green",2))
  {
    Serial.println(F("ID2 customname failed!")); 
    delay(100);
  }  

  while (!huskylens.setCustomName("Yellow",3))
  {
    Serial.println(F("ID3 customname failed!")); 
    delay(100);
  }  

  while (!huskylens.setCustomName("Red",4))
  {
    Serial.println(F("ID4 customname failed!")); 
    delay(100);
  }  

  switchLeds(0,0,0,0);

}

void loop()
{
  boton_blanco.update();
  boton_verde.update();
  boton_amarillo.update();
  boton_rojo.update();

  if ( boton_blanco.fell() ) {
    switch(1,0,0,0);
    learn(1);
  }

  if ( boton_verde.fell() ) {
    switch(0,1,0,0);
    learn(2);
  }

  if ( boton_amarillo.fell() ) {
    switch(0,0,1,0);
    learn(3);
  }

  if ( boton_rojo.fell() ) {
    switch(0,0,0,1);
    learn(4);
  }

  if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
  else if(!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
  else if(!huskylens.available()) Serial.println(F("Nothing detected"));
  else
  {
    while (huskylens.available())
    {

      HUSKYLENSResult result = huskylens.read();
      // Serial.println(String()+F("Result:") + result.ID );
      switch (result.ID)
      {
      case 1:
        switchLeds(1,0,0,0);
        break;
      
      case 2:
        switchLeds(0,2,0,0);
        break;

      case 3:
        switchLeds(0,0,1,0);
        break;

      case 4:
        switchLeds(0,0,0,4);
        break;

      default:
        switchLeds(0,0,0,0);
        break;
      }
    }    
  }

}

void learn(int id)
{
  while ( !huskylens.writeLearn(id) )  // bool writeLearn(int ID)
  {
    //Serial.println(F("learn object failed!")); 
    delay(100);
  }
  Serial.println(String()+F("learn object success ")+id);
}

void switchLeds(int a, int b, int c, int d) {
  digitalWrite(LED_BLANCO, a);
  digitalWrite(LED_VERDE, b);
  digitalWrite(LED_AMARILLO, c);
  digitalWrite(LED_ROJO, d);   
}