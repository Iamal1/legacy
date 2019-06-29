# coding: utf-8
'''
准备用beatifulsoup先开始做一些尝试
scrapy放在后面有时间在学
'''

# from urllib.request import urlopen
from bs4 import BeautifulSoup
import re
html = """
<html><head><title>The Dormouse's story</title></head>
<body>
<p class="title" name="dromouse"><b>The Dormouse's story</b></p>
<p class="story">Once upon a time there were three little sisters; and their names were
<a href="http://example.com/elsie" class="sister" id="link1"><!-- Elsie --></a>,
<a href="http://example.com/lacie" class="sister" id="link2">Lacie</a> and
<a href="http://example.com/tillie" class="sister" id="link3">Tillie</a>;
and they lived at the bottom of a well.</p>
<p class="story">...</p>
"""
soup = BeautifulSoup(html,'html.parser')

# print soup.prettify()
# tag:
# print soup.name
# print soup.head.name
# print soup.p.attrs
# print soup.p['class']
# print soup.p.get('class')

#navigablestring:
# print soup.p.string
# print type(soup.p.string)

#beautifulsoup
# print type(soup.name)
# #<type 'unicode'>
# print soup.name 
# # [document]
# print soup.attrs 
# #{} 空字典

#comment
# 基本同上，不打了

#遍历
# print soup.head.string
# print soup.title.string
# strings,stripped_strings
# .parent  .parents
# sibling next previous
#next/previous element不分兄弟关系

#搜索文档树
# print soup.find_all(id='link1')
# #关键字加下划线
# print soup.find_all('a',class_='sister')
# #有些tag属性在搜索不能使用,比如HTML5中的 data-* 属性
## 但是可以通过 find_all() 方法的 attrs 参数定义一个字典参数来搜索包含特殊属性的tag
#compile匹配包含的
# print soup.find_all(text=re.compile('Dormouse'))

#select CSS 选择器