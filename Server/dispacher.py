import logging
import json

from db.Db import db



"""
Note properties : note_tag, note_property, note_edge ...

eveytime user clicks them, the changes are not stored to servers.
After web/app changes, user clicks "save", all changes will be delivered to server.

Hence, server deletes the existing setting and add new settings instead of finding diff and 
partial updates

"""


"""
create note

* after add, behavior
1. add, return page_id, get page_id
2. add, return all all infos  (preferred). possible?  how can I get return values?
3. add, return page id. : doesn't work.  picture location is changed from original to mine.

upload json data as POST

http://<host>/<path>/db 


{
    "cmd": "create_note",
    "pages": [
    {
        # add can't get 'id' because it is not created yet
        "type": "a",    <-- question or answer
        "content": "xxxxx" <-- html format
    },
    {
        "type": "b",
        "content": "yyyyy"
    }
    ],
    # for ex. this note can be mapped to 111, 222, 33
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
def create_note(data):
    note_id = db.create_note(None, data["pages"])
    if "edge" in data and data["edge"]:
        db.update_note_edge(note_id, data["edge"], False)

    if "property_id" in data and data["property_id"]:
        db.update_note_property(note_id, data["property_id"], False)

    if "tag_id" in data and data["tag_id"]:
        db.update_note_tag(note_id, data["tag_id"], False)

    if "kv" in data and data["kv"]:
        db.update_note_kv(note_id, data["kv"], False)

    db.commit()

def get_note(data):
    return db.get_note(data["note_id"])


"""
Create property (in the window app)
{
    "cmd": "create_property",
    "name": "xxxxx"
}

return new property id
"""
def create_property(data):
    return db.create_property(data["name"])

"""
Delete property (in the window)
{
    "cmd": "delete_property",
    "property_id" : "yyyy"
}
"""
def delete_property(data):
    db.delete_property_by_id(data["property_id"])

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
    return db.create_tag(data["name"])

"""
Delete tag : in windows app. (not web)
{
    "cmd": "delete_tag",
    "tag_id" : "yyyy" 
}

"""
def delete_tag(data):
    db.delete_tag(data["tag_id"])

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
update note kv
{
    "cmd": "update_note_kv",
    "note_id": "xxxx"
    "kv": {         // should provide full set.  Not partial 
        "ka": "va",
        "kb": "vb"
    }
}

"""
# no partial update.  delete and add all
def update_note_kv(data):  
    db.update_note_kv(data["note_id"], data["kv"])

"""

"""
def get_note_kv(data):
    return db.get_note_kv(data["note_id"])


"""
update note edge

upload json data as POST
http://<host>/<path>/db 
{
    "cmd": "update_note_edge",
    "note_id": "xxxx"
    "dst" : [11, 22, 33, 44]
}

# for note, all settings are delete and add all
"""

def update_note_edge(data):
    db.update_note_edge(data["note_id"], data["dst"])

"""
Get note edge (it will be included in page loading)


{
    "cmd": "get_note_edge"
}

[
    {  note ... (add more)
        "a":
        "b": ..
    },
    {
    }
]
"""
def get_note_edge(data):
    return db.get_note_edges()


"""
Update property 

(web shows all property as checkbox.   if any diff, update)
(if no diff, no update.   check in javascript)

- create "SAVE" button in web.  in clicking it, update tag/property/key,value all
upload json data as POST

http://<host>/<path>/db 


{
    "cmd": "update_note_property",
    "note_id": "xx",
    "property_id": ["11", "22", "3", "5"]
}
"""
def update_note_property(data):
    db.update_note_property(data["note_id"], data["property_id"])

"""
Get note_property (Need it?   In refreshing page, download all info once and for all)

pages + property + tags + .....

Just in case, create it.  but, I don't know whether it will be used.

{
    "cmd": "get_property"
    "note_id": "xxx"
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
def get_note_property(data):
    return db.get_note_property()

"""
Create note_tag / delete note_tag. : NOT needed.

Update tag 

