# IoT_Airtrackingproject

Proyecto IoT para monitorear condiciones ambientales (temperatura y humedad) en un jardín familiar usando un sensor DHT22, ESP8266, MQTT y TimescaleDB.

## Descripción
Este proyecto recolecta datos de un sensor DHT22 conectado a una ESP8266, los envía vía MQTT a un script Python que los almacena en TimescaleDB, y los visualiza con Grafana o un script personalizado.

## Archivos
- `graficadordb.py`: Script Python para generar gráficos de temperatura y humedad.
- `mqtt_to_influxdb.py`: Script para recibir datos MQTT y almacenarlos en TimescaleDB.
- `arduino_to_mqtt.cpp`: Código de Arduino para la ESP8266.
- `database_setup.sql`: Script básico para configurar TimescaleDB.
- `pic1.png` y `pic2.png`: Gráfico del Dashboard de Grafana como resultado.

## Instrucciones
1. Configurar TimescaleDB y un broker MQTT (Mosquitto) en la máquina.
2. Sube el código `dht22_mqtt.ino` a la ESP8266.
3. Ejecuta `mqtt_to_influxdb.py` y visualiza con `graficadordb.py` o Grafana.

## Tecnologías
- Hardware: ESP8266, DHT22.
- Software: Python, Arduino, TimescaleDB, MQTT, Git.

## Resultados
- 2890 mediciones recolectadas en 7 días.
- Gráfico funcional generado y almacenado como `temperatura_humedad.png`.

## Autor
Mateo Velazco
Contacto: mateovelazco12@gmail.com
