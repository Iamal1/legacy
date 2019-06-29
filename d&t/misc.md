### develop and test
d&t
时间是2016年年末-2017年年初

当时心血来潮在包图自习，找了个python的练手教程
应该就叫：show me the code
跟着玩了一下
很多功能
还有很多心得，都在readme的txt里面

---
### C++
一个特别简单的移除字符串
还有一个MT不知道是做啥的，算了真，想不起来了，有可能是dip课上的东西

---
### pys
我印象深刻的是当时研究了很久的encoding-decoding转换
现在都忘得差不多了
还为此改了签名。“我好恨啊”---“xxxxxx”很非主流的六个字。就是utf8和gbk用到的字节数不一样，一个是两个，一个是三个
我粘贴一下吧
```
test是针对文件编码的测试，保存的编码方式和code里面声明的coding方式
文件里的字符会按照保存方式编码保存，并用声明的coding去解释
一般字符串直接就是字节流了
u‘’字符串要根据声明的编码去解码为unicode保存， 所以不匹配时会出错
保存为utf-8声明gbk会出现乱码
反之会报错，cannot decode

后面的是用gbk去解读utf-8编码，
gbk版本适合idle
utf8版本适合sublime。但是需要手动复制输入，不支持input
```
但是，这都是古老的python2的玩法了，python3又改了（知识点忘记）

 [ ] python2和3字符串编码的差别 
 - py2有str和unicode，用u' '表示
- py3 默认用unicode，另外的有一种字符串类型是byte

来了老弟
```
a='我好恨啊'
>>> type(a)
<type 'str'>
>>> b=u'我好恨啊'
>>> type(b)
<type 'unicode'>
>>> repr(a)
"'\\xce\\xd2\\xba\\xc3\\xba\\xde\\xb0\\xa1'"
>>> repr(b)
"u'\\u6211\\u597d\\u6068\\u554a'"
>>> s=a.decode('utf-8')
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
  File "C:\Program Files (x86)\Anaconda2\lib\encodings\utf_8.py", line 16, in decode
    return codecs.utf_8_decode(input, errors, True)
UnicodeDecodeError: 'utf8' codec can't decode byte 0xce in position 0: invalid continuation byte
>>> s=a.encode('utf-8')
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
UnicodeDecodeError: 'ascii' codec can't decode byte 0xce in position 0: ordinal not in range(128)

>>> s=a.decode('gbk')
>>> s
u'\u6211\u597d\u6068\u554a'
>>> s_utf8=s.encode('utf-8')
>>> repr(s_utf8)
"'\\xe6\\x88\\x91\\xe5\\xa5\\xbd\\xe6\\x81\\xa8\\xe5\\x95\\x8a'"
>>> u_gbk_d= s_utf8.decode('gbk')
>>> print(u_gbk_d)
鎴戝ソ鎭ㄥ晩
```
gbk-2个字节
utf8-3个字节
完美复现，开心
```
>>> a='我好菜啊'
>>> a
'\xce\xd2\xba\xc3\xb2\xcb\xb0\xa1'
>>> a.decode('gbk').encode('utf-8')
'\xe6\x88\x91\xe5\xa5\xbd\xe8\x8f\x9c\xe5\x95\x8a'
>>> b=a.decode('gbk').encode('utf-8').decode('gbk')
>>> b
u'\u93b4\u621d\u30bd\u947f\u6ec3\u6669'
>>> print(b)
鎴戝ソ鑿滃晩
```
长度满足条件的限制下，len(s)*3/2是整数，就是只要原来是偶数个。是否一定可以gbk解码utf8，但是gbk无法解码。好了，我知道了，并不是的，那还挺巧，只有一部分重合。

剩下的，玩了下tk，crawler，还有专门的showme the code
还有个smtp写邮件的，要邮箱那边给开个门。

### NLP
我找到我nlp作业了
最大熵模型

还有那个文本分类的
当时我记得我说过，人生要是没有bsl就好了
这作业我还帮助了几个同学呢，都是刚好过线

- keras
- word2vec模型，就是glove当时下不下来
就是
- 隐约记得试过两种方法，一个是句子弄成2维的当图片，一个是lstm，用embedding。...但是check发现我好像是，卷积，MLP，LSTM我都用了，还
调试了一堆参数，包括词向量的表示，训练词向量要怎么做，维度多少，然后模型用什么参数，什么优化器。
d5是模型文件，model是预训练的词向量模型
语料库是很小的text8
100d和200d关系不是线性的，不懂
bsl在0.38左右，现在看来这个东西是真的简单，好小的模型啊，就几行代码定义完了。


D盘清理接近尾声


