

//------------- Librerias utilizadas --------------------------------------------//

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <time.h>
#include <ESP8266HTTPClient.h>

// ------------------------ Se definen los valores fundamentales del proyecto ---//
// Host del proyecto -- sin https:// y sin la / al final únicamente la dirección del proyecto
#define FIREBASE_HOST "int.firebaseio.com"
// Secret de la Base de Datos
#define FIREBASE_AUTH "SECRETO"
// Nombre de mi red Wi-Fi
#define WIFI_SSID "INFINITUM0C72_2.4"
// Password de mi red WiFi
#define WIFI_PASSWORD "3M2FE7mxL7"
//variable de cadena de texto
String msjenviado;

//Pin digital GPIO4 - D2 del ESP8266 para el Trigger del sensor SENSOR ULTRASÓNICO
const int Trigger = 4;
//Pin digital GPIO2 - D4 del ESP8266 para el Trigger del sensor SENSOR ULTRASÓNICO   
const int Echo = 2;
//Se declara el pin al que se conecta el LED
int ledPinAlarma1 = 5; // GPIO14 es el D5 de la placa ESP8266
// Se define una variable que tomará la hora para Mex Central
// del NTP de la función strFechaHora()
String strFechaHoraNTP;
String strFechaNTP;
String strHoraNTP;

// --------------------------------------------------------------------------------//

// -----------------------INICIO DEL SETUP ---------------------------------------//
void setup() {
  Serial.begin(115200);

  // PINES DEL SENSOR ULTRASONICO
  pinMode(Trigger, OUTPUT); //pin como salida  SENSOR ULTRASÓNICO
  pinMode(Echo, INPUT);  //pin como entrada  SENSOR ULTRASÓNICO
  digitalWrite(Trigger, LOW);//Inicializamos el pin con 0 SENSOR ULTRASÓNICO
  

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

  // ---- Se inicia la conexión al HOST y a la Base de Datos -------------------//
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

  String strHora(){
  time_t now;
  time(&now);
  char hora[30];
  // strftime(time_output, 30, "%a  %d-%m-%y %T", localtime(&now));
  // %d = día   %m = mes   %y = año   %T = 24 hrs
  strftime(hora, 30, "%%T", localtime(&now));
  return String(hora); // Respuesta: Día, Fecha y Hora  
  }
  
  // ------  FIN DE CODIGO DEL CALCULO DE LA HORA Y FECHA  DE LA RED NTP -------//


// ------------------------------ LOOP ------------------------------------------//

void loop(){

            // se guarda en la variable strFechaHoraNTP la fecha y hora de la función strFechaHora()
            strFechaHoraNTP = strFechaHora();
            // se guarda en la variable strFechaNTP la fecha de la función fecha()
            strFechaNTP = fecha();
            // se guarda en la variable strHoraNTP la hora de la función strHora()
            strHoraNTP = strHora();

  // ------------------  CODIGO DEL SENSOR SENSOR ULTRASÓNICO -------------------//

    long t; //timepo que demora en llegar el eco -- variable para el sensor PIR
    long d; //distancia en centimetros -- variable para el sensor PIR

    digitalWrite(Trigger, HIGH);
    delayMicroseconds(10);          //Enviamos un pulso de 10us
    digitalWrite(Trigger, LOW);
  
    t = pulseIn(Echo, HIGH); //obtenemos el ancho del pulso
    d = t/59;             //escalamos el tiempo a una distancia en cm


// iniciando la condicional para la detección del objeto
    if (d <= 25){
      Serial.print("Distancia: ");
      Serial.print(d);      //Enviamos serialmente el valor de la distancia
      Serial.print("cm");
      Serial.println();
      Serial.println("Objeto Detectado");
/*
// --------------------- BLOQUE ENVÍO MSJ A WHATSAPP ------------------------------------
              //Enviando mensaje a WhatsApp
              Serial.print("Establececiendo conexión a");
              Serial.println(" mensajería WhatsApp");

              HTTPClient http; //Se declara un objeto de la clase HTTPClient
  
              msjenviado = "http://api.callmebot.com/whatsapp.php?";
              msjenviado = msjenviado + "phone=+5219611502656";
              msjenviado = msjenviado + "&text=Alerta+se+ha+detectado+un+objeto";
              msjenviado = msjenviado + "&apikey=129814";

              http.begin(msjenviado);
              int httpCode = http.GET(); //Enviando la petición
              if (httpCode > 0){
                 //se verifica el regreso del código
                 String payload = http.getString(); //obteniendo la respuesta a la solicitud
                 Serial.println(payload); //Imprimiendo la respuesta    
              }
              http.end(); //cerrando la conexión
// --------------------- FIN BLOQUE ENVÍO MSJ A WHATSAPP ------------------------------------
*/
      // -------------- envío del estado a Firebase Estado del Sensor Objeto Detectado -------//

    
            // Asignando valores de tipo String
            Firebase.setString("Tienda_SuperH/"+strFechaNTP+"/"+strFechaHoraNTP+"/DeptoElectronica", "Objeto Detectado");
            // manejo de error
            if (Firebase.failed()) {
              Serial.print("Seteando /DeptoElectronica Fallido:");
              Serial.println(Firebase.error());  
            return;
            }

            // Actualizando Valor de la fecha y hora
            Firebase.setString("Tienda_SuperH/"+strFechaNTP+"/"+strFechaHoraNTP+"/fechaHora", strFechaHoraNTP);
            // handle Manejo de errores
            if (Firebase.failed()) {
              Serial.print("Seteando /fechaHora Fallido:");
              Serial.println(Firebase.error());  
              return;
            }

            // Actualizando Valor distancia
            Firebase.setFloat("Tienda_SuperH/"+strFechaNTP+"/"+strFechaHoraNTP+"/distanciacm", d);
            // handle Manejo de errores
            if (Firebase.failed()) {
              Serial.print("Seteando /distanciacm Fallido:");
              Serial.println(Firebase.error());  
              return;
            }
// terminando la condicional de la detección del objeto

             // CONTADOR

            Serial.print("Contador:");
            Serial.println(conta);
            
            Firebase.setInt("Tienda_SuperH/"+strFechaNTP+"/contador/", conta);
            conta = conta+1;
            
      // ------------------------------------------------------------------------------//
      delay(2000); //Hacemos una pausa de 2s
    }

    else{

      /*
      Firebase.remove("Nodo_Principal/Nodo_hijo1/fechaHora");
      Firebase.remove("Nodo_Principal/Nodo_hijo1/distanciacm");
      */
      
      Serial.println("En Espera...");
      Serial.println(strFechaHora());

            // --------- envío del estado a Firebase Estado del Sensor DISPONIBLE -----//
            // Asignando valores de tipo String
            /*
            Firebase.setString("Nodo_Principal/Nodo_hijo1/Control_Escolar", "En Espera...");
            */
            // manejo de error
            if (Firebase.failed()) {
              Serial.print("Seteando /Nodo_hijo1 Fallido:");
              Serial.println(Firebase.error());  
            return;
            }
            
            // ------------------------------------------------------------------------//     
      delay(2000); //Hacemos una pausa de 2s
    }

  // ------------------ FIN DEL CÓDIGO DEL SENSOR SENSOR ULTRASÓNICO-------------------//
}