(web shows all tags as checkbox.   if any diff, update)
(if no diff, no update.   check in javascript)

- create "SAVE" button in web.  in clicking it, update tag/property/key,value all
upload json data as POST

http://<host>/<path>/db 


{
    "cmd": "update_note_tag",
    "note_id": "xx",
    "tag_id": ["11", "22", "3", "5"]
}
"""
def update_note_tag(data):
    db.update_note_tag(data["note_id"], data["tag_id"])

"""
Get note_tag (Need it?   In refreshing page, download all info once and for all)

pages + property + tags + .....

Just in case, create it.  but, I don't know whether it will be used.

{
    "cmd": "get_tag"
    "note_id": "xxx"
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
def get_note_tag(data):
    return db.get_note_tag()


cmd_map = {
    "create_note" : create_note,
    "get_note" : get_note,

    "create_property" : create_property,
    "delete_property" : delete_property,
    "update_property" : update_property,
    "update_property_all" : update_property_all,
    "get_property" : get_property,
    
    "create_tag": create_tag,
    "delete_tag": delete_tag,
    "update_tag": update_tag,
    "update_tag_all" : update_tag_all,
    "get_tag": get_tag,

    "update_note_kv": update_note_kv,
    "get_note_kv": get_note_kv,

    "update_note_edge": update_note_edge,
    "get_note_edge": get_note_edge,

    "update_note_property": update_note_property,
    "get_note_property": get_note_property,

    "update_note_tag": update_note_tag,
    "get_note_tag": get_note_tag,
}

def dispacher(data):
    if "cmd" not in data:
        raise Exception("invalid command")

    cmd = data["cmd"]
    if cmd not in cmd_map:
        raise Exception("Invalid command")

    return cmd_map[cmd](data)

def test_property():
    data = { "cmd" : "create_property", "name" : "property1" }
    dispacher(data)
    data = { "cmd" : "create_property", "name" : "property2" }
    dispacher(data)

    # [{'property_id': 1, 'seq': 1, 'name': 'property1'}]
    data = { "cmd" : "get_property" }
    res = dispacher(data)
    print(res)

    data = { "cmd" : "delete_property", "property_id" : "1" }
    dispacher(data)

    data = { "cmd" : "get_property" }
    res = dispacher(data)
    print(res)

    data = {
        "cmd": "update_property_all",
        "properties": [
            {"seq": 10, "name": "prop1"},
            {"seq": 20, "name": "prop2"},
        ]
    }
    res = dispacher(data)
    data = { "cmd" : "get_property" }
    res = dispacher(data)
    print(res)

def test_tag():
    data = { "cmd" : "create_tag", "name" : "tag1" }
    dispacher(data)
    data = { "cmd" : "create_tag", "name" : "tag2" }
    dispacher(data)

    # [{'property_id': 1, 'seq': 1, 'name': 'property1'}]
    data = { "cmd" : "get_tag" }
    res = dispacher(data)
    print(res)

    data = { "cmd" : "delete_tag", "tag_id" : "1" }
    dispacher(data)

    data = { "cmd" : "get_tag" }
    res = dispacher(data)
    print(res)

    data = {
        "cmd": "update_tag_all",
        "tags": [
            {"seq": 10, "name": "tag10"},
            {"seq": 20, "name": "tag20"},
        ]
    }
    res = dispacher(data)
    data = { "cmd" : "get_tag" }
    res = dispacher(data)
    print(res)

def test_create():
    data = {
        "cmd": "create_note",
        "pages": [
            {
                "type": "a",  
                "content": "xxxxx"
            },
            {
                "type": "b",
                "content": "yyyyy"
            }
        ],
        "edge":["111", "222", "333"],
        "property_id":[],
        "tag_id":[],
        "kv": {},        
    }
    res = dispacher(data)
    data = { "cmd" : "get_note", "note_id": 1 }
    res = dispacher(data)
    print(res)


def test():
    db.resetTables(True)
    # test_property()
    # test_tag()
    test_create()

if __name__ == "__main__":
    test()