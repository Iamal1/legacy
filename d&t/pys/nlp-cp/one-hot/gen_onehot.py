#-*- coding=utf-8 -*-
from keras.layers import Input, Dense
from keras.models import Model
from keras.models import Sequential  
from keras.layers.core import Dense, Dropout, Activation  
from keras.optimizers import SGD  
from keras.utils import np_utils
import numpy as np
from gensim.models import word2vec 


v_dim = 100
#...word to vector model

def dict2vec(wd,wdd):
	dim = len(wdd)
	vec = np.zeros(dim)
	vec[wdd[wd]] = 1
	return vec

wordset = set()

with open('segedwords.txt','r') as file:
    for line in file.readlines():
        newline = line[1:].split()
        wordset.update(newline)

v_dim = len(wordset)
# assert '.' in wordset , 'not in'
print v_dim

worddict={}

if len(wordset)> 0:
	# worddict ={word:idn, for word , idn in ws, range(len(wordset))}
	idn=0
	for word in wordset:
		worddict[word] = idn
		idn += 1

# np.set_printoptions(threshold='nan')
# print dict2vec('lovely',worddict)
# print worddict['lovely']
#prepare data and label from ...
def get_word_vectors(words):
    vec = np.zeros(v_dim)
    count = 0
    for word in words:
        try:
            vec += dict2vec(word,worddict)
            # print vecmodel[word]
            count += 1
        except KeyError:
            continue
    vec = vec/count
    return vec

data = []
labels = []
with open('dev.txt') as file:
    for line in file.readlines():
        words = line.split()
        label = int(words[0])
        labels.append(label)
        words = words[1:]
        vec = get_word_vectors(words)
        data.append(vec)

#change the shape of input
nb_samples = len(labels)
print nb_samples
labels = np.array(labels)
labels.reshape(nb_samples,1)
labels = np_utils.to_categorical(labels,5)
# print labels
data = np.array(data)
data.reshape(nb_samples,v_dim)
# data1 = np.zeros([nb_samples,v_dim])
# for i in range(nb_samples):
# 	data1[i,:]+=data[i]
# data = data1
#尝试量化？？
# data = (data*100).round()
# print data
#随机数据
# nb_samples=1000
# data=np.random.random((nb_samples,v_dim))
# labels=np.random.randint(5,size=(nb_samples,1))
# labels = np_utils.to_categorical(labels,5)

## model MLP 
model = Sequential()  

model.add(Dense(256, input_dim=v_dim, init='uniform'))
model.add(Activation('tanh'))
model.add(Dropout(0.5))
model.add(Dense(64, init='uniform'))
model.add(Activation('tanh'))
model.add(Dropout(0.5))
model.add(Dense(5, init='uniform'))
model.add(Activation('softmax'))

sgd = SGD(lr=0.5, decay=1e-6, momentum=0.9, nesterov=True) 
model.compile(loss='mse', optimizer='adagrad', metrics=["accuracy"])


# # model.load_weights('testadammlp.d5', overwrite=True)
model.fit(data,labels,batch_size=100,nb_epoch=30,shuffle=True,\
	verbose=2,show_accuracy=True,validation_split=0.1)

model.save_weights('onehotdev.d5', overwrite=True)

# tdata = []
# tlabels = []
# with open('dev.txt') as file:
#     for line in file.readlines():
#         words = line.split()
#         label = int(words[0])
#         tlabels.append(label)
#         words = words[1:]
#         vec = get_word_vectors(words)
#         tdata.append(vec)


# tnb_samples = len(tlabels)
# print tnb_samples
# tlabels = np.array(tlabels)
# tlabels.reshape(tnb_samples,1)
# tlabels = np_utils.to_categorical(labels,5)
# # print labels
# tdata = np.array(tdata)
# tdata.reshape(nb_samples,v_dim)
# err,acc =model.evalute(tdata, tlabels, batch_size=100, show_accuracy=True,verbose=1, sample_weight=None)
# print err, acc
np.set_printoptions(threshold='nan')
yt = model.predict(data)
print yt
yc = model.predict_classes(data)
print yc




