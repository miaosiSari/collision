#测试基本形状
from collision import Rect, Vars
import matplotlib.pyplot as plt

v = Vars()
r = Rect(10, 2, 5, 2, 30*v.iarc)
print(r)
coords = r.get()
coords.append(coords[0]) #Get a closed loop!
xs, ys = zip(*coords)
plt.figure()
plt.plot(xs, ys)
plt.show()
