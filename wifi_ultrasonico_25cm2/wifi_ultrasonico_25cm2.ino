//------------- Librerias utilizadas --------------------------------------------//

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <time.h>
#include <ESP8266HTTPClient.h>

//Host del proyecto
#define FIREBASE_HOST ""
//Secret de la BD
#define FIREBASE_AUTH ""


// Nombre de mi red Wi-Fi
#define WIFI_SSID ""
// Password de mi red WiFi
#define WIFI_PASSWORD ""

//variable de cadena de texto
String msjenviado;

//Pin digital GPIO4 - D2 del ESP8266 para el Trigger del sensor SENSOR ULTRASÓNICO
const int Trigger = 4;
//Pin digital GPIO2 - D4 del ESP8266 para el Trigger del sensor SENSOR ULTRASÓNICO   
const int Echo = 2;
//Se declara el pin al que se conecta el LED
int ledPinRojo = 15; // GPIO14 es el D5 de la placa ESP8266
int ledPinVer = 14; // GPIO14 es el D5 de la placa ESP8266


//Pin digital GPIO4 - D2 del ESP8266 para el Trigger del sensor SENSOR ULTRASÓNICO
const int Trigger1 = 12;
//Pin digital GPIO2 - D4 del ESP8266 para el Trigger del sensor SENSOR ULTRASÓNICO   
const int Echo1 = 13;
//Se declara el pin al que se conecta el LED
int ledPinRojo1 = 5; // GPIO14 es el D5 de la placa ESP8266
int ledPinVer1 = 16; // GPIO14 es el D5 de la placa ESP8266
// Se define una variable que tomará la hora para Mex Central
// del NTP de la función strFechaHora()
String strFechaHoraNTP;
String strFechaNTP;

// --------------------------------------------------------------------------------//

// -----------------------INICIO DEL SETUP ---------------------------------------//
void setup() {
  Serial.begin(115200);

  // PINES DEL SENSOR ULTRASONICO
  pinMode(Trigger, OUTPUT); //pin como salida  SENSOR ULTRASÓNICO
  pinMode(Echo, INPUT);  //pin como entrada  SENSOR ULTRASÓNICO
  digitalWrite(Trigger, LOW);//Inicializamos el pin con 0 SENSOR ULTRASÓNICO
  pinMode(ledPinRojo,OUTPUT);//Declaramos el pin rojo como salida
  pinMode(ledPinVer,OUTPUT);//Declaramos el pin verde como salida

  
  // PINES DEL SENSOR ULTRASONICO
  pinMode(Trigger1, OUTPUT); //pin como salida  SENSOR ULTRASÓNICO
  pinMode(Echo1, INPUT);  //pin como entrada  SENSOR ULTRASÓNICO
  digitalWrite(Trigger1, LOW);//Inicializamos el pin con 0 SENSOR ULTRASÓNICO
  pinMode(ledPinRojo1,OUTPUT);//Declaramos el pin rojo como salida
  pinMode(ledPinVer1,OUTPUT);//Declaramos el pin verde como salida
  

  // conectando al Wi-Fi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  //------- Haciendo petición del tiempo a una NTP (Network Time Protocol) -------//
  // CONFIGURACIÓN DE ZONA HORARIA EN ÉSTE CASO PARA MÉXICO
  // configtime (timezone, dst, servidores ntp
  // para México el time zone = -6 * 3600, dst = 0, "pool.ntp.org", "time.nist.gov"
  
  configTime(-21600, 0, "pool.ntp.org", "time.nist.gov");
  
  //---------------------- Fin de petición del tiempo ---------------------------//
  
  Serial.print("Conectando");
  
  // Si el estado de conección es diferente al de "Conectado"
  // envía un punto suspensivo en espera de conección
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  // Al activarse la conección se envía un mensaje
  Serial.print("Conectado: ");
  // Se visualiza la dirección IP asignada al módulo ESP8266
  Serial.println(WiFi.localIP());
  Serial.println();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

}
// ------------------------- FIN DEL SETUP ---------------------------------------//

