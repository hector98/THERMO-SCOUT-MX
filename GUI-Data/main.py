import tkinter as tk
from tkinter import ttk, messagebox
import serial
import serial.tools.list_ports
from threading import Thread, Event
import time
import re
from math import sqrt, atan2, degrees
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import numpy as np

class CanSatGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Monitor Thermo-Scout MX")
        self.root.geometry("1200x800")
        
        # Variables para almacenar datos
        self.data = {
            'altitud_mar': 0,
            'altitud_actual': 0,
            'temperatura': 0,
            'presion': 0,
            'calidad_aire': 0,
            'gps_lat': 0,
            'gps_lon': 0,
            'acelerometro_x': 0,
            'acelerometro_y': 0,
            'acelerometro_z': 0,
            'magnetometro_x': 0,
            'magnetometro_y': 0,
            'magnetometro_z': 0,
            'liberado': False,
            'paracaidas': False
        }
        
        # Configuración serial
        self.serial_port = None
        self.ser = None
        self.serial_thread = None
        self.stop_event = Event()
        
        # Listas para almacenar histórico de datos
        self.historial_altitud = []
        self.historial_temperatura = []
        self.historial_presion = []
        self.historial_calidad_aire = []
        self.historial_tiempo = []
        self.tiempo_inicio = time.time()
        
        self.setup_ui()
        
    def setup_ui(self):
        # Frame principal
        main_frame = ttk.Frame(self.root, padding="10")
        main_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # Configurar grid
        self.root.columnconfigure(0, weight=1)
        self.root.rowconfigure(0, weight=1)
        main_frame.columnconfigure(1, weight=1)
        
        # Configuración serial
        serial_frame = ttk.LabelFrame(main_frame, text="Configuración Serial", padding="5")
        serial_frame.grid(row=0, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=5)
        
        ttk.Label(serial_frame, text="Puerto:").grid(row=0, column=0, sticky=tk.W)
        self.port_combo = ttk.Combobox(serial_frame, values=self.get_serial_ports(), width=15)
        self.port_combo.grid(row=0, column=1, padx=5)
        if self.port_combo['values']:
            self.port_combo.set(self.port_combo['values'][0])
        
        ttk.Label(serial_frame, text="Baudrate:").grid(row=0, column=2, sticky=tk.W, padx=(20, 0))
        self.baud_entry = ttk.Entry(serial_frame, width=10)
        self.baud_entry.insert(0, "9600")
        self.baud_entry.grid(row=0, column=3, padx=5)
        
        self.connect_btn = ttk.Button(serial_frame, text="Conectar", command=self.toggle_serial)
        self.connect_btn.grid(row=0, column=4, padx=5)
        
        # Frame para datos principales
        data_frame = ttk.LabelFrame(main_frame, text="Datos del CanSat", padding="5")
        data_frame.grid(row=1, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), pady=5)
        
        # Datos ambientales
        env_frame = ttk.Frame(data_frame)
        env_frame.grid(row=0, column=0, sticky=(tk.W, tk.N), padx=5)
        
        ttk.Label(env_frame, text="Altitud nivel del mar:").grid(row=0, column=0, sticky=tk.W)
        self.alt_mar_var = tk.StringVar(value="0 m")
        ttk.Label(env_frame, textvariable=self.alt_mar_var).grid(row=0, column=1, sticky=tk.W, padx=(5, 0))
        
        ttk.Label(env_frame, text="Altitud actual:").grid(row=1, column=0, sticky=tk.W)
        self.alt_act_var = tk.StringVar(value="0 m")
        ttk.Label(env_frame, textvariable=self.alt_act_var).grid(row=1, column=1, sticky=tk.W, padx=(5, 0))
        
        ttk.Label(env_frame, text="Temperatura:").grid(row=2, column=0, sticky=tk.W)
        self.temp_var = tk.StringVar(value="0 °C")
        ttk.Label(env_frame, textvariable=self.temp_var).grid(row=2, column=1, sticky=tk.W, padx=(5, 0))
        
        ttk.Label(env_frame, text="Presión:").grid(row=3, column=0, sticky=tk.W)
        self.pres_var = tk.StringVar(value="0 hPa")
        ttk.Label(env_frame, textvariable=self.pres_var).grid(row=3, column=1, sticky=tk.W, padx=(5, 0))
        
        ttk.Label(env_frame, text="Calidad aire:").grid(row=4, column=0, sticky=tk.W)
        self.aire_var = tk.StringVar(value="0")
        ttk.Label(env_frame, textvariable=self.aire_var).grid(row=4, column=1, sticky=tk.W, padx=(5, 0))
        
        # Datos GPS
        gps_frame = ttk.Frame(data_frame)
        gps_frame.grid(row=0, column=1, sticky=(tk.W, tk.N), padx=20)
        
        ttk.Label(gps_frame, text="GPS Latitud:").grid(row=0, column=0, sticky=tk.W)
        self.lat_var = tk.StringVar(value="0°")
        ttk.Label(gps_frame, textvariable=self.lat_var).grid(row=0, column=1, sticky=tk.W, padx=(5, 0))
        
        ttk.Label(gps_frame, text="GPS Longitud:").grid(row=1, column=0, sticky=tk.W)
        self.lon_var = tk.StringVar(value="0°")
        ttk.Label(gps_frame, textvariable=self.lon_var).grid(row=1, column=1, sticky=tk.W, padx=(5, 0))
        
        # Estado del CanSat
        status_frame = ttk.Frame(data_frame)
        status_frame.grid(row=0, column=2, sticky=(tk.W, tk.N), padx=20)
        
        ttk.Label(status_frame, text="Estado CanSat:").grid(row=0, column=0, sticky=tk.W)
        self.estado_var = tk.StringVar(value="En espera")
        self.estado_label = ttk.Label(status_frame, textvariable=self.estado_var, foreground="blue")
        self.estado_label.grid(row=0, column=1, sticky=tk.W, padx=(5, 0))
        
        ttk.Label(status_frame, text="Paracaídas:").grid(row=1, column=0, sticky=tk.W)
        self.paracaidas_var = tk.StringVar(value="No desplegado")
        self.paracaidas_label = ttk.Label(status_frame, textvariable=self.paracaidas_var, foreground="red")
        self.paracaidas_label.grid(row=1, column=1, sticky=tk.W, padx=(5, 0))
        
        # Datos del acelerómetro y magnetómetro
        sensor_frame = ttk.LabelFrame(data_frame, text="Sensores IMU", padding="5")
        sensor_frame.grid(row=1, column=0, columnspan=3, sticky=(tk.W, tk.E), pady=5)
        
        # Acelerómetro
        ttk.Label(sensor_frame, text="Acelerómetro X:").grid(row=0, column=0, sticky=tk.W)
        self.acc_x_var = tk.StringVar(value="0 g")
        ttk.Label(sensor_frame, textvariable=self.acc_x_var).grid(row=0, column=1, sticky=tk.W, padx=(5, 0))
        
        ttk.Label(sensor_frame, text="Acelerómetro Y:").grid(row=0, column=2, sticky=tk.W, padx=(20, 0))
        self.acc_y_var = tk.StringVar(value="0 g")
        ttk.Label(sensor_frame, textvariable=self.acc_y_var).grid(row=0, column=3, sticky=tk.W, padx=(5, 0))
        
        ttk.Label(sensor_frame, text="Acelerómetro Z:").grid(row=0, column=4, sticky=tk.W, padx=(20, 0))
        self.acc_z_var = tk.StringVar(value="0 g")
        ttk.Label(sensor_frame, textvariable=self.acc_z_var).grid(row=0, column=5, sticky=tk.W, padx=(5, 0))
        
        # Magnetómetro
        ttk.Label(sensor_frame, text="Magnetómetro X:").grid(row=1, column=0, sticky=tk.W)
        self.mag_x_var = tk.StringVar(value="0 μT")
        ttk.Label(sensor_frame, textvariable=self.mag_x_var).grid(row=1, column=1, sticky=tk.W, padx=(5, 0))
        
        ttk.Label(sensor_frame, text="Magnetómetro Y:").grid(row=1, column=2, sticky=tk.W, padx=(20, 0))
        self.mag_y_var = tk.StringVar(value="0 μT")
        ttk.Label(sensor_frame, textvariable=self.mag_y_var).grid(row=1, column=3, sticky=tk.W, padx=(5, 0))
        
        ttk.Label(sensor_frame, text="Magnetómetro Z:").grid(row=1, column=4, sticky=tk.W, padx=(20, 0))
        self.mag_z_var = tk.StringVar(value="0 μT")
        ttk.Label(sensor_frame, textvariable=self.mag_z_var).grid(row=1, column=5, sticky=tk.W, padx=(5, 0))
        
        # Orientación calculada
        ttk.Label(sensor_frame, text="Inclinación:").grid(row=2, column=0, sticky=tk.W)
        self.inclinacion_var = tk.StringVar(value="0°")
        ttk.Label(sensor_frame, textvariable=self.inclinacion_var).grid(row=2, column=1, sticky=tk.W, padx=(5, 0))
        
        ttk.Label(sensor_frame, text="Dirección:").grid(row=2, column=2, sticky=tk.W, padx=(20, 0))
        self.direccion_var = tk.StringVar(value="0°")
        ttk.Label(sensor_frame, textvariable=self.direccion_var).grid(row=2, column=3, sticky=tk.W, padx=(5, 0))
        
        # Gráficos
        graph_frame = ttk.LabelFrame(main_frame, text="Gráficos", padding="5")
        graph_frame.grid(row=2, column=0, sticky=(tk.W, tk.E, tk.N, tk.S), pady=5)
        main_frame.rowconfigure(2, weight=1)
        
        # Crear figura para matplotlib
        self.fig = Figure(figsize=(10, 6), dpi=100)
        self.ax1 = self.fig.add_subplot(221)
        self.ax2 = self.fig.add_subplot(222)
        self.ax3 = self.fig.add_subplot(223)
        self.ax4 = self.fig.add_subplot(224)
        
        self.canvas = FigureCanvasTkAgg(self.fig, master=graph_frame)
        self.canvas.draw()
        self.canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)
        
        # Botones de control
        button_frame = ttk.Frame(main_frame)
        button_frame.grid(row=3, column=0, sticky=(tk.E, tk.W), pady=5)
        
        ttk.Button(button_frame, text="Guardar Datos", command=self.guardar_datos).pack(side=tk.LEFT, padx=5)
        ttk.Button(button_frame, text="Limpiar Gráficos", command=self.limpiar_graficos).pack(side=tk.LEFT, padx=5)
        ttk.Button(button_frame, text="Salir", command=self.salir).pack(side=tk.RIGHT, padx=5)
        
    def get_serial_ports(self):
        ports = serial.tools.list_ports.comports()
        return [port.device for port in ports]
    
    def toggle_serial(self):
        if self.ser is None or not self.ser.is_open:
            self.conectar_serial()
        else:
            self.desconectar_serial()
    
    def conectar_serial(self):
        try:
            port = self.port_combo.get()
            baudrate = int(self.baud_entry.get())
            
            self.ser = serial.Serial(port, baudrate, timeout=1)
            self.stop_event.clear()
            self.serial_thread = Thread(target=self.leer_serial)
            self.serial_thread.daemon = True
            self.serial_thread.start()
            
            self.connect_btn.config(text="Desconectar")
            self.port_combo.config(state='disabled')
            self.baud_entry.config(state='disabled')
            
        except Exception as e:
            messagebox.showerror("Error", f"No se pudo conectar al puerto serial: {e}")
    
    def desconectar_serial(self):
        self.stop_event.set()
        if self.ser and self.ser.is_open:
            self.ser.close()
        
        self.connect_btn.config(text="Conectar")
        self.port_combo.config(state='normal')
        self.baud_entry.config(state='normal')
    
    def leer_serial(self):
        buffer = ""
        while not self.stop_event.is_set():
            try:
                if self.ser and self.ser.is_open:
                    data = self.ser.readline().decode('utf-8', errors='ignore')
                    if data:
                        buffer += data
                        if '\n' in buffer:
                            lines = buffer.split('\n')
                            for line in lines[:-1]:
                                self.procesar_datos(line.strip())
                            buffer = lines[-1]
                time.sleep(0.01)
            except Exception as e:
                print(f"Error leyendo serial: {e}")
                time.sleep(0.1)
    
    def procesar_datos(self, linea):
        # Expresión regular para buscar patrones en los datos
        patrones = {
            'altitud_mar': r"Altitud nivel del mar: ([-\d.]+)",
            'altitud_actual': r"Altitud actual: ([-\d.]+)",
            'temperatura': r"Temperatura: ([-\d.]+)",
            'presion': r"Presión: ([-\d.]+)",
            'calidad_aire': r"Calidad del aire: ([-\d.]+)",
            'gps_lat': r"GPS Lat: ([-\d.]+)",
            'gps_lon': r"GPS Lon: ([-\d.]+)",
            'acelerometro_x': r"Acelerómetro X: ([-\d.]+)",
            'acelerometro_y': r"Acelerómetro Y: ([-\d.]+)",
            'acelerometro_z': r"Acelerómetro Z: ([-\d.]+)",
            'magnetometro_x': r"Magnetómetro X: ([-\d.]+)",
            'magnetometro_y': r"Magnetómetro Y: ([-\d.]+)",
            'magnetometro_z': r"Magnetómetro Z: ([-\d.]+)",
            'liberado': r"Liberado: (\w+)",
            'paracaidas': r"Paracaídas: (\w+)"
        }
        
        # Buscar coincidencias en la línea
        for clave, patron in patrones.items():
            coincidencia = re.search(patron, linea)
            if coincidencia:
                valor = coincidencia.group(1)
                try:
                    if clave in ['liberado', 'paracaidas']:
                        self.data[clave] = valor.lower() in ['true', '1', 'si', 'yes', 'activado']
                    else:
                        self.data[clave] = float(valor)
                except ValueError:
                    pass
        
        # Actualizar la interfaz
        self.actualizar_ui()
        
        # Interpretar datos para detectar eventos
        self.interpretar_datos()
    
    def actualizar_ui(self):
        # Actualizar variables con los datos recibidos
        self.alt_mar_var.set(f"{self.data['altitud_mar']} m")
        self.alt_act_var.set(f"{self.data['altitud_actual']} m")
        self.temp_var.set(f"{self.data['temperatura']} °C")
        self.pres_var.set(f"{self.data['presion']} hPa")
        self.aire_var.set(f"{self.data['calidad_aire']}")
        
        self.lat_var.set(f"{self.data['gps_lat']}°")
        self.lon_var.set(f"{self.data['gps_lon']}°")
        
        self.acc_x_var.set(f"{self.data['acelerometro_x']} g")
        self.acc_y_var.set(f"{self.data['acelerometro_y']} g")
        self.acc_z_var.set(f"{self.data['acelerometro_z']} g")
        
        self.mag_x_var.set(f"{self.data['magnetometro_x']} μT")
        self.mag_y_var.set(f"{self.data['magnetometro_y']} μT")
        self.mag_z_var.set(f"{self.data['magnetometro_z']} μT")
        
        # Actualizar estado
        if self.data['liberado']:
            self.estado_var.set("Liberado")
            self.estado_label.config(foreground="green")
        else:
            self.estado_var.set("No liberado")
            self.estado_label.config(foreground="red")
            
        if self.data['paracaidas']:
            self.paracaidas_var.set("Desplegado")
            self.paracaidas_label.config(foreground="green")
        else:
            self.paracaidas_var.set("No desplegado")
            self.paracaidas_label.config(foreground="red")
        
        # Añadir datos al histórico
        tiempo_actual = time.time() - self.tiempo_inicio
        self.historial_tiempo.append(tiempo_actual)
        self.historial_altitud.append(self.data['altitud_actual'])
        self.historial_temperatura.append(self.data['temperatura'])
        self.historial_presion.append(self.data['presion'])
        self.historial_calidad_aire.append(self.data['calidad_aire'])
        
        # Actualizar gráficos
        self.actualizar_graficos()
    
    def interpretar_datos(self):
        # Calcular orientación a partir del acelerómetro y magnetómetro
        ax, ay, az = self.data['acelerometro_x'], self.data['acelerometro_y'], self.data['acelerometro_z']
        mx, my, mz = self.data['magnetometro_x'], self.data['magnetometro_y'], self.data['magnetometro_z']
        
        # Calcular inclinación (pitch y roll)
        pitch = degrees(atan2(ay, sqrt(ax**2 + az**2)))
        roll = degrees(atan2(-ax, az))
        
        # Calcular dirección (yaw) usando el magnetómetro
        # Primero compensar por inclinación
        try:
            cos_roll = np.cos(np.radians(roll))
            sin_roll = np.sin(np.radians(roll))
            cos_pitch = np.cos(np.radians(pitch))
            sin_pitch = np.sin(np.radians(pitch))
            
            # Compensar magnetómetro por inclinación
            mx_comp = mx * cos_pitch + mz * sin_pitch
            my_comp = mx * sin_roll * sin_pitch + my * cos_roll - mz * sin_roll * cos_pitch
            
            # Calcular yaw (dirección)
            yaw = degrees(atan2(-my_comp, mx_comp))
            if yaw < 0:
                yaw += 360
            
            self.inclinacion_var.set(f"{pitch:.1f}° / {roll:.1f}°")
            self.direccion_var.set(f"{yaw:.1f}°")
        except:
            # En caso de error en los cálculos
            self.inclinacion_var.set("Error")
            self.direccion_var.set("Error")
        
        # Detectar liberación basada en cambios de altitud y aceleración
        if len(self.historial_altitud) > 10:
            # Calcular tasa de cambio de altitud
            altitud_actual = self.data['altitud_actual']
            altitud_anterior = self.historial_altitud[-10]
            cambio_altitud = altitud_actual - altitud_anterior
            
            # Si hay una caída rápida, podría indicar liberación
            if cambio_altitud < -10 and not self.data['liberado']:
                self.data['liberado'] = True
                self.estado_var.set("Liberado (detectado)")
                self.estado_label.config(foreground="orange")
            
            # Detectar apertura de paracaídas basada en desaceleración
            if self.data['liberado'] and not self.data['paracaidas']:
                # Calcular la magnitud de la aceleración
                acc_magnitude = sqrt(ax**2 + ay**2 + az**2)
                
                # Si la aceleración disminuye bruscamente (desaceleración)
                if acc_magnitude < 0.8:  # Valor empírico, ajustar según necesidades
                    self.data['paracaidas'] = True
                    self.paracaidas_var.set("Desplegado (detectado)")
                    self.paracaidas_label.config(foreground="orange")
    
    def actualizar_graficos(self):
        # Limpiar gráficos
        self.ax1.clear()
        self.ax2.clear()
        self.ax3.clear()
        self.ax4.clear()
        
        # Configurar gráfico de altitud
        self.ax1.plot(self.historial_tiempo, self.historial_altitud, 'b-')
        self.ax1.set_title('Altitud vs Tiempo')
        self.ax1.set_ylabel('Altitud (m)')
        self.ax1.grid(True)
        
        # Configurar gráfico de temperatura
        self.ax2.plot(self.historial_tiempo, self.historial_temperatura, 'r-')
        self.ax2.set_title('Temperatura vs Tiempo')
        self.ax2.set_ylabel('Temperatura (°C)')
        self.ax2.grid(True)
        
        # Configurar gráfico de presión
        self.ax3.plot(self.historial_tiempo, self.historial_presion, 'g-')
        self.ax3.set_title('Presión vs Tiempo')
        self.ax3.set_ylabel('Presión (hPa)')
        self.ax3.set_xlabel('Tiempo (s)')
        self.ax3.grid(True)
        
        # Configurar gráfico de calidad del aire
        self.ax4.plot(self.historial_tiempo, self.historial_calidad_aire, 'm-')
        self.ax4.set_title('Calidad del Aire vs Tiempo')
        self.ax4.set_ylabel('Calidad del Aire')
        self.ax4.set_xlabel('Tiempo (s)')
        self.ax4.grid(True)
        
        # Ajustar diseño y redibujar
        self.fig.tight_layout()
        self.canvas.draw()
    
    def guardar_datos(self):
        # Aquí iría el código para guardar los datos en un archivo
        try:
            with open('cansat_data.csv', 'w') as f:
                f.write("Tiempo,Altitud,Temperatura,Presion,Calidad_Aire\n")
                for i in range(len(self.historial_tiempo)):
                    f.write(f"{self.historial_tiempo[i]},{self.historial_altitud[i]},{self.historial_temperatura[i]},{self.historial_presion[i]},{self.historial_calidad_aire[i]}\n")
            messagebox.showinfo("Guardar", "Datos guardados correctamente en cansat_data.csv")
        except Exception as e:
            messagebox.showerror("Error", f"No se pudieron guardar los datos: {e}")
    
    def limpiar_graficos(self):
        self.historial_tiempo.clear()
        self.historial_altitud.clear()
        self.historial_temperatura.clear()
        self.historial_presion.clear()
        self.historial_calidad_aire.clear()
        self.tiempo_inicio = time.time()
        self.actualizar_graficos()
    
    def salir(self):
        self.desconectar_serial()
        self.root.quit()
        self.root.destroy()

if __name__ == "__main__":
    root = tk.Tk()
    app = CanSatGUI(root)
    root.mainloop()
