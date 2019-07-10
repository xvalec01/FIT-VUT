import socket
import json
import sys

# kontrola správného počtu argumentů
if len(sys.argv) != 3:
    sys.exit("Wrong number of arguments. It should be <API_KEY> <CITY>.")
# uložení načteného API key ze vstupu
API = sys.argv[1]
# uložení načteného města ze vstupu
city = sys.argv[2]
# řetězec příkazu, který získá data ze serveru
get_data = 'GET /data/2.5/weather?q={0}&APPID={1}&units=metric HTTP/1.1\r\nHost: api.openweathermap.org\r\n\r\n'.format(
    city, API)
# adresa serveru ke kterému se připojuje skript
HOST = 'api.openweathermap.org'
# port na kterém bude vedena komunikace
PORT = 80

# navázání komunikace se servrem a záskání dat
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as stream:
    stream.connect((HOST, PORT))
    stream.sendall(get_data.encode('utf-8'))
    data_bytes = stream.recv(1024)

# přijatá data konvertovaná na string
data = data_bytes.decode("utf-8")
# data předělána na json
data = json.loads(data[data.find("{"):])

# ošetření při zadání špatného města nebo API keye
if data['cod'] == '404':
    sys.exit("City not found!")
elif data['cod'] == 401:
    sys.exit("Invalid API KEY!")

# uložení jednotlivých žádoucích dat
try:
    city_name = str(data['name'])
except:
    print('Name of city is not available.')
try:
    description = str(data['weather'][0]['description'])
except:
    print('Description is not available.')
try:
    temperature = str(data['main']['temp'])
except:
    print('Temperature is not available.')
try:
    humidity = str(data['main']['humidity'])
except:
    print('Humidity is not available.')
try:
    pressure = str(data['main']['pressure'])
except:
    print('Pressure is not available.')
try:
    wind_speed = str(data['wind']['speed'] * 3.6)
except:
    print('Speed of wind is not available.')
try:
    wind_deg = str(data['wind']['deg'])
except:
    print('Degree of wind is not available.')

# vypsání dat na výstup
print(city_name)
print(description)
print('temp: ' + temperature + '°C')
print('humidity: ' + humidity + '%')
print('pressure: ' + pressure + ' hPa')
print('wind-speed: ' + wind_speed + ' km/h')
print('wind-deg: ' + wind_deg)
