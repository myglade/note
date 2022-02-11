import logging
import json

from db.Db import db
from NoteException import *

"""
page properties : page_tag, page_property, page_edge ...

eveytime user clicks them, the changes are not stored to servers.
After web/app changes, user clicks "save", all changes will be delivered to server.

Hence, server deletes the existing setting and add new settings instead of finding diff and 
partial updates

"""


"""
Create section (in the window app)
{
    "cmd": "create_section",
    "name": "xxxxx"
}

return new section id
"""


def create_section(data):
    db.create_section(data["name"])
    return get_section(data)


"""
Delete section (in the window)
{
    "cmd": "delete_section",
    "section_id" : "yyyy"
}
"""


def delete_section(data):
    db.delete_section_by_id(data["section_id"])
    return db.get_section()

"""
update section (in the window)


{
    "cmd": "update_section",
    "name": "xxxxx"
    "section_id" : "yyyy"
}
"""


def update_section(data):
    db.update_section(data["section_id"], data["name"])
    return db.get_section()


"""
update all sections (in the window)
delete and add.  For handling seq by user, use this.

{
    "cmd": "update_section_all",
    "sections": [
        {"seq": 1, "name": "section1"},
        {"seq": 2, "name": "section2"},
    ]
}
"""


def update_section_all(data):
    db.update_section_all(data["sections"])
    return db.get_section()

"""
Get section (windows / web)
{
    "cmd": "get_section"
}

[
    {
        "tag_id": xxx
        "seq":
        "name": ..
    },
    {
    }
]
"""


def get_section(data):
    return db.get_section()


"""
create page

* after add, behavior
1. add, return subpage_id, get subpage_id
2. add, return all all infos  (preferred). possible?  how can I get return values?
3. add, return subpage id. : doesn't work.  picture location is changed from original to mine.

upload json data as POST

http://<host>/<path>/db 


{
    "cmd": "create_page",
    "section_id": 111,
    "seq": -1,
    "title", "",
    "summary", "",
    "subpages": {
        // "a",    <-- question or answer
        // value :  html format
        "a" : "xxxxxx",
        "b" " "yyyyy",  
    },
    # for ex. this page can be mapped to 111, 222, 33
    # can be used for grouping purpose.  In leecode, 
    # question-1 q-2, q-3.  similar
    # edge : node id list 
    "edge":["111", "222", "333"],

    no clear guideline for property and tag.  
    If you think, something is permanent, use property.  If can be changed often, use tag.

    # rarely change.  difficulty, area(array, dp, ..) could be multiple
    "property_id":[],

    # can change often.  how many failures
    "tag_id":[],

    # Leecode/company?  key/value
    "kv": {},

}
"""


def create_page(data):
    section_id = data["section_id"]
    seq = -1
    if "seq" in data:
        seq = data["seq"]

    title = data["title"] if "title" in data else ""
    summary = data["summary"] if "summary" in data else ""

    page_id = db.create_page(
        section_id, data["subpages"], title, summary, seq
    )
    if "edge" in data and data["edge"]:
        db.update_page_edge(page_id, data["edge"], False)

    if "property_id" in data and data["property_id"]:
        db.update_page_property(page_id, data["property_id"], False)

    if "tag_id" in data and data["tag_id"]:
        db.update_page_tag(page_id, data["tag_id"], False)

    if "kv" in data and data["kv"]:
        db.update_page_kv(page_id, data["kv"], False)

    db.commit()

    return db.get_page(page_id)


"""
Update page

upload json data as POST

http://<host>/<path>/db 


client(web javascript) checks it's dirty or not.  If not dirty, skip
 * can use md5.

 important.  Editor doesn't have a way to detect modification.  If 
 modified, save to db, otherwise skip

{
    "cmd": "update_page",
    "page_id": 1111,
    "subpages": 
    {
        "subpage_id": 1234,
        "content": "xxxxx"
    },
    {
        "subpage_id": 2345,
        "content": "yyyyy"
    }
    ],
    "edge":["111", "222", "333"],
    "property_id":[],
    "tag_id":[],
    "kv": {},
}
"""


def update_page(data):
    if "subpages" in data and data["subpages"]:
        for p in data["subpages"]:
            db.update_subpage(p, False)

    page_id = data["page_id"]
    if "edge" in data and data["edge"]:
        db.update_page_edge(page_id, data["edge"], False)

    if "property_id" in data and data["property_id"]:
        db.update_page_property(page_id, data["property_id"], False)

    if "tag_id" in data and data["tag_id"]:
        db.update_page_tag(page_id, data["tag_id"], False)

    if "kv" in data and data["kv"]:
        db.update_page_kv(page_id, data["kv"], False)

    db.commit()

    return db.get_page(page_id)


"""
delete page (in the window app)
{
    "cmd": "delete_page",
    "page_id": "xxxxx"
}

"""


def delete_page(data):
    db.delete_page(data["page_id"])


