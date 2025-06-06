/*
ver ::cl 20120520
Configuracion basica para modulo receptor  RR 10
Utiliza libreria VirtualWire.h
pin 01 5v
pin 02 Tierra
pin 03 antena externa
pin 07 tierra
pin 10 5v
pin 11 tierra
pin 12 5v
pin 14 Arduino pin digital 2
pin 15 5v
*/

#include <VirtualWire.h>

// Definiciones de protocolo
#define HEADER_BYTE 0xAA
#define EMITTER_ID 0x01
#define RECEIVER_ID_1 0x02
#define RECEIVER_ID_2 0x03 // El ID que nuestro receptor va a filtrar

// Tamaño de la imagen (32x32 píxeles)
const int IMAGE_WIDTH = 32;
const int IMAGE_HEIGHT = 32;
const int TOTAL_PIXELS = IMAGE_WIDTH * IMAGE_HEIGHT;
// leds
const int LED_PIN_BLUE = 12;
const int LED_PIN_GREEN = 13;

// Buffer para almacenar la imagen recibida
uint8_t received_image[TOTAL_PIXELS];
bool pixels_received[TOTAL_PIXELS] = {false}; // Array para verificar si un pixel ha sido recibido

// Flag para indicar si la imagen completa ha sido recibida
bool image_received_flag = false;

// Función para calcular el checksum
// CRC-8 (polinomio x^8 + x^2 + x + 1, 0x07)
uint8_t calculateChecksum(uint8_t *data, uint8_t len) {
  uint8_t crc = 0x00;
  for (uint8_t i = 0; i < len; i++) {
    crc ^= data[i];
    for (uint8_t j = 0; j < 8; j++) {
      if (crc & 0x80)
        crc = (crc << 1) ^ 0x07;
      else
        crc <<= 1;
    }
  }
  return crc;
}

const int DATA_BYTES_PER_PACKET = 20;
const int PACKET_SIZE = 1 + 1 + 1 + 1 + 1 + DATA_BYTES_PER_PACKET + 1;
// 25 bytes => Header + Emisor + Receptor + Start Index + Data + Checksum



bool is_complete(uint8_t *pixels_received) {
  // Verifica si todos los píxeles han sido recibidos
  for (int i = 0; i < TOTAL_PIXELS; i++) {
    if (pixels_received[i] == false) {
      return false; // Si algún pixel no ha sido recibido, retorna false
    }
  }
  return true; // Todos los píxeles han sido recibidos
}

void setup() {
  Serial.begin(9600); //  <--- CAMBIO AQUÍ: Velocidad del serial a 9600
  Serial.println("Configurando Recepcion de Imagen...");
  vw_set_ptt_inverted(true); // Requiere true para el modulo RR-10
  vw_setup(2000);             // Velocidad de transmisión de 2000 bps (VirtualWire)
  vw_set_rx_pin(2);           // Pin de recepción (digital pin 2)
  vw_rx_start();              // Iniciar el receptor VirtualWire

  pinMode(LED_PIN_GREEN, OUTPUT); // Configurar el pin 13 (LED incorporado) como salida
  digitalWrite(LED_PIN_GREEN, LOW); // Asegurarse de que el LED esté apagado al inicio

}

