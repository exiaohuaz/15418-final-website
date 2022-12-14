# Note: This file contains actual measured speedup data. DO NOT MODIFY speedup numbers

from matplotlib import pyplot as plt
import numpy as np

def plotIt(dt, color, title, xLabel, yLabel, sceneLabel):
    x = dt[:, 0].reshape(dt.shape[0], 1)
    y = dt[:, 1].reshape(dt.shape[0], 1)
    plt.plot(x, y, color=color, label=sceneLabel)
    plt.scatter(x, y, color=color)
    plt.title(title)
    plt.xlabel(xLabel)
    plt.ylabel(yLabel)

def plotOfTerraSeqVsParOnMac():
    oneSourceSpeedup = np.array([
        [1, 1.000552881042825],
        [2, 1.9822300976087563],
        [4, 3.748199261545911],
        [8, 4.878900899090617],
        [16, 4.800114745250956]
    ])

    fiveSourcesSpeedup = np.array([
        [1, 1.041150889145979],
          [2, 2.0541120507830826],
          [4, 3.8764166036903855],
          [8, 5.1293736805785635],
          [16, 5.0108752085048],
    ])

    tenSourcesSpeedup = np.array([
        [1, 1.0328410443249183],
        [2, 2.053155692971682],
        [4, 3.76025084567074],
        [8, 5.0717494575146596],
        [16, 5.067638226650999]
    ])

    plt.rcParams["figure.figsize"] = [9.50, 5.50]
    plt.rcParams["figure.autolayout"] = True

    plt.rcParams.update({'text.color': "red",
                        'axes.labelcolor': "green"})
                        
    xLabel = "Number of Threads"
    yLabel = "Speedup"
    title = "Speedup vs. Number of Threads with Varying Light Source Counts on Large Image"

    plotIt(oneSourceSpeedup, "orange", title, xLabel, yLabel, "1 Source")
    plotIt(fiveSourcesSpeedup, "purple", title, xLabel, yLabel, "5 Sources")
    plotIt(tenSourcesSpeedup, "limegreen", title, xLabel, yLabel, "10 Sources")

    plt.rcParams.update({'text.color': "black",
                     'axes.labelcolor': "black"})
    plt.legend(title="Scene")
    plt.show()


plotOfTerraSeqVsParOnMac()
# graphIt(slt, "yellow", graphTitle, xLabel, yLabel, "sparse-200000")


