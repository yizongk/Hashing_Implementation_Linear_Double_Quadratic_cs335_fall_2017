To compile on terminal type
  make all

To delete executables and object file type
  make clean

//***********************************

i.   All part of the assignment were completed.
ii.  No bugs were encountered
iii. Be at the correct directory on the terminal, type in "make all" without quotes.
     This will generate two program with it's own way of command line:
         ./CreateAndTestHash <words file name> <query words file name> <flag>
                            words.txt         query_words.txt         linear or quadratic or double
         ./SpellCheck <document file> <dictionary file>
                       document1.txt    wordsEn.txt 

iv.  Input files are four:
         "words.txt" to populate the hash table in first command.
         "query_words.txt" to probe the hash table and find words in query_words.txt.
         "document1.txt" to probe the hash table and find possible corrections.
         "wordsEn.txt" to populate the has table in the second command.  

NOTES:
  * Double Hash Table use R = 83.
  * SpellCheck uses Double Hash Table.
  * To provide the list of candidate corrections, I used all three option, add one char, remove one char and swap adjacent char.
  * To avoid repeating misspelled words, if misspelled word has come up before, it won't be outputed. So if two "hashin" comes up, will only output "hashin" with its corrections ONCE.
      
