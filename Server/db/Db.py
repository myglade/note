'''
Created on Apr 9, 2014

@author: hekim
'''

import sqlite3 as lite
import collections
import Format
import sys

'''
TODO
* Reduce duplication codes.  : reorderSection reorderPage.
    Most codes are the same except for WHERE clause.  Page requires sectionId=''



execute parameter SUBSTITION PROBLEM

http://stackoverflow.com/questions/228912/sqlite-parameter-substitution-problem

'''

'''
1. structure

a. One database per one NOTE
b. One database
    * tables  : 
        1. env
        2. pages
        3. section
        4. tag
        5. tagList : pageId - tagId
    * image or object of HTML : save separately in local host
        folder structure : media - db name - section id - page id


c. page
    * One page : Conceptually 1 key - multiple contents
        physical structure :
            Page : 1 key, 1 content
            SubPage : 1 keyId, 1 content

    * Page structure
        id INTEGER PRIMARY KEY, 
        seqId INTEGER UNIQUE, 
        mtime INTEGER, 
        sectionId INTEGER, 
        level CHAR, 
        title BLOB, 
        content BLOB, 

d. content
    * structure
        id INTEGER PRIMARY KEY, 
        pageId INTEGER,
        index INTEGER,    # content index
        content BLOB

    * when a page is created, this content is not created by default
    * total number of content is variable
    * realign of index is not allowed


    Get Page : return all ordered pages that have page "id" by subIndex
    Create Page : subIndex should be provided
    Delete page : if subIndex == 0, remove all pages that have the same pageid
        

* page add
    1. if any, create directory of media : media/db/section_id/page_id
    2. create page data
    3. determine seqId
        a. get previous, next seqId in the same section
        b. if no gap between previous and next, reassign seqId from next
        c. set assign seqid
        
* Page delete
    1. delete directory of media :  media/db/section_id/page_id
    2. delete data from tagList
    3. delete page
 
    
* section delete
    1. get all pages of that section
    2. for all pages
        a. delete directory of media :  media/db/section_id/page_id
        b. delete data from tagList
    3. delete all pages from page table (if foreign is not defined)
    4. delete section
    

* order of section and tag
    * whenever user changes display order, reassign seqId of those tables
    
    
* Use OBJECT concept 

class Pages
class Tags
class Sections
class TextPage

'''
def dict_factory(cursor, row):
    d = {}
    for idx, col in enumerate(cursor.description):
        d[col[0]] = row[idx]
    return d


def dictToSqlStrForInsert(dict):
    k = ""
    v = ""

    for key, value in dict.iteritems():
        key = key.replace("'", "\''")
        value = value.replace("'", "\''")

        if k != "":
            k = k + ","
            v = v + ","
        k = k + "'" + key + "'"
        if type(value) is str:
            v = v + "'" + value + "'"
        else: 
            v = v + str(value)

    return (k, v)

def dictToSqlStrForUpdate(dict, exclude=["id", "seqId"]):
    s = ""

    for key, value in dict.iteritems():
        if key in exclude:
            continue
        key = key.replace("'", "\''")
        value = value.replace("'", "\''")

        if s != "":
            s = s + ","
        s = s + "'" + key + "'="
        if type(value) is str:
            s = s + "'" + value + "'"
        else: 
            s = s + str(value)

    return s

def toSqlStr(value):
    if type(value) is str:
        return "'" + value + "'"
    else: 
        return str(value)

            
class DbError(Format.ExceptionFmt):
    def __init__(self, *message, **kwarg):
        Format.ExceptionFmt.__init__(self, message, kwarg)
        
