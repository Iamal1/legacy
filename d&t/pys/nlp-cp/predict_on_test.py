#-*- coding=utf-8 -*-
from keras.layers import Input, Dense
from keras.models import Model
from keras.models import Sequential  
from keras.layers.core import Dense, Dropout, Activation  
from keras.optimizers import SGD  
from keras.utils import np_utils
import numpy as np
from gensim.models import word2vec 

v_dim = 200
#...word to vector model
vecmodel = word2vec.Word2Vec.load("./trainvec200sg.model")

#prepare data and label from ...
def get_word_vectors(words):
    vec = np.zeros(v_dim)
    count = 0
    for word in words:
        try:
            vec += vecmodel[word]
            # print vecmodel[word]
            count += 1
        except KeyError:
            continue
    if count != 0:
        vec /= count
    return vec

path = './test-release.txt'
data = []
with open('test-release.txt') as file:
    for line in file.readlines():
        words = line.split()
        vec = get_word_vectors(words)
        data.append(vec)

#change the shape of input
nb_samples = len(data)
print nb_samples
# print labels
data = np.array(data)
data = data.reshape(nb_samples,v_dim)


## model MLP 
model = Sequential()  

model.add(Dense(128, input_dim=v_dim, init='uniform'))
model.add(Activation('tanh'))
model.add(Dropout(0.5))
model.add(Dense(64, init='uniform'))
model.add(Activation('relu'))
model.add(Dropout(0.25))
model.add(Dense(5, init='uniform'))
model.add(Activation('softmax'))

sgd = SGD(lr=0.1, decay=1e-6, momentum=0.9, nesterov=True) 
model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=["accuracy"])


model.load_weights('mlptrav200_1.d5')

yc = model.predict_classes(data,verbose=0)

with open('test-release.txt') as file:
    lines = file.readlines()


with open('test-result.txt','wb') as outfile:   
    for i in range(len(yc)):
        line = lines[i]
        line = str(yc[i])+ ' ' + line
        outfile.write(line)


# with open('test-release.txt') as file:
#     lines = file.readlines()


#     for line in lines:
#         label = int(words[0])
#         tlabels.append(label)
#         words = words[1:]
#         vec = get_word_vectors(words)
#         tdata.append(vec)

