#-*- coding=utf-8 -*-
from keras.layers import Input, Dense
from keras.models import Model
from keras.models import Sequential  
from keras.layers.core import Dense, Dropout, Activation  
from keras.optimizers import SGD  
from keras.utils import np_utils
import numpy as np
from gensim.models import word2vec 

v_dim = 50
#...word to vector model
vecmodel = word2vec.Word2Vec.load("../trainvec50cb.model")

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

data = []
labels = []
path1 = '../../train.txt'
path2 ='../../dev.txt'
with open(path2) as file:
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
# labels.reshape(nb_samples,1)

labels[labels!=0]=1
# labels[labels==0]=1
y0 = [0 for i in labels if i ==0 ]
y1 = [1 for i in labels if i ==1 ]
y2 = [2 for i in labels if i ==2 ]
y3 = [3 for i in labels if i ==3 ]
y4 = [4 for i in labels if i ==4 ]
# labels = np_utils.to_categorical(labels,2)
# print labels
data = np.array(data)
data.reshape(nb_samples,v_dim)
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

model.add(Dense(128, input_dim=v_dim, init='uniform'))
model.add(Activation('tanh'))
model.add(Dropout(0.5))
model.add(Dense(64))
model.add(Activation('tanh'))
model.add(Dropout(0.25))
model.add(Dense(1))
model.add(Activation('sigmoid'))

sgd = SGD(lr=0.1, decay=1e-6, momentum=0.9, nesterov=True) 
model.compile(loss='binary_crossentropy', optimizer='sgd', metrics=["accuracy"])


# model.load_weights('mlp_50d_1.d5')
model.fit(data,labels,batch_size=5,nb_epoch=10,shuffle=True,\
	verbose=2,show_accuracy=True,validation_split=0.1)
# model.save_weights('mlp_50d_1.d5', overwrite=True)
# tmp ="The most hopelessly monotonous film of the year , noteworthy only for the gimmick of being filmed as a single unbroken 87-minute take ."
# tmp = tmp.split()
# # print tmp
# xt = get_word_vectors(tmp).reshape(1,v_dim)
# # print xt
# err,acc =model.evaluate(data, labels, batch_size=100, show_accuracy=True,verbose=1)
# print err, acc
# np.set_printoptions(threshold='nan')
yt = model.predict(data)
print yt
yc = model.predict_classes(data)
print yc


#show class counts
yc0 = [0 for i in yc if i ==0 ]
yc1 = [1 for i in yc if i ==1 ]
yc2 = [2 for i in yc if i ==2 ]
yc3 = [3 for i in yc if i ==3 ]
yc4 = [4 for i in yc if i ==4 ]

print len(y0),len(y1),len(y2),len(y3),len(y4)
print len(yc0),len(yc1),len(yc2),len(yc3),len(yc4)
# tdata = []
# tlabels = []
# with open('../dev.txt') as file:
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
# tlabels = np_utils.to_categorical(tlabels,5)
# # print labels
# tdata = np.array(tdata)
# tdata.reshape(tnb_samples,v_dim)
# # s_w = np.ones(tnb_samples)
# err,acc =model.evaluate(tdata, tlabels, batch_size=100, show_accuracy=True,verbose=1)
# # err, acc = model.test_on_batch(tdata, tlabels, sample_weight=None)
# ### ValueError: Input arrays should have the same number of samples as target arrays. Found 1101 input samples and 8544 target samples.
# print err, acc