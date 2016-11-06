/*
 * Este programa sirve de plantilla para utilizar la librería
 * TaskScheduler en el curso IE0731: Sistemas en Tiempo discreto
 * 
 * Se recomienda revisar la documentación de la librería que está
 * en el sitio virtual del curso, junto con la librería.
 * El sitio donde el autor de la librería aloja el código es el siguiente:
 * https://github.com/arkhipenko/TaskScheduler
 */
#include "Arduino.h"
#include "TaskScheduler.h" //Se carga la librería del Scheduler
Scheduler RealTimeCore; //Esto crea un objeto del tipo Scheduler (definido por la librería) en este caso, el objeto se llama RealTimeCore, pero se puede poner cualquier nombre

// Se deben definir los prototipos de las funciones de cada una de las tareas, en este caso se definen tres funciones. En inglés es lo que se llaman Callbacks
void tarea01Fun();
//void tarea02Fun();
//void tarea03Fun();
void PID();

// Acá se crean las tareas. Las tareas son objetos del tipo Task definidos por la librería:
Task tarea01(100, TASK_FOREVER, &tarea01Fun, &RealTimeCore); //Tarea que se repite cada 1000 milisegundos indefinidamente
//Task tarea02(3000, TASK_FOREVER, &tarea02Fun, &RealTimeCore); //Tarea que se repite cada 3000 milisegundos indefinidamente
//Task tarea03(5000, 3, &tarea03Fun, &RealTimeCore); //Tarea que se repite sólo tres veces cada 5000 milisegundos

//Salida del controlador
double u = 0;
double u_w = 0;
//Valores iniciales 
double y_actual = 0;
double y_pasado = 0;
double r_actual = 0; //Valor entre 0 y 5 volts
double r_pasado = 0;
double Int = 0;
int M = 0;
double X = 0;

//Parametros del Controlador
double Kp = 6.1364;
double Ti = 8.1264;
double Td = 0.3305;
double Ts = 0.01;
double Tt = sqrt(Ti*Td);
double alfa = 0.1*Td;

//Pines Analogicos
int pinR = A0;
int pinX  = A1;
int pinY = A3;
int pinU = A5;

//Pines Digitales

int pinLED =12;
int pinM = 13;

Task PID_task(10, TASK_FOREVER, &PID, &RealTimeCore); 


// Ahora se deben definir explícitamente las funciones
void tarea01Fun(){
    //Serial.print("El valor deseado es: ");
    Serial.print(r_actual);
    Serial.print(",");
    Serial.print(y_actual);
    Serial.print(",");
    Serial.print(u);
    Serial.print(",");
    Serial.println(X);
}

//void tarea02Fun(){
//  Serial.print("Se ejecuta la tarea 02 a los "); //Escribe un string en el puerto serial
//  Serial.println(millis());
//}
//
//void tarea03Fun(){
//  Serial.print("Se ejecuta la tarea 03 a los "); //Escribe un string en el puerto serial
//  Serial.println(millis());
//}

void setup() {
  // El código que se ponga acá se ejecuta una única vez al inicio:
  Serial.begin(9600); //se inicia la comunicación serial a 9600 bauds
  RealTimeCore.init(); //Inicializa el scheduler
  Serial.println("Se inicializo el Scheduler");
  RealTimeCore.addTask(tarea01); //Se agrega la tarea 01 al scheduler
//  RealTimeCore.addTask(tarea02); //Se agrega la tarea 02 al scheduler
//  RealTimeCore.addTask(tarea03); //Se agrega la tarea 03 al scheduler
  RealTimeCore.addTask(PID_task);
  Serial.println("Se agregaron las tareas al Scheduler");
  tarea01.enable(); // Se pone el flag de enable para la tarea 01. Por default, las tareas están desabilitadas
//  tarea02.enable(); // Se pone el flag de enable para la tarea 02. Por default, las tareas están desabilitadas
//  tarea03.enable(); // Se pone el flag de enable para la tarea 03. Por default, las tareas están desabilitadas
  PID_task.enable();
  pinMode(pinY, INPUT);
  pinMode(pinU, OUTPUT);
  pinMode(pinR, INPUT);
  pinMode(pinM, INPUT);
  pinMode(pinLED, OUTPUT);
  pinMode(pinX, INPUT);
  
}

void loop() {
  // Acá va el código que se repite indefinidamente:
  RealTimeCore.execute(); // Cuando se usa un scheduler, esta instrucción es la única que debería estar en el loop
  M = digitalRead(pinM);
  if(M){
    digitalWrite(pinLED,HIGH);  
    X = analogRead(pinX)*5/1023;
    u = X;
    analogWrite(pinU,u*255/5);//Se escriben valores de 0 a 255, u varia de 0 a 5 volts 
  }else{
    digitalWrite(pinLED,LOW);
    
    }
    
  y_pasado = y_actual;
  y_actual = analogRead(pinY)*5/1023;//se convierte la lectura a un valor entre 0 y 5 volts, se leen valores de 0 a 1023
  
  r_pasado = r_actual;
  r_actual = analogRead(pinR)*5/1023;
  
}


void PID () {
  //Se utiliza aproximacion rectangular hacia atras
  //Int = Int + r_actual-y_actual;
  if(!M){
    double P = Kp*(r_actual-y_actual);
    double D = alfa*Td*D/(alfa*Td+Ts)+Kp*Td*((r_pasado-y_pasado)-(r_actual-y_actual))/(alfa*Td+Ts);
    //double I = ((Kp/Ti)+((u-u_w)/Tt))*Int;
    double I = I + ((Kp*Ts/Ti))*(r_pasado-y_pasado);
    //I = ((K/Ti)*(r-y)+((1/Tt)*))/s;
    u = P + D + I;
    if(u > 5 ) {
      u = 5; 
    } else if(u < 0) {
      u = 0; 
    }
    analogWrite(pinU,u*255/5);//Se escriben valores de 0 a 255, u varia de 0 a 5 volts
  }
}
