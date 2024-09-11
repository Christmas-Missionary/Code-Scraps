This was from r/dailyprogrammer challenge 100, where you must load up a list of words to censor,  
then censor those words when inputted into the terminal.  

Couple flags are --censor, --new --quit, and --reload.  

--censor:  
Changes the ASCII character used for censored content.  
`--censor &`  

--new:  
Loads a new list.  
    --new  
    Please drag and drop the list you want.  
    '/the/absolute/path/to/the/list/you/want/list.txt'  

List doesn't have to be `list.txt`, nor do you need singular quotes around the file path.  

--quit:  
Quits the program. No further input needed.  
`--quit`  

--reload:  
Reloads the current list. No further input needed.  
`--reload`.  
