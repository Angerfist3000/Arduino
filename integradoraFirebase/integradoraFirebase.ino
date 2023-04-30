
//------------- Librerias utilizadas --------------------------------------------//

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <time.h>
#include <ESP8266HTTPClient.h>

// ------------------------ Se definen los valores fundamentales del proyecto ---//
// Host del proyecto -- sin https:// y sin la / al final únicamente la dirección del proyecto
#define FIREBASE_HOST "cleancar-179a8-default-rtdb.firebaseio.com"
//Secret de la BD
#define FIREBASE_AUTH "2J6dxBEJxXyPhkrAbDtXSjEKXoJZtdyhsR66boOZ"
// Nombre de mi red Wi-Fi
#define WIFI_SSID "IoT_Practs"

// Password de mi red WiFi
#define WIFI_PASSWORD "PracticasI0T"
//variable de cadena de texto
String msjenviado;

//Pin digital GPIO16 - D0 del ESP8266 para el Trigger del sensor SENSOR ULTRASÓNICO
const int Trigger = 16;
//Pin digital GPIO5 - D1 del ESP8266 para el Trigger del sensor SENSOR ULTRASÓNICO   
const int Echo = 5;
//Pin digital GPIO0 - D2 del ESP8266 para el LED rojo
int ledPinRojo = 0; 
//Pin digital GPIO4 - D3 del ESP8266 para el LED verde
int ledPinVer = 4; 

//Pin digital GPIO2 - D4 del ESP8266 para el Trigger del sensor SENSOR ULTRASÓNICO  
const int Trigger1 = 2;
//Pin digital GPIO14 - D3 del ESP8266 para el Trigger del sensor SENSOR ULTRASÓNICO  
const int Echo1 = 14;
//Pin digital GPIO13 - D7 del ESP8266 para el LED rojo
int ledPinRojo1 = 13; 
//Pin digital GPIO12 - D6 del ESP8266 para el LED verde 
int ledPinVer1 = 12; 

// Se define una variable que tomará la hora para Mex Central
// del NTP de la función strFechaHora()
String strFechaHoraNTP;
String strFechaNTP;
String strHoraNTP;

// --------------------------------------------------------------------------------//

// -----------------------INICIO DEL SETUP ---------------------------------------//
void setup() {
  Serial.begin(115200);

 // PINES DEL SENSOR ULTRASONICO 1
  pinMode(Trigger, OUTPUT); //pin como salida  SENSOR ULTRASÓNICO
  pinMode(Echo, INPUT);  //pin como entrada  SENSOR ULTRASÓNICO
  digitalWrite(Trigger, LOW);//Inicializamos el pin con 0 SENSOR ULTRASÓNICO
  pinMode(ledPinRojo,OUTPUT);//Declaramos el pin rojo como salida
  pinMode(ledPinVer,OUTPUT);//Declaramos el pin verde como salida

  // PINES DEL SENSOR ULTRASONICO 2
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

  // ---- Se inicia la conexión al HOST y a la Base de Datos -------------------//
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
}
// ------------------------- FIN DEL SETUP ---------------------------------------//

  //---CONTADOR PARA LOS VEHICULOS----//

  //------SENSOR1-------//
  int conta;//contador1 de las condicionales
  int contaFire;//contador firebase de espacio1
  int estado;
  //------SENSOR2-------//
  int conta1;//contador2 de las condicionales
  int contaFire1;//contador firebase de espacio2
  int estado1;
  //-----VENTAS DEL DIA----
  int contaTotal;//contador total de los dos espacios

  

  // ------CODIGO DEL CALCULO DE LA HORA Y FECHA  DE LA RED NTP---------------//  
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
  //------FIN DE CODIGO DEL CALCULO DE LA HORA Y FECHA  DE LA RED NTP--------//


// ------------------------------ LOOP ------------------------------------------//

