#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <tuple>
#include <random>
#include <algorithm>


class Tester {

private:
    int vertex_amounts[5] = {100, 200,300,400,500}; //Badane ilości wierzchołków
    double densities[4] = {0.25, 0.50, 0.75, 0.99}; // Badane gęstości grafów

public:
    Tester();
    ~Tester();

    void test_dijkstra(const std::string& list_results_path, const std::string& matrix_results_path);

    void test_bellmanford(const std::string& list_results_path, const std::string& matrix_results_path);

    void test_kruskal(const std::string& list_results_path, const std::string& matrix_results_path);

    void test_prim(const std::string& list_results_path, const std::string& matrix_results_path);

    void generate_graph_data(const std::string& result_graph_path, int vertex_amount, double graph_density, int starting_vertex, int ending_vertex, bool is_directed);
};

