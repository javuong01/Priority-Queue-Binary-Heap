#include <string>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "json.hpp"    //remember to use nlohmann::
#include <bits/stdc++.h>

#include "priorityqueue.h"

int main(int argc, char** argv) {

    std::ifstream file;     //a file stream is open under the name of file
    file.open(argv[1]);     //The inputted json is opened in the file stream
    nlohmann::json jsonObject;    //A jsonobject called jsonObject is made
    if (file.is_open()) {         //This if statement checks if the file opened actually opened
        file >> jsonObject;       //the file opened in file is put into the json object
    }    

    std::ifstream file2;                //a file stream is open under the name of file2
    file2.open("heap_result.json");     //A new json is made and opened in the file2 stream
    nlohmann::json jsonResult;        //A jsonobject called jsonResult is made
    if (file2.is_open()) {             //This if statement checks if the file opened actually opened
        file2 >> jsonResult;         //the file opened in file2 is put into the json object
    }    
    
    size_t maxSize = jsonObject["metadata"]["maxHeapSize"];        //This gets the maxsize for the priority queue from the designated maxHeapSize inside the jsonObject
    auto* heap = new PriorityQueue(maxSize);                       //This initializes the priority queue
    Key keyValue;                                                  //This initializes a variable to hold a key type value
    
    for (auto itr = jsonObject.begin(); itr != jsonObject.end(); ++itr) {            //This for loop iterates through the keys of the inputted json
        if (itr.key() == "metadata"){                                              //If the key that the for loop is iterating through is metadata
            break;                                                                  //Break the for loop when the key is metadata
        }
        if (jsonObject[itr.key()]["operation"] == "insert"){                              //If the operation key has value insert
            keyValue = jsonObject[itr.key()]["key"];                                //the keyvalue gains the value of the key "key" in the json
            heap->insert(keyValue);                                                  //The key is inserted into the heap and is given a value of (0,0) to make the keyvaluepair
        } 
        if (jsonObject[itr.key()]["operation"] == "removeMin"){                      //if the operation key has value removemin
            heap->removeMin();                                                    //The root of the heap which has the minimum key is removed from the heap
        }       
    }
    
    jsonResult = heap->JSON();                                                    //json object jsonResult now has a json formatted heap written in it
    jsonResult["metadata"]["numOperations"] = jsonObject["metadata"]["numOperations"];                  //meta data for the number of operations is printed into jsonResult
    std::cout << jsonResult.dump(2) << "\n";    //This prints the jsonResult into the terminal
    file.close();                              //This closes the first file stream
    file2.close();                            //This closes the second file stream

    delete heap;                                                                    //The heap is deleting to free up the memory
}
