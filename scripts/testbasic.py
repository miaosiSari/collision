#测试基本形状
from collision import Rect, Vars
import matplotlib.pyplot as plt

v = Vars()
r = Rect(10, 2, 10, 4, 30*v.iarc)
r2 = Rect(12, 6, 6, 4, 0)
print(r, r.collide(r2))
coords = r.get()
coords.append(coords[0]) #Get a closed loop!
xs, ys = zip(*coords)
coords2 = r2.get()
coords2.append(coords2[0])
xs2, ys2 = zip(*coords2)
plt.figure()
plt.plot(xs, ys)
plt.plot(xs2, ys2)
plt.show()
