from cmu_112_graphics import *
import math
from PIL import Image
import subprocess


def appStarted(app):
    app.imx = 2560/2
    app.imy = 1440/2
    app.imgDir = subprocess.check_output("pwd", shell=True).decode().strip() + "/../data"
    app.fImg = Image.open(app.imgDir + "/frame0.png")
    app.imgCount = 0
    app.lastFrameNum = 2760
    app.paused = False

def mousePressed(app, event):
    return

def keyPressed(app, event):
    if event.key == 'r':
        appStarted(app)
    elif event.key == 'p':
        app.paused = True

def timerFired(app):
    if not app.paused:
        app.imgCount += 20
        if (app.imgCount > app.lastFrameNum):
            app.imgCount = 0
        app.fImg = Image.open(app.imgDir + f"/frame{app.imgCount}.png")
    print("framestep")

def redrawAll(app, canvas):
    canvas.create_image(app.imx, app.imy, image=ImageTk.PhotoImage(app.fImg))
    return

runApp(width=2560, height=1440)