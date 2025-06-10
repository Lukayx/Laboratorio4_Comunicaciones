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

const uint64_t E = 3;
const uint64_t D = 171;
const uint64_t N = 256; // n debe ser mayor que cualquier valor de MM



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
// ✅ CORREGIDO: Estructura correcta del paquete
// Cabecera(1) + Emisor(1) + Receptor(1) + Índice(2) + Datos(20) + Checksum(1) = 26 bytes
const int PACKET_SIZE = 1 + 1 + 1 + 2 + DATA_BYTES_PER_PACKET + 1; // 26 bytes

// Función para calcular (base^exp) % mod de forma eficiente
uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1)  // Si el exponente es impar
            result = (result * base) % mod;
        exp = exp >> 1;     // exp /= 2
        base = (base * base) % mod;
    }
    return result;
}

void cifrado_cesar(uint8_t *data, int k) {
    for (int i = 0; i < TOTAL_PIXELS; i++) {
        data[i] = (data[i] + k) % 256;
    }
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

void cifrado_asimetrico(uint8_t *data, uint64_t e, uint64_t n) {
  for (int i = 0; i < TOTAL_PIXELS; i++)
    data[i] = (uint8_t)(mod_pow(data[i], e, n));
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
  static bool cifrado_aplicado = false;
  if (!cifrado_aplicado) {
    // Aplica el cifrado SOLO UNA VEZ por encendido
    //cifrado_asimetrico((uint8_t*)data_to_send, E, N);
    cifrado_cesar((uint8_t*)data_to_send, 3);

    cifrado_aplicado = true;
  }
  delay(2000);
  
  uint8_t packet[PACKET_SIZE];
  int total_pixels = TOTAL_PIXELS;
  int packets_to_send = (total_pixels + DATA_BYTES_PER_PACKET - 1) / DATA_BYTES_PER_PACKET;
  
  for (int pkt = 0; pkt < packets_to_send; pkt++) {
    // ✅ PRIMERO: Calcular índices
    int start_index = pkt * DATA_BYTES_PER_PACKET;
    int end_index = min(start_index + DATA_BYTES_PER_PACKET - 1, total_pixels - 1);
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
    for (int i = 0; i < DATA_BYTES_PER_PACKET; i++) {
      int data_index = start_index + i;
      if (data_index < total_pixels) {
        packet[5 + i] = data_to_send[data_index];
      } else {
        packet[5 + i] = 0; // relleno con 0
      }
    }

    // Calcular checksum
    packet[PACKET_SIZE - 1] = calculateChecksum(packet, PACKET_SIZE - 1);

     // --- INICIO DE LA MODIFICACIÓN PARA SIMULAR CHECKSUM INVÁLIDO ---
    // if (pkt + 1 == 10) { // Si es el paquete número 10 (pkt es base 0, así que pkt+1 es el número de paquete)
    //   packet[PACKET_SIZE - 1] = ~packet[PACKET_SIZE - 1]; // Invertir todos los bits del checksum
    //   Serial.println("¡¡¡ATENCIÓN: CHECKSUM ALTERADO PARA EL PAQUETE 10!!!");
    // }
    // --- FIN DE LA MODIFICACIÓN ---
    

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

    delay(500);
  }

  Serial.println("Envio de imagen completado. Reenviando en 2 segundos...");
  delay(2000);
}