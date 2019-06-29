#-*-coding: GBK-*-
#run in idle not here 
#sublime 不能支持input 需要插件
#在idle跑也报错，还是input的问题SyntaxError: invalid syntax
#x= input("sss:")
#y= unicode(x,'gbk')
#还是直接来简单粗暴的吧
y=u'你好啊啊'
c=y.encode('utf-8')
print c
