import sqlite3 as lite
import collections
import os, sys
lib_path = os.path.abspath(os.path.join('Util'))
sys.path.append(lib_path)
import Format
from OrderedTable import OrderedTable
from Db import *

class Sections(OrderedTable):
    """description of class"""
    def __init__(self, db):
        OrderedTable.__init__(self, db, "section")


def prepare_reorder(sections):
    sections.removeAll()
    r = sections.getItems()

    item = { "name" : "section 0" }     
    sections.addItem(-1, item, Format.TestError)
    item = { "name" : "section 1" }
    sections.addItem(-1, item, Format.TestError)
    item = { "name" : "section 2" }     
    sections.addItem(-1, item, Format.TestError)
    item = { "name" : "section 3" }
    sections.addItem(-1, item, Format.TestError)
    item = { "name" : "section 4" }     
    sections.addItem(-1, item, Format.TestError)
    item = { "name" : "section 5" }
    sections.addItem(-1, item, Format.TestError)

    r = sections.getItems()
    ex = [  {'id': 0, 'name': u'section 0', 'seqId': 0}, \
            {'id': 1, 'name': u'section 1', 'seqId': 1}, \
            {'id': 2, 'name': u'section 2', 'seqId': 2}, \
            {'id': 3, 'name': u'section 3', 'seqId': 3}, \
            {'id': 4, 'name': u'section 4', 'seqId': 4}, \
            {'id': 5, 'name': u'section 5', 'seqId': 5}]	
    assert (r == ex), "mismatch"


