import matplotlib.pyplot as plot

fig, ax = plot.subplots()

ax.set_xlim(0, 100)
ax.set_ylim(0, 4)
ax.set_xlabel('Time')
ax.set_yticks([0.5, 2.5])
ax.set_yticklabels(['LP', 'HP'])
ax.grid(True)

energy = 0.0

def plot_bars(y, li, ids):
	ax.broken_barh(li, (y, 1), facecolors=('tab:blue', 'tab:green', 'tab:cyan'), edgecolor='black')

	for i in range(0, len(li)):
		ax.text(li[i][0] + li[i][1]/2, 1.1 + y, str(ids[i]))

def extract(line):
	line_data = line.split(' ')
	ids = []
	li = []
	for i in range(0, len(line_data), 3):
		ids.append(line_data[i])
		bar_data = (float(line_data[i + 1]), float(line_data[i + 2]))
		li.append(bar_data)
	return li, ids

line = input('')
li, ids = extract(line)
plot_bars(2.5, li, ids)
max1 = li[-1][0] + li[-1][1]

line = input('')
li, ids = extract(line)
plot_bars(0.5, li, ids)
max2 = li[-1][0] + li[-1][1]

ax.text(50, 4, f'Total Time: {max(max1, max2)}ms')

plot.show()
