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
#include <TaskScheduler.h> //Se carga la librería del Scheduler
Scheduler RealTimeCore; //Esto crea un objeto del tipo Scheduler (definido por la librería) en este caso, el objeto se llama RealTimeCore, pero se puede poner cualquier nombre

// Se deben definir los prototipos de las funciones de cada una de las tareas, en este caso se definen tres funciones. En inglés es lo que se llaman Callbacks
void tarea01Fun();
void tarea02Fun();
void tarea03Fun();
void tarea04Fun();
const int buttonPin = 2;     // the number of the pushbutton pin
// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int outputPort[] = {3,5,6};
bool bLedIsOn=false;
int TickCont = 0;
int OnTime = 1; // 50ms

// Acá se crean las tareas. Las tareas son objetos del tipo Task definidos por la librería:
Task tarea01(2000, TASK_FOREVER, &tarea01Fun, &RealTimeCore); //Tarea que se repite cada 1000 milisegundos indefinidamente
Task tarea02(3000, TASK_FOREVER, &tarea02Fun, &RealTimeCore); //Tarea que se repite cada 3000 milisegundos indefinidamente
Task tarea03(5000, TASK_FOREVER, &tarea03Fun, &RealTimeCore); //Tarea que se repite cada 5000 milisegundos indefinidamente
Task tarea04(50,TASK_FOREVER,&tarea04Fun, &RealTimeCore);///tarea de apagar

// Ahora se deben definir explícitamente las funciones
void tarea01Fun(){
  digitalWrite(outputPort[0], HIGH); 
  
}

void tarea02Fun(){
 digitalWrite(outputPort[1], HIGH); 
}

void tarea03Fun(){
  digitalWrite(outputPort[2], HIGH); 
}
void tarea04Fun(){
  //Serial.print("Checcking LEDs");
  TickCont++;
  if(TickCont == 41){
     digitalWrite(outputPort[0], LOW);
  }else if(TickCont == 61){
     digitalWrite(outputPort[1], LOW);
  }else if(TickCont==81){
     digitalWrite(outputPort[2], LOW);
  }
}

void setup() {
  
  // El código que se ponga acá se ejecuta una única vez al inicio:
  Serial.begin(9600); //se inicia la comunicación serial a 9600 bauds
  RealTimeCore.init(); //Inicializa el scheduler
  Serial.println("Se inicializo el Scheduler");
  RealTimeCore.addTask(tarea01); //Se agrega la tarea 01 al scheduler
  RealTimeCore.addTask(tarea02); //Se agrega la tarea 02 al scheduler
  RealTimeCore.addTask(tarea03); //Se agrega la tarea 03 al scheduler
  RealTimeCore.addTask(tarea04); //Se agrega la tarea 04 al scheduler
  Serial.println("Se agregaron las tareas al Scheduler");
  tarea01.enable(); // Se pone el flag de enable para la tarea 01. Por default, las tareas están desabilitadas
  tarea02.enable(); // Se pone el flag de enable para la tarea 02. Por default, las tareas están desabilitadas
  tarea03.enable(); // Se pone el flag de enable para la tarea 03. Por default, las tareas están desabilitadas
  tarea04.enable(); // Se pone el flag de enable para la tarea 04. Por default, las tareas están desabilitadas
  // initialize digital pin LED_BUILTIN as an output.
  
  for(int i=0; i<3; i++){
    pinMode(outputPort[i], OUTPUT);
  }
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

void loop() {
  // Acá va el código que se repite indefinidamente:
  RealTimeCore.execute(); // Cuando se usa un scheduler, esta instrucción es la única que debería estar en el loop
}
