//libreria del servomotor
#include <Servo.h>

//variable para controlar el sevo 
Servo entradaSer;
//variable para el buzzer
const int pinBuzzer=17;
int trig=10;
int echo=9;
int distancia;
int duracion;
int bandera = 0;
int rojo=3; //valor del pin para led color rojo
int verde=2; //valor del pin para led color verde

void setup() {
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
   delay(5000);
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
       delay(500);
   }
   
   
   //delay(200);
 }
}
void loop() {
 //vamos a tener dos bucles uno para mover en sentido positivo y sentido negativo
 //-----SENTIDO POSITIVO----------
 digitalWrite(trig,HIGH);
 delay(1);
 digitalWrite(trig,LOW);
 duracion = pulseIn(echo,HIGH);
 distancia = duracion/59;
 Serial.println(distancia);
 delay(1000);
 sonido();
 


 

}
