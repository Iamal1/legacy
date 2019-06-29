# -*- coding: UTF-8 -*- 
from Tkinter import *
import MySQLdb
import tkFont

root = Tk()
#root.geometry('500x300')

def hello():
	print 'hello menu'

def getres(sss):
	print(sss)

# def q1():
# 	tl = Toplevel()
# 	tl.title('q1')
# 	Label(tl,text = 'Disp: given a name, find the entity').pack()
# 	Label(tl,text = 'input your text here').pack()
# 	e = StringVar()
# 	entry = Entry(tl,textvariable = e)
# 	e.set('')
# 	entry.pack()
# 	btn = Button(tl,text = 'query',relief=SOLID,command = lambda:getres(entry.get())).pack()
class scrolltxt(Frame):
	def __init__(self, master,desp):
		Frame.__init__(self, master)
		self.grid(row=0, column=0, sticky="nsew")
		self.value=[]
		frame_center = LabelFrame(self, text="This is Q&A system")
		frame_center.pack(fill="x")
		query_field = LabelFrame(frame_center,text='query')
		query_field.pack(fill="y")
		self.q_lab1 = Label(query_field,text="what is")  
		self.q_lab1.pack(fill="none", expand=0, side=LEFT, anchor=SE)
		self.q_ent = Entry(query_field)  
		self.q_ent.pack(fill="none", expand=0, side=LEFT, anchor=SE)
		self.q_lab2 = Label(query_field,text="what is")  
		self.q_lab2.pack(fill="none", expand=0, side=LEFT, anchor=SE)
		self.q_ent2 = Entry(query_field)  
		self.q_ent2.pack(fill="none", expand=0, side=LEFT, anchor=SE)
		self.q_btn = Button(query_field, text="query", width=10, height=1, command=self.getres)
		self.q_btn.pack(fill="none", expand=0, side=LEFT, anchor=SE)

		lfc = LabelFrame(frame_center)
		lfc.pack(fill="x")
		self.lfc_l = Label(lfc, text=desp, width=15,wraplength = 130,justify = 'left',anchor = 'nw')

		self.lfc_l.pack(fill="none", expand=0, side=LEFT)
		

		##########文本框与滚动条
		self.lfc_t_sv = Scrollbar(lfc, orient=VERTICAL)  #文本框-竖向滚动条  
		self.lfc_t_sh = Scrollbar(lfc, orient=HORIZONTAL)  #文本框-横向滚动条
		self.lfc_t = Text(lfc, height=15, yscrollcommand=self.lfc_t_sv.set,  
                                          xscrollcommand=self.lfc_t_sh.set, wrap='none')  #设置滚动条-不换行

		#滚动事件
		self.lfc_t_sv.config(command=self.lfc_t.yview)
		self.lfc_t_sh.config(command=self.lfc_t.xview)

		#布局
		self.lfc_t_sv.pack(fill="y", expand=0, side=RIGHT, anchor=N)
		self.lfc_t_sh.pack(fill="x", expand=0, side=BOTTOM, anchor=N)
		self.lfc_t.pack(fill="x", expand=1, side=LEFT)
		

	# def getres(self):
	# 	aaa = self.q_ent.get().encode('utf-8')

	# 	if aaa[0]=='a':
	# 		print(str(self.value))
	# 		popup(self.value)

	# 	else:
	# 		if aaa:
	# 			print(aaa)
	# 		else :
	# 			print("no")

class BQue(Frame):
	def __init__(self, master,desp):
		Frame.__init__(self, master)
		self.grid(row=0, column=0, sticky="nsew")
		frame_center = LabelFrame(self, text="This is requirement 1")

		frame_center.pack(fill="x")
		query_field = LabelFrame(frame_center,text='ggggggg')
		query_field.pack(fill="y")
		self.q_lab1 = Label(query_field,text="what is")  
		self.q_lab1.pack(fill="none", expand=0, side=LEFT, anchor=SE)
		self.q_ent = Entry(query_field)  
		self.q_ent.pack(fill="none", expand=0, side=LEFT, anchor=SE)
		self.q_btn = Button(query_field, text="query", width=10, height=1, command=self.getres)
		self.q_btn.pack(fill="none", expand=0, side=LEFT, anchor=SE)

		lfc = LabelFrame(frame_center)
		lfc.pack(fill="x")
		self.lfc_l = Label(lfc, text=desp, width=20,wraplength = 180,justify = 'left',anchor = 'w')

		self.lfc_l.pack(fill="none", expand=0, side=LEFT)
		

		##########文本框与滚动条
		self.lfc_t_sv = Scrollbar(lfc, orient=VERTICAL)  #文本框-竖向滚动条  
		self.lfc_t_sh = Scrollbar(lfc, orient=HORIZONTAL)  #文本框-横向滚动条
		self.lfc_t = Text(lfc, height=15, yscrollcommand=self.lfc_t_sv.set,  
                                          xscrollcommand=self.lfc_t_sh.set, wrap='none')  #设置滚动条-不换行

		#滚动事件
		self.lfc_t_sv.config(command=self.lfc_t.yview)
		self.lfc_t_sh.config(command=self.lfc_t.xview)

		#布局
		self.lfc_t_sv.pack(fill="y", expand=0, side=RIGHT, anchor=N)
		self.lfc_t_sh.pack(fill="x", expand=0, side=BOTTOM, anchor=N)
		self.lfc_t.pack(fill="x", expand=1, side=LEFT)
	

	def getres(self):
		aaa = self.q_ent.get().encode('utf-8')
		if aaa:
			print(aaa)
		else :
			print("no")
		for i in range (30):
			self.lfc_t.insert(1.0,'111\n')


