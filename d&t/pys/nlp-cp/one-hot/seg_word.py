# -*-coding= utf-8 -*-

import io, re

# count = 0

# with io.open('train.txt', 'r') as file:
#     for line in file.readlines():
#         if line.endswith('\n'):
#         	count += 1
# print(count)
'''
io.open()打开文件是当作unicode
一般open是str 按照什么解读的呢？gbk？之前encode gbk就还原了
但是文本打开看到有乱码的。？？？
print type(line) #unicode
'''
#暂时失败，不管这个了
# with open('dev.txt', 'r') as file:
#     for line in file.readlines():
#         if not re.match("-*[A-Za-z]+-*", line):
#           print line


# with open('dev.txt', 'r') as file:
#     with open('segedwords.txt','wb') as outfile:
#         for line in file.readlines():
            
#             newline = line[1:]
#             outfile.write(newline)

with open('train.txt', 'r') as file:
    with open('segedwords_train.txt','wb') as outfile:
        for line in file.readlines():
            
            newline = line[1:]
            outfile.write(newline)