//
//  TSP.h
//  election
//
//  Created by John Maxey on 4/3/17.
//  Copyright © 2017 John Maxey. All rights reserved.
//
#include <climits>
#include <math.h>
#include <vector>
#include <algorithm>
#include <deque>

#ifndef TSP_h
#define TSP_h

class TSP {
    struct State {
        double distance;
        int x;
        int y;
        int index;
        int prevState;
        bool visited;
    };
    
public:
    //constructor
    TSP(int xIn, int yIn, int numStates){
        states.reserve(numStates);
        order.reserve(numStates);
        State nextState;
        nextState.x = xIn;
        nextState.y = yIn;
        nextState.index = 0;
        nextState.prevState = 0;
        nextState.distance = std::numeric_limits<double>::infinity();
        nextState.visited = true;
        states.push_back(nextState);
    }
    
    //add state to vector
    void addState(int xIn, int yIn, int indexIn){
        State nextState;
        nextState.x = xIn;
        nextState.y = yIn;
        nextState.index = indexIn;
        nextState.prevState = -1;
        nextState.visited = false;
        nextState.distance = std::numeric_limits<double>::infinity();
        states.push_back(nextState);
    }
    
    //get distance between states
    inline double distance(const State &x, const State &y){
        return sqrt((x.x-y.x)*(x.x-y.x) + (x.y-y.y)*(x.y-y.y));
    }
    
    //get luum path distance
    void luumDistance(std::vector<int> &luum){
        least = 0;
        for(size_t i = 0; i < states.size() - 1; ++i){
            least += distance(states[luum[i]], states[luum[i+1]]);
            order.push_back(luum[i]);
        }
        order.push_back(luum[states.size()-1]);
        least += distance(states[luum[0]], states[luum[states.size()-1]]);
    }
    
    //solve TSP
    void solve(){
        std::deque<State> unused;
        std::vector<State> path;
        path.reserve(states.size());
        path.push_back(states[0]);
        for(size_t i = 1; i < states.size(); ++i){
            unused.push_back(states[i]);
        }
        genPerms(unused, path);
    }
    
    //generate permutations to find best path
    void genPerms(std::deque<State> &unused, std::vector<State> &path){
        //see if there are not any unused left
        if(unused.empty()){
            double total = 0;
            for(size_t i = 0; i < states.size() - 1; ++i){
                total += distance(path[i], path[i+1]);
            }
            total += distance(path[0], path[states.size() - 1]);
            
            if(total < least){
                least = total;
                for(size_t k = 0; k < states.size(); ++k){
                    order[k] = path[k].index;
                }
            }
        }
        
        //check to see if remaining states are promising
        if(path.size() > 2 && !promising(unused, path)){
            return;
        }
        
        for(size_t j = 0; j < unused.size(); ++j){
            path.push_back(unused.front());
            unused.pop_front();
            genPerms(unused, path);
            unused.push_back(path.back());
            path.pop_back();
        }
    }
    
    //check to see if remaining states are promising in tsp
    bool promising(std::deque<State> &unused, std::vector<State> &path){
        mst.clear();
        
        MST(unused[0].x, unused[0].y);
        for(size_t i = 1; i < unused.size(); ++i){
            addMSTState(unused[i].x, unused[i].y);
        }
        
        solveMST();
        
        //get lower bound
        double tot = 0;
        for(size_t i = 0; i < path.size() - 1; ++i){
            tot += distance(path[i], path[i+1]);
        }
        
        for(size_t i = 0; i < mst.size(); ++i){
            tot += mst[i].distance;
        }
        
        //get shortest distance from end points of path to unused
        double least1 = std::numeric_limits<double>::infinity();
        double least2 = std::numeric_limits<double>::infinity();
        for(size_t k = 0; k < mst.size(); ++k){
            double dist1 = sqrt(((path[0].x - mst[k].x) * (path[0].x - mst[k].x))
                                + ((path[0].y - mst[k].y) * (path[0].y - mst[k].y)));
            if(dist1 < least1){
                least1 = dist1;
            }
        }
        
        for(size_t k = 0; k < mst.size(); ++k){
            double dist2 = sqrt(((path[path.size()-1].x - mst[k].x) *
                                 (path[path.size()-1].x - mst[k].x)) +
                                ((path[path.size()-1].y - mst[k].y) *
                                 (path[path.size()-1].y - mst[k].y)));
            if(dist2 < least2){
                least2 = dist2;
            }
        }
        tot = tot + least1 + least2;
        
        if(tot > least){
            return false;
        }else{
            return true;
        }
    }
    
    //print out results
    void printResults(std::ostringstream &os){
        os << "The total campaign length is: " << least << '\n';
        os << "Lukefahr should visit each state in the following order: " << '\n';
        
        for(size_t i = 0; i < order.size(); ++i){
            os << order[i] << '\n';
        }
    }
    
    //////////////////////////////////////////
    //////////////////////////////////////////
    //////////////////////////////////////////
    
    void MST(int xIn, int yIn){
        State nextState;
        nextState.x = xIn;
        nextState.y = yIn;
        nextState.prevState = 0;
        nextState.distance = 0;
        nextState.index = 0;
        nextState.visited = true;
        mst.push_back(nextState);
    }
    
    void addMSTState(int xIn, int yIn){
        State nextState;
        nextState.x = xIn;
        nextState.y = yIn;
        nextState.distance = std::numeric_limits<double>::infinity();
        nextState.prevState = -1;
        nextState.index = 0;
        nextState.visited = false;
        mst.push_back(nextState);
    }
    
    //solve MST
    void solveMST(){
        size_t size = mst.size();
        size_t i = 0;
        int lastState = 0;
        while(i < size){
            for(size_t j = 0; j < size; ++j){
                if(mst[j].visited == false){
                    
                    double dist = distance(mst[lastState], mst[j]);
                    if(dist < mst[j].distance){
                        mst[j].distance = dist;
                        mst[j].prevState = lastState;
                    }
                }
            } //for()
            
            double least = std::numeric_limits<double>::infinity();
            for(size_t k = 0; k < size; ++k){
                if(mst[k].visited == false && mst[k].distance < least){
                    least = mst[k].distance;
                    lastState = int(k);
                }
            } //for()
            //std::cout << least << '\n';
            mst[lastState].visited = true;
            ++i;
        } //while()
    } //solve()
    
private:
    std::vector<State> states;
    std::vector<int> order;
    double least;
    std::vector<State> mst;
};

#endif /* TSP_h */
