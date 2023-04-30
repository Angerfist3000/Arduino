//librerias utilizadas
#include <ESP8266Wifi.h>
#include <FirebaseArduino.h>
#include <time.h>
#include <ESP8266HTTPClient.h>

//nombre de mi red wi-fi
#define WIFI_SID ""
//password de mi red wi-fi
#define WIFI_PASSWORD ""

//variable de cadena de texto
String msjenviado;

//pin digital GPI02 - D2 del ESP8266 para el Trigger del sensor SENSOR ULTRASONICO
const int Trigger = 4;
//pin digital GPI02 - D4 del ESP8266 para el Trigger del sensor SENSOR ULTRASONICO
const int Echo = 2;
//se declara el pin al que se conecta el Led
int ledPinAlarma1 = 5; //GPI014 es el D5 de la placa ESP8266
//se define una variable que tomara la hora para MEX central
//del NIP de la funcion strFechaHora()
String strFechaHoraNTP;
string strFechaNTP;

//----------------INICIO DE SETUP--------------------//
void setup() {
  Serial.begin(115200);

  //PINES DEL SENSOR ULTRASONICO
  pinMode(Trigger,OUTPUT);//pin como salida SENSOR ULTRASONICO
  pinMode(Echo,INPUT);//pin como entrada SENSOR ULTRASONICO
  digitalWrite(Trigger,LOW);//iniciamos el pin como 0 SENSOR ULTRASONICO

  //conectando al wi-fi
  Wifi.begin(WIFI_SID,WIFI_PASSWORD);

  //----------HACIENDO PETICION DEL TIRMPO A UNA NTP (Network Time Protocol)-------//
  //CONFIGURACION DE ZONA HORARIA, EN ESTE CASO PARA MEXICO
  //configtime (timezone,dst,servidores ntp
  //para mexico el time zone = -6 * 3600,dst=0, "pool.ntp.org", "time.nist.gov"

  configTime(-21600,0,"pool.ntp.org", "time.nist.gov");
  
  //--------FIN DE PETICION DE TIEMPO----------//

  Serial.print("Conectando");
  //si el estado de coneccion es diferente al de "Conectado"
  //envia un punto suspensivo en espera de coneccion

  while(Wifi.status()!= WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  //al activarse la conexion se envia un mensaje 
  Serial.println(Wifi.localIP());
  Serial.println();

}
//----------FIN DEL SETUP----------//

int conta=1;

  //---------CODIGO DEL CALCULO DE LA HORA Y FECHA DE LA RED NTP--------//
  String strFechaHora(){
    time_t now;
    time(&now);
    char time_output[30];
    //strftime(time_output,30, "%a %d-%m-%y %T", localtime(&now));
    // %d=dia %m=mes %y=año %T=24 hrs
    strftime(time_OUTPUT,30,"%d-%m-%y %T", localtime(&now));
    return String(time_output);//respuesta: dia, fecha y hora
  }

  String fecha(){
    time_t now;
    time(&now);
    char date[30];
    //strftime(time_output,30, "%a %d-%m-%y %T", localtime(&now));
    // %d=dia %m=mes %y=año %T=24 hrs
    strftime(date,30,"%d-%m-%y %T", localtime(&now));
    return String(date);//respuesta: dia, fecha y hora
  }
  //-------FIN DE CODIGO DE CALCULO DE LA HORA Y FECHA DE LA RED NTP-------//

//---------INICIO DE LOOP----------//  
void loop() {
  //--------CODIGO DE SENSOR ULTRASONICO---------//
  long t;//tiempo que demora en llegar el eco-- variable para el sensor PIR
  long d;//distancia en centimetros-- variable para sensor PIR

  digitalWrite(Trigger,HIGH);
  delayMicrosecounds(10); //enviamos un pulso de 10us
  digitalWrite(Trigger,LOW);

  t = pulseIn(Echo,HIGH); //obtenemos el ancho del pulso
  d = t/59; //escalamos el tiempo a una distancia a cm

  if(d<=25){
    Serial.print("Distancia: ");
    Serial.print(d);// enviamos serialmente el valor de la distancia
    Serial.print("cm");
    Serial.printl();
    Serial.println("Objeto Detectado");

    HTTPClient http; //se declara un obejeto de la HTTPClient

    msjenviado = "http://api.callmeboot.com/whatsapp.php?";
    msjenviado = msjenviado + "phone=+521";
    msjenviado = msjenviado + "&text=Alerta+se+ha+detectado+un+movimiento";
    msjenviado = msjenviado + "&apikey=";
    http.begin(msjenviado);
    int httpCode = http.GET();//enviando la peticion
    if(httpCode > 0){
      //se verifica el regreso del codigo
      String payload = http.getString();//obteniendo la respuesta a la solicitud
      Serial.println(payload);//imprimiendo la tarea
    }
  http.end();//cerrando la conexion

    delay(200)//hacemos una pausa de 2s

  }

  else{
    Serial.println("En espera...")
    Serial.println(strFechaHora());

    delay(2000);//hacemos pausa de 2s

  }


}
