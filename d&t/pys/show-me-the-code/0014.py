# -*- coding: utf-8 -*-
'''
jhk
hjkjk
hjk
哎，我可真蠢
今天才发现这么好用的注释方法，之前的代码真是弱智极了。
读文本的时候那个eval太厉害了
将字符串str当成有效的表达式来求值并返回计算结果。
可以把list,tuple,dict和string相互转化
'''
import xlwt

with open('Student.txt', 'r') as f:
    content = f.read()

dic = eval(content)

#创建工作簿
file = xlwt.Workbook()
#创建sheet
table = file.add_sheet('Test', cell_overwrite_ok=True)

#write在第几行第几列写入str
def deal(key, value):
    table.write(int(key) - 1, 0, key)
    for x in range(len(value)):
        table.write(int(key) - 1, x + 1, str(value[x]).decode('gbk'))

for key, value in dic.items():
    deal(key, value)

file.save('result0014.xls')

with open('numbers.txt') as f:
    content = f.read()
    lists = eval(content)

file = xlwt.Workbook()
table = file.add_sheet('Test',cell_overwrite_ok=True)

for row in range(len(lists)):
    for col in range(len(lists[row])):
        table.write(row, col, lists[row][col])
#列表的遍历，用index真的好方便，内存循环踩了个坑，要改用row而不是l作为索引对象
    # for row in l:
    #     for col in row:
    #         table.write(l.index(row),row.index(col),col) 
    #         # row表示行，col表示列，后者的英文不一定匹配，
    #         # list.index()可以得到列表中对于元素的索引值
file.save('result0016.xls')