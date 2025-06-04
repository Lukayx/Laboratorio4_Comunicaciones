import serial
import time
import numpy as np
from PIL import Image

# Cambia 'COM3' por el puerto donde esté tu Arduino (en Linux algo como '/dev/ttyACM0' o '/dev/ttyUSB0')
SERIAL_PORT = 'COM4'
BAUD_RATE = 9600

try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    print(f"Conectado a {SERIAL_PORT} a {BAUD_RATE} baudios.")
    time.sleep(2)  # Esperar que Arduino resetee al abrir puerto
except serial.SerialException as e:
    print(f"No se pudo conectar: {e}")
    exit()

try:
    lectura_img=False
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8', errors='replace').rstrip()
            if lectura_img:
                line = line.replace("DATA: ","")
                # linea de datos de image => '123,123,123,...'
                line = line.replace("'","")
                datos_img = line.split(',')
                # Convertir los datos a enteros
                datos_img = [int(valor) for valor in datos_img if valor.isdigit()]
                if len(datos_img) != 1024:
                    print(f"Advertencia: Se esperaban 1024 valores, pero se encontraron {len(datos_img)}.")
                else:
                    imagen_array = np.array(datos_img, dtype=np.uint8).reshape((32, 32))
                    img = Image.fromarray(imagen_array, mode='L')
                    img.save('imagen.png')
                    print("Imagen guardada como 'imagen.png'.")
                                    
                lectura_img = False
            if (line == "--- IMAGEN COMPLETA RECIBIDA ---"):
                lectura_img = True
            
except KeyboardInterrupt:
    print("Cerrando conexión...")
finally:
    ser.close()