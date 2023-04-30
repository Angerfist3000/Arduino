//librerias a utilizar 

//#include <ESP8266Wifi.h>
//#include <FirebaseArduino.h>
//#include <time.h>
#include <Servo.h>


//--------PINES DE ENTRADA-------------//
Servo entradaSer;//variable para controlar el servo 
const int pinBuzzer=17;//variable para el buzzer
int trig=10;//pin de trigger
int echo=11;//pin de echo
long distancia;//variable distancia
long duracion;//variable duracion
int bandera = 0;//define el sonido
int rojo=12; //valor del pin para led color rojo
int verde=13; //valor del pin para led color verde

//--------PINES DE SALIDA-------------//
Servo salidaSer;//variable para controlar el servo 
const int pinBuzzer1=17;//variable para el buzzer
int trig1=1;//pin de trigger
int echo1=2;//pin de echo
long distancia1;//variable distancia
long duracion1;//variable duracion
int bandera1 = 0;//define el sonido
int rojo1=3; //valor del pin para led color rojo
int verde1=4; //valor del pin para led color verde

/*//----ULTRASONICO 1-----//
//Pin del trigger
const int Trigger = 24;
//Pin del echo
const int Echo = 26;
//pin de led rojo
int ledPinRojo = 28; 
//pin de led verde
int ledPinVer = 30; 

//----ULTRASONICO 2-----//
//Pin del trigger
const int Trigger1 = 32;
//Pin del echo
const int Echo1 = 34;
//pin de led rojo
int ledPinRojo1 = 36; 
//pin de led verde
int ledPinVer1 = 38; */


void setup(){
  Serial.begin(9600);

  //----------ENTRADA---------//
  pinMode(trig,OUTPUT);//declaramos pin verde como salida
  pinMode(echo,INPUT);//declaramos pin amarillo como salida
  pinMode(verde,OUTPUT);//declaramos pin verde como salida
  pinMode(rojo,OUTPUT);//declaramos pin rojo como salida  
  entradaSer.attach(9);//pin para servomotor

  //----------SALIDA---------//
  pinMode(trig1,OUTPUT);//declaramos pin verde como salida
  pinMode(echo1,INPUT);//declaramos pin amarillo como salida
  pinMode(verde1,OUTPUT);//declaramos pin verde como salida
  pinMode(rojo1,OUTPUT);//declaramos pin rojo como salida  
  salidaSer.attach(5);//pin para servomotor
 
  //-----PINES DEL SENSOR ULTRASONICO 1-------//
  /*pinMode(Trigger, OUTPUT); //pin como salida  SENSOR ULTRASÓNICO
  pinMode(Echo, INPUT);  //pin como entrada  SENSOR ULTRASÓNICO
  digitalWrite(Trigger, LOW);//Inicializamos el pin con 0 SENSOR ULTRASÓNICO
  pinMode(ledPinRojo,OUTPUT);//Declaramos el pin rojo como salida
  pinMode(ledPinVer,OUTPUT);//Declaramos el pin verde como salida

  //-------PINES DEL SENSOR ULTRASONICO 2-------//
  pinMode(Trigger1, OUTPUT); //pin como salida  SENSOR ULTRASÓNICO
  pinMode(Echo1, INPUT);  //pin como entrada  SENSOR ULTRASÓNICO
  digitalWrite(Trigger1, LOW);//Inicializamos el pin con 0 SENSOR ULTRASÓNICO
  pinMode(ledPinRojo1,OUTPUT);//Declaramos el pin rojo como salida
  pinMode(ledPinVer1,OUTPUT);//Declaramos el pin verde como salida*/


}

void sonido1(){
  //-----------CODIGO DE ENTRADA-----------//
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  
  duracion = pulseIn(echo,HIGH);
  distancia = duracion/59;

  Serial.println("Sensor1 :" + String(distancia));
  
  if(distancia<=4 && distancia > 0){
   entradaSer.write(90);//definimos la rotacion del servo
   digitalWrite(rojo,LOW);//led rojo permanece apagado
   digitalWrite(verde,HIGH);//led verde se enciende
   Serial.println("Entrada-Abierto");//mensaje a monitor serial
   delay(5000);//Tiempo en bajar el servo

   //si la condicion es true, se activa el Buzzer
   if(bandera == 0){
   //(indica el pin digital,frecuencia en HZ,duracion en milisegundos)      
   tone(pinBuzzer,440,300);
   }

   //ahora bandera le damos el valor de 1
   bandera = 1;

  }
  else
  {
   entradaSer.write(0);//definimos la rotacion del servo
   digitalWrite(rojo,HIGH);//led rojo permanece encendido
   digitalWrite(verde,LOW);//led verde permanece apagado
   Serial.println("Entrada-Cerrado");//mensaje a monitor serial

   //si la condicion es TRUE se ejecuta el bloque de codigo
   if(bandera == 1)
   {
    //(indica el pin digital,frecuencia en HZ,duracion en milisegundos)     
    tone(pinBuzzer,440,300);
    bandera = 0;//ahora bandera vale 0
   }
   
  }
}