class Db(object):
    '''
    classdocs
    '''
    def _test(self):
        pass

    def __init__(self, name):
        '''
        Constructor
        '''
        self.name = name
        self.conn = None #sqlite3.connect(name) 
        self.isOpen = False
        
    def open(self):
        if self.isOpen:
            return
        
        self.conn = lite.connect(self.name)
        if self.conn == None:
            return False
        
        # sqlite returns result row as tuple based on index
        # With this option, dict can be created
        self.conn.row_factory = dict_factory

        if self.tableExists("env") == False:
            self.createEnvTable()
            self.createTable()
            
        self.isOpen = True
        return True
    
    def close(self):
        if self.isOpen == False:
            return
        
        self.conn.close()
        self.isOpen = False
    
    def tableExists(self, table):
        cur = self.conn.cursor()
        cur.execute("select count(*) from sqlite_master where type='table' and name='%s'" % table)
        row = cur.fetchone()
        if row == None or row.values()[0] == 0:
            return False
        
        return True
    
    def createEnvTable(self):
        cur = self.conn.cursor()
        cur.execute("CREATE TABLE env(key varchar(255) PRIMARY KEY, value varchar(255));")
        self.conn.commit()
        
    def createTable(self):
        # if one of tables exist, skip
        if self.tableExists("page"):
            return

        cur = self.conn.cursor()
        
        '''
        page, tag, section have the same structure in the starting part.   

        id : unique identifier
        seqId : ordered id.  It should be always recalculated when user data changes data order

        '''
        # id : unique id
        # titleType : dictionary or not (plain or html)  <==== XXXX
        # level : root 0. child += 1
        # mtime : last modify time
        # seqId : maintain ordered list
        # text_page TABLE : table for full text search
        # DO NOT USE Foreign key.  It becomes complicated.
        

        # id, seq is the common in orderedTable
        # UNIQUE can't be used at seqId.   Ideally, it's unique, but, it can't be unique during
        # UPDATE procedure.
        cur.executescript("""
            CREATE TABLE page(
                id INTEGER PRIMARY KEY, 
                seqId INTEGER, 
                mtime INTEGER, 
                sectionId INTEGER, 
                level CHAR, 
                title BLOB, 
                content BLOB, 
                FOREIGN KEY(sectionId) REFERENCES section(sectionId) ON UPDATE CASCADE ON DELETE CASCADE);
   
            CREATE TABLE page_content(
                id INTEGER PRIMARY KEY,
                pageId INTEGER,
                seqId INTEGER,
                content BLOB,
                UNIQUE(pageId, seqId)
            );

            CREATE VIRTUAL TABLE text_page USING fts4(title, content);
            
            CREATE TABLE tag(id INTEGER PRIMARY KEY, 
                             seqId INTEGER, 
                             name VARCHAR(255) UNIQUE COLLATE NOCASE);
            CREATE TABLE section(id INTEGER PRIMARY KEY, 
                                 seqId INTEGER, 
                                 name VARCHAR(255) UNIQUE COLLATE NOCASE);
             
            CREATE TABLE tagList(tagId INTEGER, pageId INTEGER, 
                FOREIGN KEY(tagId) REFERENCES tag(tagId) ON UPDATE CASCADE ON DELETE CASCADE);

            CREATE INDEX IDX_PAGE_SEQID on page(seqId);
            CREATE INDEX IDX_PAGE_SECTION on page(sectionId);
            
            CREATE INDEX IDX_TAG_SEQID on tag(seqId);
            CREATE INDEX IDX_SECTION_SEQID on section(seqId);
             
            CREATE INDEX IDX_TAGLIST_TAGID on tagList(tagId);
            CREATE INDEX IDX_TAGLIST_PAGEID on tagList(pageId);
           
        """)
        
        self.conn.commit()
 
    ''' 
        helper function to recalculate seqId

        # move (or insert) selected items by user to the specific position (insertBefore) in LIST

        table : table name
        data : selected items by user  (dict : seqId => id)
        insertBefore : the position to be moved
    '''

    def resetTables(self, resetAll = False):
        cur = self.conn.cursor()
        cur.executescript("""
            DROP TABLE IF EXISTS section;
            DROP TABLE IF EXISTS page;
            DROP TABLE IF EXISTS page_content;
            DROP TABLE IF EXISTS text_page;
            DROP TABLE IF EXISTS tag;
            DROP TABLE IF EXISTS tagList;
        """)

        if resetAll == True:
            cur.executescript("""
                DROP TABLE IF EXISTS env;
            """)
        self.conn.commit()


    
###################################################################
 
class T1:
    def __init__(self, s):
        print "init"

    def __call__(self, s):
        print "__call__"
    
    
if __name__ == '__main__':
    db = Db("test.db")
    db.open()
    db.resetTables()
    db.createTable()

        
    print "done"