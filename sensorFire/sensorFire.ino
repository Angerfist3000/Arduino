#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <time.h>
#include <ESP8266HTTPClient.h>




//Host del proyecto
#define FIREBASE_HOST "cleancar-f7ec5-default-rtdb.firebaseio.com"
//Secret de la BD
#define FIREBASE_AUTH "HT5BiwjVQqZjOsJSwrDBxdVWyWxBbGkEHk5IJUuz"

//Nombre de mi red Wifi
#define WIFI_SSID "LAPTOP Laura"
//password de mi red WiFi
#define WIFI_PASSWORD "laura123"

//-------Ultrasonico 1--------
const int Trigger1 = 4;
const int Echo1 = 2;
int ledPinVer1 = 14; //definimos el valor del pin para el led verde
int ledPinRojo1 = 15; //definimos el valor del pin para el led rojo

void setup() {

Serial.begin(15200);
Serial.println();

//------ULTRASONICO 1--------
//Se inicia el disparador
pinMode(Trigger1,OUTPUT);
//Iniciando el Echo
pinMode(Echo1, INPUT);
//Enviamos la señal al disparador
digitalWrite(Trigger1, LOW);
//declaramos el pin rojo como salida
pinMode(ledPinRojo1,OUTPUT);
//Declaramos el pin verde como salida
pinMode(ledPinVer1,OUTPUT);

 //--------CODIGO PARA CONECTAR AL WIFI----------//
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando");
  while (WiFi.status() != WL_CONNECTED){
  Serial.print(".");
  delay(500);
  } 
  Serial.println();
  //Al activarse la conección se envia un mensaje 
  Serial.print("Conectado: ");
  //Se visualiza la dirección IP asignada al módulo ESP8266
  Serial.println(WiFi.localIP());
  Serial.println();
  //Se inicia la conección al HOST y a la BD
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

}

void loop() {
  //---------ULTRASONICO 1----------
  //Declarando variable para el tiempo
  long t1;
  //Declaramos una variable para la distancia
  long d1;
  
  digitalWrite(Trigger1, HIGH);
  //El tiempo de espera en que enviara el mensaje
  delayMicroseconds(10);
  digitalWrite(Trigger1,LOW);

  t1=pulseIn(Echo1, HIGH);
  d1=t1/59;

  if (d1<=10)
  {
  Serial.print("Objeto Detectadogggrrw: ");
  Serial.print(d1);
  Serial.print("cm");
  Serial.println();
  digitalWrite(ledPinRojo1,HIGH);
  digitalWrite(ledPinVer1,LOW);
  delay(500);
  
  Firebase.setString("SENSOR1/Objeto Detectado","Vehiculo1");
  if(Firebase.failed())
  {
    Serial.print("Seteando/estado sensor Fallido:");
    Serial.println(Firebase.error());
  }
  }
  else
  {
  Serial.print("En espera...");
  Serial.println();
  digitalWrite(ledPinVer1,HIGH);
  digitalWrite(ledPinRojo1,LOW);
  Firebase.remove("SENSOR1/Objeto Detectado");
  Firebase.setString("SENSOR1/Objeto Detectado","En Espera");
  delay(500);
  }


}