int conta = 1;


  // ------  CODIGO DEL CALCULO DE LA HORA Y FECHA  DE LA RED NTP ---------------// 
  String strFechaHora(){
  time_t now;
  time(&now);
  char time_output[30];
  // strftime(time_output, 30, "%a  %d-%m-%y %T", localtime(&now));
  // %d = día   %m = mes   %y = año   %T = 24 hrs
  strftime(time_output, 30, "%d-%m-%y %T", localtime(&now));
  return String(time_output); // Respuesta: Día, Fecha y Hora  
  }

  String fecha(){
  time_t now;
  time(&now);
  char date[30];
  // strftime(time_output, 30, "%a  %d-%m-%y %T", localtime(&now));
  // %d = día   %m = mes   %y = año   %T = 24 hrs
  strftime(date, 30, "%d-%m-%y", localtime(&now));
  return String(date); // Respuesta: Día, Fecha y Hora  
  }
  
  // ------  FIN DE CODIGO DEL CALCULO DE LA HORA Y FECHA  DE LA RED NTP -------//


// ------------------------------ LOOP ------------------------------------------//

void loop(){

  // ------------------  CODIGO DEL SENSOR SENSOR ULTRASÓNICO 1 -------------------//

    long t; //timepo que demora en llegar el eco -- variable para el sensor PIR
    long d; //distancia en centimetros -- variable para el sensor PIR

    digitalWrite(Trigger, HIGH);
    delayMicroseconds(10);          //Enviamos un pulso de 10us
    digitalWrite(Trigger, LOW);
  
    t = pulseIn(Echo, HIGH); //obtenemos el ancho del pulso
    d = t/59;             //escalamos el tiempo a una distancia en cm



    if (d <= 25){
      Serial.print("Objeto detectado: ");
      Serial.print(d);      //Enviamos serialmente el valor de la distancia
      Serial.print("cm");
      Serial.println();
      digitalWrite(ledPinRojo,HIGH);
      digitalWrite(ledPinVer,LOW);  
      delay(500); //Hacemos una pausa de 2s
    
    
      Firebase.setString("SENSOR1/Objeto Detectado","Vehiculo1");
    
      if(Firebase.failed())
      {
        Serial.print("Seteando/estado sensor Fallido:");
        Serial.println(Firebase.error());
        return;
      }
    }
    else
    {
    Serial.print("En espera...");
    Serial.println();
    Serial.println(strFechaHora());
    
    digitalWrite(ledPinVer,HIGH);
    digitalWrite(ledPinRojo,LOW);
    Firebase.remove("SENSOR1/Objeto Detectado");
    Firebase.setString("SENSOR1","En Espera");
    delay(500);
    }

    // ------------------  CODIGO DEL SENSOR SENSOR ULTRASÓNICO 2 -------------------//

    long t1; //timepo que demora en llegar el eco -- variable para el sensor PIR
    long d1; //distancia en centimetros -- variable para el sensor PIR

    digitalWrite(Trigger1, HIGH);
    delayMicroseconds(10);          //Enviamos un pulso de 10us
    digitalWrite(Trigger1, LOW);
  
    t1 = pulseIn(Echo1, HIGH); //obtenemos el ancho del pulso
    d1 = t1/59;             //escalamos el tiempo a una distancia en cm



    if (d1 <= 25){
      Serial.print("Objeto detectado: ");
      Serial.print(d1);      //Enviamos serialmente el valor de la distancia
      Serial.print("cm");
      Serial.println();
      digitalWrite(ledPinRojo1,HIGH);
      //digitalWrite(ledPinVer1,LOW);  
      delay(500); //Hacemos una pausa de 2s
    
    
    Firebase.pushString("SENSOR2/Objeto Detectado","Vehiculo1");

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
    Serial.println(strFechaHora());
    
    //digitalWrite(ledPinVer1,HIGH);
    digitalWrite(ledPinRojo1,LOW);
    //Firebase.remove("SENSOR2/Objeto Detectado");
    Firebase.setString("SENSOR2","En Espera");
    delay(500);
    }

   // ------------------ FIN DEL CÓDIGO DEL SENSOR SENSOR ULTRASÓNICO-------------------//
}
