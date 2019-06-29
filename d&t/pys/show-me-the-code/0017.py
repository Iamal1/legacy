# -*-coding:utf-8-*-
'''
这一篇有点高级，还没看懂
json
xml
本以为理解了中文问题，这里还是很无力
据说python3就好多了。。。g了个g
1.如果你在python中进行编码和解码的时候，不指定编码方式，那么python就会使用defaultencoding。 
而python2.x的的defaultencoding是ascii,
这也就是大多数python编码报错：“UnicodeDecodeError: 'ascii' codec can't decode byte ......”的原因。

2.关于头部的# coding：utf-8,有以下几个作用
2.1如果代码中有中文注释，就需要此声明------原来这样
2.2比较高级的编辑器（比如我的emacs），会根据头部声明，将此作为代码文件的格式。
2.3程序会通过头部声明，解码初始化 u"人生苦短"，这样的unicode对象，（所以头部声明和代码的存储格式要一致）

另，17 18 19 大同小异，差别在于输入的格式处理
'''
import xlrd, re, json
import sys
reload(sys)
sys.setdefaultencoding('utf-8')
#上面这一段还是有用的，不然出现ascii cannot decode

# xlrd.Book.encoding = 'gbk'
with xlrd.open_workbook('result0014.xls') as file:
    table = file.sheet_by_index(0)

rows = table.nrows
cols = table.ncols

dic = {}

content = '<?xml version="1.0" encoding="UTF-8"?>\n<root>\n<students>\n<!--\n    学生信息表\n    "id" : [名字, 数学, 语文, 英文]\n-->\n'

for row in range(rows):
    stu = table.row_values(row)
    list = []
    for x in range(len(stu)-1):
        list.append(stu[x+1])
        # print(isinstance(stu[x+1],unicode)) # 判断是否是unicode编码
    dic[stu[0]] = list

s = json.dumps(dic, indent=4, ensure_ascii=False)

content = content + s + '\n</students>\n</root>'
with open('student.xml', 'w') as f:
    f.write(content)