void loop(){
  
//---------VARIABLES PARA LA FECHA Y HORA----------------------------//

    // se guarda en la variable strFechaHoraNTP la fecha y hora de la función strFechaHora()
    strFechaHoraNTP = strFechaHora();
    // se guarda en la variable strFechaNTP la fecha de la función fecha()
    strFechaNTP = fecha();
    // se guarda en la variable strHoraNTP la hora de la función strHora()
    strHoraNTP = strHora();
    

// ------------------  CODIGO DEL SENSOR SENSOR ULTRASÓNICO 1 -------------------//
    long t; //tiempo que demora en llegar el eco -- variable para el sensor PIR
    long d; //distancia en centimetros -- variable para el sensor PIR

    digitalWrite(Trigger, HIGH);
    delayMicroseconds(10);          //Enviamos un pulso de 10us
    digitalWrite(Trigger, LOW);
  
    t = pulseIn(Echo, HIGH); //obtenemos el ancho del pulso
    d = t/59;             //escalamos el tiempo a una distancia en cm

    
    
   
    //---DETECCION DE OBJETO--//
    if (d <= 4){
      Serial.print("Ocupado1:");
      Serial.print(d);      //Enviamos serialmente el valor de la distancia
      Serial.print("cm");
      Serial.println();
      digitalWrite(ledPinRojo,HIGH);
      digitalWrite(ledPinVer,LOW);  
      delay(500); //Hacemos una pausa de 2s

      //--------------ENVIO DE DATOS A FIREBASE-------//

      //-----DISPONIBILIDAD-----------//
      if(conta==0){
      estado=1;
      Firebase.setInt("Espacios Disponibles/Sensor1/Disponibilidad/",estado);
      //Manejo de error
      if(Firebase.failed()){
        Serial.print("Seteando /estado sensor Fallido");
        Serial.println(Firebase.error());  
      return;
      }
      }

      //--------CONTADOR-----------------------//
      
      if(conta==0){//si es TRUE manda el contador a Firebase/si es FALSE no se envia los datos
        contaFire++;      
      Firebase.setInt("Espacios Disponibles/Sensor1/Contador/",contaFire);//envio de datos de contador 
      //Firebase.setInt("Espacios Disponibles/Sensor1/Contador/"+strFechaNTP+"/",contaFire);//envio de datos de contador 
        contaTotal = contaFire+contaFire1;
        
      //----CONTADOR DEL DIA------//        
      Firebase.setInt("Ventas del Dia/"+strFechaNTP+"/",contaTotal);

      /*if(contaTotal==2){
      Firebase.setString("/"+strFechaNTP+"/","Sin Espacios Disponibles");
      }*/
      }
      conta++;//ahora conta vale 1
      delay(500); //Hacemos una pausa de 2s
      
    }
    
    //---FIN DE CONDICIONAL DE OBJETO DETECTADO-----//

    //---SI EL OBJETO NO ESTA EN EL RANGO---//
    else{

      estado=0;
      conta=0;//conta vuelve a su valor inicial de 0
      Serial.println("Disponible1");
      Serial.println(strFechaHora());
      digitalWrite(ledPinVer,HIGH);
      digitalWrite(ledPinRojo,LOW);
      
      
      Firebase.setInt("Espacios Disponibles/Sensor1/Disponibilidad/",estado);

     
      //Manejo de error     
      if(Firebase.failed()){
      Serial.print("Seteando/Nodo_hijo1 Fallido");
      Serial.println(Firebase.error());  
      return;
      }
     
      
      delay(5000); //Hacemos una pausa de 2s
    }
     //--CONTADOR DE LOS VEHICULOS INGRESADOS--//
// ------------------ FIN DEL CÓDIGO DEL SENSOR SENSOR ULTRASÓNICO 1-------------------// 


// ------------------CODIGO DEL SENSOR SENSOR ULTRASÓNICO 2 -------------------//

    long t1; //tiempo que demora en llegar el eco -- variable para el sensor PIR
    long d1; //distancia en centimetros -- variable para el sensor PIR

    digitalWrite(Trigger1, HIGH);
    delayMicroseconds(10);          //Enviamos un pulso de 10us
    digitalWrite(Trigger1, LOW);
  
    t1 = pulseIn(Echo1, HIGH); //obtenemos el ancho del pulso
    d1 = t1/59;             //escalamos el tiempo a una distancia en cm


    //---DETECCION DE OBJETO--//
    if (d1 <= 4){
      //----LECTURA DE DATOS DE MONITOR SERIAL------//
      Serial.print("Ocupado2");
      Serial.print(d1);     
      Serial.print("cm");
      Serial.println();
      digitalWrite(ledPinRojo1,HIGH);
      digitalWrite(ledPinVer1,LOW);  
      delay(500);//pausa de 1/2 segundo

      //--------------ENVIO DE DATOS A FIREBASE-------// 

      //-----DISPONIBILIDAD--------//
      if(conta1==0){
      estado1=1;
      Firebase.setInt("Espacios Disponibles/Sensor2/Disponibilidad/",estado1);
      //Manejo de error
      if(Firebase.failed()){
        Serial.print("Seteando/estado sensor Fallido");
        Serial.println(Firebase.error());  
      return;
      }
      }


      //--------CONTADOR-----------------------//
      
      if(conta1==0){//si es TRUE manda el contador a Firebase/si es FALSE no se envia los datos
      contaFire1++;//incrementamos el valor para la proxima condicion
      Firebase.setInt("Espacios Disponibles/Sensor2/Contador/",contaFire1);//envio de datos de contador
        

      //------CONTADOR DEL DIA---------//
      contaTotal = contaFire+contaFire1;
      Firebase.setInt("Ventas del Dia/"+strFechaNTP+"/",contaTotal);
      }
      conta1++;//ahora conta vale 1
    }

    else{
      estado1=0;
      conta1=0;//conta1 vuelve a su valor inicial de 0
      Serial.println("Disponible2");
      Serial.println(strFechaHora());
      digitalWrite(ledPinVer1,HIGH);
      digitalWrite(ledPinRojo1,LOW);

      Firebase.setInt("Espacios Disponibles/Sensor2/Disponibilidad/",estado1);
      
      //manejo de error de Firebase
      if (Firebase.failed()) {
        Serial.print("Seteando / Estado Sensor Fallido");
        Serial.println(Firebase.error());  
      return;
      }
      delay(500); //Hacemos una pausa de 2s
    }
}




 

 



  

