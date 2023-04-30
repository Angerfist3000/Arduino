#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <time.h>
#include <ESP8266HTTPClient.h>
//incluir nuevos
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

//-----------------Se definen los valores fundamentales del proyecto----------------- 
//Host del proyecto
#define FIREBASE_HOST "clean-car-f26de-default-rtdb.firebaseio.com"
//Secret de la BD
#define FIREBASE_AUTH "QBerFezVwA6cT3liU1p0h26flKqxGHRMycChiVeP"

//Nombre de mi red Wifi
#define WIFI_SSID "Realmadrid"
//password de mi red WiFi
#define WIFI_PASSWORD "heito1997"

//variable de cadena de texto 
String msjenviado;

//Pin digital GPIO4 - D2 del ESP8266 para el Trigger del sensor SENSOR ULTRASÓNICO
const int Trigger = 4; 
//Pin digital GPIO2 - D4 del ESP8266 para el Trigger del sensor SENSOR UNTRASÓNICO
const int Echo = 2;
//Se declara el pin al que se conecta el LED
int ledPinAlarma1 = 3; //GPIO14 es el D5 de la placa ESP8266
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

  configTime(-21600, 0, "pool.ntp.org", "time.nist.gov");

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

int conta = 1;

//------------CODIGO DEL CALCULO DE LA HORA Y FECHA DE LA RED NTP-------------//
String strFechaHora(){
  time_t now;
  time(&now);
  char time_output[30];
  //strftime(time_output, 30, "%a %d-%m-%y %T", localtime(&now));
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

//-----------------FIN DE CODIGO DEL CALCULO DE LA HORA Y FECHA DE LA RED NTP---------------------//

//---------------------LOOP-----------------------------------//
void loop(){
  //------------------CODIGO DEL SENSOR ULTRASÓNICO------------------------//

  long t; //tiempo de demora en llegar el eco -- variable para el sensor PIR
  long d; //distancia en centimetro -- variable para el sensor PIR

  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10); //Enviamos un pulso de 10us
  digitalWrite(Trigger, LOW);

t = pulseIn(Echo, HIGH);  //obtenemos el ancho del pulso
d = t/59;  //escalamos el tiempo a una distancia en cm  

if (d<=15){
  Serial.print("Distancia: ");
  Serial.print(d); //Enviamos serialmente el valor de la distancia
  Serial.print("cm");
  Serial.println();
  Serial.println("Objeto Detectado");
  Firebase.setString("x","2");

  HTTPClient http; //Se declara un objeto de la clase HTTPClient

  msjenviado = "http://api.callmebot.com/whatsapp.php?";
  msjenviado = msjenviado + "phone=+5219321259931";
  msjenviado = msjenviado + "&text=Alerta+se+ha+detectado+movimiento";
  msjenviado = msjenviado + "&apikey=8275609";

  http.begin(msjenviado);
  int httpCode = http.GET(); //Enviando la petición 
  if (httpCode > 0){
    //se verifica el regreso del código
    String payload = http.getString(); //obteniendo la respuesta a la solicitud
    Serial.println(payload); //Imprimiendo la respuesta
  }
  http.end(); //cerrando la conexión
  delay(2000); //Hacemos una pausa de 2s
  }

else{
    Serial.println("En espera...");
    Serial.println(strFechaHora());

    //----------------------------------------------------------------------//
    delay(2000); //Hacemos una pausa de 2s  
  }
//------------------FIN DEL CÓDIGO DEL SENSOR ULTRASÓNICO---------------------------//
}   
