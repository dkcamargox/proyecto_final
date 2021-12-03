// declaracion de pines de salida
int ledPin2 = 9;  // LED connected to digital pin 13
int ledPin1 = 10;  // LED connected to digital pin 13

// declaracion de pines para lectura de los fines de carrera
int inPin1 = 12;    // pushbutton connected to digital pin 7
int inPin2 = 13;    // pushbutton connected to digital pin 7
int inPin3 = 11;    // pushbutton connected to digital pin 7

// declaracioon de variables para lectura
int val1 = 0;      // variable to store the read value
int val2 = 0;      // variable to store the read value
int val3 = 0;      // variable to store the read value

// estado de giro del motor
bool stateLeft = false;
bool stateRight = false;

void setup() {
  // declaracion de tipo de trabajo de cada pin
  pinMode(ledPin1, OUTPUT);  // sets the digital pin 13 as output
  pinMode(ledPin2, OUTPUT);  // sets the digital pin 13 as output
  
  pinMode(inPin1, INPUT);    // sets the digital pin 7 as input
  pinMode(inPin2, INPUT);    // sets the digital pin 7 as input
  pinMode(inPin3, INPUT);    // sets the digital pin 7 as input

}

void loop() {
  // lectura de los fines de carrera
  val1 = digitalRead(inPin1);   // read the input pin
  val2 = digitalRead(inPin2);   // read the input pin
  val3 = digitalRead(inPin3);   // read the input pin

  //si el valor del fin de carrera de paro es falso
  if(!val2) {
    // si el valor del fin de carrera del lado derecho esta en high y el otro pin no esta en high
    if(val3 && !stateLeft) {
      // espera un segundo para que pare correctamente el motor
      delay(1000);

      // gira para el lado derecho
      digitalWrite(ledPin1, HIGH);
      stateRight = true;
    }

    // si el valor del fin de carrera del lado izquierdo esta en high y el otro pin no esta en high
    if(val1 && !stateRight) {
      // espera un segundo para que pare correctamente el motor
      delay(1000);

      // gira para el lado izquierdo
      digitalWrite(ledPin2, HIGH);
      stateLeft = true;
    } 
  // si el fin de carrera de paro fue activado
  } else {
    // apaga todos los pines
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);

    // resetea los estados
    stateRight = false;
    stateLeft = false;
    
  }
  
}
