# coding:utf-8
'''
with有强大而方便的功能
with是从Python 2.5 引入的一个新的语法，更准确的说，是一种上下文的管理协议，
用于简化try…except…finally的处理流程。
有一些任务，可能事先需要设置，事后做清理工作。
对于这种场景，Python的with语句提供了一种非常方便的处理方式。
一个很好的例子是文件处理，你需要获取一个文件句柄，从文件中读取数据，然后关闭文件句柄。
基本思想是with所求值的对象必须有一个__enter__()方法，一个__exit__()方法。
还可以处理（接受）异常
对资源进行访问的场合，确保不管使用过程中是否发生异常都会执行必要的“清理”操作，释放资源，
比如文件使用后自动关闭、线程中锁的自动获取和释放等。
还涉及到上下文管理这个东西，暂时没看了。
2016/12/8
'''

class Sample:
    def __enter__(self):
        return self

    def __exit__(self, type, value, trace):
        print "type:", type
        print "value:", value
        print "trace:", trace

    def do_something(self):
        bar = 1/0
        return bar + 10

with Sample() as sample:
    sample.do_something()