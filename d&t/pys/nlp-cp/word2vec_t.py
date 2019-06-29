# -*-coding= utf-8 -*-
from gensim.models import word2vec 
import numpy as np
import logging
logging.basicConfig(format='%(asctime)s : %(levelname)s : %(message)s', level=logging.INFO)

v_dim = 20

# 两种方式读的词数是一样的。
# sentences = word2vec.Text8Corpus('segedwords_train.txt')

# sentences = word2vec.LineSentence('segedwords_all.txt')

# # print type(sentences)
# model = word2vec.Word2Vec(sentences,size=v_dim,min_count=1, sg= 1)
# # #sg 有区分了，用cbow全是0.99xxx
# # # model.save('./trainvec100line.model')
# model.save('./allvec20sg.model')
#继续训练
model = word2vec.Word2Vec.load("allvec20sg.model")
mysentences = word2vec.LineSentence('segedwords_all.txt')
model.train(mysentences)
model.save('./allvec20sg.model')

# model = word2vec.Word2Vec.load_word2vec_format('./GoogleNews-vectors-negative300.bin', binary=True)
# model.save_word2vec_format('./GoogleNews-vectors-negative300.txt', binary=False)



# 
# using  text8
# sentences = word2vec.Text8Corpus('text8')
# model = word2vec.Word2Vec(sentences,size=v_dim,min_count=3, sg= 1)
# model.save('./text8v100.model')
# #用自己的数据 继续训练
# model = word2vec.Word2Vec.load("text8v100.model")
# mysentences = word2vec.Text8Corpus('text8')
# # mysentences = word2vec.LineSentence('segedwords_train.txt')
# model.train(mysentences)
# model.save('./text8v100m.model')

# # 对应的加载方式  
# # model_2 = word2vec.Word2Vec.load("text.model")  
# model.save_word2vec_format('./devtestb.model.bin', binary=True)
# model = word2vec.Word2Vec.load("devtest.model")
y2 = model.most_similar("good", topn=20)  # 20个最相关的  
print "和good最相关的词有：\n"
for item in y2:
	print item[0], item[1]
print "--------\n"
# print model['RRB']

print model['lovely']
# print model['.']
# print model['\n']
# print model['-RRB-']