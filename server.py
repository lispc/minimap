#! /usr/bin/python2
from flask import Flask
from flask import request
from ctypes import *
import json
lib = CDLL("./map.so")
lib.init.res_type = c_int
lib.init()
lib.json_query.restype = c_char_p
lib.json_query.argtypes = [c_int,c_double,c_double,c_char_p]
app = Flask(__name__)
@app.route('/q',methods=['GET'])
def qf():
	x = float(request.args['x'])
	y = float(request.args['y'])
	s = request.args['s']
	res = lib.json_query(20,x,y,s)
	return json.dumps(json.loads(res))
app.run(debug=True)
