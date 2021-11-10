#include <SoftwareSerial.h>

SoftwareSerial esp8266(3,2); 

const int pin10 = 10;
const int pin9 = 9;

void setup() {

  Serial.begin(9600);  // monitor serial del arduino

  esp8266.begin(9600); // baud rate del ESP8255

  pinMode(pin10, OUTPUT);
  digitalWrite(pin10, LOW);
  pinMode(pin9, OUTPUT);
  digitalWrite(pin9, LOW);

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
    if (esp8266.find((char*)"+IPD,")) {

      delay(1500); // esperar que lleguen los datos hacia el buffer
      
      int conexionID = esp8266.read()-48; // obtener el ID de la conexión para poder responder

//    getting query params
      esp8266.find((char*)"?"); // bucar el texto "led="
      String query_params = "";
      for (int i = 0 ; esp8266.available() > 0 && i != 14; i++) {
        char c = esp8266.read();
        query_params += c;
      }
      Serial.println("\n");
      Serial.print("query params: ");
      Serial.println(query_params);
      Serial.print("\n");
//      pin9=0&pin10=1
//      012345678901234
      int state_pin9 = query_params[5] == '1'? 1 : 0;
      int state_pin10 = query_params[13] == '1'? 1 : 0;
      
      
      //responder y cerrar la conexión para que el navegador no se quede cargando 
      // página web a enviar
      String webpage = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";

      webpage += state_pin9 ? "{ pin9: true," : "{ pin9: false,";
      webpage += state_pin10 ? " pin10: true }" : " pin10: false }";

      digitalWrite(pin9, state_pin9);
      digitalWrite(pin10, state_pin10);
      
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
