import matplotlib
import matplotlib.pyplot as plt
from matplotlib.font_manager import FontProperties
import os
from PIL import Image
import numpy as np

# plt.rcParams['figure.figsize'] = [3, 3]
# matplotlib.rcParams['figure.dpi'] = 300

#PROBLEM = True
PROBLEM = "ftv47.atsp"
#PROBLEM = "ftv170.atsp"
#PROBLEM = "rbg403.atsp"
HIGHER_MUT = False

graph_id = 1

def generate_results(lines):
    results = {"0.01": {"1000": {"inverse": [[], []], "scramble": [[], []]},
                    "10000": {"inverse": [[], []], "scramble": [[], []]},
                    "100000": {"inverse": [[], []], "scramble": [[], []]}},
           "other": {"0.02": {"inverse": [[], []], "scramble": [[], []]},
                     "0.05": {"inverse": [[], []], "scramble": [[], []]},
                     "0.1": {"inverse": [[], []], "scramble": [[], []]}}}
    #print(results)
    line_id = 0
    while (line_id != len(lines)):
        if lines[line_id].startswith("file"):
            # if files == 10:
            #    break
            problem = lines[line_id].split(";")[1]
            optimal = int(lines[line_id].split(";")[2])
            line_id += 1
        elif lines[line_id][0].isalpha():
            label = lines[line_id].split(";")
            params = [str(item) for item in label]
            for i in range(len(params)):
                params[i] = params[i].rstrip()
            line_id += 1
        else:
            while not lines[line_id][0].isalpha():
                if problem==PROBLEM:
                    line = lines[line_id].split(";")
                    if str(params[5]) == '0.01':
                        results["0.01"][str(params[3])][str(params[1])][0].append(int(float(line[0])/1000))
                        results["0.01"][str(params[3])][str(params[1])][1].append(int(100*(((int)(float(str(line[1]).rstrip()))-optimal)/optimal)))
                    else:
                        results["other"][str(params[5])][str(params[1])][0].append(int(float(line[0])/1000))
                        results["other"][str(params[5])][str(params[1])][1].append(int(100*(((int)(float(str(line[1]).rstrip()))-optimal)/optimal)))
                line_id += 1
                if line_id == len(lines):
                    break
    return results
                    

def problemType(fname):
    if fname.startswith("48"):
        return "ftv47.atsp;1776"
    if fname.startswith("171"):
        return "ftv170.atsp;2755"
    else:
        return "rbg403.atsp;2465"


if __name__ == '__main__':
    lines = []
    files = os.listdir()

    for file in files:
        if file.endswith(".csv"):
            with open(file, 'r') as f:
                lines += [f"file;{problemType(file)}"]
                lines += f.readlines()
                
    results = generate_results(lines)

    colors = ['y', 'r', 'g', 'c', 'm', 'b']
    i = 0

    for problem in ("ftv47.atsp", "ftv170.atsp", "rbg403.atsp"):
        for higher in (False, True):
            fig, ax = plt.subplots(figsize=(10,10), dpi=300)
            plt.ylim(0, 55)
            title_text = f"Wykres {graph_id}"
            plt.text(0.5, 0.9, title_text, fontsize=10, horizontalalignment='center', verticalalignment='top',
            weight='bold', transform=plt.gcf().transFigure)
            text = f"Problem {problem}"
            
            if higher is False:
                text += " Wsp. mutacji = 0.01"
                for mutation in ("inverse", "scramble"):
                    for factor in ("1000", "10000", "100000"):
                        xpoints = np.array(results["0.01"][factor][mutation][0])
                        ypoints = np.array(results["0.01"][factor][mutation][1])
                        plt.plot(xpoints, ypoints, colors[i]+'o', label=mutation+" p="+factor)
                        i=(i+1)%len(colors)
            else:
                colors = ['y', 'r', 'g', 'c', 'm', 'b']
                i = 0
                for mutation in ("inverse", "scramble"):
                    for factor in ("0.02", "0.05", "0.1"):
                        xpoints = np.array(results["other"][factor][mutation][0])
                        ypoints = np.array(results["other"][factor][mutation][1])
                        plt.plot(xpoints, ypoints, colors[i]+'o', label=mutation+" p="+factor)
                        i=(i+1)%len(colors)
            plt.text(0.3, 0.8, text, fontsize=8, transform=plt.gcf().transFigure)
            #plt.text(0.5, 0.65, text, fontsize=5, horizontalalignment='center', verticalalignment='top')
            source_text = "Źródło: opracowanie własne"
            plt.text(0.5, 0.05, source_text, fontsize=9, horizontalalignment='center', verticalalignment='top', 
                     transform=plt.gcf().transFigure)
            
            plt.xlabel("Czas [s]")
            plt.ylabel("Błąd [%]")
            plt.grid()
            plt.legend()
            #plt.show()
            path = f'graphs/g{graph_id}.jpg'
            plt.savefig(path, bbox_inches='tight', dpi=300)
            plt.close()
            graph_id+=1
