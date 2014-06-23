#! /usr/bin/python
from ctypes import *
import json
lib = CDLL("map.so")
lib.init.res_type = c_int
res = lib.init()
if res!=17:
	print "err"
	1/0
x = 103.811516
y = 1.2744
s = "drive Singapore Blan"
n = 10
lib.j_query.restype = c_char_p
lib.j_query.argtypes = [c_int,c_double,c_double,c_char_p]
res = lib.j_query(n,x,y,s)
print res
print json.loads(res)
