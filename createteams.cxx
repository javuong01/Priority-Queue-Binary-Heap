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
    file2.open("team_result.json");     //A new json is made and opened in the file2 stream
    nlohmann::json jsonResult;        //A jsonobject called jsonResult is made
    if (file2.is_open()) {             //This if statement checks if the file opened actually opened
        file2 >> jsonResult;         //the file opened in file2 is put into the json object
    }

    size_t maxSize = jsonObject["teamStats"].size();        //setting size of the priority queue to be the number of team combinations in "teamstats"
    auto* heap = new PriorityQueue(maxSize);                //initializing a priority queue

    int nTeams = maxSize;                                  //initializing nTeams as a integer variable with the value of maxsize to use as part of a for loop
    double win_percentage;                                //Initializing a variable to hold a double to become a key for a keyvalue pair
    int playerOne, playerTwo;                              //initializing two integer variables to hold data for the values in keyvalue pair
    KeyValuePair two_team, fair_team;                    //initializing two variables of the keyvariablepair type. 


    for (int i = 0; i < nTeams; i++){                                        //This for loop iterates through array from "teamStats" in the json
        win_percentage = jsonObject["teamStats"][i]["winPercentage"];        //This collects the winpercentage from the element 
        win_percentage = 50 - win_percentage;                                //This gets the difference of the win percentage from 50 to find how close the win percentage is from 50
        if (win_percentage < 0){                                          //If the difference is negative
            win_percentage = win_percentage - (2*win_percentage);           //This makes the difference a positive number
        }
        playerOne = jsonObject["teamStats"][i]["playerOne"];               //This collects the player number from player one
        playerTwo = jsonObject["teamStats"][i]["playerTwo"];              //This collects the player number from player two
        two_team = std::make_pair(win_percentage, std::make_pair(playerOne, playerTwo));             //This turns the edited win percentage and player numbers into a keyvaluepair with winpercentage as the key and the player numbers as the value
        heap->insert(two_team);                                                      //The keyvaluepair is inserted into the heap
    }

    std::vector<int> usedPlayerNums;                                //A vector holding integers is initialized to hold the numbers of players that are already used
    std::vector<int> goodTeam;                                     //This vector holds two integers of a team being picked as fair
    std::vector<std::vector <int>> finalTeam;                     //This vector holds vectors of the final team pairings
    int usedSize, notUsed;                                       //usedSize is an integer to hold the size of the vector usedPlayerNums. notUsed is a variable used to signal whether or not to use the team pairing as a fair team


        //check if team numbers are in vector. if not, print team
        //add team numbers into vector if they dont exist
    while (heap->isEmpty() != true){                                  //The while loop continues if the heap is not empty
        notUsed = 1;                                                   //notUsed is set as 1. if it stays as one, then the team pairing is kept
        fair_team = heap->removeMin();                                  //Removing the fairest team's keyvaluepair and saving it as fair_team
        if (usedPlayerNums.empty()){                                   //if the vector usedPlayerNums is empty
            usedPlayerNums.push_back(fair_team.second.first);                  //the first team number of the pair "value" is saved into usedPlayerNums
            usedPlayerNums.push_back(fair_team.second.second);             //the second team number of the pair "value" is saved into usedPlayerNums
            goodTeam.push_back(fair_team.second.first);                  //the first team number of the pair "value" is saved into goodTeam
            goodTeam.push_back(fair_team.second.second);                 //the second team number of the pair "value" is saved into goodTeam
            finalTeam.push_back(goodTeam);                              //the team pairing in good team is added into the vector finalTeam
            continue;                                                     //The while loop restarts as the rest of the while loop is unnecessary for the first loop
        }
        usedSize = usedPlayerNums.size();                              //setting the usedSize to the size of the vector usedPlayerNums
        for (int j = 0; j < usedSize; j++){                             //a for loop that checks the values of the currently removed team to see if any players that were already used are being used again
            if (fair_team.second.first == usedPlayerNums[j]             //if the first value/ team player or the second value/ team plater are also in usedPlayerNums
                || fair_team.second.second == usedPlayerNums[j]){
                    notUsed = 0;                                        //notUsed is set to 0 to prevent this team combination from being put into the final list of teams
                    break;                                               //Break is used since there is no point in comparing more used numbers if there already is a match
            }

        }
        if (notUsed == 1){                                                 //if notUsed is still 1 and not changed to 0 from the forloop that checks for used player numbers
            usedPlayerNums.push_back(fair_team.second.first);               //the first team player's number is inserted into the vector of used Player Numbers
            usedPlayerNums.push_back(fair_team.second.second);             //the second team player's number is inserted into the vector of used Player Numbers
            goodTeam.clear();                                             //The contents of goodTeam is cleared in order to fit in the fair team
            goodTeam.push_back(fair_team.second.first);                    //the first team number of the pair "value" is saved into goodTeam
            goodTeam.push_back(fair_team.second.second);                    //the second team number of the pair "value" is saved into usedPlayerNums
            finalTeam.push_back(goodTeam);                                   //the team pairing in good team is added into the vector finalTeam
        }
    }

    jsonResult["teams"] = finalTeam;                    //The vector of vector of integers finalTeam is written into jsonResult as the value of the key "teams"  This prints the final list of fair team pairings 

    std::cout << jsonResult.dump(2) << "\n";    //This prints the jsonResult into the terminal
    file.close();                              //This closes the first file stream
    file2.close();                            //This closes the second file stream

    delete heap;                                //The heap is deleting to free up the memory
}