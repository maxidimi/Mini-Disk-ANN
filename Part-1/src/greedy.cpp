#include "../include/header.h"

// Greedy Algorithm [returning k-nearest aprx. points]

pair<list<Graph_Node>,list<Graph_Node>> greedy_search(Graph_Node s, Data q, int k, int L){
    list<Graph_Node> visited_list;
    list<Graph_Node> searching_list;
    list<Graph_Node> lv;

    searching_list.push_back(s); //The first node of the graph
    lv.push_back(s);
    
    while(!lv.empty()){
        double mindist;
        mindist = euclidean_distance(q,lv.front()->data);
        Graph_Node p = lv.front();
        for(auto i : lv) {
            double dist = euclidean_distance(i->data,q);
            if(dist<mindist) {
                mindist = dist;
                p = i; 
            }    
        }
        for( auto i : p->out_neighbours) {
            if(find_node_in_graph(searching_list,i->data) == NULL) {   
                searching_list.push_back(i);
            }
            if(find_node_in_graph(lv,i->data) == NULL) {   
                lv.push_back(i);
            }
        }
        if(find_node_in_graph(visited_list,p->data) == NULL) {   
            visited_list.push_back(p);
        }
        if (static_cast<int>(searching_list.size()) > L) {
            list<Graph_Node> max_list;   
            for(auto j : searching_list){
                double dista = euclidean_distance(j->data, q);
                int index = 0;
                for(auto i : max_list){
                    print_out_neighbours(i);
                    if(euclidean_distance(i->data,q)<=dista){
                        break;
                    }
                    index++;
                }
                if(index == (int)max_list.size()){
                    max_list.push_back(j);
                }
                else{
                    list<Graph_Node>::iterator it = max_list.begin();
                    advance(it,index);
                    max_list.insert(it,j);
                }
            }
            for(int i = 1; i <= ((int)(searching_list.size()) - k); i++) {
                max_list.pop_front();
            }
            searching_list.clear();
            searching_list.assign(max_list.begin(),max_list.end());
        }
        for(auto i : visited_list) {
            if(find_node_in_graph(searching_list,i->data) != NULL) {
                remove_node_from_graph(lv,i);
            }
        }
    }
    for(int i = 1; i <= ((int)(searching_list.size()) - k); i++) {
        searching_list.pop_front();
    }
    pair<list<Graph_Node>,list<Graph_Node>> result;
    result.first = searching_list;
    result.second = visited_list;
    
    return result;
}