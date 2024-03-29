#!/bin/python

import sys
import math
import matplotlib.pyplot as plot

fig, ax = plot.subplots()

ax.set_xlim(-1, 101)
Y_max=101
ax.set_ylim(-1, Y_max)
ax.set_xlabel('Utilization %')
ax.set_ylabel('Normalised Energy Consumption %')
ax.grid(True)

if len(sys.argv) != 2:
	print("usage: plot_energy.py <plot-data>")
	sys.exit(1)

plot_dat = []
with open(sys.argv[1]) as plot_data:
	for line in plot_data:
		vals = tuple(map(float, line.split(' ')))
		plot_dat.append(vals)
plot_dat = sorted(plot_dat)

print(plot_dat)

utl = []
ltf = []
ltf_us = []
for pts in plot_dat:
	if math.inf not in pts:
		utl.append(pts[0])
		ltf.append(pts[1])
		ltf_us.append(pts[2])
	# ax.plot([pts[0], pts[0]], [pts[1], pts[2]], '-', color="blue", linewidth=0.01)
e_ltf_max = max(ltf)
e_ltf_us_max = max(ltf_us)
for i in range(len(utl)):
	ltf[i] = ltf[i]/e_ltf_max * 100.0
	ltf_us[i] = ltf_us[i]/e_ltf_us_max * 100.0

ltf_line = ax.plot(utl, ltf, '.-', linewidth=0.2, markersize=3.0, color="red", label="LTF")
ltf_us_line = ax.plot(utl, ltf_us, '.-', markersize=3.0, linewidth=0.2, color="green", label="LTF US")

# fig.suptitle(f'Energy: {round(energy, 4)} mJ     Finish Time: {round(max(max1, max2), 4)} ms', fontsize=16)
fig.legend(markerscale=4, fontsize=10)
plot.show()
