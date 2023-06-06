#include <chrono>
#include "Tester.h"
#include "File_operator.h"
#include "List_Graph.h"
#include "Matrix_Graph.h"


Tester::Tester() = default;

Tester::~Tester() = default;

void Tester::generate_graph_data(const std::string &result_graph_path, int vertex_amount,
                                 double graph_density, int starting_vertex,
                                 int ending_vertex, bool is_directed) {

    std::ofstream file(result_graph_path);

    int max_edges = vertex_amount * (vertex_amount - 1);

    int number_of_edges = static_cast<int>(graph_density * max_edges);

    //Jak nie jest skierowany to zmniejszamy dwukrotnie ilosc krawedzi, ponieważ będziemy je dwukrotnie wpisywac
    //(u,v) = (v,u)
    if(!is_directed)
        number_of_edges /= 2;


    //Dodatkowe krawędzie
    std::vector<std::tuple<int, int, int>> additional_edges;

    //Zbiór krawedzi tworzących drzewo spinające
    std::vector<std::tuple<int, int, int>> st_edges;

    for(int i = 0; i < vertex_amount - 1; i++) {
        st_edges.push_back(std::make_tuple(i, i + 1, std::rand() % 1000 + 1));
    }

    //graf nieskierowany
    if(!is_directed){
        for (int i = 0; i < vertex_amount - 1; ++i) {
            for (int j = i + 2; j < vertex_amount; ++j) {//Warunek dobrany tak, żeby nie generować krawędzi, które już należą do drzewa spinającego
                //Tworzymy krawędzie po kolei w formacie wierzchołek poczatkowy, wierzchołek końcowy, waga
                additional_edges.push_back(std::make_tuple(i, j, std::rand() % 1000 + 1));
            }
        }
    }
    else{ //graf skierowany
        for (int i = 0; i < vertex_amount; ++i) {
            for (int j = 0; j < vertex_amount; ++j) {
                if (i != j && j != i + 1) { //nie generujemy krawędzi, które są w grafie spinającym
                    //Tworzymy krawędzie po kolei w formacie wierzchołek poczatkowy, wierzchołek końcowy, waga
                    additional_edges.push_back(std::make_tuple(i, j, std::rand() % 1000 + 1));
                }
            }
        }
    }

    //Korzystamy z shuffle aby zmienic kolejnosc w krawedzi w tablicy
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(additional_edges.begin(), additional_edges.end(), gen);

    // Zmniejszamy ilosc krawędzi zapisanych do pliku ze względu na podaną gęstość po odjęciu krawędzi tworzących graf spójny
    additional_edges.resize(number_of_edges - (vertex_amount - 1));

    for(int i = 0; i < st_edges.size(); i++) {
        additional_edges.push_back(st_edges[i]);
    }

    //Korzystamy drugi raz z shuffle aby wymieszać krawędzie tworzące graf spójny
    std::shuffle(additional_edges.begin(), additional_edges.end(), gen);

    //Zapisujemy nasz graf do pliku
    file << number_of_edges << " " << vertex_amount << " " << starting_vertex << " " << ending_vertex << std::endl;

    for(int i = 0; i < additional_edges.size(); i++) {
        file << std::get<0>(additional_edges[i]) << " " << std::get<1>(additional_edges[i])
             << " " << std::get<2>(additional_edges[i]) << std::endl;
    }

    file.close();


}

void Tester::test_dijkstra(const std::string &list_results_path, const std::string &matrix_results_path) {

    std::vector<long long int> resultsList, resultsMatrix;
    std::vector<long long int> tempList, tempMatrix;

    File_operator* file_operator = new File_operator;

    for (int vertex_amount : vertex_amounts) {
        for (double density : densities) {
            std::cout << "vertex amount: " << vertex_amount << " Density: " << density << std::endl;

            for (int k = 0; k < 100; k++) {


                List_Graph* listGraph = new List_Graph(true);
                Matrix_Graph* matrixGraph = new Matrix_Graph(true);

                generate_graph_data("Generated_Graph.txt", vertex_amount, density, 0, std::rand() % vertex_amount + 1, true);
                listGraph->load_graph("Generated_Graph.txt");


                auto start = std::chrono::high_resolution_clock::now();
                listGraph->dijksta_algorithm(false);
                auto end = std::chrono::high_resolution_clock::now();
                tempList.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());


                matrixGraph->load_graph("Generated_Graph.txt");

                start = std::chrono::high_resolution_clock::now();
                matrixGraph->dijksta_algorithm(false);
                end = std::chrono::high_resolution_clock::now();

                tempMatrix.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());

                delete listGraph;
                delete matrixGraph;
            }

            long long int sum = 0;
            for (int i = 0; i < tempList.size(); i++) {
                sum += tempList[i];
            }
            resultsList.push_back(sum / tempList.size());

            sum = 0;
            for (int i = 0; i < tempMatrix.size(); i++) {
                sum += tempMatrix[i];
            }
            resultsMatrix.push_back(sum / tempMatrix.size());

            tempList.clear();
            tempMatrix.clear();

        }
    }

    file_operator->save_results_to_file(list_results_path, resultsList);
    file_operator->save_results_to_file(matrix_results_path, resultsMatrix);

    delete file_operator;
}

