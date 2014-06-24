#! /usr/bin/python
from flask import Flask
from flask import request
app = Flask(__name__)
from ctypes import *
import json
lib = CDLL("map.so")
lib.init.res_type = c_int
res = lib.init()
x = 103.811516
y = 1.2744
s = "drive Singapore B"
n = 10
lib.j_query.restype = c_char_p
lib.j_query.argtypes = [c_int,c_double,c_double,c_char_p]
data = "asia"
for i in range(10):
	for j in range(1,5):
		res = lib.j_query(20,x,y,data)
		print res
		print i,j

