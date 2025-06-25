/*
  Sistema de Estacionamiento Automático con Arduino
  - 4 fotorresistencias detectan lugares ocupados/libres.
  - LEDs rojos para ocupado, verdes para libre.
  - Botón de entrada activa servo solo si hay lugares libres.
  - Botón de salida activa servo siempre.
*/

// Incluimos la librería para manejar los servomotores
#include <Servo.h>

// --- CONFIGURACIÓN DE PINES ---
const int pinesLDR[] = {A0, A1, A2, A3};
const int pinesLedRojo[] = {2, 4, 6, 8};
const int pinesLedVerde[] = {3, 5, 7, 9};
const int numSensores = 4;
const int pinBotonEntrada = 12;
const int pinBotonSalida = 13;
const int pinServoEntrada = 10;
const int pinServoSalida = 11;

// --- OBJETOS Y VARIABLES GLOBALES ---
Servo servoEntrada;
Servo servoSalida;
int umbral = 400;
int lugaresOcupados = 0;

// <<< PROTOTIPO DE LA FUNCIÓN (AQUÍ ESTÁ LA CORRECCIÓN)
// Esta línea le dice al compilador: "Más adelante encontrarás una función con este nombre y estos parámetros".
void abrirCerrarBarrera(Servo& motor);

void setup() {
  Serial.begin(9600);
  Serial.println("Sistema de Estacionamiento Activado");

  // Configurar pines de LEDs como SALIDA
  for (int i = 0; i < numSensores; i++) {
    pinMode(pinesLedRojo[i], OUTPUT);
    pinMode(pinesLedVerde[i], OUTPUT);
  }

  // Configurar pines de botones como ENTRADA con resistencia PULLUP interna
  pinMode(pinBotonEntrada, INPUT_PULLUP);
  pinMode(pinBotonSalida, INPUT_PULLUP);

  // Conectar los objetos servo a sus pines
  servoEntrada.attach(pinServoEntrada);
  servoSalida.attach(pinServoSalida);

  // Posición inicial de las barreras (0 grados = cerradas)
  servoEntrada.write(0);
  servoSalida.write(0);
  delay(500);
}

void loop() {
  // --- 1. ACTUALIZAR ESTADO DE LOS LUGARES ---
  lugaresOcupados = 0;
  for (int i = 0; i < numSensores; i++) {
    int valorLuz = analogRead(pinesLDR[i]);
    if (valorLuz < umbral) { // Lugar Ocupado
      digitalWrite(pinesLedRojo[i], HIGH);
      digitalWrite(pinesLedVerde[i], LOW);
      lugaresOcupados++;
    } else { // Lugar Libre
      digitalWrite(pinesLedRojo[i], LOW);
      digitalWrite(pinesLedVerde[i], HIGH);
    }
  }

  Serial.print("Lugares Ocupados: ");
  Serial.print(lugaresOcupados);
  Serial.print(" de ");
  Serial.println(numSensores);

  // --- 2. VERIFICAR BOTONES Y ACCIONAR BARRERAS ---
  if (digitalRead(pinBotonEntrada) == LOW) {
    if (lugaresOcupados < numSensores) {
      Serial.println("ACCESO PERMITIDO. Abriendo barrera de entrada...");
      abrirCerrarBarrera(servoEntrada);
    } else {
      Serial.println("ACCESO DENEGADO. Estacionamiento lleno.");
      delay(500); 
    }
  }

  if (digitalRead(pinBotonSalida) == LOW) {
    Serial.println("SALIDA. Abriendo barrera...");
    abrirCerrarBarrera(servoSalida);
  }
  
  delay(50);
}

// --- DEFINICIÓN DE LA FUNCIÓN ---
// La función se queda aquí al final, pero ya fue "anunciada" por el prototipo.
void abrirCerrarBarrera(Servo& motor) {
  motor.write(90);  // Mover a 90 grados (abierto)
  delay(2000);      // Esperar 2 segundos
  motor.write(0);   // Mover a 0 grados (cerrado)
  delay(500);       // Pausa para evitar dobles pulsaciones
}