def test_section(db):
    sections = Sections(db)

    try:
        item = { "name" : "section 1" }
        sections.addItem(-1, item, Format.TestError)
        item = { "name" : "section\' 2" }
        sections.addItem(-1, item, Format.TestError)
        item = { "name" : "section 3" }
        sections.addItem(-1, item, Format.TestError)
        item = { "name" : "section 0" }
        sections.addItem(0, item, Format.TestError)
        item = { "name" : "section 4" }
        sections.addItem(100, item, Format.TestError)
    except:
        print sys.exc_info()
  
    r = sections.getItems()
    ex = [  {'id': 3, 'name': u'section 0', 'seqId': 0}, \
            {'id': 0, 'name': u'section 1', 'seqId': 1}, \
            {'id': 1, 'name': u"section' 2", 'seqId': 2}, \
            {'id': 2, 'name': u'section 3', 'seqId': 3}, \
            {'id': 4, 'name': u'section 4', 'seqId': 4}]	
    assert (r == ex), "mismatch"

    r = sections.getByCol("id", "10")
    ex = []
    assert (r == ex), "mismatch"

    r = sections.getByCol("id", "0")
    ex = [{'id': 0, 'name': u'section 1', 'seqId': 1}]
    assert (r == ex), "mismatch"

    sections.deleteItem(2)
    r = sections.getItems()
    ex = [  {'id': 3, 'name': u'section 0', 'seqId': 0}, \
            {'id': 0, 'name': u'section 1', 'seqId': 1}, \
            {'id': 1, 'name': u"section' 2", 'seqId': 2}, \
            {'id': 4, 'name': u'section 4', 'seqId': 3}]	
    assert (r == ex), "mismatch"

    dict = {'id':1234,'name':"section 5"}
    sections.updateItem(1, dict)
    r = sections.getItems()
    ex = [  {'id': 3, 'name': u'section 0', 'seqId': 0}, \
            {'id': 0, 'name': u'section 1', 'seqId': 1}, \
            {'id': 1, 'name': u'section 5', 'seqId': 2}, \
            {'id': 4, 'name': u'section 4', 'seqId': 3}]	
    assert (r == ex), "mismatch"


    #############################################################
    ## reorder test
    '''
    ex = [  {'id': 0, 'name': u'section 0', 'seqId': 0}, \
            {'id': 1, 'name': u'section 1', 'seqId': 1}, \
            {'id': 2, 'name': u'section 2', 'seqId': 2}, \
            {'id': 3, 'name': u'section 3', 'seqId': 3}, \
            {'id': 4, 'name': u'section 4', 'seqId': 4}, \
            {'id': 5, 'name': u'section 5', 'seqId': 5}]	
    '''
    
    prepare_reorder(sections)
    # move seqId 3 to 0 pos
    list = [3]	
    sections.reorderItem(list, 0)
    r = sections.getItems()
    ex = [  {'id': 3, 'name': u'section 3', 'seqId': 0}, \
            {'id': 0, 'name': u'section 0', 'seqId': 1}, \
            {'id': 1, 'name': u'section 1', 'seqId': 2}, \
            {'id': 2, 'name': u'section 2', 'seqId': 3}, \
            {'id': 4, 'name': u'section 4', 'seqId': 4}, \
            {'id': 5, 'name': u'section 5', 'seqId': 5}]	
    assert (r == ex), "mismatch"

    prepare_reorder(sections)
    list = [2, 4]	
    sections.reorderItem(list, 0)
    r = sections.getItems()
    ex = [  {'id': 2, 'name': u'section 2', 'seqId': 0}, \
            {'id': 4, 'name': u'section 4', 'seqId': 1}, \
            {'id': 0, 'name': u'section 0', 'seqId': 2}, \
            {'id': 1, 'name': u'section 1', 'seqId': 3}, \
            {'id': 3, 'name': u'section 3', 'seqId': 4}, \
            {'id': 5, 'name': u'section 5', 'seqId': 5}]	
    assert (r == ex), "mismatch"

    prepare_reorder(sections)
    list = [2, 4]	
    sections.reorderItem(list, 0)
    r = sections.getItems()
    ex = [  {'id': 2, 'name': u'section 2', 'seqId': 0}, \
            {'id': 4, 'name': u'section 4', 'seqId': 1}, \
            {'id': 0, 'name': u'section 0', 'seqId': 2}, \
            {'id': 1, 'name': u'section 1', 'seqId': 3}, \
            {'id': 3, 'name': u'section 3', 'seqId': 4}, \
            {'id': 5, 'name': u'section 5', 'seqId': 5}]	
    assert (r == ex), "mismatch"

    prepare_reorder(sections)
    list = [2, 4]	
    sections.reorderItem(list, 1)
    r = sections.getItems()
    ex = [  {'id': 0, 'name': u'section 0', 'seqId': 0}, \
            {'id': 2, 'name': u'section 2', 'seqId': 1}, \
            {'id': 4, 'name': u'section 4', 'seqId': 2}, \
            {'id': 1, 'name': u'section 1', 'seqId': 3}, \
            {'id': 3, 'name': u'section 3', 'seqId': 4}, \
            {'id': 5, 'name': u'section 5', 'seqId': 5}]	
    assert (r == ex), "mismatch"
    
    prepare_reorder(sections)
    list = [2, 4]	
    sections.reorderItem(list, -1)
    r = sections.getItems()
    ex = [  {'id': 0, 'name': u'section 0', 'seqId': 0}, \
            {'id': 1, 'name': u'section 1', 'seqId': 1}, \
            {'id': 3, 'name': u'section 3', 'seqId': 2}, \
            {'id': 5, 'name': u'section 5', 'seqId': 3}, \
            {'id': 2, 'name': u'section 2', 'seqId': 4}, \
            {'id': 4, 'name': u'section 4', 'seqId': 5}, \
            ]	
    assert (r == ex), "mismatch"
    
    prepare_reorder(sections)
    list = [2, 4]	
    sections.reorderItem(list, 6)
    r = sections.getItems()
    ex = [  {'id': 0, 'name': u'section 0', 'seqId': 0}, \
            {'id': 1, 'name': u'section 1', 'seqId': 1}, \
            {'id': 3, 'name': u'section 3', 'seqId': 2}, \
            {'id': 5, 'name': u'section 5', 'seqId': 3}, \
            {'id': 2, 'name': u'section 2', 'seqId': 4}, \
            {'id': 4, 'name': u'section 4', 'seqId': 5}, \
            ]	
    assert (r == ex), "mismatch"

    prepare_reorder(sections)
    list = [2, 4]	
    sections.reorderItem(list, 5)
    r = sections.getItems()
    ex = [  {'id': 0, 'name': u'section 0', 'seqId': 0}, \
            {'id': 1, 'name': u'section 1', 'seqId': 1}, \
            {'id': 3, 'name': u'section 3', 'seqId': 2}, \
            {'id': 2, 'name': u'section 2', 'seqId': 3}, \
            {'id': 4, 'name': u'section 4', 'seqId': 4}, \
            {'id': 5, 'name': u'section 5', 'seqId': 5}]	
    assert (r == ex), "mismatch"

    prepare_reorder(sections)
    list = [1,2,3,4,5]	
    sections.reorderItem(list, 5)
    r = sections.getItems()
    ex = [  {'id': 0, 'name': u'section 0', 'seqId': 0}, \
            {'id': 1, 'name': u'section 1', 'seqId': 1}, \
            {'id': 2, 'name': u'section 2', 'seqId': 2}, \
            {'id': 3, 'name': u'section 3', 'seqId': 3}, \
            {'id': 4, 'name': u'section 4', 'seqId': 4}, \
            {'id': 5, 'name': u'section 5', 'seqId': 5}]	
    assert (r == ex), "mismatch"

    prepare_reorder(sections)
    list = [1,2,3,4,5]	
    sections.reorderItem(list, 0)
    r = sections.getItems()
    ex = [  
            {'id': 1, 'name': u'section 1', 'seqId': 0}, \
            {'id': 2, 'name': u'section 2', 'seqId': 1}, \
            {'id': 3, 'name': u'section 3', 'seqId': 2}, \
            {'id': 4, 'name': u'section 4', 'seqId': 3}, \
            {'id': 5, 'name': u'section 5', 'seqId': 4}, \
            {'id': 0, 'name': u'section 0', 'seqId': 5}, \
            ]	
    assert (r == ex), "mismatch"

    prepare_reorder(sections)
    list = [0,1,2,3,4,5]	
    sections.reorderItem(list, 5)
    r = sections.getItems()
    ex = [  {'id': 0, 'name': u'section 0', 'seqId': 0}, \
            {'id': 1, 'name': u'section 1', 'seqId': 1}, \
            {'id': 2, 'name': u'section 2', 'seqId': 2}, \
            {'id': 3, 'name': u'section 3', 'seqId': 3}, \
            {'id': 4, 'name': u'section 4', 'seqId': 4}, \
            {'id': 5, 'name': u'section 5', 'seqId': 5}]	
    assert (r == ex), "mismatch"

    print "done"
    
    
if __name__ == '__main__':
    db = Db("test.db")
    db.open()
    db.resetTables()
    db.createTable()

    test_section(db)
        
    print "done"