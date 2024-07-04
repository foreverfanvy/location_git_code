import math

pi = math.pi
g = float(9.8)  # m/s^2
L = float(0.72)  # m
D = float(3.9 * 0.001)  # m
H = float(0.6878)  # m
k1 = float(6.3 * 0.001)  # m/kg
k2 = float(6.6 * 0.001)  # m/kg
d = float(0.5 * 0.001)  # m
E1 = float((8 * g * L * H) / (pi * d * d * D * k1))
E2 = float((8 * g * L * H) / (pi * d * d * D * k2))
print(f"{E1}\n{E2}")
#  不准确度的计算
uH = float(0.5)
uL = float(0.5)
uD = float(0.02)
ux = float(0.5)
ud = float(0.02)
m = float(ux / 6.1 + ux / 12.4 + ux / 18.9 + ux / 26 + ux / 31.8 + ux / 38.2 + ux / 44.1)
yao = math.sqrt(pow(uH / H, 2) + pow(uL / L, 2) + pow(uD / D, 2) + pow(2 * ud / d, 2) + pow(ux / (m / 7), 2))
x = float(yao)
uE = math.sqrt(pow(uH / H, 2) + pow(uL / L, 2) + pow(uD / D, 2) + pow(2 * ud / d, 2) + pow(ux / x, 2))
print(uE*0.001*E1)
