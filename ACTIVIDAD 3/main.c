#include <Arduino.h>

//Pines de los LEDs
const int LED_ROJO = 2;
const int LED_AZUL = 4;

//TAREAS CORTAR Y LAVAR
void cortar(){
    Serial.println("chop chop")
    digitalWrite(LED_ROJO, HIGH); //Se enciende el LED rojo
    delay(1000); //En este tiempo se corta
    digitalWrite(LED_ROJO, LOW);
}

void lavarPlatos(){
    Serial.println("wash wash")
    digitalWrite(LED_AZUL, HIGH); //Se enciende el LED azul
    delay(1000); //En este tiempo se lava
    digitalWrite(LED_AZUL, LOW);
}

void setup(){
    //Se inicializa Serial y los LEDS
    Serial.begin(9600);
    pinMode(LED_ROJO, OUTPUT);
    pinMode(LED_AZUL, OUTPUT);

    //PARA PARALELIZAR HAY QUE  REALIZAR LAS TAREAS EN DIFERENTES CORES

    xTaskCreatePinnedToCore(
        cortar,         // Función de la tarea
        "Cortar",       // Nombre de la tarea
        1024,           // Tamaño de pila
        NULL,           // Parámetros de entrada
        1,              // Prioridad
        NULL,           // Identificador de tarea
        0               // Núcleo 0
    );

    xTaskCreatePinnedToCore(
        lavarPlatos,    // Función de la tarea
        "LavarPlatos",  // Nombre de la tarea
        1024,           // Tamaño de pila
        NULL,           // Parámetros de entrada
        1,              // Prioridad
        NULL,           // Identificador de tarea
        1               // Núcleo 1
    );
}

void loop(){
    cortar();
    lavarPlatos();
}