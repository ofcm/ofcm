from sklearn.decomposition import PCA

def fashion_scatter(x, colors):
    # choose a color palette with seaborn.
    num_classes = len(np.unique(colors))
    palette = np.array(sns.color_palette("hls", num_classes))

    # create a scatter plot.
    f = plt.figure(figsize=(8, 8))
    ax = plt.subplot(aspect='equal')
    sc = ax.scatter(x[:,0], x[:,1], lw=0, s=40, c=palette[colors.astype(np.int)])
    plt.xlim(-25, 25)
    plt.ylim(-25, 25)
    ax.axis('off')
    ax.axis('tight')

    # add the labels for each digit corresponding to the label
    txts = []

    for i in range(num_classes):

        # Position of each label at median of data points.

        xtext, ytext = np.median(x[colors == i, :], axis=0)
        txt = ax.text(xtext, ytext, str(i), fontsize=24)
        txt.set_path_effects([
            PathEffects.Stroke(linewidth=5, foreground="w"),
            PathEffects.Normal()])
        txts.append(txt)

    plt.show()
    return f, ax, sc, txts


f = open("traindata.txt")
text = f.read()
l = text.split()

data_train = []
for i in range(len(l)):
    data_train.append(l[i].split(','))
f.close()
f = open("trainlabel.txt")
text = f.read()

label_train = text.split()

from decimal import *
for it in range(len(data_train)):
    for i in range(len(data_train[it])):
        data_train[it][i] = Decimal(data_train[it][i])
    label_train[it] = int(Decimal(label_train[it]))
    
#print(data_train[0][:10])
#print(label_train[0])
print(len(data_train), len(data_train[0]))
print(len(label_train))

import numpy as np
import time
import seaborn as sns
sns.set_style('darkgrid')
sns.set_palette('muted')
sns.set_context("notebook", font_scale=1.5,
                rc={"lines.linewidth": 2.5})
import matplotlib.patheffects as PathEffects
import matplotlib.pyplot as plt

data_train  = np.array([np.array(xi) for xi in data_train])
label_train = np.array(label_train, dtype=np.int32)

time_start = time.time()

from sklearn.manifold import TSNE
tsne = TSNE(n_components=2, random_state=0)

X_2d = tsne.fit_transform(data_train)

fashion_scatter(X_2d, label_train)
time_end = time.time()


print("time = ", time_end - time_start, "s" )