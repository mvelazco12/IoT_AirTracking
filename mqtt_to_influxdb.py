import paho.mqtt.client as mqtt
import json
import psycopg2
from datetime import datetime, timezone

# Configuración de MQTT
MQTT_BROKER = "localhost"
MQTT_PORT = 1883
MQTT_TOPIC = "sensor/dht"

# Configuración de TimescaleDB
DB_HOST = "localhost"
DB_PORT = 5432  # Asegúrate de que este puerto coincida con el que usaste (5432 o 5433)
DB_USER = "iot_user"
DB_PASSWORD = "iot_password"
DB_NAME = "iot_project"

# Conexión a TimescaleDB
def connect_db():
    try:
        conn = psycopg2.connect(
            host=DB_HOST,
            port=DB_PORT,
            user=DB_USER,
            password=DB_PASSWORD,
            dbname=DB_NAME
        )
        print(f"Conexión exitosa a TimescaleDB en {DB_HOST}:{DB_PORT}")
        return conn
    except Exception as e:
        print(f"Error al conectar a TimescaleDB en {DB_HOST}:{DB_PORT}: {e}")
        return None

# Callback para la conexión MQTT
def on_connect(client, userdata, flags, rc, properties=None):
    print(f"Conectado a MQTT con código: {rc}")
    client.subscribe(MQTT_TOPIC)

# Callback para cuando se recibe un mensaje MQTT
def on_message(client, userdata, msg):
    try:
        # Decodificar y parsear el mensaje JSON
        data = json.loads(msg.payload.decode())
        temp = float(data["temperature"])  # Ajustado a "temperature" para coincidir con tu formato
        hum = float(data["humidity"])     # Ajustado a "humidity" para coincidir con tu formato
        print(f"Datos recibidos del DHT22: temp={temp}°C, hum={hum}%")

        # Conectar a TimescaleDB
        conn = connect_db()
        if conn is None:
            return

        cur = conn.cursor()
        # Insertar los datos
        timestamp = datetime.now(timezone.utc)
        cur.execute(
            "INSERT INTO dht_sensor (time, temperature, humidity) VALUES (%s, %s, %s)",
            (timestamp, temp, hum)
        )
        conn.commit()
        print(f"Datos insertados en TimescaleDB: {timestamp}, Temp: {temp}, Hum: {hum}")

        # Cerrar la conexión
        cur.close()
        conn.close()

    except json.JSONDecodeError as e:
        print(f"Error al parsear JSON: {e}")
    except KeyError as e:
        print(f"Error en los datos recibidos, falta campo: {e}")
    except Exception as e:
        print(f"Error al escribir en TimescaleDB: {e}")

# Configuración del cliente MQTT
mqtt_client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
mqtt_client.on_connect = on_connect
mqtt_client.on_message = on_message

# Conectar al broker MQTT
mqtt_client.connect(MQTT_BROKER, MQTT_PORT, 60)

# Iniciar bucle para escuchar mensajes
print("Iniciando cliente MQTT...")
mqtt_client.loop_forever()

