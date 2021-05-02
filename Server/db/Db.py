"""
Created on Apr 9, 2014

@author: hekim
"""

import collections
import sqlite3 as lite
import sys
import time
from threading import Lock, Thread

from Util.Format import ExceptionFmt

"""
TODO
* Reduce duplication codes.  : reorderSection reorderPage.
    Most codes are the same except for WHERE clause.  Page requires sectionId=''



execute parameter SUBSTITION PROBLEM

http://stackoverflow.com/questions/228912/sqlite-parameter-substitution-problem

"""

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
        if self.isOpen:
            return

        self.conn = lite.connect(self.name)
        if self.conn == None:
            raise Exception(f"Fail to open {self.name}")

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
        cur.execute(
            "select count(*) as cnt from sqlite_master where type='table' and name='%s'"
            % table
        )
        row = cur.fetchone()

        if row == None or row["cnt"] == 0:
            return False

        return True

    def createEnvTable(self):
        cur = self.conn.cursor()
        cur.execute(
            "CREATE TABLE env(key varchar(255) PRIMARY KEY, value varchar(255));"
        )
        self.conn.commit()

    def createTable(self):
        # if one of tables exist, skip
        if self.tableExists("page"):
            return

        cur = self.conn.cursor()

        """
        page, tag, section have the same structure in the starting part.

        id : unique identifier
        seqId : ordered id.  It should be always recalculated when user data changes data order

        """

        cur.executescript(
            """
            CREATE TABLE note(
                note_id INTEGER PRIMARY KEY AUTOINCREMENT,
                seq INTEGER,
                mtime INTEGER,
                page_a_id INTEGER,
                page_b_id INTEGER
            );
            CREATE INDEX IDX_NOTE_SEQ on note(seq);

            CREATE TABLE page(
                page_id INTEGER PRIMARY KEY AUTOINCREMENT,
                note_id INTEGER,
                type CHAR, -- page-a?  page-b?
                mtime INTEGER,
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

        """
        )

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
        cur.executescript(
            """
            DROP TABLE IF EXISTS note;
            DROP TABLE IF EXISTS page;
            DROP TABLE IF EXISTS page_text;
            DROP TABLE IF EXISTS edge;
            DROP TABLE IF EXISTS property;
            DROP TABLE IF EXISTS note_property;
            DROP TABLE IF EXISTS tag;
            DROP TABLE IF EXISTS note_tag;
        """
        )

        if resetAll == True:
            cur.executescript(
                """
                DROP TABLE IF EXISTS env;
            """
            )
        self.conn.commit()

    def query_one(self, q):
        cur = self.conn.cursor()
        cur.execute(q)
        return cur.fetchone()

    def query_all(self, q):
        cur = self.conn.cursor()
        cur.execute(q)
        return cur.fetchall()

    def execute(self, q, commit=True):
        cur = self.conn.cursor()
        res = cur.execute(q)
        if commit:
            self.conn.commit()

        return (res.rowcount, res.lastrowid)

    def get_new_seq_id(self, table_name):
        res = self.query_one(
            f"SELECT seq FROM sqlite_sequence WHERE name='{table_name}'"
        )

        return int(res["seq"]) + 1

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

    def update(self, table, dict, cond_key=["id", "seqId"], commit=True):
        update = ""
        cond = ""

        for key, value in dict.items():
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

    def delete(self, table, dict, commit=True):
        cond = "TRUE"

        for key, value in dict.items():
            k = key.replace("'", "''")

            if type(value) is str:
                v = value.replace("'", "''")
                v = "'" + v + "'"
            else:
                v = str(value)

            cond = f"{cond} AND {k}={v}"

        query = f"DELETE FROM {table} WHERE {cond}"
        self.execute(query, commit)

    #########################################################
    ###### note
    #########################################################
    def add_note(self, seq=None, page_a=None, page_b=None):
        d = {"seq": 1, "mtime": time.time()}

        _, note_id = self.insert("note", d, False)
        if not seq:
            d["seq"] = note_id

        if page_a:
            d["page_a_id"] = self.add_page(note_id, page_a, PAGE_TYPE_A)

        if page_b:
            d["page_b_id"] = self.add_page(note_id, page_b, PAGE_TYPE_B)

        d["note_id"] = note_id
        # now commit
        self.update("note", d, ["note_id"])

        return note_id

    def delete_note(self, note_id):
        self.delete("note", {"note_id": note_id})

    def get_note(self, note_id):
        res = {}
        # note
        q = f"SELECT * FROM note WHERE note_id={note_id}"
        note = self.query_all(q) 

        # pages
        pages = self.pages([note["page_a_id"], note["page_b_id"]])
        for p in pages:
            if p["type"] == 0: # page a
                res["page_a"] = p
            else:
                res["page_b"] = p

        # edge
        res["edge"] = self.get_edges(note_id)

        # property
        res["property"] = self.get_property(note_id)
        res["note_property"] = self.get_note_property(note_id)

        # tag
        res["tag"] = self.get_tag(note_id)
        res["note_tag"] = self.get_note_tag(note_id)

        # kv
        res["kv"] = self.get_note_kv(note_id)

        return kv   

    #########################################################
    ###### page
    #########################################################
    def add_page(self, note_id, content, type):
        d = {"note_id": note_id, "type": type, "mtime": time.time()}

        d["compressed"] = 0
        d["content"] = content
        _, page_id = self.insert("page", d, False)

        return page_id

    # no need delete_page.   
    def get_pages(self, page_ids):
        t = str(page_ids)[1:-1]
        q = f"SELECT * FROM page WHERE page_id IN ({t})"
        res = self.query_all(q) 
        if res["compressed"] == 1:
            raise "Not supported"

        return res

    #########################################################
    ###### note
    #########################################################
    def add_edge(self, note_id_a, note_id_b):
        d = {"a": note_id_a, "b": note_id_b}
        self.insert("edge", d)

    def delete_edge(self, note_id_a, note_id_b):
        d = {"a": note_id_a, "b": note_id_b}
        self.delete("edge", d)

    def get_edges(self, note_id):
        q = f"SELECT b FROM edge WHERE a = {note_id}"
        return self.query_all(q) 

    #########################################################
    ###### note
    #########################################################
    def add_property(self, name, seq=None):
        if not seq:
            seq = self.get_new_seq_id("propety")

        d = {"name": name, "seq": seq}
        _, id = self.insert("propety", d)
        return id

    def delete_property(self, name):
        if not name:
            return

        self.delete("property", {"name": name})

    def get_property(self):
        q = f"SELECT * FROM property"
        return self.query_all(q) 

    #########################################################
    ###### note
    #########################################################
    def add_note_property(self, note_id, property_id):
        d = {"note_id": note_id, "property_id": property_id}

        _, id = self.insert("note_property", d)
        return id

    def delete_note_property(self, note_id, property_id):
        d = {"note_id": note_id, "property_id": property_id}

        self.delete("note_property", d)

    def get_note_property(self, note_id):
        q = f""""
        SELECT b.name FROM note_property a
        JOIN property b
        ON a.property_id = b.property_id
        WHERE
            a.note_id = {note_id}
        ORDER BY
            b.seq
        """
        return self.query_all(q) 


    #########################################################
    ###### note
    #########################################################
    def add_tag(self, name, seq=None):
        if not seq:
            seq = self.get_new_seq_id("tag")

        d = {"name": name, "seq": seq}
        _, id = self.insert("tag", d)
        return id

    def delete_tag(self, name):
        if not name:
            return

        self.delete("tag", {"name": name})

    def get_tag(self):
        q = f"SELECT * FROM tag"
        return self.query_all(q) 

    #########################################################
    ###### note
    #########################################################
    def add_note_tag(self, note_id, tag_id):
        d = {"note_id": note_id, "tag_id": tag_id}

        _, id = self.insert("note_tag", d)

    def delete_note_tag(self, note_id, tag_id):
        d = {"note_id": note_id, "tag_id": tag_id}

        self.delete("note_tag", d)

    def get_note_tag(self, note_id):
        q = f""""
        SELECT b.name FROM note_tag a
        JOIN tag b
        ON a.tag_id = b.tag_id
        WHERE
            a.note_id = {note_id}
        ORDER BY
            b.seq
        """
        return self.query_all(q) 

    #########################################################
    ###### note
    #########################################################
    def update_note_kv(self, note_id, key, value):
        d = {"note_id": note_id, "key": key, "value": value}

        rowcount, _ = self.update("note_kv", d, ["note_id", "key"])
        if rowcount == 0:
            self.insert("note_kv", d)

    def delete_node_kv(self, note_id, key):
        d = {"note_id": note_id, "key": key}

        self.delete("note_tag", d)

    def get_note_kv(self, note_id):
        q = f"SELECT key,value FROM note_kv WHERE note_id={note_id}"
        return self.query_all(q) 

###################################################################


if __name__ == "__main__":
    db = Db("note.db")

    #   db.execute("delete from note")
    #db.add_note(page_a="aaaaa", page_b="bbbbb")
    #db.update_note_kv(2, "k1", "v1")
    #db.update_note_kv(2, "k1", "v2")
    res = db.get_pages([2,3,4])
    print(res)
    print("done")
