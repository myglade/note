import sqlite3 as lite
import collections
import os, sys
lib_path = os.path.abspath(os.path.join('Util'))
sys.path.append(lib_path)
import Format
from OrderedTable import *
from Db import *

class Pages(OrderedTable):
    def __init__(self, sectionId):
        filter = "sectionId = %s" % sectionId
        OrderedTable.__init__(self, db, "page", filter)
        self.sectionId = sectionId

    def addItem(self, insertBefore, dict, err = Format.NullFmt):
        OrderedTable.addItem(self, insertBefore, dict, err)

