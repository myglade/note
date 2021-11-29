"""
Created on Apr 9, 2014

@author: hekim
"""

import collections
import os
import sqlite3 as lite
import sys
import time
from threading import Lock, Thread
import hashlib
import zlib

from Util.Format import ExceptionFmt

"""
TODO
* Reduce duplication codes.  : reorderSection reorderPage.
    Most codes are the same except for WHERE clause.  Page requires sectionId=''



execute parameter SUBSTITION PROBLEM

http://stackoverflow.com/questions/228912/sqlite-parameter-substitution-problem

"""

NOTE_DB = "note.db"

PAGE_TYPE_A = 0
PAGE_TYPE_B = 1


def dict_factory(cursor, row):
    d = {}
    for idx, col in enumerate(cursor.description):
        d[col[0]] = row[idx]
    return d


def toSqlStr(value):
    if type(value) is str:
        return "'" + value + "'"
    else:
        return str(value)


class DbErrorExceptionFmt:
    def __init__(self, *message, **kwarg):
        ExceptionFmt.__init__(self, message, kwarg)


class Db(object):
    """
    classdocs
    """

    def _test(self):
        pass

    def __init__(self, name):
        """
        Constructor
        """
        self.name = name
        # self.conn = sqlite3.connect(name)
        self.isOpen = False
        self.open()

    def open(self):
        import os
        cwd = os.getcwd()

        if self.isOpen:
            return

        self.conn = lite.connect(self.name)
        if self.conn == None:
            raise Exception(f"Fail to open {self.name}")

        # sqlite returns result row as tuple based on index
        # With this option, dict can be created
        self.conn.row_factory = dict_factory

        if self.tableExists("env") == False:
            self.createTable()

        self.isOpen = True
        return True

    def close(self):
        if self.isOpen == False:
            return

        self.conn.close()
        self.isOpen = False

    def reset(self):
        self.close()
        os.remove(NOTE_DB)
        self.open()

    def tableExists(self, table):
        cur = self.conn.cursor()
        cur.execute("select count(*) as cnt from sqlite_master where type='table' and name='%s'" % table)
        row = cur.fetchone()

        if row == None or row["cnt"] == 0:
            return False

        return True


    def createTable(self):
        # if one of tables exist, skip
        if self.tableExists("page"):
            return

        cur = self.conn.cursor()

        """
        page, tag, section have the same structure in the starting part.

        id : unique identifier
        seqId : ordered id.  It should be always recalculated when user data changes data order

        note: NO page_a_id, page_b_id, 
                page_a_id INTEGER,
                page_b_id INTEGER

        """

        cur.executescript("""
            CREATE TABLE note(
                note_id INTEGER PRIMARY KEY AUTOINCREMENT,
                seq INTEGER,
                mtime INTEGER
            );
            CREATE INDEX IDX_NOTE_SEQ on note(seq);

            CREATE TABLE page(
                page_id INTEGER PRIMARY KEY AUTOINCREMENT,
                note_id INTEGER,
                type CHAR, -- page-a?  page-b?
                mtime INTEGER,
                size INTEGER,
                md5 CHAR(32),
                compressed CHAR,
                content BLOB,
                UNIQUE(note_id, type),
                FOREIGN KEY(note_id) REFERENCES note(note_id) ON DELETE CASCADE
            );
            CREATE VIRTUAL TABLE page_text USING fts4(note_id, page_id, content);

            CREATE TABLE edge(
                a INTEGER, 
                b INTEGER,
                UNIQUE(a, b),
                FOREIGN KEY(a) REFERENCES note(note_id) ON DELETE CASCADE,
                FOREIGN KEY(b) REFERENCES note(note_id) ON DELETE CASCADE
            );
            CREATE INDEX IDX_EDGE_A on edge(a);
            CREATE INDEX IDX_EDGE_B on edge(b);

            CREATE TABLE property(
                property_id INTEGER PRIMARY KEY AUTOINCREMENT,
                seq INTEGER,
                name VARCHAR(255) UNIQUE COLLATE NOCASE,
                UNIQUE(name)
            );
            CREATE INDEX IDX_PROPERTY_SEQ on property(seq);

            -- level, segment ...
            CREATE TABLE note_property(
                note_id INTEGER,
                property_id INTEGER,
                UNIQUE(note_id, property_id),
                FOREIGN KEY(property_id) REFERENCES property(property_id) ON DELETE CASCADE
                FOREIGN KEY(note_id) REFERENCES note(note_id) ON DELETE CASCADE
            );
            CREATE INDEX IDX_NOTE_PROPERTY_PROPERTYID on note_property(property_id);
            CREATE INDEX IDX_NOTE_PROPERTY_NOTEID on note_property(note_id);

            CREATE TABLE tag(
                tag_id INTEGER PRIMARY KEY AUTOINCREMENT,
                seq INTEGER,
                name VARCHAR(255) UNIQUE COLLATE NOCASE,
                UNIQUE(name)
            );
            CREATE INDEX IDX_TAG_SEQ on tag(seq);

            CREATE TABLE note_tag(
                note_id INTEGER,
                tag_id INTEGER,
                UNIQUE(note_id, tag_id),
                FOREIGN KEY(tag_id) REFERENCES tag(tag_id) ON DELETE CASCADE
                FOREIGN KEY(note_id) REFERENCES note(note_id) ON DELETE CASCADE
            );
            CREATE INDEX IDX_NOTE_TAG_NOTEID on note_tag(note_id);
            CREATE INDEX IDX_NOTE_TAG_TAGID on note_tag(tag_id);

            CREATE TABLE note_kv(
                note_id INTEGER,
                key VARCHAR(255),
                value VARCHAR(255),
                UNIQUE(note_id, key),
                FOREIGN KEY(note_id) REFERENCES note(note_id) ON DELETE CASCADE
            );
            CREATE INDEX IDX_NOTE_KV_NOTEID on note_kv(note_id);
            CREATE INDEX IDX_NOTE_KV_KEY on note_kv(key);
            CREATE INDEX IDX_NOTE_KV_VALUE on note_kv(value);

            CREATE TABLE env(
                key VARCHAR(255),
                value VARCHAR(255),
                UNIQUE(key, value)
            );

        """)

        self.conn.commit()

    """
        helper function to recalculate seqId

        # move (or insert) selected items by user to the specific position (insertBefore) in LIST

        table : table name
        data : selected items by user  (dict : seqId => id)
        insertBefore : the position to be moved
    """

    def resetTables(self, resetAll=False):
        cur = self.conn.cursor()
        cur.executescript("""
            DROP TABLE IF EXISTS note;
            DROP TABLE IF EXISTS page;
            DROP TABLE IF EXISTS page_text;
            DROP TABLE IF EXISTS edge;
            DROP TABLE IF EXISTS property;
            DROP TABLE IF EXISTS note_property;
            DROP TABLE IF EXISTS tag;
            DROP TABLE IF EXISTS note_tag;
            DROP TABLE IF EXISTS note_kv;
        """)

        if resetAll == True:
            cur.executescript("""
                DROP TABLE IF EXISTS env;
            """)
        self.conn.commit()
        self.close()
        self.open()

    def query_one(self, q):
        cur = self.conn.cursor()
        cur.execute(q)
        return cur.fetchone()

    def query_all(self, q):
        cur = self.conn.cursor()
        cur.execute(q)
        return cur.fetchall()

    def commit(self):
        self.conn.commit()

    def execute(self, q, commit=True):
        cur = self.conn.cursor()
        res = cur.execute(q)
        if commit:
            self.conn.commit()

        return (res.rowcount, res.lastrowid)

    def get_new_seq_id(self, table_name):
        res = self.query_one(f"SELECT seq FROM sqlite_sequence WHERE name='{table_name}'")
        if not res:
            return 0

        return int(res["seq"])

    def reset_auto_inc_start(self, table_name):
        self.execute(f"UPDATE FROM sqlite_sequence SET seq=0 WHERE name='{table_name}'")

    def insert(self, table, dict, commit=True):
        col = ""
        val = ""

        for key, value in dict.items():
            k = key.replace("'", "''")
            if type(value) is str:
                v = value.replace("'", "''")
                v = "'" + v + "'"
            else:
                v = str(value)

            if not col:
                col = k
            else:
                col = f"{col},{k}"

            if not val:
                val = v
            else:
                val = f"{val},{v}"

        query = f"INSERT INTO {table} ({col}) VALUES ({val})"

        lock = Lock()
        with lock:
            return self.execute(query, commit)

    def insert_bulk(self, table, columns, rows, commit=True):
        col = ""
        val = ""

        for c in columns:
            k = c.replace("'", "''")
            if not col:
                col = k
            else:
                col = f"{col},{k}"

        s = ""
        for row in rows:
            if len(row) != len(columns):
                raise ValueError("Column mismatch")

            val = ""
            for value in row:
                if type(value) is str:
                    v = value.replace("'", "''")
                    v = "'" + v + "'"
                else:
                    v = str(value)

                if not val:
                    val = "(" + v
                else:
                    val = f"{val},{v}"

            val += ")"
            if s:
                s += "," + val
            else:
                s = val

        query = f"INSERT INTO {table} ({col}) VALUES {s}"

        lock = Lock()
        with lock:
            return self.execute(query, commit)

    def update(self, table, dict, cond_key=["id", "seqId"], commit=True):
        update = ""
        cond = ""

        for key, value in dict.items():
            if value is None:
                continue

            k = key.replace("'", "''")

            if type(value) is str:
                v = value.replace("'", "''")
                v = "'" + v + "'"
            else:
                v = str(value)

            eq = f"{k}={v}"
            if key in cond_key:
                if cond == "":
                    cond = eq
                else:
                    cond = f"{cond} AND {eq}"
            else:
                if not update:
                    update = eq
                else:
                    update = f"{update},{eq}"

        query = f"UPDATE {table} SET {update} WHERE {cond}"

        return self.execute(query, commit)


    def delete_and(self, table, dict, commit=True):
        cond = "TRUE"

        for key, value in dict.items():
            if value is None:
                continue

            k = key.replace("'", "''")

            if type(value) is str:
                v = value.replace("'", "''")
                v = "'" + v + "'"
            else:
                v = str(value)

            cond = f"{cond} AND {k}={v}"

        query = f"DELETE FROM {table} WHERE {cond}"
        self.execute(query, commit)


    def delete_or(self, table, dict, commit=True):
        cond = ""

        for key, value in dict.items():
            if value is None:
                continue

            k = key.replace("'", "''")

            if type(value) is str:
                v = value.replace("'", "''")
                v = "'" + v + "'"
            else:
                v = str(value)

            if cond == "":
                cond =  f"{k}={v}"
            else:
                cond = f"{cond} OR {k}={v}"

        query = f"DELETE FROM {table} WHERE {cond}"
        self.execute(query, commit)


    #########################################################
    ###### note
    #########################################################
    def create_note(self, seq, pages):
        d = {"seq": 1, "mtime": time.time()}

        _, note_id = self.insert("note", d, False)
        if not seq:
            d["seq"] = note_id

        for p in pages:
            self.create_page(note_id, p)

        d["note_id"] = note_id
        # now commit
        self.update("note", d, ["note_id"])

        return note_id

    def delete_note(self, note_id):
        self.delete_and("note", {"note_id": note_id})

    def get_note(self, note_id):
        res = {}
        # pages
        self.get_pages(note_id, res)

        # edge
        res["edge"] = self.get_note_edges(note_id)

        # property
        res["property"] = self.get_property()
        res["note_property"] = self.get_note_property(note_id)

        # tag
        res["tag"] = self.get_tag()
        res["note_tag"] = self.get_note_tag(note_id)

        # kv
        res["kv"] = self.get_note_kv(note_id)

        return res   

    #########################################################
    ###### page
    #########################################################
    def create_page(self, note_id, page):
        d = {"note_id": note_id, "type": page["type"], "mtime": time.time()}

        content = page["content"]
        d["size"] = len(content)
        d["md5"] = hashlib.md5(content.encode('utf-8')).hexdigest()

        d["compressed"] = 0
        d["content"] = content
        # d["compressed"] = 1
        # d["content"] = zlib.compress(content)
        _, page_id = self.insert("page", d, False)

        return page_id

    def update_page(self, page_id, page):
        d = {"page_id": page_id, "mtime": time.time()}

        content = page["content"]
        d["compressed"] = 0
        d["size"] = len(content)
        d["md5"] = hashlib.md5(content.encode('utf-8')).hexdigest()
        d["content"] = zlib.compress(content)
        self.update("page", d, ["page_id"])

        return

    # no need delete_page.
    def get_pages(self, note_id, res):
        q = f"""
            SELECT a.note_id,
                   seq,
                   a.mtime AS note_mtime,
                   page_id,
                   type,
                   b.mtime AS page_mtime,
                   size,
                   md5,
                   compressed,
                   content
              FROM page a
                   JOIN
                   note b ON a.note_id = b.note_id
               WHERE b.note_id = {note_id};
        """
        pages = self.query_all(q) 
        for p in pages:
            if p["compressed"] == 1:
                p["content"] = zlib.decompress(res["content"]) 
            res["page_" + p["type"]] = p

        return

    #########################################################
    ###### edge. connect pages. NOT DAG.  
    #########################################################
    def update_note_edge(self, note_id, dst, commit=True):
        # delete a -> b or b -> a.

        d = {"a": note_id, "b": note_id}
        self.delete_or("edge", d, False)

        if not dst:
            self.commit()
            return

        col = ["a", "b"]
        rows = []

        for v in dst: 
            # connect both sides. 
            rows.append((note_id, v))
            rows.append((v, note_id))

        self.insert_bulk("edge", col, rows, commit)


    def get_note_edges(self, note_id):
        q = f"SELECT b FROM edge WHERE a = {note_id}"
        return self.query_all(q) 

    #########################################################
    ###### note
    #########################################################
    def create_property(self, name, seq=None):
        if not seq:
            seq = self.get_new_seq_id("property")

        d = {"name": name, "seq": seq}
        _, id = self.insert("property", d)
        return id

    def delete_property_by_id(self, property_id):
        if not property_id:
            return

        self.delete_and("property", {"property_id": property_id})

    def update_property(self, property_id, name):
        if not id or not name:
            return

        self.update("property", {"property_id": id, "name": name}, ["property_id"])

    def update_property_all(self, properties, commit=True):
        self.delete_and("property", {}, False)

        col = ["seq", "name"]
        rows = []
        for prop in properties:
            rows.append((prop["seq"], prop["name"]))

        self.insert_bulk("property", col, rows, commit)

    def get_property(self):
        q = f"SELECT * FROM property ORDER BY seq"
        return self.query_all(q) 

    #########################################################
    ###### note
    #########################################################
    def update_note_property(self, note_id, property_ids, commit=True):
        d = {"note_id": note_id}
        self.delete_and("note_property", d, False)

        if not property_ids:
            self.commit()
            return

        col = ["note_id", "property_id"]
        rows = []
        for id in property_ids:
            rows.append((note_id, id))

        self.insert_bulk("note_property", col, rows, commit)

    def get_note_property(self, note_id):
        q = f"""
        SELECT b.name
          FROM note_property a
               JOIN
               property b ON a.property_id = b.property_id
         WHERE a.note_id = {note_id}
         ORDER BY b.seq;
        """
        return self.query_all(q) 


    #########################################################
    ###### note
    #########################################################
    def create_tag(self, name, seq=None):
        if not seq:
            seq = self.get_new_seq_id("tag")

        d = {"name": name, "seq": seq}
        _, id = self.insert("tag", d)
        return id

    def delete_tag(self, tag_id):
        if not tag_id:
            return

        self.delete_and("tag", {"tag_id": tag_id})

    def update_tag(self, tag_id, name):
        if not tag_id or not name:
            return

        self.update("tag", {"tag_id": tag_id, "name": name}, ["tag_id"])

    def update_tag_all(self, tags, commit=True):
        self.delete_and("tag", {}, False)

        col = ["seq", "name"]
        rows = []
        for tag in tags:
            rows.append((tag["seq"], tag["name"]))

        self.insert_bulk("tag", col, rows, commit)

    def get_tag(self):
        q = f"SELECT * FROM tag"
        return self.query_all(q) 

    #########################################################
    ###### note
    #########################################################
    def update_note_tag(self, note_id, tag_ids, commit=True):
        d = {"note_id": note_id}
        self.delete_and("note_tag", d, False)

        if not tag_ids:
            self.commit()
            return

        col = ["note_id", "tag_id"]
        rows = []
        for id in tag_ids:
            rows.append((note_id, id))

        self.insert_bulk("note_tag", col, rows, commit)

    def get_note_tag(self, note_id):
        q = f"""
        SELECT b.name
            FROM note_tag a
                JOIN
                tag b ON a.tag_id = b.tag_id
            WHERE a.note_id = {note_id}
            ORDER BY b.seq;
        """
        return self.query_all(q) 

    #########################################################
    ###### note_kv   Not paritial update
    #########################################################
    def update_note_kv(self, note_id, kvs, commit=True):
        # delete 
        d = {"note_id": note_id}
        self.delete_and("note_tag", d, False)

        if not kvs:
            self.commit()
            return

        # add
        col = ["note_id", "key", "value"]
        rows = []
        for kv in kvs:
            rows.append((note_id, kv[0], kv[1]))

        self.insert_bulk("note_tag", col, rows, commit)

    def get_note_kv(self, note_id):
        q = f"SELECT key,value FROM note_kv WHERE note_id={note_id}"
        return self.query_all(q) 

db = Db(NOTE_DB)

###################################################################
if __name__ == "__main__":
    #   db.execute("delete from note")
    #db.add_note(page_a="aaaaa", page_b="bbbbb")
    #db.update_note_kv(2, "k1", "v1")
    #db.update_note_kv(2, "k1", "v2")
    res = db.get_pages([2,3,4])
    print(res)
    print("done")
