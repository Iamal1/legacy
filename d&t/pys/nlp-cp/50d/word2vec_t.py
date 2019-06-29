# -*-coding= utf-8 -*-
from gensim.models import word2vec 
import numpy as np
import logging
logging.basicConfig(format='%(asctime)s : %(levelname)s : %(message)s', level=logging.INFO)

v_dim = 50
# model = word2vec.Word2Vec.load("devtest.model")

# def get_word_vectors(words):
#     vec = np.zeros(v_dim).reshape(1,v_dim)
#     count = 0
#     for word in words:
#         try:
#             vec += model[word].reshape(1,v_dim)
#             count += 1
#         except KeyError:
#             continue
#     if count != 0:
#         vec /= count
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

# labels =[]
# with open('dev.txt') as file:
#     for line in file.readlines():
#         words = line.split()
#         label = int(words[0])
#         labels.append(label)



# nb_samples = len(labels)
# labels = np.array(labels)
# labels.reshape(nb_samples,1)
# print labels

# nb_samples = len(labels)
# labels = np.array(labels)
# labels.reshape(nb_samples,1)
# data = np.array(data)
# data.reshape(nb_samples,v_dim)
# 两种方式读的词数是一样的。
# sentences = word2vec.Text8Corpus('segedwords_train.txt')


# # #sg / cbow
#setting mc =2 , sg = 1, window=5
##first train
# sentences = word2vec.LineSentence('../segedwords_train.txt')
# model = word2vec.Word2Vec(sentences,size=v_dim,min_count=2, sg= 0, window = 3)
# model.save('./trainvec50cb.model')

#继续训练
# sentences = word2vec.LineSentence('../segedwords_train.txt')
# model = word2vec.Word2Vec.load("trainvec50cb.model")
# model.train(sentences)
# model.save('./trainvec50cb.model')

# 
# using  text8
# sentences = word2vec.Text8Corpus('../text8')
# model = word2vec.Word2Vec(sentences,size=v_dim,min_count=3, sg= 1)
# model.save('./text8v50_sg.model')
#用自己的数据 继续训练
model = word2vec.Word2Vec.load("./text8v50_sg.model")
mysentences = word2vec.LineSentence('segedwords_train.txt')
model.train(mysentences)
model.save('./text8v50_sg_t.model')

# # 对应的加载方式  
# # model_2 = word2vec.Word2Vec.load("text.model")  
# model.save_word2vec_format('./devtestb.model.bin', binary=True)
# model = word2vec.Word2Vec.load("devtest.model")
testlist = ['good', 'film','hopeless','boring','horror', 'it', ',']
for hhh in testlist:
	y2 = model.most_similar(hhh, topn=20)  # 20个最相关的  
	print "和%s最相关的词有：\n"%hhh
	for item in y2:
		print item[0], item[1]
	print "--------\n"
# KeyError: "word 'n't' not in vocabulary"
# KeyError: "word ',' not in vocabulary"
# print model['RRB']

# print model['lovely']
# print model['.']
# # print model['\n']
# print model['-RRB-']