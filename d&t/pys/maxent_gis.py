#coding=utf8

import sys;
import math;
from collections import defaultdict

input_data ="\
Outdoor Sunny Happy\n\
Outdoor Sunny Happy Dry\n\
Outdoor Sunny Happy Humid\n\
Outdoor Sunny Sad Dry\n\
Outdoor Sunny Sad Humid\n\
Outdoor Cloudy Happy Humid\n\
Outdoor Cloudy Happy Humid\n\
Outdoor Cloudy Sad Humid\n\
Outdoor Cloudy Sad Humid\n\
Indoor Rainy Happy Humid\n\
Indoor Rainy Happy Dry\n\
Indoor Rainy Sad Dry\n\
Indoor Rainy Sad Humid\n\
Indoor Cloudy Sad Humid\n\
Indoor Cloudy Sad Humid"


class MaxEnt(object):
    def __init__(self):
        #f(x,y)={0,1} feats{(label,feature):count}
        self.feats = defaultdict(int)
        self.trainset = []
        #label set Y
        self.labels = set()
    #default to load data from file
    def load_data(self,file):
        with open(file) as fi:
            for line in fi:
                fields = line.strip().split()
                # at least two columns
                if len(fields)<2: continue
                label = fields[0]
                self.labels.add(label)
                for f in set(fields[1:]):
                    # (label,f) tuple is key, value is count
                    self.feats[(label,f)] += 1
                self.trainset.append(fields)

    def load_data_from_str(self,inputstr):
        for line in inputstr.split('\n'):
            fields = line.strip().split()
            # at least two columns
            if len(fields)<2: continue
            label = fields[0]
            self.labels.add(label)
            for f in set(fields[1:]):
                # (label,f) tuple is key, value is count
                self.feats[(label,f)] += 1
            self.trainset.append(fields)

    def _initparams(self):
        #sample size(N)
        self.size = len(self.trainset)
        #max_{i,j}(sum_{k} f_k(x_i,y_j)) i.e count the feature in one record
        self.C = max([len(record)-1 for record in self.trainset])
        #we have m features f_k, k= 1...m, m = len(self.feats)
        # cal the empirical feature expectation
        self.Ep_ = [0.0]*len(self.feats)
        for i, f in enumerate(self.feats):
            # f =(label,x)
            self.Ep_[i] = float(self.feats[f])/float(self.size)
            # set idx for the current (y,x) pair
            self.feats[f] = i
        #init weight for each feature
        self.w = [0.0]*len(self.feats)
        self.lastw = self.w

    #calculate exp(w*f(x,y)) the numerator of P(y|x,w)
    def probwgt(self,features,label):
        wgt = 0.0
        for f in features:
            if (label,f) in self.feats:
                wgt += self.w[self.feats[(label,f)]]
        return math.exp(wgt)

    def calprob(self,features):
        wgts = [(self.probwgt(features, l),l) for l in self.labels]
        # Z is the denominator of P(y|x,w)
        Z = sum([ w for w,l in wgts])
        prob = [ (w/Z,l) for w,l in wgts]
        return prob

    """
    calculate feature expectation on model distribution
    """        
    def Ep(self):
        ep = [0.0]*len(self.feats)
        for record in self.trainset:
            features = record[1:]
            # calculate p(y|x,W)
            prob = self.calprob(features)
            for f in features:
                for w,l in prob:
                    # only focus on features from training data, f(y|x) = 1
                    if (l,f) in self.feats:
                        # get feature id
                        idx = self.feats[(l,f)]
                        # sum(1/N * f(y,x)*p(y|x)), p(x) = 1/N.
                        ep[idx] += w * (1.0/self.size)
        return ep

    def _convergence(self,lastw,w):
        for w1,w2 in zip(lastw,w):
            if abs(w1-w2) >= 0.01:
                return False
        return True

    def train(self, max_iter =1000):
        self._initparams()
        for i in range(max_iter):
            print 'iter %d ...'%(i+1)
            # calculate feature expectation on model distribution
            self.ep = self.Ep()
            self.lastw = self.w[:]
            for i, w in enumerate(self.w):
                #using the formula
                delta = 1.0/self.C * math.log(self.Ep_[i]/self.ep[i])
                # update w
                self.w[i] += delta
            print self.w
            # test if the algorithm is of convergence
            if self._convergence(self.lastw,self.w):
                break

    def predict(self,inputstr):
        features = inputstr.strip().split()
        prob = self.calprob(features)
        prob.sort(reverse=True)
        print prob
        return prob
#create a model
model = MaxEnt()
#can load data in two ways, data is the samples for training
#input canbe a string with '\n' like input_data or a file
#model.load_data('data.txt')
model.load_data_from_str(input_data)
#train the model using GIS
model.train()
'''some tests
call predict with the features as below to predict y, indoor or outdoor
you can choose the one with higher probability.
note that the first letter must be capital 
otherwise the word will be treated as a new feature,which is incorrect
'''
model.predict('Sunny')
model.predict('Sunny Happy')
model.predict('Cloudy')
model.predict('Cloudy sad')
model.predict('Rainy Happy Dry')

