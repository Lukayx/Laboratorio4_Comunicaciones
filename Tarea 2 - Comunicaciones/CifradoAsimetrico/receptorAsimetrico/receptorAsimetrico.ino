#include <VirtualWire.h>
#include <stdint.h>

// Definiciones de protocolo
#define HEADER_BYTE 0xAA
#define EMITTER_ID 0x01
#define RECEIVER_ID_1 0x02
#define RECEIVER_ID_2 0x03

// Tamaño de la imagen
const int IMAGE_WIDTH = 32;
const int IMAGE_HEIGHT = 32;
const int TOTAL_PIXELS = IMAGE_WIDTH * IMAGE_HEIGHT;
const int PIXELS_PER_PACKET = 10;
const int TOTAL_PACKETS = (TOTAL_PIXELS + PIXELS_PER_PACKET - 1) / PIXELS_PER_PACKET;

// LEDs
const int LED_PIN_BLUE = 12;
const int LED_PIN_GREEN = 13;

// Claves RSA
const uint64_t P   = 263;
const uint64_t Q   = 269;
const uint64_t N   = P * Q;            // 70747
const uint64_t PHI = (P - 1) * (Q - 1);// 70216
const uint64_t E   = 17;               // exponente público
const uint64_t D   = 57825;            // inverso

// Buffer para la imagen recibida
uint8_t received_image[TOTAL_PIXELS];

// Estado
bool image_received_flag = false;
int total_packets_received = 0;
int expected_packet_number = 1;

int index_received[TOTAL_PACKETS];
const int DATA_BYTES_PER_PACKET = 20;
const int PACKET_SIZE = 1 + 1 + 1 + 2 + DATA_BYTES_PER_PACKET + 1; // 26 bytes

// Funciones

uint64_t mod_pow(uint64_t b, uint64_t e, uint64_t m) {
  uint64_t r = 1; b %= m;
  while (e) {
    if (e & 1) r = (r * b) % m;
    e >>= 1; b = (b * b) % m;
  }
  return r;
}

bool is_index_received(int index) {
  for (int i = 0; i < TOTAL_PACKETS; i++) {
    if (index_received[i] == index) return true;
  }
  return false;
}

void add_index_received(int index) {
  for (int i = 0; i < TOTAL_PACKETS; i++) {
    if (index_received[i] == -1) {
      index_received[i] = index;
      break;
    }
  }
}

bool imagen_received() {
  int c = 0;
  for (int i = 0; i < TOTAL_PACKETS; i++) {
    if (index_received[i] != -1) c++;
  }
  return (c == TOTAL_PACKETS);
}

uint8_t calculateChecksum(const uint8_t *data, size_t len) {
  uint8_t crc = 0x00;
  const uint8_t poly = 0x31;
  for (size_t i = 0; i < len; i++) {
    crc ^= data[i];
    for (int j = 0; j < 8; j++) {
      crc = (crc & 0x80) ? (crc << 1) ^ poly : crc << 1;
    }
  }
  return crc;
}

void setup() {
  Serial.begin(9600);
  Serial.println("Receptor corregido listo - Esperando imagen...");

  vw_set_ptt_inverted(true);
  vw_setup(500);
  vw_set_rx_pin(2);
  vw_rx_start();

  pinMode(LED_PIN_GREEN, OUTPUT);
  pinMode(LED_PIN_BLUE, OUTPUT);
  digitalWrite(LED_PIN_GREEN, LOW);
  digitalWrite(LED_PIN_BLUE, LOW);

  memset(index_received, -1, sizeof(index_received));
  memset(received_image, 0, sizeof(received_image));
}

void loop() {
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen)) {
    digitalWrite(LED_PIN_GREEN, HIGH);

    if (buflen != PACKET_SIZE || buf[0] != HEADER_BYTE || buf[2] != RECEIVER_ID_2) {
      Serial.println("Paquete rechazado.");
      digitalWrite(LED_PIN_GREEN, LOW);
      return;
    }

    uint8_t received_checksum = buf[PACKET_SIZE - 1];
    uint8_t calculated_checksum = calculateChecksum(buf, PACKET_SIZE - 1);

    if (calculated_checksum != received_checksum) {
      Serial.println("Checksum inválido.");
      digitalWrite(LED_PIN_GREEN, LOW);
      return;
    }

    total_packets_received++;
    uint8_t *pixel_data = &buf[5];
    int start_index = (buf[3] << 8) | buf[4];
    int bytes_to_copy = min(PIXELS_PER_PACKET, TOTAL_PIXELS - start_index);

    Serial.print("Paquete ");
    Serial.print(expected_packet_number);
    Serial.print(" | Índice: ");
    Serial.print(start_index);
    Serial.print(" | Bytes: ");
    Serial.println(bytes_to_copy);

    if (!is_index_received(start_index)) {
        // en lugar de memcpy, recorremos cada par de bytes,
        // reconstruimos c, desciframos y guardamos m
        for (int i = 0; i < bytes_to_copy; i++) {
          uint16_t c = ((uint16_t)pixel_data[2*i] << 8) 
                     | (uint16_t)pixel_data[2*i + 1];
          uint8_t  m = (uint8_t)mod_pow(c, D, N);
          received_image[start_index + i] = m;
        }
        add_index_received(start_index);
    } else {
      Serial.println("Paquete duplicado.");
    }

    expected_packet_number++;

    if (imagen_received()) {
      digitalWrite(LED_PIN_BLUE, HIGH);

      Serial.println("\n--- IMAGEN COMPLETA RECIBIDA ---");
      Serial.print("Total paquetes recibidos: ");
      Serial.println(total_packets_received);

      Serial.println("Datos de imagen:");
      for (int i = 0; i < TOTAL_PIXELS; i++) {
        Serial.print(received_image[i]);
        if (i < TOTAL_PIXELS - 1) Serial.print(",");
      }
      Serial.println("\n--- FIN DE TRANSMISIÓN ---");
      // Reiniciar para siguiente imagen
      expected_packet_number = 1;
      total_packets_received = 0;
      memset(received_image, 0, sizeof(received_image));
      memset(index_received, -1, sizeof(index_received));
      image_received_flag = true;

      delay(1000);
      digitalWrite(LED_PIN_BLUE, LOW);
    }

    delay(50);
    digitalWrite(LED_PIN_GREEN, LOW);
  } else {
    if (image_received_flag) image_received_flag = false;
  }
}