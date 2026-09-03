#include <AccelStepper.h>
enum Comandos{
    CMD_MOVE = 3
};

// Datos para recibir y mandar comandos
struct __attribute__((packed)) MovePacket
{
    int16_t pasos[senNum];
}; MovePacket moveData;

struct __attribute__((packed)) AfirmPacket
{
    uint8_t encabezado = 0xAA;
    uint8_t confirm = 0x55;
}; AfirmPacket afirm;

// Pines CNC Shield
const int stepX = 2; const int dirX = 5;
const int stepY = 3; const int dirY = 6;
const int enablePin = 8;

AccelStepper motorX(1, stepX, dirX);
AccelStepper motorY(1, stepY, dirY);

void setup() {
  Serial.begin(250000);
  
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW); 

  motorX.setMaxSpeed(4074);
  motorY.setMaxSpeed(4074);

  motorX.setAcceleration(250);
  motorY.setAcceleration(250);

  Serial.write(0xAA); // Encabezado
  Serial.write(0x03); // Mensaje

  while (true){
    if (Serial.available() >= 2){
      uint8_t h = Serial.read();
      uint8_t c = Serial.read();

      if (h == 0xAA && c == 0x04){
        break;
      }

    }

  }

}

void comandos(){
  if (Serial.available() >= 2){

    if (Serial.read() == 0xAA){

      uint8_t cmd = Serial.read();

      if (cmd == CMD_MOVE){

        Serial.readBytes((char*)&moveData, sizeof(moveData));

        int p1 = moveData.pasos[0];
        int p2 = moveData.pasos[1];

        motorX.move(p1);
        motorY.move(p2);

        bool band = true;
        while (band){

          motorX.run();
          motorY.run();

          if (motorX.distanceToGo() == 0 && motorY.distanceToGo() == 0){
            Serial.write((uint8_t*)&afirm, sizeof(afirm));
            band = false;
          }

        }

      }

    }

  }

}

void loop(){
  motorX.runSpeed();
  motorY.runSpeed();
  comandos();
}
