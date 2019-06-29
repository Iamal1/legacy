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

v_dim = 2000
seq_length = 10
#...word to vector model


#尝试量化？？
# data = (data*100).round()
# print data
#随机数据
nb_samples=1000
data=np.random.random((nb_samples,v_dim))
labels=np.random.randint(5,size=(nb_samples,1))
labels = np_utils.to_categorical(labels,5)

## model LSTM

model = Sequential()
model.add(Embedding(v_dim, 128, dropout=0.2))
model.add(LSTM(128, dropout_W=0.2, dropout_U=0.2))  # try using a GRU instead, for fun
model.add(Dense(5))
model.add(Activation('softmax'))

model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=["accuracy"])
# model.load_weights('testlgruv200.d5')
model.fit(data,labels,batch_size=100,nb_epoch=5,shuffle=True,\
	verbose=2,show_accuracy=True,validation_split=0.1)
model.save_weights('testlgruv20010.d5', overwrite=True)
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