class popup(Toplevel):
	def __init__(self,value):
		Toplevel.__init__(self)
		
		self.str = 'invalid input'
		self.lab = Label(self,text = 'specify more details,such as time')
		self.lab.grid(row = 0,column = 0)
		self.ent = Entry(self)
		self.ent.grid(row = 1,column = 0,sticky = W)
		self.btn = Button(self,text = 'query again',relief=SOLID,command = lambda:self.queryag(value))
		self.btn.grid(row = 1,column = 1,sticky = W)
        def queryag(self,value):
			value.append(self.ent.get().encode('utf-8'))
		# if aaa:
		# 	print(aaa)
		# else :
		# 	print("no"
			self.destroy()
			

# class BQue(Frame):  
#     def __init__(self,master):  
#         frame = Frame(master)  
#         frame.grid()
#         self.lab = Label(frame,text = 'Disp:   given a name, find the entity')
#         self.lab.grid(row = 0,column = 0)  
#         self.ent = Entry(frame)  
#         self.ent.grid(row = 1,column = 0,sticky = W)  
#         self.btn = Button(frame,text = 'query',relief=SOLID,command = self.getres)
#         self.btn.grid(row = 1,column = 1,sticky = W)  
#         # self.lab = Label(frame,text = 'res').grid(row = 1,column = 0,sticky = W) 
#         self.txt = Text(frame,width = 35,height = 5,wrap = WORD)  
#         self.txt.grid(row = 4,column = 0,columnspan = 2,sticky = W)  
#     def getres(self):
#     	aaa = self.ent.get().encode('utf-8')
#         #sql2 = sql1%(aaa,aaa)
#         if aaa:
#         	print(aaa)
#         else :
#         	print("no")
#         # self.lfc_t.delete(0.0,END) 
def about():
	tl = Toplevel()
	tl.title('help')
	ft = tkFont.Font(family = 'Fixdsys',size = 20,weight = tkFont.BOLD)

	Label(tl,text = 'wikidata',fg = 'gray',font= ft,compound = 'center',image = img1).pack(fill = BOTH,expand = 0)


def q1():
	tl = Toplevel()
	tl.title('q1')
	desp = "description：\
	given a name return the entities match the name"
	app = BQue(tl,desp)

def q2():
	tl = Toplevel()
	tl.title('q2')
	desp = "description：\
	given an entity return all preceding categories it belongs to"
	app = BQue(tl,desp)

def q3():
	tl = Toplevel()
	tl.title('q3')
	desp = "description：\
	given an entity return all entities co-ocurred with this entity in one satement"
	app = BQue(tl,desp)

def q4():
	tl = Toplevel()
	tl.title('q4')
	desp = "description：\
	given an entity return all proprties and statements it possesses"
	app = BQue(tl,desp)

def qa1():
	tl = Toplevel()
	tl.title('q&a1')
	desp = "description：\
	given an entity return all proprties and statements it possesses"
	app = scrolltxt(tl,desp)
def qa2():
	app =[]
	popup(app)
	for i in range(0,len(app)):
		print(app[i])
#菜单    
menubar = Menu(root)

basicmenu = Menu(menubar,tearoff = 0)
basicmenu.add_command(label = "Q1",command = q1)
basicmenu.add_command(label = "Q2",command = q2)
basicmenu.add_command(label = "Q3",command = q3)
basicmenu.add_command(label = "Q4",command = q4)
#将menubar的menu属性指定为basicmenu，即basicmenu为menubar的下拉菜单
menubar.add_cascade(label = 'Basic Question',menu = basicmenu)

qamenu = Menu(menubar,tearoff = 0)
qamenu.add_command(label = "Q1",command = qa1)
qamenu.add_command(label = "Q2",command = qa2)
qamenu.add_command(label = "Q3",command = hello)
qamenu.add_command(label = "Q4",command = hello)
menubar.add_cascade(label = 'Q&A',menu = qamenu)

helpmenu = Menu(menubar, tearoff = 0)
helpmenu.add_command(label = "About", command = about)
menubar.add_cascade(label = "Help", menu = helpmenu)
root['menu'] = menubar

#背景
ft = tkFont.Font(family = 'Fixdsys',size = 30,weight = tkFont.BOLD)
img = PhotoImage(file='C:\Users\lethever\Desktop\shoyu\\a5.gif')
img1 = PhotoImage(file='C:\Users\lethever\Desktop\shoyu\\a3.gif')
Label(root,text = 'wikidata',fg = 'gray',font= ft,compound = 'center',image = img).pack(fill = BOTH,expand = 1)
 
# Button(root,text = 'hello button1',relief=FLAT).pack()
# Button(root,text = 'hello button2',relief=GROOVE).pack()
# Button(root,text = 'hello butto3n',relief=RAISED).pack()
# Button(root,text = 'hello butt4on',relief=RIDGE).pack()
# Button(root,text = 'hello but5ton',relief=SOLID).pack()
# Button(root,text = 'hello but6ton',relief=SUNKEN).pack()
root.title('wikidata gui')
root.mainloop()



