f = open("training/traindata.txt")
text = f.read()
l = text.split()

data_train = []
for i in range(len(l)):
    data_train.append(l[i].split(','))
f.close()
f = open("training/trainlabel.txt")
text = f.read()

label_train = text.split()
f.close()

print(len(data_train), len(data_train[0]))
print(len(label_train))

from decimal import *
for it in range(len(data_train)):
    for i in range(len(data_train[it])):
        data_train[it][i] = Decimal(data_train[it][i])
    label_train[it] = int(Decimal(label_train[it]))


"""
f = open("test/testdata.txt")
text = f.read()
l = text.split()

data_test = []
for i in range(len(l)):
    data_test.append(l[i].split(','))
f.close()
f = open("test/testlabel.txt")
text = f.read()

label_test = text.split()
f.close()

print(len(data_test), len(data_test[0]))
print(len(label_test))

for it in range(len(data_test)):
    for i in range(len(data_test[it])):
        data_test[it][i] = Decimal(data_test[it][i])
    label_test[it] = int(Decimal(label_test[it]))
"""

import numpy as np

from sklearn.model_selection import train_test_split
data_train  = np.array([np.array(xi) for xi in data_train])
label_train = np.array(label_train)

X_train, X_test, y_train, y_test = train_test_split(data_train, label_train, test_size=0.1, random_state=42)
#data_test  = np.array([np.array(xi) for xi in data_test])
#label_test = np.array(label_test)


print(X_train.shape, X_test.shape, y_train.shape, y_test.shape)
#print(data_train.shape, label_train.shape, data_test.shape, label_test.shape)

from sklearn import svm

C_ = 3.0
gamma_ = 0.01
print("C = ", C_, ", gamma = ", gamma_)

import time

s = time.time()
clf = svm.SVC(kernel='rbf', C=C_, gamma=gamma_,max_iter=50000).fit(X_train, y_train)

#clf = svm.SVC(kernel='rbf', C=C_, gamma=gamma_,max_iter=50000).fit(data_train, label_train)

# gama: alto: elementos cercanos tendran influencia en la recta de deciciona(justa mas la curva)
# gama: bajo: elementos lejanos tendran influencia en la recta de decicion

# c   : alto: correcta distincion entre clases
# c   ; medio: permite algunas equivocaciones

print("time 1 = ", time.time()-s," s")

s = time.time()
print(clf.score(X_train, y_train))
print(clf.score(X_test, y_test))
#print(clf.score(data_train, label_train))
#print(clf.score(data_test, label_test))
print("time 2 = ", time.time()-s," s")
# C = 0.1, gamma = 0.01 , acc = 0.54, 0.47 iter = 2000
# C = 1.0, gamma = 0.01 , acc = 0.58, 0.48 iter = 2000
# C = 1.0, gamma = 0.01 , acc = 0.82, 0.57 iter = 10000