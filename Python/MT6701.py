# %% Inicio
import time
import math
import keyboard
from Comandos import Motores, Sensores
PI = math.pi

mot = Motores()
mot.conectarArd() 
sen = Sensores()
sen.conectarArd()

# %% Calibrar Sensor

ang = sen.obtenerPosicion() 
desfase = ang[0]
invertido = False
sen.enviarCal(0, desfase, invertido)

# %% Mover Motor y leer Sensor

mot.mover([0,  -4*PI - (3/4)*PI])   

while True:
    if keyboard.is_pressed('space'): # Presiona espacio para terminar el programa
        break
    ang = sen.obtenerPosicion() 
    print(f"{ang[0]}\n")
    time.sleep(0.016666)


# %% Cerrar comunicación serial

sen.cerrarSerial()
mot.cerrarSerial()
