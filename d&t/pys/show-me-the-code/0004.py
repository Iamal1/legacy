# -*- coding: utf-8 -*-


import io, re

count = 0

with io.open('test4.txt', 'r') as file:
    for line in file.readlines():
        list = re.findall("[a-zA-Z]+'*-*[a-zA-Z]*", line)
        count += len(list)
print(count)
##2016/12/6
##学习正则表达式，好像没什么用，我又记不住规则，每次还得百度
##理解了好久那个*-*是什么意思，结果。。就只是匹配前一个字符
##但是[a-zA-Z]+这种就可以匹配到一个连续的字母串。
##后面的是针对英文的'和-的，匹配0次或多次,但是无法针对'-一起出现，不过好像正确文本不用考虑
##感觉正则式的写法是从你要的字符串抽象一个表达，像我这样拿着表达式去反推有点难，也很蠢。。。主要还是不熟悉
# s="123ab'c456e-abc7lo-op8aa     a-'ddd9aa'-we98"
# list = re.findall(r"[a-zA-Z]+'*-*[a-zA-Z]*",s)
# print list
# re.match("^[\\s&&[^\\n]]*$", line):
#             whiteLines += 1
# \s匹配任何不可见字符，包括空格、制表符、换页符等等。等价于[ \f\n\r\t\v]。
# "^[a-x]*$"匹配到'\n'，暂时不理解 为什么能匹配到
whiteline = 0

with io.open('test4.txt', 'r') as file:
    for line in file.readlines():
        if re.match("^[\\s&&[^\\n]]*$", line):
        	whiteline+=1
        	print repr(line),len(line)
print(whiteline)
#“^”表示行的开始；“$”表示行的结尾
# 点字符（.）可以匹配除换行符外的任意一个字符。
#\b还不懂
# "^[\\w-]+(\\.[\\w-]+)*@[\\w-]+(\\.[\\w-]+)+$"　　　　//email地址 