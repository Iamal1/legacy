#-*-coding: GBK-*-
#run in idle not here 
#sublime ����֧��input ��Ҫ���
#��idle��Ҳ��������input������SyntaxError: invalid syntax
#x= input("sss:")
#y= unicode(x,'gbk')
#����ֱ�����򵥴ֱ��İ�
y=u'��ð���'
c=y.encode('utf-8')
print c