def get_page(data):
    if "page_id" in data:
        page_id = data["page_id"]
    else:
        page_id = None

    return db.get_page(page_id)


"""
Create property (in the window app)
{
    "cmd": "create_property",
    "name": "xxxxx"
}

return new property id
"""


def create_property(data):
    db.create_property(data["name"])
    return db.get_property()


"""
Delete property (in the window)
{
    "cmd": "delete_property",
    "property_id" : "yyyy"
}
"""


def delete_property(data):
    db.delete_property_by_id(data["property_id"])
    return db.get_property()

"""
update property (in the window)


{
    "cmd": "delete_property",
    "name": "xxxxx"
    "property_id" : "yyyy"
}
"""


def update_property(data):
    db.update_property(data["property_id"], data["name"])
    return db.get_property()

"""
update all property (in the window)
delete and add.  For handling seq by user, use this.

{
    "cmd": "update_property_all",
    "properties": [
        {"seq": 1, "name": "prop1"},
        {"seq": 2, "name": "prop2"},
    ]
}
"""


def update_property_all(data):
    db.update_property_all(data["properties"])
    return db.get_property()

"""
Get property (windows / web)
{
    "cmd": "get_property"
}

[
    {
        "tag_id": xxx
        "seq":
        "name": ..
    },
    {
    }
]
"""


def get_property(data):
    return db.get_property()


"""
Create tag : in windows app. (not web)

upload json data as POST
http://<host>/<path>/db 

{
    "cmd": "add_tag",
    "name": "xxxxx"
}

"""


def create_tag(data):
    db.create_tag(data["name"])
    return db.get_tag()


"""
Delete tag : in windows app. (not web)
{
    "cmd": "delete_tag",
    "tag_id" : "yyyy" 
}

"""


def delete_tag(data):
    db.delete_tag(data["tag_id"])
    return db.get_tag()

"""
Update tag in windows app (Not web)

{
    "cmd": "update_tag",
    "id": "xxxxx"
    "name" : "new_name"
}
"""


def update_tag(data):
    db.update_tag(data["tag_id"], data["name"])
    return db.get_tag()

"""
update all tags (in the window)
delete and add.  For handling seq by user, use this.

{
    "cmd": "update_tag_all",
    "tags": [
        {"seq": 1, "name": "tag1"},
        {"seq": 2, "name": "tag2"},
    ]
}
"""


def update_tag_all(data):
    db.update_tag_all(data["tags"])
    return db.get_tag()

"""
Get tag (web will call it and show all so that user picks up)
{
    "cmd": "get_tag"
}

[
    {
        "tag_id": xxx
        "seq":
        "name": ..
    },
    {
    }
]
"""


def get_tag(data):
    return db.get_tag()


"""
update page kv
{
    "cmd": "update_page_kv",
    "page_id": "xxxx"
    "kv": {         // should provide full set.  Not partial 
        "ka": "va",
        "kb": "vb"
    }
}

"""
# no partial update.  delete and add all
def update_page_kv(data):
    db.update_page_kv(data["page_id"], data["kv"])
    return db.get_page_kv(data["page_id"])


"""

"""


def get_page_kv(data):
    return db.get_page_kv(data["page_id"])


"""
update page edge

upload json data as POST
http://<host>/<path>/db 
{
    "cmd": "update_page_edge",
    "page_id": "xxxx"
    "dst" : [11, 22, 33, 44]
}

# for page, all settings are delete and add all
"""


def update_page_edge(data):
    db.update_page_edge(data["page_id"], data["dst"])
    return db.get_page_edges(data["page_id"])


"""
Get page edge (it will be included in subpage loading)


{
    "cmd": "get_page_edge"
}

[
    {  page ... (add more)
        "a":
        "b": ..
    },
    {
    }
]
"""


def get_page_edge(data):
    return db.get_page_edges(data["page_id"])


"""
Update property 

(web shows all property as checkbox.   if any diff, update)
(if no diff, no update.   check in javascript)

- create "SAVE" button in web.  in clicking it, update tag/property/key,value all
upload json data as POST

http://<host>/<path>/db 


{
    "cmd": "update_page_property",
    "page_id": "xx",
    "property_id": ["11", "22", "3", "5"]
}
"""


def update_page_property(data):
    db.update_page_property(data["page_id"], data["property_id"])
    return db.get_page_property(data["page_id"])


"""
Get page_property (Need it?   In refreshing subpage, download all info once and for all)

subpages + property + tags + .....

Just in case, create it.  but, I don't know whether it will be used.

{
    "cmd": "get_page_property",
    "page_id": "xxx"
}

[
    {
        "property_id": xxx
        "seq":
        "name": ..
    },
    {
    }
]
"""


def get_page_property(data):
    return db.get_page_property(data["page_id"])


