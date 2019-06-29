#-*-coding:UTF-8-*-
import chardet


# s.decode(‘GBK’)，表示s是一个编码后的字节流，现在要把它使用GBK标准进行解码得到Unicode对象。
# 如果s本身的编码方式不是GBK，则可能会产生错误。注意不要对一个Unicode对象调用decode方法。
# 另外，使用a = s.decode(‘GBK’)和使用a = unicode(s, ‘GBK’)效果一样。
# s.encode(‘GBK’)，表示s是一个Unicode对象，现在用GBK标准把它编码成字节流。
# 注意不要对一个str字符串调用encode方法，因为其本身已经经过了编码。

# #------俄语测试

# s1="сегодня очен рад"
# print chardet.detect(s1)
# print s1,' | len: ',len(s1)
# s2 = s1.decode('utf-8')
# print "decoded: ", s2
# print "encoded: ", s2.encode('utf-8')

# #------日语测试
# s3 = "今日はとてもうれしいです"
# print chardet.detect(s3)
# print s3,' | len: ',len(s3)
# s4 = s3.decode('utf-8')
# print "decoded: ", s4
# print "encoded: ", s4.encode('utf-8')
#=================在本代码中已声明utf8编码方式，所以以上chardet测试一般都是utf8
#=================但是在控制台，shell中，
#=================输入数据的格式取决于所用shell终端的编码设置
#=================一个字符串的编码是根据系统默认的，windows系统对该字符串
#=================有自己的解释，之所以在shell和这里不一样是因为字符的默认编码不同，
#=================print repr(s)可以看到字节流
#=================>>> s = '你好'
#=================>>> s
#================='\xc4\xe3\xba\xc3'
#=================>>> x= u'\xc4\xe3\xba\xc3'
#=================>>> x
#=================u'\xc4\xe3\xba\xc3'
#=================>>> print x
#=================ÄãºÃ
#=================Unsupported characters in input 还没解决这个在idle出现的问题
#=================text='Iàäï†n$§&0ñŒ≥Q¶µù`o¢y—œº'
#=================2016/12/3 updated: 猜想idle gbk字符库不含某些字符。但utf8支持，试了其中个别字符暂时是这样
#=================之后的测试也验证了同样的东西，对于一个字符s='你好'
#=================处理得到可以得到她的unicode，utf8,gbk等字节流表示，
#=================但是在不同环境下print结果不一样
#=================因为sublime和idle编码设置不同，例如：y = '\xe4\xbd\xa0\xe5\xa5\xbd' print y
#=================在sub可以直接把utf8的字节流输出，但是idle里面会出现乱码 
#=================所以有些地方结果会有区别
#=================还有个问题是，强制转码后输出不能显示。以及多处输出之间的影响。
#=================2016/12/3 updated:不能显示大致理解了，乱码显示是一样的原理。
#=================2016/12/3 updated:有时候在IDLE  u'中文'会出现u'\x--\x--\x--\x--'这种，然后unicode无法识别的，转utf8q全是乱码错字
# ------中文
# str0="学习python中文"
# print chardet.detect(str0)
# ##说明原字符串是utf8，不用在意了

# # str1 = u'学习python中文'
# # # print str1
# # #str1 此时是unicode(多半utf16大端)但是print 还是按默认编码输出
# # str2 = str1.encode('gbk')
# #encode gbk 自然会出错    ѧϰpython
# print 'str2',str2, ' | len: ',len(str2)
# #这里输出会影响后面的输出显示，原因还未弄清楚
##print 'str2', ' | len: ',len(str2)
##str2  | len:  14    是有len的但是str2无法在下面显示出来
# print chardet.detect(str2)
###可以看到这个chardet并不一定准，还要看confidence的。
###他居然说{'confidence': 0.5119798157077455, 'encoding': 'KOI8-R'}
## >>> str2
## '\xd1\xa7\xcf\xb0python\xd6\xd0\xce\xc4'
## >>> print str2
## 学习python中文
# 在idle可以正常打印显示，很迷
# ustr1=u'\u5b66\u4e60python'
# ustr2 = u'\u0467\u03f0python'
# print ustr1," | ",ustr2 
# xstr1 = '\xd1\xa7\xcf\xb0python\xd6\xd0\xce\xc4'
# print xstr1
##有错，不能显示和输出 print xstr1.decode('gbk')就正确了，所以还是编码的问题。
##在utf8设定把gbk格式当作utf8的编码来编，然后出了问题他不能理解吧
###----更新之前的想法，不是来编，是把（gbk编码的）字节流对应到字符这个过程用的是UTF8的规则。

# # ------尝试强制编码并输出，可惜并不会输出，结论在最后。
# s = '你好'
# print repr(s), s
# u = u'你好'
# print repr(u),u
# x= s.decode('utf-8').encode('gbk')
# # print repr(x), x
# print len(x)
# q = '我好恨啊'
# print repr(q), q
# w='\xc4\xe3\xba'
# w2='\xe8\x9b\xa4'
# #w不能输出，w2可以输出
# #w在IDLE用UTF-8 DECODE会报错error。感觉不是乱码，而是没有这个东西，所以print才会没有,
# #但是又不属于异常
# # e=w.encode('utf-8','ignore')
# # print e
# # UnicodeDecodeError: 'ascii' codec can't decode byte 0xc4 in position 0: ordinal not in range(128)

#以上是没错的
# print ('GBK %s %d'%(u.encode('GBK'),len(u.encode('GBK'))))
###贼tmd的奇怪，下面这个跑三次结果还不一样，要么输出4，要么gbk，要么什么都没有。
###不知道他这个输出什么原理
##print 'GBK ',u.encode('GBK'),len(u.encode('GBK'))
# # print 'UTF8 ',u.encode('UTF-8')
# #print 'UTF16 ',len(u.encode('UTF-16'))
# # 好奇怪啊，单独运行每一行都可以，虽然只有utf8是对的，其他的都没有内容，这个输出会相互影响？
#================现在结论是其实他已经按照要求encode成字节流了，但是这个编译器会全部当作utf8去解读
#================但他解释不了gbk的和utf16的字符流