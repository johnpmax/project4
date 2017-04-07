//
//  KNAP.h
//  election
//
//  Created by John Maxey on 4/4/17.
//  Copyright © 2017 John Maxey. All rights reserved.
//
#include <climits>
#include <math.h>
#include <vector>
#include <algorithm>

#ifndef KNAP_h
#define KNAP_h

class KNAP {
    struct State {
        double cost;
        int votes;
        int index;
    };
    
public:
    //constructor
    KNAP(double costIn, int votesIn, int numStatesIn, double budgetIn){
        budget = budgetIn;
        this->bestVotes = 0;
        this->numStates = numStatesIn;
        totalVotes = 0;
        State nextState;
        states.reserve(numStates);
        nextState.cost = costIn;
        leastCost = costIn;
        nextState.votes = votesIn;
        nextState.index = 0;
        states.push_back(nextState);
    }
    
    //add states to vector
    void addState(double costIn, int votesIn, int indexIn){
        if(costIn < leastCost){
            leastCost = costIn;
        }
        State nextState;
        nextState.cost = costIn;
        nextState.votes = votesIn;
        nextState.index = indexIn;
        states.push_back(nextState);
    }
    
    //solve knapsack
    void solve(){
        //std::cout << "least: " << leastCost << '\n';
        std::vector<int> visited;
        totalVotes = knapsack(budget, numStates, visited);
        //std::cout << "votes: " << totalVotes << '\n';
    }
    
    //recursively solve knapsack problem
    int knapsack(int currentBudget, int n, std::vector<int> visited){
        if(n == 0 || currentBudget < leastCost){
            int currentTotal = 0;
            for(size_t i = 0; i < visited.size(); ++i){
                currentTotal += states[visited[i]].votes;
            }
            if(currentTotal > bestVotes){
                bestVotes = currentTotal;
                best_visited = visited;
            }
            return 0;
        }
        
        if(states[n-1].cost > currentBudget){
            return knapsack(currentBudget, n-1, visited);
        }else{
            std::vector<int> visCopy = visited;
            visited.push_back(n-1);
            return maxVotes((states[n-1].votes + knapsack(currentBudget - states[n-1].cost, n-1, visited)), (knapsack(currentBudget, n-1, visCopy)));
        }
    }
    
    //helper function to determine max
    int maxVotes(int a, int b){
        if(a >= b){
            return a;
        }else{
            return b;
        }
    }
    
    //print results
    void printResults(std::ostringstream &os){
        os << "Expected number of votes on Election day: " << totalVotes << '\n';
        os << "Senator Lukefahr should campaign in the following states:" << '\n';
        std::sort(best_visited.begin(), best_visited.end());
        for(size_t i = 0; i < best_visited.size(); ++i){
            os << best_visited[i] << '\n';
        }
    }
    
public:
    std::vector<State> states;
    //std::vector<int> order;
    double budget;
    double leastCost;
    int totalVotes;
    int numStates;
    
    int bestVotes;
    std::vector<int> best_visited;
};

#endif /* KNAP_h */
