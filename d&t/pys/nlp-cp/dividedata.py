# -*-coding= utf-8 -*-
from random import shuffle
import io, re

count =0
with open('train.txt', 'r') as file:
    lines=file.readlines()

shuffle(lines)

for i in range(int(len(lines)/1000)):
    line = lines[1000*i:1000*(i+1)]
    print len(line)
    with open('sdata/strain_%d.txt'%(i+1),'wb') as outfile:
        for lin in line:
            outfile.write(lin)
line = lines[8000:]
with open('sdata/strain_9.txt','wb') as outfile:
    for lin in line:
        outfile.write(lin)