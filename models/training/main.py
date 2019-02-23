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


from sklearn.model_selection import train_test_split

X_train, X_test, y_train, y_test = train_test_split(data_train, label_train, test_size=0.1, random_state=0)

print(len(X_train), len(X_test), len(y_train), len(y_test))

from sklearn import svm

clf = svm.SVC(kernel='rbf', C=10, gamma=0.01, max_iter=100000).fit(X_train, y_train)


print(clf.score(X_train, y_train))
print(clf.score(X_test, y_test))