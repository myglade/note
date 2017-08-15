'''
Created on Apr 9, 2014

@author: hekim
'''
import Format


def test(err1, err2):
    err1("error1")
 #   err2("error2")
    
if __name__ == '__main__':
    a = Format.TestError
    a = Format.NullFmt
    
    try:
        test(a, None)
    except Format.TestError:
        print "db error......."
    except:
        print "any"
    print "done"