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


const uint8_t HEADER_EXPECTED = 0x05; // igual que emisor
const uint8_t MY_ID = 0x02;
const uint8_t EXPECTED_EMITTER_ID = 0x01;

const int LED_PIN_BLUE = 12;
const int LED_PIN_GREEN = 13;

const int TOTAL_PACKETS = 8; // cantidad total de paquetes esperados (debes ajustar según datos enviados)

void setup() {
  Serial.begin(9600);
  vw_set_ptt_inverted(true);
  vw_setup(2000);
  vw_set_rx_pin(2);
  vw_rx_start();

  int* arr;

  pinMode(LED_PIN_BLUE, OUTPUT);
  pinMode(LED_PIN_GREEN, OUTPUT);
  digitalWrite(LED_PIN_BLUE, LOW);
  digitalWrite(LED_PIN_GREEN, LOW);

  Serial.println("Receptor listo");
}

uint8_t calculate_checksum(const uint8_t *data, int len) {
  uint8_t s = 0;
  for(int i=0; i<len; i++) s += data[i];
  return s;
}

int packets_received = 0;

void loop() {
  uint8_t buf[10];
  uint8_t buflen = sizeof(buf);
  if (vw_get_message(buf, &buflen)) {
    if (buflen == 4) {
      uint8_t checksum = buf[3];
      if (calculate_checksum(buf, 3) == checksum) {
        uint8_t header = (buf[0] >> 5) & 0x07;
        uint8_t emitter_id = (buf[0] >> 2) & 0x07;
        uint8_t seq_high = buf[0] & 0x03;
        uint8_t seq_low = (buf[1] >> 2) & 0x3F;
        uint8_t sequence_num = (seq_high << 6) | seq_low;
        uint8_t payload = buf[2];

        if (header == HEADER_EXPECTED && emitter_id == EXPECTED_EMITTER_ID) {
           Serial.println(payload);
          // Enciende LED azul para paquete válido
          digitalWrite(LED_PIN_BLUE, HIGH);
          delay(1);
          digitalWrite(LED_PIN_BLUE, LOW);
          

          // Cuenta paquetes recibidos
          packets_received++;

          // Si recibimos el último paquete, enciende LED verde y mantiene
          if (packets_received >= TOTAL_PACKETS) {
            digitalWrite(LED_PIN_GREEN, HIGH);
            delay(100);
            digitalWrite(LED_PIN_GREEN, LOW);
            delay(100);
          }

        }
    }
  }}
 }
