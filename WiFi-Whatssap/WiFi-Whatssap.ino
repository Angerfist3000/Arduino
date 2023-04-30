#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <time.h>
#include <ESP8266HTTPClient.h>
//incluir nuevos
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <stdio.h>

//-----------------Se definen los valores fundamentles del proyecto----------------- 
//Host del proyecto
#define FIREBASE_HOST "cleancar-f7ec5-default-rtdb.firebaseio.com"
//Secret de la BD
#define FIREBASE_AUTH "HT5BiwjVQqZjOsJSwrDBxdVWyWxBbGkEHk5IJUuz"

//Nombre de mi red Wifi
#define WIFI_SSID "ANDREY"
//password de mi red WiFi
#define WIFI_PASSWORD "T1c51t0sDSM2"

//variable de cadena de texto 
String msjenviado;


//------ULTRASONICO 1--------//
//Pin digital GPIO4 - D2 del ESP8266 para el Trigger del sensor SENSOR ULTRASÓNICO
const int Trigger = 4; 
//Pin digital GPIO2 - D4 del ESP8266 para el Echo del sensor SENSOR UNTRASÓNICO
const int Echo = 2;
//definimos el valor del pin para el led verde
int ledPinVer1 = 18; 
//definimos el valor del pin para el led rojo
int ledPinRojo1 = 15; 
//Se declara el pin al que se conecta el LED
int ledPinAlarma1 = 3; //GPIO14 es el D5 de la placa ESP8266

//---------ULTRASONICO 2--------//
//Pin digital GPIO4 - 16 del ESP8266 para el Trigger del sensor SENSOR ULTRASÓNICO
const int Trigger2 = 16;
//Pin digital GPIO4 - D2 del ESP8266 para el Echo del sensor SENSOR ULTRASÓNICO
const int Echo2 = 5;
//definimos el valor del pin para el led verde
int ledPinVer2 = 7; 
 //definimos el valor del pin para el led rojo
int ledPinRojo2 = 8;

//Se define una variable que tomará la hora para Mex Central 
//del NTP de la función strFechaHora()
String strFechaHoraNTP;
String strFechaNTP;

//----------------------------------------------------------------------------------------//
//-----------------------INICIO DEL SETUP-----------------------------------------------//
void setup() {
  Serial.begin(115200);

  //PINES DEL SENSOR ULTRASONICO
  pinMode(Trigger, OUTPUT); //pin como salida SENSOR ULTRASÓNICO
  pinMode(Echo,INPUT); //pin como entrada SENSOR ULTRASÓNICO 
  digitalWrite(Trigger, LOW); //Inicialaizamos el pin con 0 SENSOR ULTRASÓNICO

  //conectando al Wi-Fi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  //-------------Haciendo petición del tiempo a una NTP (Network Time Protocol)------------//
  //CONFIGURACIÓN DE ZONA HORARIA EN ÉSTE CASO PARA MÉXICO
  //configtime (timezone,dst, servidores ntp)
  //para México el time zone = -6 * 3600, dst = 0, "pool.ntp.org", "time.nist.gov"

  //configTime(-21600, 0, "pool.ntp.org", "time.nist.gov");

  //--------------------------------------Fin de petición del tiempo---------------------------//
  Serial.print("Conectando");

  //Si el estado de conección es diferente al de "Conectado"
  //envía un punto suspensivo en espera de conección

  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  } 
  Serial.println();
  //Al activarse la conección se envia un mensaje 
  Serial.print("Conectado:");
  //Se visualiza la dirección IP asignada al módulo ESP8266
  Serial.println(WiFi.localIP());
  Serial.println();

   //Se inicia la conección al HOST y a la BD
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

}
//--------------------FIN DEL SETUP------------------------------------------//

/*int conta = 1;

//------------CODIGO DEL CALCULO DE LA HORA Y FECHA DE LA RED NTP-------------//
String strFechaHora(){
  time_t now;
  time(&now);
  char time_output[30];
  //strftime(time_output, 30, "%a %d-%m-%y %T", localtime(&now));.
  // %d = día %m = mes %y=año %T= 24 hrs
  strftime(time_output, 30," %d-%m-%y %T", localtime(&now));
  return String(time_output); //Respuesta: Día,Fecha y Hora
}

String fecha(){
  time_t now;
  time(&now);
  char date[30];
//strftime(time_output, 30, "%a %d-%m-%y %T", localtime(&now));
// %d = día %m = mes %y=año %T= 24 hrs
strftime(date, 30, "%d-%m-%y", localtime(&now));
return String(date); //Respuesta: Día,Fecha y Hora
}
*/
//-----------------FIN DE CODIGO DEL CALCULO DE LA HORA Y FECHA DE LA RED NTP---------------------//
//---------------------LOOP-----------------------------------//

void loop(){
  //------------------CODIGO DEL SENSOR ULTRASÓNICO------------------------//

  long t; //tiempo de demora en llegar el eco -- variable para el sensor PIR
  long d; //distancia en centimetro -- varaible para el sensor PIR

  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10); //Enviamos un pulso de 10us
  digitalWrite(Trigger, LOW);

t = pulseIn(Echo, HIGH);  //obtenemos el ancho del pulso
d = t/59;  //escalamos el tiempo a una distancia en cm  

if (d<=25){
  Serial.print("Distancia: ");
  Serial.print(d); //Enviamos serialmente el valor de la distancia
  Serial.print("cm");
  Serial.println();
  Serial.println("Objeto Detectado");

  Firebase.setString("SENSOR1","Objeto Detectado");

  if(Firebase.failed())
  {
    Serial.print("Setendo/estado sensor Fallido:");
    Serial.println(Firebase.error());
  }


//  actualizadno valor 
Firebase.setFloat("edificio B/Lab de computo/distanciacm",d);
if(Firebase.failed())
{
  Serial.print("Seteando/ distanciacm Fallido:");
  Serial.println(Firebase.error());
}

  else{
    Serial.println("En espera...");
    
    Firebase.remove("edificio B/Lab de computo/distanciacm");
    Serial.println("Objeto fuera de rango");

    Firebase.setString("edificio B/Lab de computo/ estado sensor","Objeto fuera de rango...");
    
    if(Firebase.failed())
    {
      Serial.print("Setendo / estadoSensor Fallido");
      Serial.println(Firebase.error());
      return;
    }

    //----------------------------------------------------------------------//
    delay(2000); //Hacemos una pausa de 2s  
  }
//------------------FIN DEL CÓDIGO DEL SENSOR ULTRASÓNICO---------------------------//
}   
