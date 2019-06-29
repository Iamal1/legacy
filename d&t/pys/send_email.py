# coding:utf-8
'''
输入授权码后成功发送
'''
import smtplib  
from email.mime.text import MIMEText
from email.header import Header


#设置服务器(我用的gmail，你们用的什么就换成什么
#比如qq，163～记得登录网页版邮箱然后在设置里面修改支持smtp
mail_host = 'smtp.qq.com'  # 设置发件服务器地址
mail_port = 25  # 设置发件服务器端口号。注意，这里有SSL和非SSL两种形式
mail_pwd="uxobtmuwrjztbejf" #口令,QQ邮箱是输入授权码，在qq邮箱设置 里用验证过的手机发送短信获得，不含空格  


sender = '944197468@qq.com'  # 设置发件邮箱
receiver = ['1398565205@qq.com'] # 设置邮件接收人，这里是我的公司邮箱



contents="test for python email"
# 三个参数：第一个为文本内容，第二个 plain 设置文本格式，第三个 utf-8 设置编码
message = MIMEText(contents, 'plain', 'utf-8')
message['From'] = Header("your dear father", 'utf-8')
message['To'] =  Header("my lorder", 'utf-8')

subject = 'Python SMTP 邮件测试'
message['Subject'] = Header(subject, 'utf-8')
try:
	# s = smtplib.SMTP(host, port)  # 注意！如果是使用SSL端口，这里就要改为SMTP_SSL
	smtpobj = smtplib.SMTP_SSL(mail_host, 465)
	#邮箱设置由于手机没电了暂无法修改,所以这个程序也暂时没法测试了
	#报错信息：请使用授权码登录。详情请看
	#但是端口号从25改到465就可以了？这什么情况
	#QQ邮箱的SMTP服务端口不是默认的25.改为465之后就好了
	smtpobj.set_debuglevel(1)
	smtpobj.login(sender, mail_pwd)  # 登陆邮箱
	smtpobj.sendmail(sender, receiver, message.as_string())  # 发送邮件！
	smtpobj.quit()
	print "succucess"
except smtplib.SMTPException,e:
	print ("Error: ",e)