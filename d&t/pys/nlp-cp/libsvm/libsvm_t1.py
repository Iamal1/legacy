# import sys
# path = 'D:\Program Files\libsvm-3.22\python'
# sys.path.append(path)

from svmutil import *
y, x = svm_read_problem('libsvmtrain.txt')
# print y
# print x
m1 = svm_train(y[:7500],x[:7500], '-s 0 -t 2 -g 0.0001 -c 2 -v 5 -e 0.01 -h 0')
m2 = svm_train(y[:800],x[:800], '-s 0 -t 2 -g 0.0001 -c 2 -e 0.01')
# m = svm_train(y[200:],x[200:], '-s 0 -t 0')
p_label, p_acc, p_val = svm_predict(y[7500:],x[7500:],m2)
print p_label

# y, x = svm_read_problem('libsvmdev.txt')
# # print y
# # print x
# m1 = svm_train(y[:750],x[:750], '-s 0 -t 2 -g 0.0001 -c 2 -v 5 -e 0.01')
# # m2 = svm_train(y[:750],x[:750], '-s 0 -t 2 -g 0.0001 -c 10 -e 0.01')
# # m = svm_train(y[200:],x[200:], '-s 0 -t 0')
# p_label, p_acc, p_val = svm_predict(y[750:],x[750:],m1)
# print p_label