# ldb_bypass

**roadmap:**
- external memory management:
	- attach to application :white_check_mark:
	- read/write bytes :white_check_mark:
	- scan for patterns found in ida :white_check_mark:

- application bypasses:
	- find pattern in ida :white_check_mark:
	- patch necessary bytes in memory to bypass specific checks/callbacks :white_check_mark:
	
- spefic bypasses:	
	- keyboard callbacks ( unblocks alt tab, windows keys, etc ) :white_check_mark: 
	- mouse callbacks ( unblocks double click, right click, middle, etc ) :white_check_mark:
	- shell callback ( unsure, it filters what i think is WH_MOUSE and WH_CALLWNDPROCRET. might aswell bypass it :shrug: ) :white_check_mark:

***NOTE***: 
this is a work in progress, and will be updated as i continue to reverse the browser. i will be adding more bypasses as i find them.
as of NOW, i have only tested OUTSIDE of quizzes. i am 90% sure that there is an integrity check somewhere. after i add some more patches, i'll look for it

**i'm thinking of abandoning this for internal atm**