void loop() {
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen)) { // Si se recibe un mensaje
    digitalWrite(LED_PIN_GREEN, HIGH); // Encender el LED al recibir un mensaje

    // Verificar la longitud del paquete
    if (buflen != PACKET_SIZE) {
      Serial.print("ERROR: tamaño de paquete incorrecto.");
      Serial.print(PACKET_SIZE);
      Serial.print(", recibido ");
      Serial.print(buflen);
      Serial.println("");
      digitalWrite(LED_PIN_GREEN, LOW);
      return;
    }

    // Extraer campos del paquete
    uint8_t header = buf[0];
    uint8_t emitter_id = buf[1];
    uint8_t receiver_id = buf[2];
    uint8_t start_index_high = buf[3]; // Este byte indica el índice de inicio de los datos
    uint8_t start_index_low = buf[4];
    int start_index = (start_index_high << 8) | start_index_low; // Combinar los dos bytes para obtener el índice de inicio
    uint8_t *pixel_data = &buf[5]; // puntero a los 20 bytes de datos
    uint8_t received_checksum = buf[PACKET_SIZE - 1]; // Último byte es el checksum
    uint8_t calculated_checksum = calculateChecksum(buf, PACKET_SIZE - 1); // Calcular checksum de los primeros 24 bytes


    // Filtrar por ID Receptor
    if (receiver_id != RECEIVER_ID_2) {
      Serial.print("Paquete filtrado: ID Receptor (0x");
      Serial.print(receiver_id, HEX);
      Serial.println("");
      // Serial.println(") no coincide con el esperado (0x03).");
      digitalWrite(13, LOW);
      return;
    }

    // Validar Cabecera
    if (header != HEADER_BYTE) {
      Serial.println("ERROR: Cabecera invalida.");
      digitalWrite(13, LOW);
      return;
    }

    // Validar Checksum
    if (received_checksum != calculated_checksum) {
      Serial.println("ERROR: Checksum invalido. Paquete corrupto.");
      // Serial.print("  Recibido: 0x");
      // Serial.print(received_checksum, HEX);
      // Serial.print(", Calculado: 0x");
      // Serial.println(calculated_checksum, HEX);
      digitalWrite(13, LOW);
      return;
    }

    // Si todas las validaciones son correctas, el paquete es válido
    // Serial.println("Paquete valido recibido."); // Descomentar para depuración
    // Serial.print("  Header: 0x"); Serial.print(header, HEX); // Descomentar para depuración
    // Serial.print(", Emisor: 0x"); Serial.print(emitter_id, HEX); // Descomentar para depuración
    // Serial.print(", Receptor: 0x"); Serial.print(receiver_id, HEX); // Descomentar para depuración
    // Serial.print(", Pixeles: 0x"); Serial.print(pixel_data[0], HEX); // Descomentar para depuración
    // Serial.print(", Start Index: 0x"); Serial.print(start_index, HEX); // Descomentar para depuración
    // Serial.print(" 0x"); Serial.print(pixel_data[1], HEX); // Descomentar para depuración
    // Serial.print(" 0x"); Serial.print(pixel_data[2], HEX); // Descomentar para depuración
    // Serial.print(", Checksum: 0x"); Serial.println(received_checksum, HEX); // Descomentar para depuración

    if (!image_received_flag) {
      // Usar start_index como el índice de inicio real para copiar los datos recibidos
      int start = start_index;
      int remaining_pixels = TOTAL_PIXELS - start;
      int bytes_to_copy = (remaining_pixels >= DATA_BYTES_PER_PACKET) ? DATA_BYTES_PER_PACKET : remaining_pixels;

      for (int i = 0; i < bytes_to_copy; i++) {
        received_image[start + i] = pixel_data[i];
        pixels_received[start + i] = true; // Marcar el pixel como recibido
      }

      if (is_complete(pixels_received) ){
        digitalWrite(LED_PIN_BLUE, HIGH);
        Serial.println("\n--- IMAGEN COMPLETA RECIBIDA ---");
        Serial.print("DATA: ");
        Serial.print("'");
        for (int i = 0; i < TOTAL_PIXELS; i++) {
          Serial.print(received_image[i]);
          if (i < TOTAL_PIXELS - 1) {
            Serial.print(",");
          }
        }
        Serial.print("'");
        Serial.println("");
        Serial.println("--- FIN DE EXPORTACION ---");

        // Limpiar el buffer para la próxima imagen
        for (int i = 0; i < TOTAL_PIXELS; i++) {
          pixels_received[i] = false;
        }
        
        image_received_flag = true;
      }
      delay(50);
      digitalWrite(LED_PIN_BLUE,LOW);
    }

    delay(50);
    digitalWrite(LED_PIN_GREEN, LOW);
  } else {
    if (image_received_flag) image_received_flag = false;
  }
}
