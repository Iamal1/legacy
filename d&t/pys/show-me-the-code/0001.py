# -*- coding: utf-8 -*-
# 第 0001 题：做为 Apple Store App 独立开发者，你要搞限时促销，
# 为你的应用生成激活码（或者优惠券），使用 Python 如何生成 200 个激活码（或者优惠券）？

import random, string
import uuid


def rand_str(num, length = 7):
	f = open('random_string.txt', 'wb')
	for i in range(num):
		chars = string.letters + string.digits
		s = [random.choice(chars) for i in range(length)]
		#f.write(''.join(s) + '\n') 
		#windows 下用\r\n，写入最好用wb二进制
		f.write(''.join(s) + '\r\n')
	f.close()

# random.choice()可以从任何序列，比如list列表中，选取一个随机的元素返回，可以用于字符串、列表、元组等。


def uuid_str(num):
	f = open('uuid.txt','wb')
	for i in range(num):
		f.write(str(uuid.uuid1())+"\r\n")

	f.close()
# # 生成基于计算机主机ID和当前时间的UUID
# >>> uuid.uuid1()
# # 基于命名空间和一个字符的MD5加密的UUID
# >>> uuid.uuid3(uuid.NAMESPACE_DNS, 'python.org')
# # 随机生成一个UUID
# >>> uuid.uuid4()
# # 基于命名空间和一个字符的SHA-1加密的UUID
# >>> uuid.uuid5(uuid.NAMESPACE_DNS, 'python.org')
# uuid2()——基于分布式计算环境DCE（Python中没有这个函数）
# 首先，Python中没有基于DCE的，所以uuid2可以忽略；
# 其次，uuid4存在概率性重复，由无映射性，最好不用；

#===========一些其他方法
# chr(random.randrange(97, 97 + 26 + 1))
# ''.join(random.sample(string.ascii_lowercase,5))

if __name__ == '__main__':
    #rand_str(200,20)
	uuid_str(200)