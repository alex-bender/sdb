#!/usr/bin/python

from SimpleDB import *

db = Sdb(None, "test.sdb", False)
db.set("foo", "World",0)
print("Hello "+db.get("foo", None))
db.query(b"foo=Patata")
print("--> "+db.querys(("foo").decode("utf-8"), 0, None))
db.sync()
