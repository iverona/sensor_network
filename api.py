import subprocess
from bottle import route, run, post, request

@post('/sensor')
def sensor():
	sensor_data = request.body.getvalue()

	for token in sensor_data.split():
		try:
			print float(token)
			loadValue(float(token))
		except ValueError:
			pass

def loadValue(value):
	print "Loading value..."
	subprocess.call(['/usr/bin/zabbix_sender', '-z', '127.0.0.1', '-s', 'TempSensor', '-k', 'sensors.temp', '-o', str(value)] )

run(host='0.0.0.0',port=8080, debug=True)
