//
//  main.cpp
//  election
//
//  Created by John Maxey on 3/29/17.
//  Copyright © 2017 John Maxey. All rights reserved.
//

#include <iostream>
#include <getopt.h>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <limits>
#include "MST.h"
#include "TSP.h"
#include "KNAP.h"
using namespace std;

string getMode(int argc, char * argv[]);
//double findCampaign(vector<State> &states, double budget, int n, int votes, vector<int> &visited);

int main(int argc, char * argv[]) {
    ios_base::sync_with_stdio(false);
    ostringstream os;
    cout << setprecision(2); //always show 2 decimal places
    cout << fixed; //disable scientific notation for large numbers
    os << setprecision(2);
    os << fixed;
    
    string mode = "CAMPAIGN";//getMode(argc, argv); //get mode to run: CAMPAIGN, MST, or PATH
    
    if(mode != "CAMPAIGN" && mode != "MST" && mode != "PATH"){
        cerr << "Invalid mode specified" << endl;
        exit(1);
    }
    
    vector<int> first;
    first.push_back(1);
    first.push_back(2);
    
    vector<int> second = first;
    
    
    if(mode == "CAMPAIGN"){
        string dummy;
        char dollar;
        int numberOfStates;
        double budget;
        cin >> dummy >> dummy >> dummy >> dummy;
        cin >> numberOfStates;
        
        cin >> dummy >> dummy >> dummy;
        cin >> dollar;
        cin >> budget;
        if(numberOfStates < 0){
            cerr << "Number of states must a non-negative integer" << endl;
            exit(1);
        }
        
        double cost;
        int votes;
        cin >> dollar >> cost >> votes;
        if(cost <= 0){
            cerr << "Cost must be a positive decimal value" << endl;
            exit(1);
        }
        if(votes <= 0){
            cerr << "Votes must be a positive integer value" << endl;
            exit(1);
        }
        KNAP campaign(cost, votes, numberOfStates, budget);
        
        //read in all states costs/votes
        for(int i = 1; i < numberOfStates; ++i){
            cin >> dollar >> cost >> votes;
            if(cost <= 0){
                cerr << "Cost must be a positive decimal value" << endl;
                exit(1);
            }
            if(votes <= 0){
                cerr << "Votes must be a positive integer value" << endl;
                exit(1);
            }
            campaign.addState(cost, votes, i);
        }
        
        //solve knapsack problem
        campaign.solve();
        //print out results
        campaign.printResults(os);
        
    }else if(mode == "MST"){
        
        string dummy;
        int numPosts;
        cin >> dummy >> dummy >> dummy >> dummy;
        cin >> numPosts;
        if(numPosts <= 0){
            cerr << "Number of states must a positive integer" << endl;
            exit(1);
        }
        
        int x;
        int y;
        cin >> x >> y;
        MST posts(numPosts, x, y);
        //get all states coordinates
        for(int i = 1; i < numPosts; ++i){
            cin >> x >> y;
            posts.addPost(x, y, i);
        }
        
        double difficulty;
        int numDifficult;
        vector<int> diffPosts;
        cin >> dummy >> dummy >> dummy;
        cin >> numDifficult;
        cin >> dummy >> dummy;
        cin >> difficulty;
        
        if(numDifficult < 0){
            cerr << "Number of difficult posts must be non-negative integer" << endl;
            exit(1);
        }else if(difficulty <= 0){
            cerr << "Difficulty must be positive" << endl;
            exit(1);
        }
        
        //get posts with difficulty
        diffPosts.reserve(numDifficult);
        for(int i = 0; i < numDifficult; ++i){
            int nextPost;
            cin >> nextPost;
            if(nextPost < 0 || nextPost >= numPosts){
                cerr << "Index in difficulty list must be between 0 and number of states" << endl;
            }
            diffPosts.push_back(nextPost);
        }
        
        //adjust difficulties
        posts.difficulty(difficulty, diffPosts);
        //solve MST
        posts.solve();
        //print out results
        posts.print(os);
        
    }else if(mode == "PATH"){
        
        string dummy;
        int numStates;
        cin >> dummy >> dummy >> dummy >> dummy;
        cin >> numStates;
        if(numStates < 0){
            cerr << "Number of states must a non-negative integer" << endl;
            exit(1);
        }
        
        int x;
        int y;
        cin >> x >> y;
        TSP states(x, y, numStates);
        //get coordinates
        for(int i = 1; i < numStates; ++i){
            cin >> x >> y;
            states.addState(x, y, i);
        }
        
        //get luum's path
        cin >> dummy >> dummy >> dummy;
        vector<int> luum;
        luum.reserve(numStates);
        for(int i = 0; i < numStates; ++i){
            int next;
            cin >> next;
            if(next < 0 || next >= numStates){
                cerr << "Node in Luum path must be between 0 and number or states" << endl;
                exit(1);
            }
            luum.push_back(next);
        }
        
        //check to see if luum list hits all nodes
        sort(luum.begin(), luum.end());
        for(int i = 0; i < numStates; ++i){
            if(luum[i] != i){
                cerr << "Luum path must visit every node" << endl;
                exit(1);
            }
        }
        
        //calculate luum path distance
        states.luumDistance(luum);
        //solve tsp
        states.solve();
        //print out results
        states.printResults(os);
    }
    
    cout << os.str();
    return 0;
}

/*
 double findCampaign(vector<State> &states, double budget, int n, vector<int> &visited){
 if(n == 0 || budget < 0){
 return 0;
 }
 
 if(states[n-1].cost > budget){
 findCampaign(states, budget, n-1, visited);
 }else{
 return maxVotes(states[n-1].votes + findCampaign(states, budget-states[n-1].cost, n, visited), )
 }
 }
 */


////////////////////
//GETMODE FUNCTION//
////////////////////
string getMode(int argc, char * argv[]){
    string mode = "none";
    // These are used with getopt_long()
    opterr = true; // Give us help with errors
    int choice;
    int option_index = 0;
    static struct option long_options[] = {
        { "mode",         required_argument,        nullptr, 'm'},
        { nullptr,        0,                		nullptr, '\0'}
    };
    
    // TODO: Fill in the double quotes, to match the mode and help options.
    while ((choice = getopt_long(argc, argv, "m:", long_options, &option_index))
           != -1) {
        switch(choice) {
            case 'm':
                mode = optarg;
                break;
        } // switch
    } // while
    
    return mode;
}
///////////////////
//END OF GET MODE//
///////////////////
