# -*- coding: UTF-8 -*- 
from Tkinter import *
import MySQLdb
import tkFont
import time
import threading
from time import sleep, ctime
# 打开数据库连接

dbname = "wikidata"
db = MySQLdb.connect(host='10.189.104.29',user='root',passwd='12345678',db=dbname)
# 使用cursor()方法获取操作游标


sql2 = """SELECT Item_id as eid, Name
		FROM item
		WHERE Item_id  IN (SELECT P_id 
		FROM %s
		WHERE C_id = '%s')
		Union
		SELECT Pro_id as eid, Name
		FROM property
		WHERE Pro_id  IN (SELECT P_id 
		FROM %s
		WHERE C_id = '%s')
		"""
        
def runmysql(tablename,mysql,sid):
	print 'search in table:', tablename
	esql = mysql%(tablename,sid,tablename,sid)
	cursor = db.cursor()
	threadLock.acquire()
	try:
		cursor.execute(esql)
		results = cursor.fetchall()
		tresult.extend(results)
		cursor.close()
	except Exception,e:
		print Exception,":",e
	threadLock.release()
	print 'done the query', tablename

tresult = []
aaa= "p2016"
threadpool=[]
threadLock = threading.Lock()
for i in range(0,10):
	th = threading.Thread(target= runmysql,args= ('belong'+str(i),sql2,aaa))
	threadpool.append(th)

time1 = time.time()
for th in threadpool:
	th.start()
for th in threadpool :
	#th.join()
	threading.Thread.join( th )
# for i in range(0,10):
# # 	runmysql('belong'+str(i),sql2,aaa)

db.close()
time2 = time.time()
timecost = str(time2 - time1)
print timecost
if tresult:
	for row in tresult:
		it1 = row[0]
		it2 = row[1]
		print it1,' | ',it2
