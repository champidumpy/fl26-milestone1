# M1 DESIGN.md

Replace this template with your own concise engineering explanation.

## 1. System structure
Describe the major responsibilities in your M1 subsystem and how they interact.
The major responsibilities in the subsystem are tokenization and normalization which happen in text processor .cpp file, which is sent to chunker to be turned into chunks. Corpus Index .cpp makes it able to be searched in the order they occur. Retrieval engine does the query processing and gets the rankings. Context builder puts everything together and puts the top ranked chunks together. 
## 2. Design decisions
Explain the principal data structures, interfaces, and ownership decisions in your implementation and why you selected them.
Ownership is in processing_core.cpp, since that was where everything would be used, temporary data was only used locally. Main data structures used were vectors and unordered map once. 
## 3. Correctness and consistency
Identify the important invariants or failure cases your design must preserve and explain how your design addresses them.
The important invariants and fail cases my design perserves are making sure normalization happens through textprocessor and making sure chunks terminate. This is done by not allowing the next chunk start if it goes over the limit. 
## 4. Testing strategy
Explain what your tests cover and which risks or boundaries you considered most important.
My test checks one of the examples "R2-D2" to see if it matches what the milestones example shows. I wanted to test to see if it works with the main examples for the normalization in the milestone specification. 
## 5. Alternatives considered
Discuss at least two plausible design alternatives and why you did not choose them.
One design alternative was to use global and static variables and structures for certain .cpp files, I felt it would lead to more errors with multiple instances. So I kept the original design. 

Another design alternative is to implement the majority of the project in Processing core, but that would lead to a lot of changes of the other files and would be too much work for little benefit. 