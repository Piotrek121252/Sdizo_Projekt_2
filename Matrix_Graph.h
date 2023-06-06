#pragma once

#include <vector>
#include <string>

class Matrix_Graph {
private:
    bool is_directed;

    int starting_vertex;
    std::vector<std::vector<int>> matrix;

    int edge_amount, vertex_amount;

public:
    bool is_loaded; //Informacja czy graf jest załadowany od struktury

public:
    explicit Matrix_Graph(bool is_directed);

    ~Matrix_Graph();

    void display_graph();

    void dijksta_algorithm(bool show_results, int custom_start_vertex = -1); //Jeśli jest ustawione na -1 to bierzemy wartość wczytaną z pliku

    void bellman_ford_algorithm(bool show_results, int custom_start_vertex = -1);

    void prim_algorithm(bool show_results);

    void kruskal_algorithm(bool show_results);

    void load_graph(const std::string &path);
};



