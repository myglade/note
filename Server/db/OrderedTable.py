import sqlite3 as lite
import collections
import os, sys
lib_path = os.path.abspath(os.path.join('Util'))
sys.path.append(lib_path)
import Format

class OrderedTable(object):
    def __init__(self, db, tableName, filter = None):
        self.tableName = tableName
        self.conn = db.conn

        # select filter
        # In page, filter = "sectionId = ?"
        if filter == None or filter == "":
            self.filter = "(1)"
        else:
            self.filter = "(" + filter + ")"   

    # dict : column => value
    def addItem(self, insertBefore, dict, err = Format.NullFmt):
        if not dict:
            err("dict is empty", 1)
            return False            

        cur = self.conn.cursor()

        try:
            # find the smallest available number to reuse id
            cur.execute("SELECT id FROM %s ORDER BY id ASC" % self.tableName)
            rows = cur.fetchall()
            nextId = 0
            
            for row in rows:
                if row["id"] != nextId:
                    break
                nextId += 1
            
            if insertBefore >= 0 and insertBefore < len(rows):
                query = "UPDATE %s SET seqId = seqId + 1 WHERE seqId >= %s AND %s" % \
                            (self.tableName, insertBefore, self.filter)
                cur.execute(query)
            else:
                insertBefore = len(rows)
                       
            key, value = dictToSqlStrForInsert(dict)
            query = "INSERT INTO %s('id','seqId',%s) VALUES(%s,%s,%s);" % \
                (self.tableName, key, nextId, insertBefore, value)
            cur.execute(query)
            if cur.rowcount != 1:
                err(self.tableName + " Insert Failure")
                return False
            
            self.conn.commit()  
        except lite.IntegrityError as e:
            err(e.message, 1)
            return False
            
        return True


    # list : list of dict
    def addItems(self, insertBefore, list, err = Format.NullFmt):
        if not list:
            err("dict is empty", 1)
            return False            

        cur = self.conn.cursor()

        try:
            cur.execute("SELECT id FROM %s ORDER BY id ASC" % self.tableName)
            rows = cur.fetchall()
            if insertBefore >= 0 and insertBefore < len(rows):
                query = "UPDATE %s SET seqId = seqId + 1 WHERE seqId >= %s AND %s" % \
                            (self.tableName, insertBefore, self.filter)
                cur.execute(query)
            else:
                insertBefore = len(rows)

            self.renumbering(insertBefore, insertBefore + len(list))

            # find the smallest available number to reuse id
            nextId = 0
            newIdList = []

            for row in rows:
                if row["id"] != nextId:
                    newIdList.append(nextId)
                    if len(newIdList) == len(list):
                        break
                nextId += 1
            
            while len(newIdList) < len(list):
                newIdList.append(nextId)
                nextId += 1

            idIndex = 0
                       
            for dict in list:
                key, value = dictToSqlStrForInsert(dict)
                query = "INSERT INTO %s('id','seqId',%s) VALUES(%s,%s,%s);" % \
                    (self.tableName, key, newIdList[idIndex], insertBefore, value)
                cur.execute(query)

                idIndex += 1
                insertBefore += 1
                if cur.rowcount != 1:
                    err(self.tableName + " Insert Failure")
                    self.renumbering(0, 0)
                    return False
            
            self.conn.commit()  
        except lite.IntegrityError as e:
            err(e.message, 1)
            return False
            
        return True

    def deleteItem(self, id, err = Format.NullFmt):
        cur = self.conn.cursor()

        try:
            seqId = self.getById(id)["seqId"]
            cur.execute("DELETE FROM %s WHERE id=%s;" % (self.tableName, id))

            query = "UPDATE %s SET seqId = seqId - 1 WHERE seqId >= %s AND %s" % \
                        (self.tableName, seqId, self.filter)
            cur.execute(query)
            self.conn.commit()  
        except lite.IntegrityError as e:
            err(e.message, 1)
            return False

    def deleteItems(self, idList, err = Format.NullFmt):
        cur = self.conn.cursor()

        try:
            for id in idList:
                cur.execute("DELETE FROM %s WHERE id=%s;" % (self.tableName, id))

            self.renumbering(0, 0)
       #     self.conn.commit()  
        except lite.IntegrityError as e:
            err(e.message, 1)
            return False

    def removeAll(self, err = Format.NullFmt):
        cur = self.conn.cursor()

        try:
            cur.execute("DELETE FROM %s WHERE %s;" % (self.tableName, self.filter))
            self.conn.commit()  
        except lite.IntegrityError as e:
            err(e.message, 1)
            return False

    # dict : col => value
    def updateItem(self, id, dict, err = Format.NullFmt):
        if not dict:
            err("dict is empty", 1)
            return False            

        cur = self.conn.cursor()
        try:
            s = dictToSqlStrForUpdate(dict)

            cur.execute("UPDATE %s SET %s WHERE id=%s" % (self.tableName, s, id))
            self.conn.commit()
        except lite.IntegrityError as e:
            err(e.message, 1)
            return False
        
        return True

    # list : seqId list
    def reorderItem(self, list, insertBefore, err = Format.NullFmt):
        # list empty check
        if not list:
            err("data is empty")
            return False
        
        # use tuple as result instead of dict.  reset to default
        cur = self.conn.cursor()

        cur.execute("SELECT count(*) from %s WHERE %s" % (self.tableName, self.filter))

        row = cur.fetchone()
        total = row.values()[0]
        if row == None or total == 0:
            err("no items are selected")
            return False

        selectedLen = len(list)

        # if total and selected items are the same, it means all are selected.   return
        if total == selectedLen:
            err("all items are selected.")
            return False
        
        # if insertBefore < 0, move to the end
        if insertBefore < 0:
            insertBefore = total
            
        list.sort()
        start = list[0]
        end = list[len(list)-1]
               
        # out of range
        if start < 0 or end > total - 1:
            return False

        if start > insertBefore:
            start = insertBefore
        if end < insertBefore:
            end = insertBefore - 1
            
        cur.execute("SELECT seqId, id FROM %s WHERE seqId >= %d AND seqId <= %d AND %s ORDER BY seqId ASC" % \
                    (self.tableName, start, end, self.filter))
        rows = cur.fetchall()
        
        allItems = []
        for row in rows:
            allItems.append([row["seqId"], row["id"]])
            
        if start == insertBefore:
            selected = start
            notSelected = selected + selectedLen
        else:
            notSelected = start
            selected = end - selectedLen + 1

        for item in allItems:
            if item[0] in list:
                item[0] = selected
                selected += 1
            else:
                item[0] = notSelected
                notSelected += 1
         
            cur.execute("UPDATE %s SET seqId=%d WHERE id=%d" % (self.tableName, item[0], item[1]))
        
        self.conn.commit()
        return True               
      
    def renumbering(self, seqId, newSeqId):
        cur = self.conn.cursor()
        query = "SELECT id FROM %s WHERE seqId >= %d AND %s ORDER BY seqId ASC" % \
                (self.tableName, seqId, self.filter)
        cur.execute(query)
        rows = cur.fetchall()

        for row in rows:
            cur.execute("UPDATE %s SET seqId=%d WHERE id=%s" % (self.tableName, newSeqId, row["id"]))
            newSeqId += 1

        self.conn.commit()


    def getById(self, id, err = Format.NullFmt):
        cur = self.conn.cursor()

        try:
            query = "SELECT * FROM %s WHERE id=%s" % (self.tableName, id)
            cur.execute(query)
            rows = cur.fetchall()

            if len(rows) == 0:
                return {}

            return rows[0]
        except lite.IntegrityError as e:
            err(e.message, 1)
            return {}

    def getByCol(self, col, value, err = Format.NullFmt):
        cur = self.conn.cursor()

        value = toSqlStr(value)
        try:
            query = "SELECT * FROM %s WHERE %s=%s AND %s" % (self.tableName, col, value, self.filter)
            cur.execute(query)
            rows = cur.fetchall()

            if len(rows) == 0:
                return []

            return rows
        except lite.IntegrityError as e:
            err(e.message, 1)
            return []

    # return list of sections (dict list), which is ordered by seqId
    # [{id:xx,seqId:xx,name:xx],{..} .. ] 
    def getItems(self):
        cur = self.conn.cursor()
        query = "SELECT * FROM %s WHERE %s ORDER BY seqId ASC" % (self.tableName, self.filter)
        cur.execute(query)
        rows = cur.fetchall()

        return rows