"""
Create page_tag / delete page_tag. : NOT needed.

Update tag 

(web shows all tags as checkbox.   if any diff, update)
(if no diff, no update.   check in javascript)

- create "SAVE" button in web.  in clicking it, update tag/property/key,value all
upload json data as POST

http://<host>/<path>/db 


{
    "cmd": "update_page_tag",
    "page_id": "xx",
    "tag_id": ["11", "22", "3", "5"]
}
"""


def update_page_tag(data):
    db.update_page_tag(data["page_id"], data["tag_id"])
    return db.get_page_tag(data["page_id"])


"""
Get page_tag (Need it?   In refreshing subpage, download all info once and for all)

subpages + property + tags + .....

Just in case, create it.  but, I don't know whether it will be used.

{
    "cmd": "get_tag"
    "page_id": "xxx"
}

[
    {
        "tag_id": xxx
        "seq":
        "name": ..
    },
    {
    }
]
"""


def get_page_tag(data):
    return db.get_page_tag(data["page_id"])


cmd_map = {
    "create_section": create_section,
    "update_section": update_section,
    "update_section_all": update_section_all,
    "delete_section": delete_section,
    "get_section": get_section,
    "create_page": create_page,
    "update_page": update_page,
    "delete_page": delete_page,
    "get_page": get_page,
    "create_property": create_property,
    "delete_property": delete_property,
    "update_property": update_property,
    "update_property_all": update_property_all,
    "get_property": get_property,
    "create_tag": create_tag,
    "delete_tag": delete_tag,
    "update_tag": update_tag,
    "update_tag_all": update_tag_all,
    "get_tag": get_tag,
    "update_page_kv": update_page_kv,
    "get_page_kv": get_page_kv,
    "update_page_edge": update_page_edge,
    "get_page_edge": get_page_edge,
    "update_page_property": update_page_property,
    "get_page_property": get_page_property,
    "update_page_tag": update_page_tag,
    "get_page_tag": get_page_tag,
}


def dispacher(data):
    if "cmd" not in data:
        raise InvalidRequest("Command not found")

    cmd = data["cmd"]
    if cmd not in cmd_map:
        raise InvalidCommand(f"Invalid command = {cmd}")

    res = None
    try:
        res = cmd_map[cmd](data)
    except Exception as e:
        raise InternalError(e)

    return res


def test_property():
    data = {"cmd": "create_property", "name": "property1"}
    dispacher(data)
    data = {"cmd": "create_property", "name": "property2"}
    dispacher(data)

    # [{'property_id': 1, 'seq': 1, 'name': 'property1'}]
    data = {"cmd": "get_property"}
    res = dispacher(data)
    print(res)

    data = {"cmd": "delete_property", "property_id": "1"}
    dispacher(data)

    data = {"cmd": "get_property"}
    res = dispacher(data)
    print(res)

    data = {
        "cmd": "update_property_all",
        "properties": [
            {"seq": 10, "name": "prop1"},
            {"seq": 20, "name": "prop2"},
        ],
    }
    res = dispacher(data)
    data = {"cmd": "get_property"}
    res = dispacher(data)
    print(res)


def test_tag():
    data = {"cmd": "create_tag", "name": "tag1"}
    dispacher(data)
    data = {"cmd": "create_tag", "name": "tag2"}
    dispacher(data)

    # [{'property_id': 1, 'seq': 1, 'name': 'property1'}]
    data = {"cmd": "get_tag"}
    res = dispacher(data)
    print(res)

    data = {"cmd": "delete_tag", "tag_id": "1"}
    dispacher(data)

    data = {"cmd": "get_tag"}
    res = dispacher(data)
    print(res)

    data = {
        "cmd": "update_tag_all",
        "tags": [
            {"seq": 10, "name": "tag10"},
            {"seq": 20, "name": "tag20"},
        ],
    }
    res = dispacher(data)
    data = {"cmd": "get_tag"}
    res = dispacher(data)
    print(res)


def test_page_create():
    data = {
        "cmd": "create_section",
        "name": "section1",
    }
    section_id = dispacher(data)

    data = {
        "cmd": "create_page",
        "section_id": section_id,
        "subpages": {"a": "xxxxx", "b": "yyyyy"},
        "edge": [],
        "property_id": [],
        "tag_id": [],
        "kv": {},
    }
    res = dispacher(data)

    data = {"cmd": "get_page", "page_id": 1}
    res = dispacher(data)
    print(res)


def test_page():
    test_page_create()
    test_tag()
    test_property()

    data = {
        "cmd": "update_page",
        "page_id": 2,
        "property_id": [3, 4],
        "edge": [1],
        "tag_id": [3],
        "kv": {
            "key1": "val1",
            "key2": "val2",
        },
    }
    res = dispacher(data)
    print(res)

    data = {"cmd": "get_page", "page_id": 2}
    res = dispacher(data)
    print(res)


def test():
    db.resetTables(True)
    # test_property()
    # test_tag()
    test_page()


if __name__ == "__main__":
    test()
