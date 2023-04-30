//librerias a utilizar 

//#include <ESP8266Wifi.h>
//#include <FirebaseArduino.h>
//#include <time.h>
#include <Servo.h>


//-------Ultrasonico 1--------
const int Trigger1 = 20;
const int Echo1 = 19;
int ledPinVer1 = 18; //definimos el valor del pin para el led verde
int ledPinRojo1 = 15; //definimos el valor del pin para el led rojo

//Ultrasonico 2
const int Trigger2 = 5;
const int Echo2 = 6;
int ledPinVer2 = 7; //definimos el valor del pin para el led verde
int ledPinRojo2 = 8; //definimos el valor del pin para el led rojo

//------Ultrasonico 3--------
const int Trigger3 = 10;
const int Echo3 = 11;
int ledPinVer3 = 12; //definimos el valor del pin para el led verde
int ledPinRojo3 = 13; //definimos el valor del pin para el led rojo

//--------Empesamos codigo de servomotor-------------//
//variable para controlar el servo 
Servo entradaSer;
//variable para el buzzer
const int pinBuzzer=17;
int trig=40;
int echo=42;
int distancia;
int duracion;
int bandera = 0;
int rojo=3; //valor del pin para led color rojo
int verde=2; //valor del pin para led color verde

void setup(){
Serial.begin(9600);
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

//-------ULTRASONICO 2--------
//Se inicia el disparador
pinMode(Trigger2,OUTPUT);
//Iniciando el Echo
pinMode(Echo2, INPUT);
//Enviamos la señal al disparador
digitalWrite(Trigger2, LOW);
//declaramos el pin rojo como salida
pinMode(ledPinRojo2,OUTPUT);
//Declaramos el pin verde como salida
pinMode(ledPinVer2,OUTPUT);

//-----ULTRASONICO 3--------
//Se inicia el disparador
pinMode(Trigger3,OUTPUT);
//Iniciando el Echo
pinMode(Echo3, INPUT);
//Enviamos la señal al disparador
digitalWrite(Trigger3, LOW);
//declaramos el pin rojo como salida
pinMode(ledPinRojo3,OUTPUT);
//Declaramos el pin verde como salida
pinMode(ledPinVer3,OUTPUT);

//----------Iniciamos ServoMotor---------//
pinMode(trig,OUTPUT);//declaramos pin verde como salida
  pinMode(echo,INPUT);//declaramos pin amarillo como salida
  pinMode(verde,OUTPUT);//declaramos pin verde como salida
  pinMode(rojo,OUTPUT);//declaramos pin rojo como salida  
  entradaSer.attach(21);
  //pinMode(rojo,OUTPUT);//declaramos pin rojo como salida
  //iniciamos monitor serie para mostrar el resultado
  Serial.begin(9600);


}

void sonido()
{
    if(distancia<=15)
 {
   entradaSer.write(180);
   digitalWrite(rojo,LOW);
   digitalWrite(verde,HIGH);
   Serial.println("Abierto");
  if(bandera == 0)
   {
       tone(pinBuzzer,440,300);
       
       
   }
   //Tiempo del movimiento del servo
   delay(500);
   bandera = 1;

 }
 else
 {
   entradaSer.write(0);
   digitalWrite(rojo,HIGH);
   digitalWrite(verde,LOW);
   Serial.println("Cerrado");
   if(bandera == 1)
   {
       tone(pinBuzzer,440,300);
       bandera = 0;
   }
   
   
   //delay(200);
 }
}



void loop(){

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
  Serial.print("Objeto Detectado: ");
  Serial.print(d1);
  Serial.print("cm");
  Serial.println();
  digitalWrite(ledPinRojo1,HIGH);
  digitalWrite(ledPinVer1,LOW);
  delay(500);
  }
  else
  {
  Serial.print("En espera...");
  Serial.println();
  digitalWrite(ledPinVer1,HIGH);
  digitalWrite(ledPinRojo1,LOW);
  delay(500);
  }

  //-------ULTRASONICO 2----------
  //Declarando variable para el tiempo
  long t2;
  //Declaramos una variable para la distancia
  long d2;
   digitalWrite(Trigger2, HIGH);
  //El tiempo de espera en que enviara el mensaje
  delayMicroseconds(10);
  digitalWrite(Trigger2,LOW);

  t2=pulseIn(Echo2, HIGH);
  d2=t2/59;

  if (d2<=10)
  {
  Serial.print("Objeto Detectado: ");
  Serial.print(d2);
  Serial.print("cm");
  Serial.println();
  digitalWrite(ledPinRojo2,HIGH);
  digitalWrite(ledPinVer2,LOW);
  delay(500);
  }
  else
  {
  Serial.print("En espera.....");
  Serial.println();
  digitalWrite(ledPinVer2,HIGH);
  digitalWrite(ledPinRojo2,LOW);
  delay(500);
  }

  //-------ULTRASONICO 3--------
  //Declarando variable para el tiempo
  long t3;
  //Declaramos una variable para la distancia
  long d3;

  digitalWrite(Trigger3, HIGH);
  //El tiempo de espera en que enviara el mensaje
  delayMicroseconds(10);
  digitalWrite(Trigger3,LOW);

  t3=pulseIn(Echo3, HIGH);
  d3=t3/59;

  if (d3<=10)
  {
  Serial.print("Objeto Detectado: ");
  Serial.print(d3);
  Serial.print("cm");
  Serial.println();
  digitalWrite(ledPinRojo3,HIGH);
  digitalWrite(ledPinVer3,LOW);
  delay(500);
  }
  else
  {
  Serial.print("En espera.....");
  Serial.println();
  digitalWrite(ledPinVer3,HIGH);
  digitalWrite(ledPinRojo3,LOW);
  delay(500);
  }

//-----------Inicio ServoMotor--------------//

digitalWrite(trig,HIGH);
 delay(1);
 digitalWrite(trig,LOW);
 duracion = pulseIn(echo,HIGH);
 distancia = duracion/59;
 Serial.println(distancia);
 delay(1000);
 sonido();
  
}



 

 

 