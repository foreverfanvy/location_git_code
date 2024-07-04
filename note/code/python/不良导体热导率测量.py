import math

Ds = float(13.001) * float(0.01)
Db = float(13.058) * float(0.01)
Hs = float(0.714) * float(0.01)
Hb = float(1.002) * float(0.01)

T2 = float(64)
T1 = float(93.5)

pi = math.pi
C = float(385)
k=float(-0.05016)

m = float(1.1272)
f1 = (Db / 2 + 2 * Hb) / (Db + 2 * Hb)
f2 = 4 * Hs / (pi * pow(Ds, 2) * (T2 - T1))
fc = m * C * f1 * f2 * int(-1)*k
print(fc)

fk = float(0.14) * pow((T2 + float(273.15)) / float(273.15), 3 / 2)
print(fk)
