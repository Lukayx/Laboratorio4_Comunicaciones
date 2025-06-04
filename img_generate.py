from PIL import Image, ImageTk
import numpy as np
import matplotlib.pyplot as plt
import tkinter as tk
from tkinter import filedialog, messagebox
import os # Importar el módulo os para manejar rutas de archivos

def procesar_imagen_a_array_8bit(ruta_imagen, mostrar_plots=True):
    """
    Recibe una imagen, la escala a 32x32, la convierte a escala de grises
    y la representa como un array de 8 bits (0-255). Guarda las imágenes intermedias
    y el array de píxeles en un archivo de texto.

    Args:
        ruta_imagen (str): La ruta al archivo de imagen.
        mostrar_plots (bool): Si es True, muestra las figuras con matplotlib.

    Returns:
        numpy.ndarray: Un array 32x32 de enteros de 8 bits (0-255)
                       representando la imagen en escala de grises.
        None: Si hay un error al abrir la imagen.
    """
    try:
        # 1. Cargar la imagen
        img_original = Image.open(ruta_imagen)
        print(f"Imagen original cargada. Resolución: {img_original.size[0]}x{img_original.size[1]}")

        # 2. Escalar la imagen a 32x32 píxeles
        img_escalada = img_original.resize((32, 32), Image.LANCZOS)
        print(f"Imagen escalada a: {img_escalada.size[0]}x{img_escalada.size[1]} píxeles.")

        # Obtener el nombre base del archivo para los nombres de las imágenes y el TXT guardados
        nombre_base, _ = os.path.splitext(os.path.basename(ruta_imagen))
        
        # Guardar la imagen escalada
        ruta_guardado_escalada = f"{nombre_base}_escalada_32x32.png"
        img_escalada.save(ruta_guardado_escalada)
        print(f"Imagen escalada guardada en: {ruta_guardado_escalada}")

        if mostrar_plots:
            # 3. Mostrar la imagen original y la escalada
            plt.figure(figsize=(10, 5))

            plt.subplot(1, 2, 1)
            plt.imshow(img_original)
            plt.title('Imagen Original')
            plt.axis('off')

            plt.subplot(1, 2, 2)
            plt.imshow(img_escalada)
            plt.title('Imagen Escalada a 32x32')
            plt.axis('off')
            plt.show()

        # 4. Convertir la imagen escalada a escala de grises (modo 'L' para 8-bit píxeles)
        img_gris = img_escalada.convert('L')
        print("Imagen convertida a escala de grises (8 bits por píxel).")

        # Guardar la imagen en escala de grises
        ruta_guardado_gris = f"{nombre_base}_gris_32x32.png"
        img_gris.save(ruta_guardado_gris)
        print(f"Imagen en escala de grises guardada en: {ruta_guardado_gris}")

        # 5. Convertir la imagen en escala de grises a un array de NumPy
        array_gris_8bit = np.array(img_gris, dtype=np.uint8)
        print(f"Array de NumPy creado. Dimensiones: {array_gris_8bit.shape}, Tipo de datos: {array_gris_8bit.dtype}")

        # 7. Guardar el array en un archivo de texto (.txt)
        ruta_guardado_txt = f"{nombre_base}_pixeles_32x32.txt"
        with open(ruta_guardado_txt, 'w') as f:
            for row in array_gris_8bit:
                # Convertir cada fila del array a una lista de enteros y luego a una cadena separada por comas
                f.write(','.join(map(str, row)) + '\n')
        print(f"Array de píxeles guardado en: {ruta_guardado_txt}")


        if mostrar_plots:
            # 6. Mostrar la imagen en escala de grises (desde el array)
            plt.figure(figsize=(5, 5))
            plt.imshow(array_gris_8bit, cmap='gray', vmin=0, vmax=255)
            plt.title('Imagen 32x32 en Escala de Grises (desde Array)')
            plt.axis('off')
            plt.colorbar(label='Intensidad de Píxel (0-255)')
            plt.show()

        print("\nContenido del array (primeras 5 filas y columnas):")
        print(array_gris_8bit[:5, :5])

        return array_gris_8bit

    except FileNotFoundError:
        messagebox.showerror("Error", f"La imagen '{ruta_imagen}' no fue encontrada.")
        return None
    except Exception as e:
        messagebox.showerror("Error", f"Ocurrió un error: {e}")
        return None

class ImageProcessorApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Procesador de Imagen para Arduino")

        self.image_path = tk.StringVar()
        self.processed_array = None

        # Frame principal
        main_frame = tk.Frame(root, padx=10, pady=10)
        main_frame.pack(expand=True, fill="both")

        # Etiqueta e campo para la ruta de la imagen
        tk.Label(main_frame, text="Ruta de la Imagen:").grid(row=0, column=0, sticky="w", pady=5)
        self.path_entry = tk.Entry(main_frame, textvariable=self.image_path, width=50)
        self.path_entry.grid(row=0, column=1, padx=5, pady=5)

        # Botón para buscar archivo
        browse_button = tk.Button(main_frame, text="Buscar Archivo", command=self.browse_image)
        browse_button.grid(row=0, column=2, padx=5, pady=5)

        # Botón para procesar la imagen
        process_button = tk.Button(main_frame, text="Procesar Imagen", command=self.process_image)
        process_button.grid(row=1, column=0, columnspan=3, pady=10)

        # Sección para mostrar la imagen procesada en la GUI (opcional, una miniatura)
        tk.Label(main_frame, text="Vista Previa 32x32 (Gris):").grid(row=2, column=0, columnspan=3, pady=5)
        self.preview_label = tk.Label(main_frame, borderwidth=2, relief="groove")
        self.preview_label.grid(row=3, column=0, columnspan=3, pady=5)

        # Área de texto para mostrar el array (opcional, para depuración)
        tk.Label(main_frame, text="Array de 8 bits (primeras filas):").grid(row=4, column=0, columnspan=3, pady=5)
        self.array_text = tk.Text(main_frame, height=10, width=60, wrap="word")
        self.array_text.grid(row=5, column=0, columnspan=3, pady=5)
        self.array_text_scroll = tk.Scrollbar(main_frame, command=self.array_text.yview)
        self.array_text_scroll.grid(row=5, column=3, sticky="ns")
        self.array_text.config(yscrollcommand=self.array_text_scroll.set)

    def browse_image(self):
        file_path = filedialog.askopenfilename(
            title="Seleccionar Imagen",
            filetypes=[("Archivos de Imagen", "*.png *.jpg *.jpeg *.bmp *.gif"), ("Todos los Archivos", "*.*")]
        )
        if file_path:
            self.image_path.set(file_path)
            # Limpiar previsualización y texto del array anterior
            self.preview_label.config(image='')
            self.preview_label.image = None
            self.array_text.delete(1.0, tk.END)


    def process_image(self):
        ruta = self.image_path.get()
        if not ruta:
            messagebox.showwarning("Advertencia", "Por favor, selecciona una imagen primero.")
            return

        # Aquí llamamos a la función de procesamiento que ahora guarda las imágenes y el TXT
        self.processed_array = procesar_imagen_a_array_8bit(ruta, mostrar_plots=True)

        if self.processed_array is not None:
            messagebox.showinfo("Éxito", "Imagen procesada, guardada como PNG y TXT.")

            # Mostrar vista previa en la GUI
            img_pil_preview = Image.fromarray(self.processed_array)
            img_pil_preview = img_pil_preview.resize((100, 100), Image.NEAREST) # Miniatura para la GUI
            img_tk = ImageTk.PhotoImage(img_pil_preview)
            self.preview_label.config(image=img_tk)
            self.preview_label.image = img_tk # Mantener una referencia para evitar que se borre

            # Mostrar el array en el área de texto
            self.array_text.delete(1.0, tk.END) # Limpiar contenido previo
            self.array_text.insert(tk.END, "Formato: Fila [Valores de píxeles]\n")
            for i, row in enumerate(self.processed_array):
                self.array_text.insert(tk.END, f"Fila {i}: {list(row)}\n")
                if i >= 9: # Mostrar solo las primeras 10 filas para no saturar
                    self.array_text.insert(tk.END, "...\n")
                    break
        else:
            messagebox.showerror("Error", "No se pudo procesar la imagen.")

# --- Ejecutar la aplicación ---
if __name__ == "__main__":
    root = tk.Tk()
    app = ImageProcessorApp(root)
    root.mainloop()