void sonido2(){
   //--------------SALIDA-------------------//
  digitalWrite(trig1,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig1,LOW);

  duracion1 = pulseIn(echo1,HIGH);
  distancia1 = duracion1/59;
  
  Serial.println("Sensor2 :" + distancia1);
  
  if(distancia1<=4 && distancia1 > 0){
   salidaSer.write(90);//definimos la rotacion del servo
   digitalWrite(rojo1,LOW);//led rojo permanece apagado
   digitalWrite(verde1,HIGH);//led verde permanece encendido
   Serial.println("Salida-Abierto");//mensaje a monitor serial
   delay(5000);//Tiempo en bajar el servo

   //si la condicion es true, se activa el Buzzer
   if(bandera1 == 0){
    //(indica el pin digital,frecuencia en HZ,duracion en milisegundos) 
    tone(pinBuzzer,440,300);
    }

   //ahora bandera le damos el valor de 1
   bandera1 = 1;

  }
  else
  {
   salidaSer.write(0);//definimos la rotacion del servo
   digitalWrite(rojo1,HIGH);//led rojo permanece encendido
   digitalWrite(verde1,LOW);//led verde permanece apagado
   Serial.println("Salida-Cerrado");//mensaje a monitor serial

   //si la condicion es TRUE se ejecuta el bloque de codigo
   if(bandera1 == 1)
   {
    //(indica el pin digital,frecuencia en HZ,duracion en milisegundos) 
    tone(pinBuzzer,440,300);
    bandera1 = 0;//ahora bandera vale 0
   }
   
  }
}


void loop(){

sonido1();
sonido2();

 

// ------------------  CODIGO DEL SENSOR SENSOR ULTRASÓNICO 1 -------------------//
   /* long t; //tiempo que demora en llegar el eco -- variable para el sensor PIR
    long d; //distancia en centimetros -- variable para el sensor PIR

    digitalWrite(Trigger, HIGH);
    delayMicroseconds(10);          //Enviamos un pulso de 10us
    digitalWrite(Trigger, LOW);
  
    t = pulseIn(Echo, HIGH); //obtenemos el ancho del pulso
    d = t/59;             //escalamos el tiempo a una distancia en cm

    //---DETECCION DE OBJETO--//
    if (d <= 5){
      Serial.print("Ocupado1:");
      Serial.println();
      digitalWrite(ledPinRojo,HIGH);
      digitalWrite(ledPinVer,LOW);  
      delay(500); //Hacemos una pausa de 2s
    }
    else{
      Serial.println("Disponible2");
      digitalWrite(ledPinVer,HIGH);
      digitalWrite(ledPinRojo,LOW);
      delay(500); //Hacemos una pausa de 2s
    }*/
    
// ------------------ FIN DEL CÓDIGO DEL SENSOR SENSOR ULTRASÓNICO 1-------------------// 


// ------------------CODIGO DEL SENSOR SENSOR ULTRASÓNICO 2 -------------------//

    /*long t1; //tiempo que demora en llegar el eco -- variable para el sensor PIR
    long d1; //distancia en centimetros -- variable para el sensor PIR

    digitalWrite(Trigger1, HIGH);
    delayMicroseconds(10);          //Enviamos un pulso de 10us
    digitalWrite(Trigger1, LOW);
  
    t1 = pulseIn(Echo1, HIGH); //obtenemos el ancho del pulso
    d1 = t1/59;             //escalamos el tiempo a una distancia en cm

    //---DETECCION DE OBJETO--//
    if (d1 <= 5){
      //----LECTURA DE DATOS DE MONITOR SERIAL------//
      Serial.print("Ocupado2");
      Serial.println();
      digitalWrite(ledPinRojo1,HIGH);
      digitalWrite(ledPinVer1,LOW);  
      delay(500);//pausa de 1/2 segundo
    }
    else{
      Serial.println("disponible2");
      digitalWrite(ledPinVer1,HIGH);
      digitalWrite(ledPinRojo1,LOW);
      delay(500); //Hacemos una pausa de 2s
    }*/
  
}
    





 

 

 