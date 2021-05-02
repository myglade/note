'''
Created on Apr 29, 2014

@author: hekim
'''

def import_class(cl):
    d = cl.rfind(".")
    classname = cl[d + 1:len(cl)]
    m = __import__(cl[0:d], globals(), locals(), [classname])
    return getattr(m, classname)

class NullFmt(object):
    def __init__(self, obj):
        print(obj)
    
    def __call__(self, obj):
        print(obj)
        
class ExceptionFmt(Exception):
    def __init__(self, *message, **kwarg):
        kwarg = message[1]
        if "first" in kwarg.keys():
            first = False
        else:
            first = True
            
        if first:
            module = self.__module__
            name = self.__class__.__name__
            klass = import_class(module + "." + name)
            raise klass(message[0], first=False)
        
        if len(message[0][0]) == 2:
            self.value = int(message[0][0][1])
            Exception.__init__(self, message[0][0][0])
        else:
            self.value = 0
            Exception.__init__(self, message[0][0])
            
        return

   
class TestError(ExceptionFmt):
    def __init__(self, *message, **kwarg):
        ExceptionFmt.__init__(self, message, kwarg)
        
    def __str__(self):
        return self.message
           
        
######
## Test

def test(err):
    obj = import_class("Format.TestError")
    a = obj("test...")
    b = TestError("test...")
    err(a)
    
if __name__ == '__main__':
    
    try:
        test(ExceptionFmt)
    except TestError:
        print("db error")

    print("done")