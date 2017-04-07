//
//  MST.h
//  election
//
//  Created by John Maxey on 4/2/17.
//  Copyright © 2017 John Maxey. All rights reserved.
//
#include <climits>
#include <math.h>
#include <vector>
#include <algorithm>

#ifndef MST_h
#define MST_h

class MST {
    struct Post {
        double distance;
        double difficulty;
        int x;
        int y;
        int index;
        int prevPost;
        bool visited;
    };
    
    /*struct sort_Post{
     bool operator()(const Post &first, const Post &second) const{
     return first.distance >= second.distance;
     }
     }sortPosts;
     */
public:
    //constructor
    MST(int numStates, int xIn, int yIn){
        posts.reserve(numStates);
        Post nextPost;
        nextPost.difficulty = 1;
        nextPost.x = xIn;
        nextPost.y = yIn;
        nextPost.prevPost = 0;
        nextPost.distance = 0;
        nextPost.index = 0;
        nextPost.visited = true;
        posts.push_back(nextPost);
    }
    
    //add new state to states vector
    void addPost(int xIn, int yIn, int indexIn){
        Post nextPost;
        nextPost.x = xIn;
        nextPost.y = yIn;
        nextPost.difficulty = 1;
        nextPost.prevPost = -1;
        nextPost.distance = std::numeric_limits<double>::infinity();
        nextPost.index = indexIn;
        nextPost.visited = false;
        posts.push_back(nextPost);
    }
    
    //adjust difficulty for states
    void difficulty(double diffFactor, std::vector<int> &update){
        for(size_t i = 0; i < update.size(); ++i){
            posts[update[i]].difficulty = diffFactor;
        }
    }
    
    //update distance between two states
    inline double distance(const Post &first, const Post &second){
        return (first.difficulty * second.difficulty * sqrt((first.x - second.x)*(first.x - second.x) + (first.y - second.y)*(first.y - second.y)));
    }
    
    //solve MST
    void solve(){
        size_t size = posts.size();
        size_t i = 0;
        int lastPost = 0;
        while(i < size){
            for(size_t j = 0; j < size; ++j){
                if(posts[j].visited == false){
                    
                    double dist = distance(posts[lastPost], posts[j]);
                    if(dist < posts[j].distance){
                        posts[j].distance = dist;
                        posts[j].prevPost = lastPost;
                    }
                }
            } //for()
            
            double least = std::numeric_limits<double>::infinity();
            for(size_t k = 0; k < size; ++k){
                if(posts[k].visited == false && posts[k].distance < least){
                    least = posts[k].distance;
                    lastPost = int(k);
                }
            } //for()
            //std::cout << least << '\n';
            posts[lastPost].visited = true;
            ++i;
        } //while()
    } //solve()
    
    //print out results of MST
    void print(std::ostringstream &os){
        //sort(posts.begin(), posts.end(), sortPosts);
        double total = 0;
        for(size_t i = 0; i < posts.size(); ++i){
            //std::cout << posts[i].distance << '\n';
            total += posts[i].distance;
        }
        
        os << "Total distance: " << total << '\n';
        os << "Trade routes:" << '\n';
        
        int k = 0;
        for(size_t j = 0; j < posts.size(); ++j){
            if(k < posts[j].prevPost){
                os << k << " " << posts[j].prevPost << '\n';
            }else if(k > posts[j].prevPost){
                os << posts[j].prevPost << " " << k << '\n';
            }
            ++k;
        }
    }
    
private:
    std::vector<Post> posts;
};

#endif /* MST_h */
