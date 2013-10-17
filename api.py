import subprocess, os, shlex
import bottle
from bottle import route, run, post, request

@post('/sensor')
def sensor():
	sensor_data = request.body.getvalue()

	for token in sensor_data.split():
		try:
			loadValue(float(token))
		except ValueError:
			pass

def loadValue(value):
	print "Loading value... %f" % value
	devnull = open(os.devnull, 'w')
	subprocess.call(shlex.split(
			'/usr/bin/zabbix_sender -z 127.0.0.1 -s TempSensor -k sensors.temp -o %s' % str(value-1.3)),
			stdout=devnull, stderr=devnull) 

if __name__ == "__main__":
	run(host='0.0.0.0',port=8080, debug=True)

app = bottle.default_app()
