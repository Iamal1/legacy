#-*-coding: UTF-8-*-
#run in idle not here 
#sublime 不能支持input 需要插件
################输入只能是双数个字
x='你好'
y= unicode(x,'utf-8')
print repr(x)
#需要手动替换 
#c ='x'.join(repr(x).split("x"))没用的
c= '\xe4\xbd\xa0\xe5\xa5\xbd'
print c
v=c.decode('gbk')
# print v
b = v.encode('utf-8')
#要print b就不会显示，什么问题啊
print repr(b),b
