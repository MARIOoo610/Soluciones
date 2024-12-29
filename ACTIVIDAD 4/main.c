//Hay que tener en cuenta que no he podido ejecutar esto (pq el include me da error) y que lo de los semafóros lo he usado buscando tres cosas en internet. No me hago responsable si en este código hay atrocidades. Un saludo y buen buen día.
#include <Arduino.h>

//Declaramos el  semáforo
SemaphoreHandle_t semaforoMoros;
SemaphoreHandle_t semaforoCristianos;

void moros(){
    if (xSemaphoreTake(semaforoMoros, portMAX_DELAY) == pdTRUE) {
        Serial.print("¡Moros y "); // Imprime la primera parte
        // Libera el semáforo para cristianos()
        xSemaphoreGive(semaforoCristianos);
    }
}

void cristianos(){
     if (xSemaphoreTake(semaforoCristianos, portMAX_DELAY) == pdTRUE) {
            Serial.println("cristianos!"); // Imprime la segunda parte
            // Libera el semáforo para moros()
            xSemaphoreGive(semaforoMoros);
     }
}

void setup(){
    Serial.begin(9600);

    // Crear semáforos binarios
    semaforoMoros = xSemaphoreCreateBinary();
    semaforoCristianos = xSemaphoreCreateBinary();

    //Inicializar  con moros()
    // Inicializar con moros() habilitado primero
    xSemaphoreGive(semaforoMoros);
}

void loop(){
    cristianos();
    moros();
}
