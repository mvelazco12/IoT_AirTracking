# IoT_Airtrackingproject

An IoT project designed to monitor environmental conditions (temperature and humidity) in a home garden using a DHT22 sensor, ESP8266 microcontroller, MQTT protocol, and TimescaleDB.

## Description
This project collects data from a DHT22 sensor connected to an ESP8266, sends it via MQTT to a Python script that stores the data in TimescaleDB, and visualizes it using Grafana or a custom Python script.

## Files
- `graficadordb.py`: Python script to generate temperature and humidity graphs.
- `mqtt_to_influxdb.py`: Python script to receive MQTT data and store it in TimescaleDB.
- `arduino_to_mqtt.cpp`: C++ code adapted from Arduino IDE for the ESP8266 with DHT22.
- `database_setup.sql`: Initial setup script for TimescaleDB configuration.
- `temperatura_humedad.png`: Generated graph of temperature and humidity data.
- `pic1.png`: Screenshot of the Grafana dashboard.

## Instructions
1. Set up TimescaleDB and an MQTT broker (e.g., Mosquitto) on your machine.
2. Upload `arduino_to_mqtt.cpp` to the ESP8266.
3. Run the Python scripts (`mqtt_to_influxdb.py` and `graficadordb.py`) to collect and visualize data.

## Technologies
- **Hardware**: ESP8266, DHT22 sensor.
- **Software**: Python, C++ (Arduino), TimescaleDB, MQTT, Git.

## Results
- Generated functional graphs and a Grafana dashboard for real-time monitoring.

## Author
Mateo Velazco  
Contact: mateovelazco12@gmail.com

## License
This project is licensed under the [MIT License](LICENSE).