void Tester::test_bellmanford(const std::string &list_results_path, const std::string &matrix_results_path) {
    std::vector<long long int> resultsList, resultsMatrix;
    std::vector<long long int> tempList, tempMatrix;

    File_operator* file_operator = new File_operator;

    for (int vertex_amount : vertex_amounts) {
        for (double density : densities) {
            std::cout << "vertex amount: " << vertex_amount << " Density: " << density << std::endl;

            for (int k = 0; k < 100; k++) {


                List_Graph* listGraph = new List_Graph(true);
                Matrix_Graph* matrixGraph = new Matrix_Graph(true);

                generate_graph_data("C:\\Users\\piotr\\CLionProjects\\SDIZO2_graph\\Results\\Genereted_Graph\\Generated_Graph.txt", vertex_amount, density, 0, std::rand() % vertex_amount + 1, true);
                listGraph->load_graph("C:\\Users\\piotr\\CLionProjects\\SDIZO2_graph\\Results\\Genereted_Graph\\Generated_Graph.txt");


                auto start = std::chrono::high_resolution_clock::now();
                listGraph->bellman_ford_algorithm(false);
                auto end = std::chrono::high_resolution_clock::now();
                tempList.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());


                matrixGraph->load_graph("C:\\Users\\piotr\\CLionProjects\\SDIZO2_graph\\Results\\Genereted_Graph\\Generated_Graph.txt");

                start = std::chrono::high_resolution_clock::now();
                matrixGraph->bellman_ford_algorithm(false);
                end = std::chrono::high_resolution_clock::now();

                tempMatrix.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());

                delete listGraph;
                delete matrixGraph;
            }

            long long int sum = 0;
            for (int i = 0; i < tempList.size(); i++) {
                sum += tempList[i];
            }
            resultsList.push_back(sum / tempList.size());

            sum = 0;
            for (int i = 0; i < tempMatrix.size(); i++) {
                sum += tempMatrix[i];
            }
            resultsMatrix.push_back(sum / tempMatrix.size());

            tempList.clear();
            tempMatrix.clear();

        }
    }

    file_operator->save_results_to_file(list_results_path, resultsList);
    file_operator->save_results_to_file(matrix_results_path, resultsMatrix);

    delete file_operator;
}

void Tester::test_prim(const std::string &list_results_path, const std::string &matrix_results_path) {
    std::vector<long long int> resultsList, resultsMatrix;
    std::vector<long long int> tempList, tempMatrix;

    File_operator* file_operator = new File_operator;

    for (int vertex_amount : vertex_amounts) {
        for (double density : densities) {
            std::cout << "vertex amount: " << vertex_amount << " Density: " << density << std::endl;

            for (int k = 0; k < 100; k++) {


                List_Graph* listGraph = new List_Graph(false);
                Matrix_Graph* matrixGraph = new Matrix_Graph(false);

                generate_graph_data("Generated_Graph.txt", vertex_amount, density, 0, std::rand() % vertex_amount + 1, false);
                listGraph->load_graph("Generated_Graph.txt");


                auto start = std::chrono::high_resolution_clock::now();
                listGraph->prim_algorithm(false);
                auto end = std::chrono::high_resolution_clock::now();
                tempList.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());


                matrixGraph->load_graph("Generated_Graph.txt");

                start = std::chrono::high_resolution_clock::now();
                matrixGraph->prim_algorithm(false);
                end = std::chrono::high_resolution_clock::now();

                tempMatrix.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());

                delete listGraph;
                delete matrixGraph;
            }

            long long int sum = 0;
            for (int i = 0; i < tempList.size(); i++) {
                sum += tempList[i];
            }
            resultsList.push_back(sum / tempList.size());

            sum = 0;
            for (int i = 0; i < tempMatrix.size(); i++) {
                sum += tempMatrix[i];
            }
            resultsMatrix.push_back(sum / tempMatrix.size());

            tempList.clear();
            tempMatrix.clear();

        }
    }

    file_operator->save_results_to_file(list_results_path, resultsList);
    file_operator->save_results_to_file(matrix_results_path, resultsMatrix);

    delete file_operator;

}

void Tester::test_kruskal(const std::string &list_results_path, const std::string &matrix_results_path) {
    std::vector<long long int> resultsList, resultsMatrix;
    std::vector<long long int> tempList, tempMatrix;

    File_operator* file_operator = new File_operator;

    for (int vertex_amount : vertex_amounts) {
        for (double density : densities) {
            std::cout << "vertex amount: " << vertex_amount << " Density: " << density << std::endl;

            for (int k = 0; k < 100; k++) {


                List_Graph* listGraph = new List_Graph(false);
                Matrix_Graph* matrixGraph = new Matrix_Graph(false);

                generate_graph_data("Generated_Graph.txt", vertex_amount, density, 0, std::rand() % vertex_amount + 1, false);
                listGraph->load_graph("Generated_Graph.txt");


                auto start = std::chrono::high_resolution_clock::now();
                listGraph->kruskal_algorithm(false);
                auto end = std::chrono::high_resolution_clock::now();
                tempList.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());


                matrixGraph->load_graph("Generated_Graph.txt");

                start = std::chrono::high_resolution_clock::now();
                matrixGraph->kruskal_algorithm(false);
                end = std::chrono::high_resolution_clock::now();

                tempMatrix.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());

                delete listGraph;
                delete matrixGraph;
            }

            long long int sum = 0;
            for (int i = 0; i < tempList.size(); i++) {
                sum += tempList[i];
            }
            resultsList.push_back(sum / tempList.size());

            sum = 0;
            for (int i = 0; i < tempMatrix.size(); i++) {
                sum += tempMatrix[i];
            }
            resultsMatrix.push_back(sum / tempMatrix.size());

            tempList.clear();
            tempMatrix.clear();

        }
    }

    file_operator->save_results_to_file(list_results_path, resultsList);
    file_operator->save_results_to_file(matrix_results_path, resultsMatrix);

    delete file_operator;
}
