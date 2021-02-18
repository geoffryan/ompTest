import sys
import numpy as np
import matplotlib.pyplot as plt

filenames = sys.argv[1:]

fig, ax = plt.subplots(1, 1)

for i, filename in enumerate(filenames):
    ncpu, t = np.loadtxt(filename, unpack=True)

    if i == 0:
        ax.plot(ncpu, t[0] * np.power(ncpu/ncpu[0], -1), lw=2, ls='--',
                color='grey')

    ax.plot(ncpu, t, label=filename)

ax.legend()

ax.set(xscale='log', yscale='log')

fig.savefig("time_scaling.png")
