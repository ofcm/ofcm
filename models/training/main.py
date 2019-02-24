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

print(len(data_train), len(data_train[0]))
print(len(label_train))

from decimal import *
for it in range(len(data_train)):
    for i in range(len(data_train[it])):
        data_train[it][i] = Decimal(data_train[it][i])
    label_train[it] = int(Decimal(label_train[it]))


import numpy as np

data_train  = np.array([np.array(xi) for xi in data_train])
label_train = np.array(label_train)

print(label_train)
from sklearn.model_selection import train_test_split

X_train, X_test, y_train, y_test = train_test_split(data_train, label_train, test_size=0.1, random_state=0)

print(X_train.shape, X_test.shape, y_train.shape, y_test.shape)

from sklearn import svm

C_ = 3.0
gamma_ = 0.01
print("C = ", C_, ", gamma = ", gamma_)

import time

s = time.time()
clf = svm.SVC(kernel='rbf', C=C_, gamma=gamma_,max_iter=10000).fit(X_train, y_train)

# gama: alto: elementos cercanos tendran influencia en la recta de deciciona(justa mas la curva)
# gama: bajo: elementos lejanos tendran influencia en la recta de decicion

# c   : alto: correcta distincion entre clases
# c   ; medio: permite algunas equivocaciones

print("time 1 = ", time.time()-s," s")

s = time.time()
print(clf.score(X_train, y_train))
print(clf.score(X_test, y_test))
print("time 2 = ", time.time()-s," s")
# C = 0.1, gamma = 0.01 , acc = 0.54, 0.47 iter = 2000
# C = 1.0, gamma = 0.01 , acc = 0.58, 0.48 iter = 2000
# C = 1.0, gamma = 0.01 , acc = 0.82, 0.57 iter = 10000