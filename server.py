#! /usr/bin/python
from flask import Flask
from flask import request
app = Flask(__name__)
from ctypes import *
import json
lib = CDLL("map.so")
lib.init.res_type = c_int
res = lib.init()
'''
x = 103.811516
y = 1.2744
s = "drive Singapore B"
n = 10
'''
lib.j_query.restype = c_char_p
lib.j_query.argtypes = [c_int,c_double,c_double,c_char_p]
@app.route('/q',methods=['GET'])
def qf():
	x = float(request.args['x'])
	y = float(request.args['y'])
	s = request.args['s']
	#print x,y,s
	res = lib.j_query(20,x,y,s)
	return json.dumps(json.loads(res))
app.run(debug=True)
