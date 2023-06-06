#pragma once

#include <vector>
#include <string>
#include <forward_list>

class List_Graph {
private:
    bool is_directed;

    int starting_vertex;
    std::vector<std::forward_list<std::pair<int,int>>> neighbour_list;

    int edge_amount, vertex_amount;

public:
    bool is_loaded; //Informacja czy graf jest załadowany od struktury

public:
    List_Graph(bool is_directed);

    ~List_Graph();

    void display_graph();

    void dijksta_algorithm(bool show_results, int custom_start_vertex = -1); //Jeśli jest ustawione na -1 to bierzemy wartość z pliku

    void bellman_ford_algorithm(bool show_results, int custom_start_vertex = -1);

    void prim_algorithm(bool show_results);

    void kruskal_algorithm(bool show_results);

    void load_graph(const std::string &path);

};

