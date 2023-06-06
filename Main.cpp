/*
 Projekt: Sdizo2
 Autor: Piotr Komarnicki
*/
#include <iostream>
#include "Matrix_Graph.h"
#include "List_Graph.h"
#include "Tester.h"

void minimal_spanning_tree_menu() {
    char choice;


    Matrix_Graph* matrix_graph = new Matrix_Graph(false);
    List_Graph* list_graph = new List_Graph(false);
    bool is_loaded = false;

    std::string path;

    do {
        std::cout << "\nProblem minimalnego drzewa rozpinajacego:" << std::endl;

        std::cout << "1. Wczytaj graf z pliku\n"
                  << "2. Wyswietl graf w formie macierzowej i listy sasiedztwa\n"
                  << "3. Algorytm Prima na postaci macierzowej\n"
                  << "4. Algorytm Prima na postaci listowej\n"
                  << "5. Algorytm Kruskala na postaci macierzowej\n"
                  << "6. Algorytm Kruskala na postaci listowej\n"
                  << "0. Wyjdz z menu\n\n"
                  << "Wybierz opcje:";

        std::cin >> choice;
        std::cin.clear();
        std::cin.ignore(10000, '\n');

        switch (choice) {
            case '1':   std::cout << "Podaj sciezke do pliku: " << std::endl;
                        std::cin >> path;
                        if(is_loaded) {
                        delete matrix_graph;
                        delete list_graph;
                        matrix_graph = new Matrix_Graph(false);
                        list_graph = new List_Graph(false);
                        }
                        matrix_graph->load_graph(path);
                        list_graph->load_graph(path);
                        is_loaded = true;
                        break;
            case '2':   if(matrix_graph->is_loaded && list_graph->is_loaded) {
                             matrix_graph->display_graph();
                             list_graph->display_graph();
                        } else
                            std::cout << "Graf nie zostal wczytany.";
                        break;
            case '3':   if(is_loaded) {
                            matrix_graph->prim_algorithm(true);
                        } else
                            std::cout << "Graf nie zostal wczytany.\n";
                        break;
            case '4':   if(is_loaded) {
                            list_graph->prim_algorithm(true);
                        } else
                            std::cout << "Graf nie zostal wczytany.\n";
                        break;
            case '5':   if(is_loaded) {
                            matrix_graph->kruskal_algorithm(true);
                        } else
                            std::cout << "Graf nie zostal wczytany.\n";
                        break;
            case '6':   if(is_loaded) {
                            list_graph->kruskal_algorithm(true);
                        } else
                            std::cout << "Graf nie zostal wczytany.\n";
                        break;
            case '0':
                        delete matrix_graph;
                        delete list_graph;
                        std::cout << "\nPowrot do menu glownego." << std::endl;
                        break;
            default: std::cout << "\nPodany symbol jest bledny, sprobuj jeszcze raz.\n";
        }
    } while (choice != '0');
}


