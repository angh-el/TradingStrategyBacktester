import subprocess

def makeCore():
    subprocess.run(["../core/make"])

def callCore(strategy):
    subprocess.run(["../core/main", strategy])