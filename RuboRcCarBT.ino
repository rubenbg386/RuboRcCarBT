#include "Arduino.h"
#include "SoftwareSerial.h"
#define ENABLE_MOTOR 5
#define DIR_ADELANTE 3
#define DIR_ATRAS 4

#define ENABLE_DIRECCION 10
#define DIR_IZQUIERDA 9
#define DIR_DERECHA 8

#define PIN_LUZ_1 11
#define PIN_LUZ_2 12

#define VELOCIDAD_MINIMA_ADELANTE 80
#define VELOCIDAD_MINIMA_ATRAS 80
#define VELOCIDAD_INICIAL_ADELANTE 70
#define VELOCIDAD_INICIAL_ATRAS 70

#define ESTADO_DEFECTO 83
#define ESTADO_IZQUIERDA 52
#define ESTADO_DERECHA 50
#define ESTADO_ADELANTE 85
#define ESTADO_ATRAS 68
#define ESTADO_ENCENDER_LUCES 71
#define ESTADO_APAGAR_LUCES 82

const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output

int estado;

int pulsador = 1;

int velocidad_adelante = VELOCIDAD_INICIAL_ADELANTE;
int velocidad_atras = VELOCIDAD_INICIAL_ATRAS;
int giro_izquierda = 0;
int giro_derecha = 0;



SoftwareSerial BT1(6,7); // RX, TX

void setup() {
  pinMode(PIN_LUZ_1, OUTPUT);
  pinMode(PIN_LUZ_2, OUTPUT);
  //---set pin direction
  pinMode(ENABLE_MOTOR,OUTPUT);
  pinMode(DIR_ADELANTE,OUTPUT);
  pinMode(DIR_ATRAS,OUTPUT);
  pinMode(ENABLE_DIRECCION,OUTPUT);
  pinMode(DIR_IZQUIERDA,OUTPUT);
  pinMode(DIR_DERECHA,OUTPUT);
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);

  // Iniciamos comunicación serial por USB
  Serial.begin(9600);
  // Iniciamos comunicación serial por bluetooth
  BT1.begin(9600);
  // Damos tiempo para iniciar comunicación
  delay(500);
  Serial.println("Iniciando bluetooth");
  if(BT1.available())
    Serial.println("BT1 disponible");
}



void loop() {

  if (BT1.available()) {
    // Si hay disponible comunicación bluetooth...
    estado = BT1.read();
      if(estado != ESTADO_DEFECTO){
        Serial.print("Lectura: ");
        Serial.println(estado);
      }

      if(estado == ESTADO_IZQUIERDA){
        Serial.println("ESTADO_IZQUIERDA");
        giro_izquierda = 250;
        giro_derecha = 0;
      }

      if(estado == ESTADO_DERECHA){
         Serial.println("ESTADO_DERECHA");
         giro_derecha = 250;
         giro_izquierda = 0;
      }

      if(estado == ESTADO_ADELANTE){
        velocidad_adelante+=5;
        if(velocidad_adelante>250){
          velocidad_adelante = 250;
        }
        Serial.println("ESTADO_ADELANTE");
        Serial.print("velocidad_adelante: ");
        Serial.println(velocidad_adelante);
        velocidad_atras = VELOCIDAD_INICIAL_ATRAS;
      }

      if(estado == ESTADO_ATRAS){
        Serial.println("ESTADO_ATRAS");
        Serial.print("velocidad_atras: ");
        Serial.println(velocidad_atras);
        velocidad_atras+=5;
        velocidad_adelante = VELOCIDAD_INICIAL_ADELANTE;
      }

      if(estado == ESTADO_ENCENDER_LUCES){
        Serial.println("ESTADO_ENCENDER_LUCES");
        pulsador = 0;
      }
      if(estado == ESTADO_APAGAR_LUCES){
        Serial.println("ESTADO_APAGAR_LUCES");
        pulsador = 1;
      }
  } else {
    // Si no hay disponible comunicación bluetooth...
    //Serial.println("BT1 NO disponible");
  }
  delay(25);


  if(pulsador>0){
    digitalWrite(PIN_LUZ_1, LOW);
  digitalWrite(PIN_LUZ_2, LOW);

  }else{
      digitalWrite(PIN_LUZ_1, HIGH);
  digitalWrite(PIN_LUZ_2, HIGH);
  }

  if(giro_izquierda>200){
    analogWrite(ENABLE_DIRECCION,giro_izquierda);
    digitalWrite(DIR_IZQUIERDA,HIGH);
    digitalWrite(DIR_DERECHA,LOW);
    giro_izquierda = 0;
    delay(200);
  }else if (giro_derecha>200){
    analogWrite(ENABLE_DIRECCION,giro_derecha);
    digitalWrite(DIR_DERECHA,HIGH);
    digitalWrite(DIR_IZQUIERDA,LOW);
    giro_derecha = 0;
    delay(200);
  }else{
    digitalWrite(ENABLE_DIRECCION,LOW);
  }

  if(velocidad_adelante>VELOCIDAD_MINIMA_ADELANTE){
    analogWrite(ENABLE_MOTOR,velocidad_adelante);
    digitalWrite(DIR_ADELANTE,HIGH); //one way
    digitalWrite(DIR_ATRAS,LOW);
    delay(50);
  }else if (velocidad_atras>VELOCIDAD_MINIMA_ATRAS){
    analogWrite(ENABLE_MOTOR,velocidad_atras);
    digitalWrite(DIR_ADELANTE,LOW); //one way
    digitalWrite(DIR_ATRAS,HIGH);
    delay(50);
  }else{
    digitalWrite(ENABLE_MOTOR,LOW); //all done
  }


}
