#-*- coding=utf-8 -*-
from keras.layers import Input, Dense
from keras.models import Model
from keras.models import Sequential  
from keras.layers.core import Dense, Dropout, Activation  
from keras.optimizers import SGD  
from keras.utils import np_utils
from keras.layers import Convolution1D, MaxPooling1D,Flatten
from keras.layers.convolutional import Convolution2D, MaxPooling2D
from keras.callbacks import EarlyStopping
from keras.regularizers import l2, activity_l2
import numpy as np
from gensim.models import word2vec 
from collections import Counter
from keras import backend 
backend.set_image_dim_ordering('th')
# set parameters:
# max_features = 5000
# maxlen = 20
# batch_size = 32
# embedding_dims = 50
filter_length = 3
hidden_dims = 128
nb_epoch = 2
v_dim = 20
seq_length = 30
#...word to vector model
vecmodel = word2vec.Word2Vec.load("../allvec20sg.model")

####prepare data and label from ...
data = []
labels = []
path = '../sdata/strain_1.txt'
path1='../train.txt'
path2 = '../dev.txt'
with open(path2) as file:
    for line in file.readlines():
        words = line.split()
        label = int(words[0])
        labels.append(label)
        words = words[1:]
        vec=np.zeros(v_dim)
        count = 0
        for word in words:
            if count < seq_length:
                try:
                    vec = vecmodel[word]
                    
                    # print vecmodel[word]
                    count += 1
                except KeyError:
                    continue
                data.append(vec)
            else:
                break
        if count < seq_length:
            zeros = np.zeros(v_dim)
            for i in range(seq_length-count):
                data.append(zeros)

#change the shape of input
nb_samples = len(labels)
print nb_samples
labels = np.array(labels)

cnt0=Counter(labels)

labels.reshape(nb_samples,1)
labels = np_utils.to_categorical(labels,5)
# print labels
data = np.array(data)
data = data.reshape(nb_samples,1,seq_length,v_dim)
print data.shape
#尝试量化？？
# data = (data*100).round()
# print data
#随机数据
# nb_samples=1000
# data=np.random.random((nb_samples,1,28,28))
# labels=np.random.randint(5,size=(nb_samples,1))
# labels = np_utils.to_categorical(labels,5)

## model CNN
#VGG is too slow 
weight_decay = 0.001
early_stopping = EarlyStopping(monitor = 'val_loss', patience = 5)

model = Sequential()

model.add(Convolution2D(64, 5, 5, border_mode='full', input_shape=(1, seq_length, v_dim)))
model.add(Activation('tanh'))
model.add(Convolution2D(32, 3, 3, border_mode='valid'))
model.add(Activation('tanh'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Dropout(0.5))

model.add(Flatten())
# Note: Keras does automatic shape inference.
model.add(Dense(128))
model.add(Activation('relu'))
model.add(Dropout(0.5))

model.add(Dense(5))
model.add(Activation('softmax'))


# model.add(Convolution2D(64, 5, 5, border_mode='full', input_shape=(1, seq_length, v_dim),W_regularizer=l2(weight_decay)))
# model.add(Activation('tanh'))
# model.add(MaxPooling2D(pool_size=(2, 2)))
# model.add(Dropout(0.5))

# model.add(Convolution2D(64, 3, 3, border_mode='valid',W_regularizer=l2(weight_decay)))
# model.add(Activation('tanh'))
# model.add(MaxPooling2D(pool_size=(2, 2)))
# model.add(Dropout(0.25))

# model.add(Flatten())
# # Note: Keras does automatic shape inference.
# model.add(Dense(128,W_regularizer=l2(weight_decay)))
# model.add(Activation('relu'))
# model.add(Dropout(0.5))

# model.add(Dense(5))
# model.add(Activation('softmax'))

sgd = SGD(lr=0.1, decay=1e-6, momentum=0.9, nesterov=True) 
model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=["accuracy"])


# model.load_weights('cnn2v30x20_3.d5')
hist =  model.fit(data,labels,batch_size=20,nb_epoch=10,shuffle=True,\
	verbose=2,show_accuracy=True,validation_split=0.1,callbacks=[early_stopping] )
model.save_weights('cnn2v30x20_3.d5', overwrite=True)

# print hist.history
# err,acc =model.evaluate(data, labels, batch_size=100, show_accuracy=True,verbose=0)
# print err, acc

np.set_printoptions(threshold='nan')
# yt = model.predict(data)
# print yt
yc = model.predict_classes(data,verbose=0)
# print yc

#show class counts
# yc0 = [0 for i in yc if i ==0 ]
# yc1 = [1 for i in yc if i ==1 ]
# yc2 = [2 for i in yc if i ==2 ]
# yc3 = [3 for i in yc if i ==3 ]
# yc4 = [4 for i in yc if i ==4 ]
cnt1 = Counter(yc)
print '---'
for k,v in cnt0.iteritems():
    print(k,'-->',v)
for k,v in cnt1.iteritems():
    print(k,'-->',v)  
# tdata = []
# tlabels = []
# with open('../dev.txt') as file:
#     for line in file.readlines():
#         words = line.split()
#         label = int(words[0])
#         tlabels.append(label)
#         words = words[1:]
#         vec=np.zeros(v_dim)
#         count = 0
#         for word in words:
#             if count < seq_length:
#                 try:
#                     vec = vecmodel[word]
                    
#                     # print vecmodel[word]
#                     count += 1
#                 except KeyError:
#                     continue
#                 tdata.append(vec)
#             else:
#                 break
#         if count < seq_length:
#             zeros = np.zeros(v_dim)
#             for i in range(seq_length-count):
#                 tdata.append(zeros)


# tnb_samples = len(tlabels)
# print tnb_samples
# tlabels = np.array(tlabels)
# tlabels.reshape(tnb_samples,1)
# tlabels = np_utils.to_categorical(tlabels,5)
# # print labels
# tdata = np.array(tdata)
# tdata = tdata.reshape(tnb_samples,1,seq_length,v_dim)
# # s_w = np.ones(tnb_samples)
# err,acc =model.evaluate(tdata, tlabels, batch_size=100, show_accuracy=True,verbose=0)
# # err, acc = model.test_on_batch(tdata, tlabels, sample_weight=None)
# ### ValueError: Input arrays should have the same number of samples as target arrays. Found 1101 input samples and 8544 target samples.
# print err, acc