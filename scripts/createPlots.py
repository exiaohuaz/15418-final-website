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

    oneSourceSpeedupGHC = np.array([
        [1, 0.9787300316709009],
        [2, 1.875914018741562],
        [4, 3.5342725361614002],
        [8, 6.313843886409522],
        [16, 6.605571150939321],
        [32, 6.305894450052724]
    ])

    fiveSourcesSpeedupGHC = np.array([
        [1, 0.9754093835671611],
        [2, 1.8961888507749962],
        [4, 3.6132903150220135],
        [8, 6.677453994463555],
        [16, 6.560783896993823],
        [32, 6.600475617267986]
    ])

    tenSourcesSpeedupGHC = np.array([
        [1, 0.9747589050738296],
        [2, 1.9080869393556283],
        [4, 3.6362414659663287],
        [8, 6.743353436167501],
        [16, 6.556414068675876],
        [32, 6.435335035439954]
    ])

    oneSourceSpeedupPSC = np.array([
        [1, 1.005687476040083],
        [2, 1.876116377063807],
        [4, 3.485291524325477],
        [8, 5.935644047702071],
        [16, 9.026599698430566],
        [32, 10.13043114173605],
        [64, 7.10007370060803],
        [128, 5.303234762598565],
        [256, 7.82162585159271],
        [512, 6.2775265740589745]
    ])

    fiveSourcesSpeedupPSC = np.array([
        [1, 1.010847930028542],
        [2, 1.9183311280004054],
        [4, 3.700973688388983],
        [8, 6.657848826786456],
        [16, 10.74694787425308],
        [32, 14.109037551811872],
        [64, 13.81973910168208],
        [128, 10.31316292790972],
        [256, 14.12346289798655],
        [512, 13.325150598092494]
    ])

    tenSourcesSpeedupPSC = np.array([
        [1, 1.0038298467277327],
        [2, 1.9391980823229207],
        [4, 3.7300121366718324],
        [8, 6.905506652018133],
        [16, 10.782005044338774],
        [32, 13.075275970136728],
        [64, 12.811417830879282],
        [128, 9.94417525787498],
        [256, 12.886497468260306],
        [512, 12.047554305149845]
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

# lower light source counts chosen to be sparse
def plotOfTerraSeqVsCuda():
    sourcesPointsGHC = np.array([
        [1, 14.12701693942004],
        [2, 24.783475262742492],
        [4, 30.606299100535125],
        [8, 46.841582906978665],
        [16, 57.338629948720985],
        [32, 72.48956130098914],
        [64, 91.4222757655837],
        [128, 98.23185189016053],
        [256, 102.67158996038866],
        [512, 104.97753805944215],
        [1024, 107.3367227736005]
    ])

plotOfTerraSeqVsParOnMac()
# graphIt(slt, "yellow", graphTitle, xLabel, yLabel, "sparse-200000")


