#include <SoftwareSerial.h>

SoftwareSerial esp8266(3,2); 

const int led1 = 10;
const int led2 = 9;

void setup() {

  Serial.begin(9600);  // monitor serial del arduino

  esp8266.begin(9600); // baud rate del ESP8255

  pinMode(led1, OUTPUT);
  digitalWrite(led1, LOW);

  sendData("AT+RST\r\n",2000);      // resetear módulo
  sendData("AT+CWMODE=1\r\n",1000); // configurar como cliente
  sendData("AT+CWJAP=\"Nexxt_BF9F64\",\"\"\r\n",8000); //SSID y contraseña para unirse a red 
  sendData("AT+CIFSR\r\n",1000);    // obtener dirección IP
  sendData("AT+CIPMUX=1\r\n",1000); // configurar para multiples conexiones
  sendData("AT+CIPSERVER=1,80\r\n",1000);         // servidor en el puerto 80
}

/*
Enviar comando al esp8266 y verificar la respuesta del módulo, todo esto dentro del tiempo timeout
*/
void sendData(String comando, const int timeout) {
  long int time = millis(); // medir el tiempo actual para verificar timeout
  
  esp8266.print(comando); // enviar el comando al ESP8266
  
  //mientras no haya timeout 
  while( (time+timeout) > millis()){
    //mientras haya datos por leer
    while(esp8266.available()) { 
      
      // Leer los datos disponibles
      char c = esp8266.read(); // leer el siguiente caracter
      
      Serial.print(c);
    }
  } 
  return;
}

void loop() {

  // revisar si hay mensaje del ESP8266
  if (esp8266.available())    {
    
    // revisar si el servidor recibio datos
    if (esp8266.find("+IPD,")) {

      delay(1500); // esperar que lleguen los datos hacia el buffer
      
      int conexionID = esp8266.read()-48; // obtener el ID de la conexión para poder responder
      
      esp8266.find("pin10="); // bucar el texto "led="
      int state10 = (esp8266.read()-48); // Obtener el estado del pin a mostrar
      
      digitalWrite(led1, state10); // Cambiar estado del pin
      
      esp8266.find("pin9=");
      
      int state9 = (esp8266.read()-48); // Obtener el estado del pin a mostrar
      
      digitalWrite(led2, state9); // Cambiar estado del pin
      
      
      while(esp8266.available()) {
        char c = esp8266.read();
        Serial.print(c);
      }

      //responder y cerrar la conexión para que el navegador no se quede cargando 
      // página web a enviar
      String webpage = "";
      if (state10==1) {
        webpage += "{ pin10: true, ";
      } else { 
        webpage += "{ pin10: false, ";
      }

      if (state9==1) {
        webpage += "pin9: true }";
      } else { 
        webpage += "pin9: false }";
      }
      // comando para enviar página web
      String comandoWebpage = "AT+CIPSEND=";
      
      comandoWebpage+=conexionID;

      comandoWebpage+=",";
      
      comandoWebpage+=webpage.length();
      
      comandoWebpage+="\r\n";
      
      sendData(comandoWebpage,1000);
      
      sendData(webpage,1000);
      
      // comando para terminar conexión
      String comandoCerrar = "AT+CIPCLOSE=";
      
      comandoCerrar+=conexionID;
      
      comandoCerrar+="\r\n";

      Serial.print("comandoCerrar:\t");
      Serial.println(comandoCerrar);
      
      sendData(comandoCerrar,3000);

      delay(1000);
    }
  }
}
