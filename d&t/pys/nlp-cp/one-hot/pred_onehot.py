#-*- coding=utf-8 -*-
from keras.layers import Input, Dense
from keras.models import Model
from keras.models import Sequential  
from keras.layers.core import Dense, Dropout, Activation  
from keras.optimizers import SGD  
from keras.utils import np_utils
import numpy as np
from gensim.models import word2vec 
from collections import defaultdict

v_dim = 100
#...word to vector model

def dict2vec(wd,wdd):
    dim = len(wdd)
    vec = np.zeros(dim)
    wd = wd.lower()
    vec[wdd[wd]] = 1
    return vec

wordset = set()
dic = defaultdict(int)

with open('segedwords.txt','r') as file:
    for line in file.readlines():
        for word in line[1:].split():
            dic[word.lower()] += 1

sdic = sorted(dic.iteritems(),key=lambda x:x[1],reverse = True)
print sdic
# print len(sdic)
stoplist = ['.',',','the','of','and','a','an','of', 'to', "'s", 'is', \
'that', 'in', 'it', 'the', 'as','...',"'",'?',':','movie','its',"''", '``','film','films']
fre3 = {a:b for a,b in sdic if b<300}
# print len(fre3)
idn=0
worddict={}
for word, v in sdic:
    if word not in stoplist:
        worddict[word] = idn
        idn += 1
###原来错在这里 这个遍历方式错了
# for word in sdic:
#     worddict[word] = idn
#     idn += 1

# print worddict


# v_dim = len(worddict)
# print v_dim

# #prepare data and label from ...
# def get_word_vectors(words):
#     vec = np.zeros(v_dim)
#     count = 0
#     for word in words:
#         try:
#             vec += dict2vec(word,worddict)
#             # print vecmodel[word]
#             count += 1
#         except KeyError:
#             continue
#     if count != 0:
#         vec = vec/count
#     return vec

# data = []
# labels = []
# with open('dev.txt') as file:
#     for line in file.readlines():
#         words = line.split()
#         label = int(words[0])
#         labels.append(label)
#         words = words[1:]
#         vec = get_word_vectors(words)
#         data.append(vec)

# #change the shape of input
# nb_samples = len(labels)
# print nb_samples
# labels = np.array(labels)
# labels.reshape(nb_samples,1)
# labels = np_utils.to_categorical(labels,5)
# # print labels
# data = np.array(data)
# data.reshape(nb_samples,v_dim)
# print data.shape
# np.set_printoptions(threshold='nan')
# print data[1]

# model = Sequential()  

# model.add(Dense(256, input_dim=v_dim, init='uniform'))
# model.add(Activation('tanh'))
# model.add(Dropout(0.5))
# model.add(Dense(64, init='uniform'))
# model.add(Activation('tanh'))
# model.add(Dropout(0.5))
# model.add(Dense(5, init='uniform'))
# model.add(Activation('softmax'))

# sgd = SGD(lr=0.5, decay=1e-6, momentum=0.9, nesterov=True) 
# model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=["accuracy"])


# # # model.load_weights('testadammlp.d5', overwrite=True)
# model.fit(data,labels,batch_size=20,nb_epoch=5,shuffle=True,\
#     verbose=2,show_accuracy=True,validation_split=0.1)

# model.save_weights('ponehotdev.d5', overwrite=True)

# np.set_printoptions(threshold='nan')
# yt = model.predict(data)
# print yt
# yc = model.predict_classes(data)
# print yc