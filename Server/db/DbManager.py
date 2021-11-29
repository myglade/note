import db

"""
Add
{
    "cmd": "add",
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

    # seldom change.  difficulty, area(array, dp, ..) could be multiple
    "property":[],

    # can change often.  how many failures
    "tag":[],

    # Leecode/company?  key/value
    "kv": {},

}

return note_id


Update
client(web javascript) checks it's dirty or not.  If not dirty, skip
 * can use md5.

 important.  Editor doesn't have a way to detect modification.  If 
 modified, save to db, otherwise skip

{
    "cmd": "update",
    "note_id": 1111,
    "pages": [
    {
        "id": 1234,
        "type": "a",
        "content": "xxxxx"
    },
    {
        "id": 2345,
        "type": "b",
        "content": "yyyyy"
    }
    ],
    "edge":["111", "222", "333"],
    "property":[],
    "tag":[],
    "kv": {},
}

Delete 
{
    "cmd": "delete", 
    "note_id": 1111,
}

new registration of tag, property .... 
will be done in c++ app. Not web ui

Don't try to implement various functions with javascript.   
Just editor and store.   The rest of functions like creating new tag,
property can be implemented in c++




Next item with current condition.  
curid = 1, properties = , tag=, ....
Get 
{

}


list
{

}

Summary {

}

"""


PAGES = "pages"
PAGE_A = "a"
PAGE_B = "b"

class DbManager:
    db = Db("note.db")

    @classmethod
    def add_note(cls, req):
        """
        {
          "cmd": "add_note",
          "pages": [
            {
              "id": null,
              "type": "a",
              "content": "xxxxx"
            },
            {
              "id": null,
              "type": "b",
              "content": "yyyyy"
            }
          ]
        }
        """
        if PAGES not in req:
            raise ValueError("invalid parameter")

        note_id = cls.db.add_note(None, req[PAGES])

    @classmethod
    def update_note(cls, req):
        """
        {
          "cmd": "update",
          "pages": [
            {
              "id": 1234,
              "type": "a",
              "content": "xxxxx"
            },
            {
              "id": 2345,
              "type": "b",
              "content": "yyyyy"
            }
          ],

        }
        """



