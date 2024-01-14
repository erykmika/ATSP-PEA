import matplotlib
import matplotlib.pyplot as plt
from matplotlib.font_manager import FontProperties
import os
from PIL import Image

plt.rcParams['figure.figsize'] = [3, 3]
matplotlib.rcParams['figure.dpi'] = 300

def create_tables(lines):
    line_id = 0
    tbl_id = 1
    files = 1
    while(line_id!=len(lines)):
        if lines[line_id].startswith("file"):
            #if files == 10:
            #    break
            problem = lines[line_id].split(";")[1]
            optimal = int(lines[line_id].split(";")[2])
            line_id+=1
            files+=1
        #print(line_id, len(lines))
        elif lines[line_id][0].isalpha():
            label = lines[line_id].split(";")
            params = [str(item) for item in label]
            for i in range(len(params)):
                params[i] = params[i].rstrip()
            line_id+=1
        else:
            data = [[0,0,0] for i in range(10)]
            i = 0
            while not lines[line_id][0].isalpha():
                line = lines[line_id].split(";")
                data[i][0] = "{:.2e}".format(int(float(line[0])))
                data[i][1] = str(line[1]).rstrip()
                data[i][2] = str(int(100*(((int)(str(line[1]).rstrip())-optimal)/optimal))) + "%"
                i+=1
                line_id+=1
                if line_id == len(lines):
                    break

            #plt.figure(figsize=(6,6), dpi=600)
            fig, ax = plt.subplots(figsize=(4, len(data)+2), dpi=300)

            ax.axis('off')
            
            title_text = f"Tabela {tbl_id}"
            plt.text(0.5, 0.70, title_text, fontsize=8, horizontalalignment='center', verticalalignment='top',
                     weight='bold')
            text = f"Problem {problem}\n"
            text += " ".join([f'Mutacja: {params[1]}',
                            f'Populacja: {params[3]}',
                            f'Wsp. mutacji: {params[5]}'])
                            
            text+="\n"+" ".join([f'Wsp. krzyżowania: {params[7]}',
                            f'Limit czasu: {params[9]} s.'])
            
            plt.text(0.5, 0.65, text, fontsize=5, horizontalalignment='center', verticalalignment='top')

            cell_text = []
            for j in range(i):
                cell_text.append(data[j])

            col_labels = ['$\\bf{Czas\\ [ms]}$', '$\\bf{Koszt\\ ścieżki}$', '$\\bf{ Błąd }$']

            ax.table(cellText=[col_labels] + cell_text, loc='center', rowLoc='center', cellLoc='center',
                    colLabels=None, cellColours=[['#f0f0f0', '#f0f0f0', '#f0f0f0']]*(i+1), colWidths=[0.2, 0.3, 0.1])

            source_text = "Źródło: opracowanie własne"
            plt.text(0.5, 0.385, source_text, fontsize=6, horizontalalignment='center', verticalalignment='top')

            path = f'tables/t{tbl_id}.jpg'
            plt.savefig(path, bbox_inches='tight', dpi=300)
            plt.close()
            im = Image.open(path)
            im = im.crop((150, 848, 845, 1779))
            width, height = im.size
            im = im.resize((300,402)) 
            im.save(path)

            tbl_id+=1


def problemType(fname):
    if fname.startswith("48"):
        return "ftv47.atsp;1776"
    if fname.startswith("171"):
        return "ftv170.atsp;2755"
    else:
        return "rbg403.atsp;2465"


if __name__ == '__main__':
    lines = []
    files = ['48inverse0.010000.csv', '48scramble0.010000.csv',
             '171inverse0.010000.csv', '171scramble0.010000.csv',
             '403inverse0.010000.csv', '403scramble0.010000.csv']
    
    for file in os.listdir():
        if file.endswith('.csv') and file.startswith("48") and file not in files:
            files.append(file)
    
    for file in os.listdir():
        if file.endswith('.csv') and file.startswith("171") and file not in files:
            files.append(file)

    for file in os.listdir():
        if file.endswith('.csv') and file.startswith("403") and file not in files:
            files.append(file)

    for file in files:
            with open(file, 'r') as f:
                lines += [f"file;{problemType(file)}"]
                lines += f.readlines()
    #print(lines)
    create_tables(lines)
