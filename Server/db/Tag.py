import sqlite3 as lite
import collections
import os, sys
lib_path = os.path.abspath(os.path.join('Util'))
sys.path.append(lib_path)
import Format
from OrderedTable import OrderedTable
from Db import *

class Tag(OrderedTable):
    """description of class"""


