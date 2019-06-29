#-*- coding=utf-8 -*-
import numpy as np
from gensim.models import word2vec 

v_dim = 100
#...word to vector model
vecmodel = word2vec.Word2Vec.load("trainvec100sg.model")

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
with open('train.txt') as file:
    with open('libsvmtrain.txt','wb') as outfile:
        for line in file.readlines():
            words = line.split()
            label = int(words[0])
            labels.append(label)
            words = words[1:]
            vec = get_word_vectors(words)
            data.append(vec)
            sstr = str(label) + ' '
            for idx in range(v_dim):
                val = vec[idx]
                sstr += '%d:%f '%(idx+1,val)
            outfile.write(sstr+'\n')