void shortest_path_menu() {
    char choice;

    Matrix_Graph* matrix_graph = new Matrix_Graph(true);
    List_Graph* list_graph = new List_Graph(true);
    bool is_loaded = false;

    std::string path;

    do {
        std::cout << "\nProblem najkrotszej drogi w grafie:" << std::endl;

        std::cout << "1. Wczytaj graf z pliku.\n"
                  << "2. Wyswietl graf w formie macierzowej i listy sasiedztwa.\n"
                  << "3. Algorytm Dijkstry na grafie na postaci macierzowej.\n"
                  << "4. Algorytm Dijkstry na grafie w postaci listy sasiedztwa.\n"
                  << "5. Algorytm Bellmana-Forda na postaci macierzowej.\n"
                  << "6. Algorytm Bellmana-Forda na postaci listy sasiedztwa.\n"
                  << "0. Wyjdz z menu\n\n"
                  << "Wybierz opcje:";

        std::cin >> choice;
        std::cin.clear();
        std::cin.ignore(10000, '\n');

        switch (choice) {
            case '1':    std::cout << "Podaj sciezke do pliku: " << std::endl;
                         std::cin >> path;
                         if(is_loaded) {
                             delete matrix_graph;
                             delete list_graph;
                             matrix_graph = new Matrix_Graph(true);
                             list_graph = new List_Graph(true);
                         }
                         matrix_graph->load_graph(path);
                         list_graph->load_graph(path);
                         is_loaded = true;
                         break;
            case '2':   if(matrix_graph->is_loaded && list_graph->is_loaded) {
                           matrix_graph->display_graph();
                           list_graph->display_graph();
                        } else
                            std::cout << "Graf nie zostal wczytany.";
                        break;
            case '3':   if(is_loaded) {
                        matrix_graph->dijksta_algorithm(true);
                        } else
                            std::cout << "Graf nie zostal wczytany.\n";
                        break;
            case '4':   if(is_loaded) {
                            list_graph->dijksta_algorithm(true);
                        } else
                        std::cout << "Graf nie zostal wczytany.\n";
                        break;
            case '5':   if(is_loaded) {
                            matrix_graph->bellman_ford_algorithm(true);
                        } else
                            std::cout << "Graf nie zostal wczytany.\n";
                        break;
            case '6':   if(is_loaded) {
                            list_graph->bellman_ford_algorithm(true);
                        } else
                            std::cout << "Graf nie zostal wczytany.\n";
                        break;
            case '0':   delete matrix_graph;
                        delete list_graph;
                        std::cout << "\nPowrot do menu glownego." << std::endl;
                        break;
            default: std::cout << "\nPodany symbol jest bledny, sprobuj jeszcze raz.\n";
        }
    } while (choice != '0');
}

int main(int argc, char *argv[]) {

    if (argc == 3) {
        if (std::string(argv[1]) == "test") {
            std::cout << "Tryb testow automatycznych" << std::endl;
            Tester* tester = new Tester();

            std::string testType = std::string(argv[2]);

            if(testType == "graph") {
                std::cout << "\nGenerowanie grafu." << std::endl;
                tester->generate_graph_data("Generated_Graph.txt", 400, 0.99, 0, std::rand() % 100 + 1, false);
            }else if(testType == "dijkstra") {
                std::cout << "\nWybrano test automatyczny algorytmu dijsktry." << std::endl;
                tester->test_dijkstra("dijsktra_list_results.csv", "dijkstra_matrix_results.csv");
            }else if(testType == "bellmanford") {
                std::cout << "\nWybrano test automatyczny algorytmu bellmana-forda." << std::endl;
                tester->test_bellmanford("bellmanford_list_results.csv", "bellmanford_matrix_results.csv");
            }else if(testType == "prim") {
                std::cout << "\nWybrano test automatyczny algorytmu prima." << std::endl;
                tester->test_prim("prim_list_results.csv", "prim_matrix_results.csv");
            } else if(testType == "kruskal") {
                std::cout << "\nWybrano test automatyczny algorytmu kruskala." << std::endl;
                tester->test_kruskal("kruskal_list_results.csv", "kruskal_matrix_results.csv");
            } else
                std::cout << "Niepoprawny typ testu" << std::endl;

            delete tester;
            std::cout << "Wychodzenie z trybu testow automatycznych\n";
            return 0;
        }
    }


    char choice;
    do {
        std::cout << "\nWybierz problem:\n"
                  << "1. MST - minimalne drzewo spinajace\n"
                  << "2. Najkrotsza droga w grafie\n"
                  << "0. Wyjscie z programu\n"
                  << std::endl
                  << "Podaj nr opcji:";

        std::cin >> choice;
        std::cin.clear();
        std::cin.ignore(10000, '\n');

        switch (choice) {
            case '1': minimal_spanning_tree_menu();
                      break;
            case '2': shortest_path_menu();
                      break;
            case '0': std::cout << "\nKonczenie dzialania programu..." << std::endl;
                      break;
            default:  std::cout << "\nPodany symbol jest bledny, sprobuj jeszcze raz.\n";
        }
    } while (choice != '0');

    return 0;
}