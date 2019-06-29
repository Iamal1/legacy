#coding:utf-8

from bs4 import BeautifulSoup
import urllib2
from urlparse import urljoin

# url = 'http://www.fzdm.com/manhua/002'
url ='http://www.baidu.com'
request = urllib2.Request(url)
response = urllib2.urlopen(request, timeout=20)

content = response.read()
soup = BeautifulSoup(content, 'html.parser')
# div1= soup.find_all('div',id='content')[0]
# newest_li = div1.find('li')
# txt= newest_li.string.encode('utf-8')
# print type(txt),txt
#输出有问题，但是是sublime的问题
# a = newest_li.find_all('a')[0]
# ahref = a['href']
# print ahref
# b=urljoin(url,ahref)
# print b
# c =url+'/'+ahref
# print c
#urljoin路径问题