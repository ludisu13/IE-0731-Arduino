#include "Arduino.h"
#include "TaskScheduler.h" //Se carga la libreria del Scheduler
Scheduler RealTimeCore; //Esto crea un objeto del tipo Scheduler

void tarea01Fun();
void PID(); 

//Salida del controlador
double u = 0;
double u_w = 0;

//Valores iniciales 
double y_actual = 0;
double y_pasado = 0;
double r_actual = 0; 
double r_pasado = 0;
double Int = 0;
int M = 0;
double X = 0;

//Parametros del Controlador
double Kp = 11.796;
double Ti = 6.6456;
double Td = 0.824;
double Ts = 0.1;
double alfa = 0.1;

double P = 0; // Accion proporcional
double I = 0; // Accion integral
double D = 0; // Accion derivativa

//Pines de entrada analogicos
int pinSP = A0;
int pinX  = A1;
int pinY = A3;

// Pin de salida analogica
int pinU = 5;


//Pines de entrada y salida digitales
int pinLED =12;
int pinM = 13;


// Creacion de tareas:

Task tarea01(100, TASK_FOREVER, &tarea01Fun, &RealTimeCore); //Tarea de monitoreo de señales. Se ejecuta cada 10ms
Task PID_task(Ts*1000, TASK_FOREVER, &PID, &RealTimeCore); //Tarea del controlador PID. Se ejecuta cada Ts = 0.1s


// Funcion de Monitoreo: Imprime la señales de interes ya normalizadas entre 0 y 100% en el puerto serial.
// Utiliza un formato csv para poder utilizar el plotter de Arduino.

void tarea01Fun(){
    //Serial.print("El valor deseado es: ");
    Serial.print(r_actual*100/1023);
    Serial.print(",");
    Serial.print(y_actual*100/1023);
    Serial.print(",");
    Serial.print(u*100/1023);
    Serial.print(",");
    Serial.println(X*100/1023);
}

// Controlador PID: Si esta en modo automatico (M=0V) calcula las acciones proporcional, integral y derivativa.
// La accion de control es normalizada entre los valores de 0 a 1023 para implementar el antiwindup y no 
// escribir un valor mas alto del que soportan los puertos.
// La accion integral se calcula de tal forma que sea la que aplica en el siguiente tiempo de muestreo.
void PID () {
  
  if(!M){
    P = Kp*(r_actual-y_actual);
    D = alfa*Td*D/(alfa*Td+Ts)-Kp*Td*((r_pasado-y_pasado)-(r_actual-y_actual))/(alfa*Td+Ts);
    u = P + D + I;
    if(u > 1023 ) {
      u = 1023; 
    } else if(u < 0) {
      u = 0; 
    }
    
    analogWrite(pinU,u*255/1023);
    I = I + ((Kp*Ts/Ti))*(r_actual-y_actual);
    
  }
}

/*Funcion de inicializacion: 
    - Inicializa el puerto serial
    - Inicializa el Scheduler
    - Agrega las tareas al Scheduler y las habilita
    - Configura los pines de entrada y salida
*/

void setup() {
 
  Serial.begin(9600); //se inicia la comunicación serial a 9600 bauds
  RealTimeCore.init(); //Inicializa el scheduler
  //Se agregan las tareas al scheduler
  RealTimeCore.addTask(tarea01); 
  RealTimeCore.addTask(PID_task);  
  // Se habilitan las tareas
  tarea01.enable();
  PID_task.enable();
  // Se configuran los puertos
  pinMode(pinY, INPUT);
  pinMode(pinU, OUTPUT);
  pinMode(pinSP, INPUT);
  pinMode(pinM, INPUT);
  pinMode(pinLED, OUTPUT);
  pinMode(pinX, INPUT); 
  
}

// En el loop se ejecuta el Scheduler, la accion manual del sistema (M=5V) y se actualizan
// los valores de los pines y_actual y r_actual. Se guardan los valores anteriores en las
// variables y_pasado y r_pasado.

void loop() {
  
  RealTimeCore.execute();
  M = digitalRead(pinM);
 
  if(M){
    // Accion manual
    digitalWrite(pinLED,HIGH); // Se prende un LED para indicar el cambio a manual
    X = analogRead(pinX); // Se escrive directamente del puerto X al puerto u
    u = X;
    analogWrite(pinU,u*255/1023);
  }else{
    digitalWrite(pinLED,LOW);
    
    }

  y_pasado = y_actual;
  y_actual = analogRead(pinY);
  
  r_pasado = r_actual;
  r_actual = analogRead(pinSP);
    
  
}



