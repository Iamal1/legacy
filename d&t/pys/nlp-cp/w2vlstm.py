#-*- coding=utf-8 -*-
from keras.layers import Input, Dense
from keras.models import Model
from keras.models import Sequential  
from keras.layers.core import Dense, Dropout, Activation
from keras.layers import Embedding
from keras.optimizers import SGD
from keras.layers import LSTM, SimpleRNN, GRU
from keras.utils import np_utils
import numpy as np
from gensim.models import word2vec 
import theano

# theano.config.compute_test_value = 'warn'  

v_dim = 200
seq_length = 10
#...word to vector model
vecmodel = word2vec.Word2Vec.load("trainvec200sg.model")

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
with open('dev.txt') as file:
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
labels.reshape(nb_samples,1)
labels = np_utils.to_categorical(labels,5)
# print labels
data = np.array(data)
data = data.reshape(nb_samples,seq_length,v_dim)

#尝试量化？？
# data = (data*100).round()
# print data
#随机数据
# nb_samples=1000
# data=np.random.random((nb_samples,v_dim))
# labels=np.random.randint(5,size=(nb_samples,1))
# labels = np_utils.to_categorical(labels,5)

## model LSTM

# model = Sequential()
# # model.add(Embedding(v_dim, 128, dropout=0.2))
# model.add(LSTM(v_dim, 128, dropout_W=0.2, dropout_U=0.2))  # try using a GRU instead, for fun
# model.add(Dense(5))
# model.add(Activation('softmax'))

# # print('Build model...')

model = Sequential()

model.add(GRU(output_dim=128,input_dim = v_dim, activation='tanh', inner_activation='hard_sigmoid', input_length=seq_length))  # try using a GRU instead, for fun

model.add(Dropout(0.5))

model.add(Dense(5, activation='softmax'))
model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=["accuracy"])

# model.load_weights('testlgruv200.d5')
model.fit(data,labels,batch_size=40,nb_epoch=20,shuffle=True,\
	verbose=2,show_accuracy=True,validation_split=0.1)
# model.save_weights('testlgruv20010.d5', overwrite=True)
# # tmp ="The most hopelessly monotonous film of the year , noteworthy only for the gimmick of being filmed as a single unbroken 87-minute take ."
# # tmp = tmp.split()
# # # print tmp
# # xt = get_word_vectors(tmp).reshape(1,v_dim)
# # # print xt
# np.set_printoptions(threshold='nan')
# # yt = model.predict(data)
# # print yt
# yc = model.predict_classes(data)
# print yc
