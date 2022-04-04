from matplotlib.patches import Polygon
from collision import *
from math import *
import matplotlib.pyplot as plt
import time

want_to_collide = False
v = Vars()
delta = 60*v.iarc
x = 12*v.iarc
round = 72*v.iarc
initialvector = [cos(x), sin(x)]
edge = initialvector
start = [cos(delta) + 0.0 if want_to_collide else 1, sin(delta)]
vertices = []
for i in range(5): #画一个5边形
    vertices.append(tuple(start))
    start[0] += edge[0]
    start[1] += edge[1]
    tmp1, tmp2 = edge[0] * cos(round) - edge[1] * sin(round), edge[0] * sin(round) + edge[1] * cos(round)
    edge = [tmp1, tmp2]

starttime = time.time()
for i in range(1000):
    p5 = Polygon(vertices) 
    vertices.append(vertices[0])
    p6 = Polygon([(1.0, 0.0), (cos(delta), sin(delta)), (-cos(delta), sin(delta)),\
            (-1.0, 0.0), (-cos(delta), -sin(delta)), (cos(delta), -sin(delta))])
print(time.time() - starttime)

coords = p6.get()
coords.append(coords[0])
xs, ys = zip(*vertices)
xs2, ys2 = zip(*coords)
print(p5.collide(p6))
plt.figure()
plt.plot(xs, ys)
plt.plot(xs2, ys2)
plt.show() 