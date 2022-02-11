Command

##########################################################
New page interaction

click "+"

- save current page
- empty question / answner 
- reset all properties (tag, ...)

##########################################################
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

server behave:

- store pages
- load and return all info. but how load? not page_id.  (it's same as "get").  kind of page refresh

NO.  
- return only note_id.  because, in adding pages, pictures are in remote site. not this db.
after refreshing, pic will be replaced with db's

##########################################################
Update note

upload json data as POST

http://<host>/<path>/db 


client(web javascript) checks it's dirty or not.  If not dirty, skip
 * can use md5.

 important.  Editor doesn't have a way to detect modification.  If 
 modified, save to db, otherwise skip

{
    "cmd": "update_note",
    "note_id": 1111,  
    "pages": [      // only need page_id, content
    {
        "page_id": 1234,
        "type": "a",
        "content": "xxxxx"
    },
    {
        "page_id": 2345,
        "type": "b",
        "content": "yyyyy"
    }
    ],
    "edge":["111", "222", "333"],
    "property_id":[],
    "tag_id":[],
    "kv": {},
}


server behave:  same as "add" command.  If update has new pic which is in remote site, it should be added to this db.

- store pages
- load and return all info.  (it's same as "get").  kind of page refresh


##########################################################

send delete and update with next one.  If delete last, update with previous.
- who decides next or previous?   server or client?


upload json data as POST

http://<host>/<path>/db 


Delete note

{
    "cmd": "delete", 
    "note_id": 1111,
}


##########################################################
Create tag : in windows app. (not web)

upload json data as POST

http://<host>/<path>/db 


{
    "cmd": "create_tag",
    "name": "xxxxx"
}

##########################################################
Delete tag : in windows app. (not web)



{
    "cmd": "delete_tag",
    "name": "xxxxx"
    or 
    "tag_id" : "yyyy" 
}

##########################################################
Update tag in windows app (Not web)

{
    "cmd": "update_tag",
    "id": "xxxxx"
    "name" : "new_name"
}

##########################################################
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



##########################################################
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

##########################################################
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


##########################################################
Create property (in the window app)

upload json data as POST

http://<host>/<path>/db 


{
    "cmd": "create_property",
    "name": "xxxxx"
}

##########################################################
Delete property (in the window)


{
    "cmd": "delete_property",
    "property_id" : "yyyy"
}


##########################################################
update property (in the window)


{
    "cmd": "delete_property",
    "name": "xxxxx"
    "property_id" : "yyyy"
}

##########################################################
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

##########################################################
Create note_property / delete note_property. : NOT needed.

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

##########################################################
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


##########################################################


update note edge

upload json data as POST

http://<host>/<path>/db 

{
    "cmd": "update_note_edge",
    "note_id": "xxxx"
    "dst" : [11, 22, 33, 44]
}

##########################################################
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




##########################################################
NOT clear idea yet.  (2021/09/16)
09/20 : concept is similar to tag/property. ex.  I want leetcode tag.  but, tag / property are for question.
ex. leetcode. company(google, fb)

update note kv

upload json data as POST

http://<host>/<path>/db 

{
    "cmd": "update_note_kv",
    "note_id": "xxxx"
    "kv": {         // should provide full set.  Not partial 
        "ka": "va",
        "kb": "vb"
    }
}

##########################################################
Delete kv



##########################################################
Get tag


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

##########################################################



