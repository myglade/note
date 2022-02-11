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
from typing import List
import zlib

from Util.Format import ExceptionFmt

"""
TODO
* Reduce duplication codes.  : reorderSection reorderSubpage.
    Most codes are the same except for WHERE clause.  Subpage requires sectionId=''



execute parameter SUBSTITION PROBLEM

http://stackoverflow.com/questions/228912/sqlite-parameter-substitution-problem

"""

NOTE_DB = "note.db"

SUBPAGE_TYPE_A = 0
SUBPAGE_TYPE_B = 1
SEQ_INC = 100

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

        self.conn = lite.connect(self.name, check_same_thread=False)
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
        cur.execute(
            "select count(*) as cnt from sqlite_master where type='table' and name='%s'"
            % table
        )
        row = cur.fetchone()

        if row == None or row["cnt"] == 0:
            return False

        return True

    def createTable(self):
        # if one of tables exist, skip
        if self.tableExists("subpage"):
            return

        cur = self.conn.cursor()

        """
        subpage, tag, section have the same structure in the starting part.

        id : unique identifier
        seqId : ordered id.  It should be always recalculated when user data changes data order

        page: NO subpage_a_id, subpage_b_id, 
                subpage_a_id INTEGER,
                subpage_b_id INTEGER

        """

        cur.executescript(
            """
            CREATE TABLE section(
                section_id INTEGER PRIMARY KEY AUTOINCREMENT,
                seq INTEGER,
                name TEXT UNIQUE COLLATE NOCASE,
                UNIQUE(name)
            );
            CREATE INDEX IDX_SECTION_SEQ on section(seq);

            CREATE TABLE page(
                page_id INTEGER PRIMARY KEY AUTOINCREMENT,
                section_id INTEGER,
                seq INTEGER,
                mtime INTEGER,
                title TEXT,
                summary TEXT,
                UNIQUE(section_id, seq),
                FOREIGN KEY(section_id) REFERENCES section(section_id) ON DELETE CASCADE
            );
            CREATE INDEX IDX_PAGE_SEQ on page(seq);

            CREATE TABLE subpage(
                subpage_id INTEGER PRIMARY KEY AUTOINCREMENT,
                page_id INTEGER,
                type CHAR, -- subpage-a?  subpage-b?
                mtime INTEGER,
                size INTEGER,
                md5 CHAR(32),
                compressed CHAR,
                content BLOB,
                UNIQUE(page_id, type),
                FOREIGN KEY(page_id) REFERENCES page(page_id) ON DELETE CASCADE
            );
            CREATE VIRTUAL TABLE subpage_text USING fts4(page_id, subpage_id, content);

            CREATE TABLE edge(
                a INTEGER, 
                b INTEGER,
                UNIQUE(a, b),
                FOREIGN KEY(a) REFERENCES page(page_id) ON DELETE CASCADE,
                FOREIGN KEY(b) REFERENCES page(page_id) ON DELETE CASCADE
            );
            CREATE INDEX IDX_EDGE_A on edge(a);
            CREATE INDEX IDX_EDGE_B on edge(b);

            CREATE TABLE property(
                property_id INTEGER PRIMARY KEY AUTOINCREMENT,
                seq INTEGER,
                name TEXT UNIQUE COLLATE NOCASE,
                UNIQUE(name)
            );
            CREATE INDEX IDX_PROPERTY_SEQ on property(seq);

            -- level, segment ...
            CREATE TABLE page_property(
                page_id INTEGER,
                property_id INTEGER,
                UNIQUE(page_id, property_id),
                FOREIGN KEY(property_id) REFERENCES property(property_id) ON DELETE CASCADE
                FOREIGN KEY(page_id) REFERENCES page(page_id) ON DELETE CASCADE
            );
            CREATE INDEX IDX_PAGE_PROPERTY_PROPERTYID on page_property(property_id);
            CREATE INDEX IDX_PAGE_PROPERTY_PAGEID on page_property(page_id);

            CREATE TABLE tag(
                tag_id INTEGER PRIMARY KEY AUTOINCREMENT,
                seq INTEGER,
                name TEXT UNIQUE COLLATE NOCASE,
                UNIQUE(name)
            );
            CREATE INDEX IDX_TAG_SEQ on tag(seq);

            CREATE TABLE page_tag(
                page_id INTEGER,
                tag_id INTEGER,
                UNIQUE(page_id, tag_id),
                FOREIGN KEY(tag_id) REFERENCES tag(tag_id) ON DELETE CASCADE
                FOREIGN KEY(page_id) REFERENCES page(page_id) ON DELETE CASCADE
            );
            CREATE INDEX IDX_PAGE_TAG_PAGEID on page_tag(page_id);
            CREATE INDEX IDX_PAGE_TAG_TAGID on page_tag(tag_id);

            CREATE TABLE page_kv(
                page_id INTEGER,
                key TEXT,
                value TEXT,
                UNIQUE(page_id, key),
                FOREIGN KEY(page_id) REFERENCES page(page_id) ON DELETE CASCADE
            );
            CREATE INDEX IDX_PAGE_KV_PAGEID on page_kv(page_id);
            CREATE INDEX IDX_PAGE_KV_KEY on page_kv(key);
            CREATE INDEX IDX_PAGE_KV_VALUE on page_kv(value);

            CREATE TABLE env(
                key TEXT,
                value TEXT,
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
            DROP TABLE IF EXISTS section;
            DROP TABLE IF EXISTS page;
            DROP TABLE IF EXISTS subpage;
            DROP TABLE IF EXISTS subpage_text;
            DROP TABLE IF EXISTS edge;
            DROP TABLE IF EXISTS property;
            DROP TABLE IF EXISTS page_property;
            DROP TABLE IF EXISTS tag;
            DROP TABLE IF EXISTS page_tag;
            DROP TABLE IF EXISTS page_kv;
        """
        )

        if resetAll == True:
            cur.executescript(
                """
                DROP TABLE IF EXISTS env;
            """
            )
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
        res = self.query_one(
            f"SELECT seq FROM sqlite_sequence WHERE name='{table_name}'"
        )
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
                cond = f"{k}={v}"
            else:
                cond = f"{cond} OR {k}={v}"

        query = f"DELETE FROM {table} WHERE {cond}"
        self.execute(query, commit)

    #########################################################
    ###### section
    #########################################################
    def create_section(self, name, seq=None):
        if not seq:
            seq = self.get_new_seq_id("section")

        d = {"name": name, "seq": seq}
        _, id = self.insert("section", d)
        return id

    def delete_section_by_id(self, section_id):
        if not section_id:
            return

        self.delete_and("section", {"section_id": section_id})

    def update_section(self, section_id, name):
        if not id or not name:
            return

        self.update("section", {"section_id": id, "name": name}, ["section_id"])

    def update_section_all(self, sections, commit=True):
        self.delete_and("section", {}, False)

        col = ["seq", "name"]
        rows = []
        for section in sections:
            rows.append((section["seq"], section["name"]))

        self.insert_bulk("section", col, rows, commit)

    def get_section(self):
        q = f"SELECT * FROM section ORDER BY seq"
        return self.query_all(q)

    #########################################################
    ###### page
    #########################################################
    def create_page(self, section_id, subpages, title, summary, seq=None):
        d = {
            "seq": 1,
            "section_id": section_id,
            "title": title,
            "summary": summary,
            "mtime": time.time(),
        }

        _, page_id = self.insert("page", d, False)

        commit = True
        # put the interval between 2 pages. *100
        if not seq or seq < 0:
            d["seq"] = page_id * SEQ_INC
        else:
            d["seq"] = seq + 50
            commit = False

        for key, value in subpages.items():
            self.create_subpage(page_id, key, value)

        d["page_id"] = page_id
        # now commit
        self.update("page", d, ["page_id"], commit)

        # update seq for all result.
        if commit == False:
            self.resequencing("page", seq)

        return page_id

    def delete_page(self, page_id):
        self.delete_and("page", {"page_id": page_id})

    def resequencing(self, table, seq=-1):
        sql = f"SELECT {table}_id FROM {table} WHERE seq > {seq} ORDER BY seq"
        rows = self.query_all(sql)
        next_seq = seq + SEQ_INC if seq >= 0 else 0
        for row in rows:
            id_val = row[f"{table}_id"]
            sql = f"UPDATE {table} SET seq = {next_seq} WHERE {table}_id = {id_val}"
            self.execute(sql, False)
            next_seq = next_seq + SEQ_INC

        self.commit()

    def get_page(self, page_id) -> List:
        res = {}
        if page_id is None or page_id < 0:
            page_id = self.get_latest_page_id()
            if page_id is None:
                return []

        # subpages
        self.get_subpages(page_id, res)

        # edge
        res["edge"] = self.get_page_edges(page_id)

        # property
        res["property"] = self.get_property()
        res["page_property"] = self.get_page_property(page_id)

        # tag
        res["tag"] = self.get_tag()
        res["page_tag"] = self.get_page_tag(page_id)

        # kv
        res["kv"] = self.get_page_kv(page_id)

        return [res]

    def get_latest_page_id(self):
        q = f"SELECT page_id FROM page ORDER BY seq DESC LIMIT 1"

        res = self.query_all(q)
        if not res:
            return None

        return res[0]["page_id"]

    #########################################################
    ###### subpage
    #########################################################
    def create_subpage(self, page_id, type, content):
        d = {"page_id": page_id, "type": type, "mtime": time.time()}

        d["size"] = len(content)
        d["md5"] = hashlib.md5(content.encode("utf-8")).hexdigest()

        d["compressed"] = 0
        d["content"] = content
        # d["compressed"] = 1
        # d["content"] = zlib.compress(content)
        _, subpage_id = self.insert("subpage", d, False)

        return subpage_id

    def update_subpage(self, subpage, commit=True):
        subpage_id = subpage["subpage_id"]
        d = {"subpage_id": subpage_id, "mtime": time.time()}

        content = subpage["content"]
        d["compressed"] = 0
        d["size"] = len(content)
        d["md5"] = hashlib.md5(content.encode("utf-8")).hexdigest()
        d["content"] = zlib.compress(content)
        self.update("subpage", d, ["subpage_id"], commit)

        return

    # no need delete_subpage.
    def get_subpages(self, page_id, res):
        q = f"""
            SELECT a.page_id,
                   seq,
                   a.mtime AS page_mtime,
                   subpage_id,
                   type,
                   b.mtime AS subpage_mtime,
                   size,
                   md5,
                   compressed,
                   content
              FROM subpage a
                   JOIN
                   page b ON a.page_id = b.page_id
               WHERE b.page_id = {page_id};
        """
        subpages = self.query_all(q)
        for p in subpages:
            if p["compressed"] == 1:
                p["content"] = zlib.decompress(res["content"])
            res["subpage_" + p["type"]] = p

        return

    def check_values(self, table, column, elements):
        cnt = 0
        v = None
        if isinstance(elements, int):
            v = f"({elements})"
            cnt = 1
        elif isinstance(elements, list):
            v = f"({str(elements)[1:-1]})"
            cnt = len(elements)
        elif isinstance(elements, str):
            v = f"('{elements}')"
            cnt = 1
        elif isinstance(elements, tuple):
            v = elements
            cnt = len(v)
        else:
            return False

        q = f"SELECT count(*) AS cnt FROM {table} WHERE {column} in {v}"
        res = self.query_all(q)[0]
        if res["cnt"] != cnt:
            return False

        return True

    #########################################################
    ###### edge. connect subpages. NOT DAG.
    #########################################################
    def update_page_edge(self, page_id, dst, commit=True):
        # delete a -> b or b -> a.

        if not self.check_values("page", "page_id", page_id):
            raise Exception("invalid page_id")

        if not self.check_values("page", "page_id", dst):
            raise Exception("invalid dst")

        d = {"a": page_id, "b": page_id}
        self.delete_or("edge", d, False)

        if not dst:
            self.commit()
            return

        col = ["a", "b"]
        rows = []

        for v in dst:
            # connect both sides.
            rows.append((page_id, v))
            rows.append((v, page_id))

        self.insert_bulk("edge", col, rows, commit)

    def get_page_edges(self, page_id):
        q = f"SELECT b FROM edge WHERE a = {page_id}"
        return self.query_all(q)

    #########################################################
    ###### property
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
    ###### page
    #########################################################
    def update_page_property(self, page_id, property_ids, commit=True):
        if not self.check_values("page", "page_id", page_id):
            raise Exception("invalid page_id")

        if not self.check_values("property", "property_id", property_ids):
            raise Exception("invalid property_ids")

        d = {"page_id": page_id}
        self.delete_and("page_property", d, False)

        if not property_ids:
            self.commit()
            return

        col = ["page_id", "property_id"]
        rows = []
        for id in property_ids:
            rows.append((page_id, id))

        self.insert_bulk("page_property", col, rows, commit)

    def get_page_property(self, page_id):
        q = f"""
        SELECT b.name
          FROM page_property a
               JOIN
               property b ON a.property_id = b.property_id
         WHERE a.page_id = {page_id}
         ORDER BY b.seq;
        """
        return self.query_all(q)

    #########################################################
    ###### page
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
    ###### page
    #########################################################
    def update_page_tag(self, page_id, tag_ids, commit=True):
        if not self.check_values("page", "page_id", page_id):
            raise Exception("invalid page_id")

        if not self.check_values("tag", "tag_id", tag_ids):
            raise Exception("invalid tag_ids")

        d = {"page_id": page_id}
        self.delete_and("page_tag", d, False)

        if not tag_ids:
            self.commit()
            return

        col = ["page_id", "tag_id"]
        rows = []
        for id in tag_ids:
            rows.append((page_id, id))

        self.insert_bulk("page_tag", col, rows, commit)

    def get_page_tag(self, page_id):
        q = f"""
        SELECT b.name
            FROM page_tag a
                JOIN
                tag b ON a.tag_id = b.tag_id
            WHERE a.page_id = {page_id}
            ORDER BY b.seq;
        """
        return self.query_all(q)

    #########################################################
    ###### page_kv   Not paritial update
    #########################################################
    def update_page_kv(self, page_id, kvs, commit=True):
        if not self.check_values("page", "page_id", page_id):
            raise Exception("invalid page_id")

        # delete
        d = {"page_id": page_id}
        self.delete_and("page_kv", d, False)

        if not kvs:
            self.commit()
            return

        # add
        col = ["page_id", "key", "value"]
        rows = []
        for key, value in kvs.items():
            rows.append((page_id, key, value))

        self.insert_bulk("page_kv", col, rows, commit)

    def get_page_kv(self, page_id):
        q = f"SELECT key,value FROM page_kv WHERE page_id={page_id}"
        return self.query_all(q)


db = Db(NOTE_DB)

###################################################################
if __name__ == "__main__":
    #   db.execute("delete from page")
    # db.add_page(subpage_a="aaaaa", subpage_b="bbbbb")
    # db.update_page_kv(2, "k1", "v1")
    # db.update_page_kv(2, "k1", "v2")
    res = db.get_subpages([2, 3, 4])
    print(res)
    print("done")
