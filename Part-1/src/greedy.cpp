#include "../include/header.h"

// Greedy Algorithm [returning k-nearest aprx. points]

Dataset greedy_search(Graph_Node s, Data q, int k, int L){
    list<Graph_Node> visited_list;
    list<Graph_Node> searching_list;
    list<Graph_Node> lv;

    searching_list.push_back(s); //The first node of the graph
    lv.push_back(s);
    
    while(!lv.empty()){
        double mindist;
        mindist = euclidean_distance(q,lv.front()->data);
        Graph_Node p;
        for(auto i : lv) {
            double dist = euclidean_distance(i->data,q);
            if(dist<mindist) {
                mindist = dist;
                p = i; 
            }
        }
        for( auto i : p->out_neighbours) {
            searching_list.push_back(i);
        }
        visited_list.push_back(p);
        if(searching_list.size() > L){
            list<Graph_Node> max_list(searching_list.size(),NULL);
            for(auto j : searching_list){
                double dista = euclidean_distance(j->data, q);
                auto it = max_list.front();
                while(it!= NULL){
                    if(euclidean_distance(it->data,q)<=dista){
                        break;
                    }
                    advance(it,1);
                }
                // if(it==NULL){
                //     max_list.assign()
                // }
                
            }
        }
    }
    
}