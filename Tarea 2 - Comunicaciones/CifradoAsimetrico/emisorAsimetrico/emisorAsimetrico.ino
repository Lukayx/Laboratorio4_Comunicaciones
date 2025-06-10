#include <stdio.h>
#include <stdint.h>
#include <VirtualWire.h>

#define HEADER_BYTE 0xAA
#define EMITTER_ID 0x01
#define RECEIVER_ID_1 0x02
#define RECEIVER_ID_2 0x03

const int IMAGE_WIDTH = 32;
const int IMAGE_HEIGHT = 32;
const int TOTAL_PIXELS = IMAGE_WIDTH * IMAGE_HEIGHT;

// Parámetros RSA
const uint64_t P   = 263;
const uint64_t Q   = 269;
const uint64_t N   = P * Q;            // 70747
const uint64_t PHI = (P - 1) * (Q - 1);// 70216
const uint64_t E   = 17;               // exponente público
const uint64_t D   = 57825;            // inverso de E mod PHI

const uint8_t data_to_send[TOTAL_PIXELS] = {
  255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,10,10,10,10,10,10,10,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,10,10,10,80,80,80,80,80,80,10,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,10,10,80,80,80,80,80,80,80,80,10,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,10,10,80,80,80,80,80,255,255,255,255,255,10,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,10,10,80,80,80,80,80,80,255,255,255,255,255,255,10,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,10,80,80,80,80,80,80,255,255,255,255,255,255,255,10,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,10,80,80,80,80,80,80,255,255,255,10,10,255,255,176,176,10,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,10,80,80,80,80,80,80,255,255,255,10,10,255,176,176,176,176,10,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,10,80,80,80,80,80,255,255,255,255,10,10,255,176,176,176,176,176,10,255,255,255,255,255,
255,255,255,255,255,255,255,10,80,80,10,10,10,80,255,255,255,255,10,10,255,176,176,176,176,10,255,255,255,255,255,255,
255,255,255,255,255,255,255,10,80,10,80,80,80,10,255,255,255,255,255,255,255,255,176,176,10,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,10,10,80,80,80,80,10,255,255,255,255,255,255,255,255,255,10,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,10,80,80,80,80,80,80,10,255,255,255,255,255,255,255,255,10,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,10,80,80,80,80,80,80,10,255,255,255,255,255,255,255,255,10,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,10,10,80,80,80,10,10,10,255,255,255,255,255,255,255,255,10,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,10,80,10,10,10,10,80,80,255,255,255,255,255,255,255,255,10,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,10,80,80,80,80,80,80,80,255,255,255,255,255,255,255,255,10,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,10,80,80,80,10,10,10,80,255,255,255,255,255,255,255,255,10,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,10,80,10,10,176,176,176,10,10,255,255,255,255,255,255,10,176,10,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,10,10,176,176,176,176,176,176,176,10,255,255,255,255,10,176,176,176,10,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,10,176,176,176,176,176,176,176,10,255,255,10,10,176,176,176,176,176,10,255,255,255,255,255,
255,255,255,255,255,255,255,255,10,176,176,176,176,176,176,176,176,10,10,10,176,176,176,176,176,176,10,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,10,10,10,10,10,10,10,10,255,255,255,10,10,10,10,10,10,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
};

const int DATA_BYTES_PER_PACKET = 20;
const int PIXELS_PER_PACKET = 10;
// ✅ CORREGIDO: Estructura correcta del paquete
// Cabecera(1) + Emisor(1) + Receptor(1) + Índice(2) + Datos(20) + Checksum(1) = 26 bytes
const int PACKET_SIZE = 1 + 1 + 1 + 2 + DATA_BYTES_PER_PACKET + 1; // 26 bytes

uint64_t mod_pow(uint64_t b, uint64_t e, uint64_t m) {
  uint64_t r = 1; b %= m;
  while (e) {
    if (e & 1) r = (r * b) % m;
    e >>= 1; b = (b * b) % m;
  }
  return r;
}

uint8_t calculateChecksum(const uint8_t *data, size_t len) {
    uint8_t crc = 0x00;         // Valor inicial
    const uint8_t poly = 0x31;  // Polinomio: x^8 + x^5 + x^4 + 1

    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];  // XOR del byte actual
        for (int j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ poly;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

void setup() {
  vw_set_ptt_inverted(true);
  vw_setup(500);
  vw_set_tx_pin(2);
  Serial.begin(9600);
  Serial.println("Configurando envio de imagen 20 bytes/paquete...");
  delay(2000);
}

void loop() {
  delay(2000);
  
  uint8_t packet[PACKET_SIZE];
  int packets_to_send = (TOTAL_PIXELS + PIXELS_PER_PACKET - 1) / PIXELS_PER_PACKET ;
  
  for (int pkt = 0; pkt < packets_to_send; pkt++) {
    // ✅ PRIMERO: Calcular índices
    int start_index = pkt * PIXELS_PER_PACKET;
    int end_index = min(start_index + PIXELS_PER_PACKET - 1, TOTAL_PIXELS - 1);
    int bytes_in_packet = end_index - start_index + 1;
    
    // ✅ ESTRUCTURA CORRECTA DEL PAQUETE (26 bytes):
    packet[0] = HEADER_BYTE;                    // Byte 0: Cabecera
    packet[1] = EMITTER_ID;                     // Byte 1: Emisor
    packet[2] = RECEIVER_ID_2;                  // Byte 2: Receptor
    packet[3] = (start_index >> 8) & 0xFF;      // Byte 3: Índice alto
    packet[4] = start_index & 0xFF;             // Byte 4: Índice bajo
    // Bytes 5-24: Datos (20 bytes)
    // Byte 25: Checksum
    
    // Llenar el paquete con datos
    // datos cifrados inline con RSA en 2 bytes c/u
      for (int i = 0; i < PIXELS_PER_PACKET; i++) {
        uint16_t c = 0;
        if (start_index + i < TOTAL_PIXELS)
          c = mod_pow(data_to_send[start_index + i], E, N);
        packet[5 + 2*i]     = (c >> 8) & 0xFF;
        packet[5 + 2*i + 1] =  c       & 0xFF;
      }
    // Calcular checksum
    packet[PACKET_SIZE - 1] = calculateChecksum(packet, PACKET_SIZE - 1);
    // Enviar el paquete
    vw_send(packet, PACKET_SIZE);
    vw_wait_tx();

    // Mostrar información detallada del paquete
    Serial.print("Paquete ");
    Serial.print(pkt + 1);
    Serial.print("/");
    Serial.print(packets_to_send);
    Serial.print(" | Pixeles ");
    Serial.print(start_index);
    Serial.print("-");
    Serial.print(end_index);
    Serial.print(" | Tamaño: ");
    Serial.print(PACKET_SIZE);
    Serial.print(" bytes | Checksum: 0x");
    Serial.print(packet[PACKET_SIZE - 1], HEX);
    Serial.print(" | Datos: ");
    
    // Mostrar los primeros 5 valores y los últimos 5 valores del paquete
    for (int i = 0; i < min(5, bytes_in_packet); i++) {
      Serial.print(packet[5 + i]);
      Serial.print(" ");
    }
    if (bytes_in_packet > 5) {
      Serial.print("... ");
      int start_last = max(5 + bytes_in_packet - 5, 5);
      for (int i = start_last; i < 5 + bytes_in_packet; i++) {
        Serial.print(packet[i]);
        Serial.print(" ");
      }
    }
    Serial.println();

    delay(200);
  }

  Serial.println("Envio de imagen completado. Reenviando en 2 segundos...");
  delay(2000);
}