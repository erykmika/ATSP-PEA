import matplotlib
import matplotlib.pyplot as plt
from matplotlib.font_manager import FontProperties
import os
from PIL import Image
import numpy as np


sizes = ["48","171", "403"]
optimals = [1776, 2755, 2465]
mutations = ["inverse", "scramble"]
populations = ["1000", "10000", "100000"]
factors = ["0.02", "0.05", "0.1"]

colors = ['y', 'r', 'g', 'c', 'm', 'b']
i = 0

graph_id = 7
file_idx=0

def problem(size):
    if size=="48":
        return "ftv47.atsp"
    elif size=="171":
        return "ftv171.atsp"
    else:
        return "rbg403.atsp"

def setXLimit(size):
    if size=="48":
        plt.xlim(0,30)
    elif size=="171":
        plt.xlim(0,10)
    else:
        pass

# wspolczynnik mutacji 0.01
for size in sizes:
    fig, ax = plt.subplots(figsize=(10,10), dpi=300)
    #setXLimit(size)
    if graph_id==7:
        plt.xlim(0,15)

    for mutation in mutations:
        for population in populations:
            x, y = [], []
            with open(size+mutation+population+".csv", "r") as file:    
                header = file.readline()
                for line in file.readlines():
                    x.append(int(float(line.split(";")[0])/1000))
                    y.append(100*int(float(line.split(";")[1])-optimals[file_idx])/optimals[file_idx])
            plt.plot(np.array(x), np.array(y), colors[i]+'o', label=mutation+" p="+population)
            i=(i+1)%len(colors)
    plt.xlabel("Czas [s]")
    plt.ylabel("Błąd [%]")
    source_text = "Źródło: opracowanie własne"
    plt.text(0.5, 0.05, source_text, fontsize=9, horizontalalignment='center', verticalalignment='top', 
             transform=plt.gcf().transFigure)
    title_text = f"Wykres {graph_id}" + "   Problem: " + problem(size)
    plt.text(0.5, 0.9, title_text, fontsize=10, horizontalalignment='center', verticalalignment='top',
    weight='bold', transform=plt.gcf().transFigure)
    text = f"Problem {problem}"
    plt.grid()
    plt.legend()
    #plt.show()
    path = f'graphs/gn{graph_id}.jpg'
    plt.savefig(path, bbox_inches='tight', dpi=300)
    plt.close()
    graph_id+=1
    file_idx+=1

file_idx=0

# rozne wsp mutacji
for size in sizes:
    fig, ax = plt.subplots(figsize=(10,10), dpi=300)
    #setXLimit(size)
    if graph_id==10:
        plt.xlim(0,50)
    for mutation in mutations:
        for factor in factors:
            x, y = [], []
            while(len(factor)<8):
                factor+="0"
            with open(size+mutation+factor+"mut.csv", "r") as file:    
                header = file.readline()
                for line in file.readlines():
                    x.append(int(float(line.split(";")[0])/1000))
                    y.append(100*int(float(line.split(";")[1])-optimals[file_idx])/optimals[file_idx])
            plt.plot(np.array(x), np.array(y), colors[i]+'o', label=mutation+" mf="+factor[:4])
            i=(i+1)%len(colors)
    plt.xlabel("Czas [s]")
    plt.ylabel("Błąd [%]")
    source_text = "Źródło: opracowanie własne"
    plt.text(0.5, 0.05, source_text, fontsize=9, horizontalalignment='center', verticalalignment='top', 
             transform=plt.gcf().transFigure)
    title_text = f"Wykres {graph_id}" + "   Problem: " + problem(size)
    plt.text(0.5, 0.9, title_text, fontsize=10, horizontalalignment='center', verticalalignment='top',
    weight='bold', transform=plt.gcf().transFigure)
    text = f"Problem {problem}"
    plt.grid()
    plt.legend()
    #plt.show()
    path = f'graphs/gn{graph_id}.jpg'
    plt.savefig(path, bbox_inches='tight', dpi=300)
    plt.close()
    graph_id+=1
    file_idx+=1