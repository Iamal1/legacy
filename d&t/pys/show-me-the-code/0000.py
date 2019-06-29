# -*- coding: utf-8 -*-
"""
"""

from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont

img = Image.open('p000.png')
draw = ImageDraw.Draw(img)
font = ImageFont.truetype('arial.ttf',100)
#使用font 有The _imagingft C module is not installed错误
#在linux下用freetype什么的好像可以解决，但我暂时没有linux而且之前机器学习发现linux装东西也不是那么简单，还是有bug
#问题似乎出在便宜吧哦本来的问题，需要下别人编译好的，或者自己编译
#原因是那个版本太老了，2.1，不支持后面的，dll load不上
#在国内和国外网站上都找了包，暂时用的国内的一个PIL1.1.7，国外哪个whl还没试过，版本有点高
draw.text((img.size[0]-100,30),"1",(255,0,0),font= font)
del draw
img.save('result000.jpg')
img.show()