#include <Arduino.h>

// Definiciones
#define ARRAY_SIZE 40
#define DATA_RATE 2000  // Datos por segundo

// Variables compartidas
float data[ARRAY_SIZE];  // Array de datos
volatile int indexData = 0;  // Índice para introducir datos
SemaphoreHandle_t dataReadySemaphore;  // Semáforo para sincronizar

// Función para calcular la media
float calcularMedia(float *array, int size) {
    float suma = 0;
    for (int i = 0; i < size; i++) {
        suma += array[i];
    }
    return suma / size;
}

// Función para calcular la mediana
float calcularMediana(float *array, int size) {
    float sorted[size];
    memcpy(sorted, array, size * sizeof(float));
    // Ordenar el array
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (sorted[i] > sorted[j]) {
                float temp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = temp;
            }
        }
    }
    // Calcular mediana
    if (size % 2 == 0) {
        return (sorted[size / 2 - 1] + sorted[size / 2]) / 2.0;
    } else {
        return sorted[size / 2];
    }
}

// Tarea en Core 0: Introducir datos en el array
void tareaCore0(void *pvParameters) {
    while (true) {
        // Generar datos simulados y agregarlos al array
        data[indexData] = random(0, 100) / 1.0;  // Datos entre 0 y 100
        indexData++;

        // Si el array está lleno, notificar al Core 1
        if (indexData == ARRAY_SIZE) {
            indexData = 0;  // Reiniciar índice
            xSemaphoreGive(dataReadySemaphore);  // Liberar semáforo
        }

        // Esperar el tiempo adecuado para cumplir con la tasa de 2000 datos por segundo
        delayMicroseconds(500);  // 1 segundo / 2000 = 500 µs
    }
}

// Tarea en Core 1: Calcular media y comparar con mediana
void tareaCore1(void *pvParameters) {
    while (true) {
        // Esperar a que Core 0 llene el array
        xSemaphoreTake(dataReadySemaphore, portMAX_DELAY);

        // Calcular media y mediana
        float media = calcularMedia(data, ARRAY_SIZE);
        float mediana = calcularMediana(data, ARRAY_SIZE);

        // Mostrar resultado
        if (media > mediana) {
            Serial.println("1");
        } else {
            Serial.println("0");
        }
    }
}

void setup() {
    Serial.begin(115200);

    // Crear el semáforo
    dataReadySemaphore = xSemaphoreCreateBinary();

    // Crear tareas
    xTaskCreatePinnedToCore(
        tareaCore0,        // Función de la tarea
        "Core0_TomarDatos",  // Nombre de la tarea
        1024,              // Tamaño de la pila
        NULL,              // Parámetros de entrada
        1,                 // Prioridad
        NULL,              // Identificador de la tarea
        0                  // Núcleo 0
    );

    xTaskCreatePinnedToCore(
        tareaCore1,        // Función de la tarea
        "Core1_ProcesarDatos",  // Nombre de la tarea
        1024,              // Tamaño de la pila
        NULL,              // Parámetros de entrada
        1,                 // Prioridad
        NULL,              // Identificador de la tarea
        1                  // Núcleo 1
    );
}

void loop() {
    // El loop no se utiliza, ya que todo está gestionado por tareas
}
