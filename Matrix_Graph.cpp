#include <queue>
#include <stack>
#include <algorithm>
#include "Matrix_Graph.h"
#include "File_operator.h"


Matrix_Graph::Matrix_Graph(bool is_directed) {
    this->is_directed = is_directed;
    is_loaded = false;
    vertex_amount = 0;
    edge_amount = 0;
    starting_vertex = 0;
}

Matrix_Graph::~Matrix_Graph() = default;



void Matrix_Graph::load_graph(const std::string &path) {
    File_operator file_operator;

    auto graph_data = file_operator.load_graph_from_file(path);

    //Najpierw wiersze potem kolumny
    //Wypełniamy macierz sąsiedztwa INT_MAX oznacza nieskończoność
    matrix = std::vector<std::vector<int>>(graph_data[0][1], (std::vector<int>(graph_data[0][1], INT_MAX)));

    int start_vertex, end_vertex, weight;

    edge_amount = graph_data[0][0];
    vertex_amount = graph_data[0][1];
    starting_vertex = graph_data[0][2];

    for (int i = 1; i< graph_data.size(); i++) {
        start_vertex = graph_data[i][0];
        end_vertex = graph_data[i][1];
        weight = graph_data[i][2];

        //Patrząc na to czy jest to graf skierowany czy nie wypełniamy macierz w odpowiedni sposób
            matrix[start_vertex][end_vertex] = weight;
        if (!is_directed) {
            matrix[end_vertex][start_vertex] = weight;
        }
    }

    is_loaded = true;
}

void Matrix_Graph::display_graph() {
    std::cout << std::endl;
    std::cout << "Macierz sasiedztwa" << std::endl;
    std::cout << "Legenda: os X - wierzcholek koncowy, os Y - wierzcholek poczatkowy";
    std::cout << std::endl << std::endl;

    int size = matrix.size();

    std::cout << "     ";
    for(int i = 0; i < size; i++) {
        std::cout << std::setw(3) << i << "  ";
    }

    std::cout << std::endl << "   -";
    for(int i = 0; i < size; i++)
        std::cout << "-----";
    std::cout << std::endl;

    for(int i = 0; i < size; i++) {
        std::cout << "   |" << std::endl;
        std::cout << std::setw(3) << i;
        std::cout << "|";

        for(int j = 0; j < size; j++) {
            std::cout << " ";
            if(matrix[i][j] == INT_MAX) {
                std::cout << "  . ";
            } else {
                int length = std::to_string(matrix[i][j]).length();
                if(length == 1)
                    std::cout <<  "  " << matrix[i][j] << " ";
                else if(length == 2) {
                    std::cout << " " << matrix[i][j] << " ";
                } else
                    std::cout << matrix[i][j] << " ";
            }
        }
        std::cout << std::endl;
    }

}

void Matrix_Graph::dijksta_algorithm(bool show_results, int custom_start_vertex) {

    if(custom_start_vertex != -1) {
        if(custom_start_vertex < 0 || custom_start_vertex >= vertex_amount) {
            std::cout << "\nPoczatkowy wierzcholek jest niepoprawny.";
            return;
        }
        starting_vertex = custom_start_vertex;
    }

    //Wypełniamy tablicę z odległościami, nieskończoność oznaczamy wartością INT_MAX
    std::vector<int> distance = std::vector<int>(vertex_amount, INT_MAX);

    //Wypełniamy tablicę poprzednikow wartosciami -1
    std::vector<int> previous = std::vector<int>(vertex_amount, -1);

    //Ustawiamy odległość startowego wierzchołka na 0
    distance[starting_vertex] = 0;

    //Tworzymy swój własny comparator
    auto comparator = [](std::pair<int, int> left, std::pair<int, int> right){return left.second > right.second;};

    typedef std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, decltype(comparator) > Min_heap;
    //Tworzymy kolejkę priorytetową z wierzchołkami (kopiec min)
    Min_heap vertex_queue(comparator);
    //Wypełniamy kolejkę
    for(int i = 0; i < vertex_amount; i++) {
        vertex_queue.emplace(i, distance[i]);
    }

    std::vector<bool> visited_vertices(vertex_amount, false);

    while(!vertex_queue.empty()) {
        std::pair<int, int> current_vertex = vertex_queue.top();
        vertex_queue.pop();

        //Jesli wierzcholek byl juz przez nas odwiedzany to kontunuujemy dalej
        if(visited_vertices[current_vertex.first])
            continue;

        //Dodajemy do zbioru wierzchołków odwiedzonych
        visited_vertices[current_vertex.first] = true;
        //relaksujemy
        for(int i=0; i < vertex_amount; i++) {
            if(matrix[current_vertex.first][i] != INT_MAX) {
                if(distance[i] > distance[current_vertex.first] + matrix[current_vertex.first][i]) {
                    distance[i] = distance[current_vertex.first] + matrix[current_vertex.first][i];
                    previous[i] = current_vertex.first;
                    vertex_queue.emplace(i, distance[i]);
                }
            }
        }
    }

    if(show_results) {
        std::cout << std::endl << "Start = " << starting_vertex << std::endl;
        std::cout << "End  Dist  Path" << std::endl;

        for(int i = 0; i < vertex_amount; i++) {
            std::cout << std::setw(3) << i << " | "
            << std::setw(3) << distance[i] << " | ";

            int j = i;
            std::stack<int> path;
            while(j != -1) {
                path.push(j);
                j = previous[j];
            }

            while(!path.empty()) {
                std::cout << path.top() << " ";
                path.pop();
            }
            std::cout << std::endl;
        }


    }
}

void Matrix_Graph::bellman_ford_algorithm(bool show_results, int custom_start_vertex) {

    if(custom_start_vertex != -1) {
        if(custom_start_vertex < 0 || custom_start_vertex >= vertex_amount) {
            std::cout << "\nPoczatkowy wierzcholek jest niepoprawny.";
            return;
        }
        starting_vertex = custom_start_vertex;
    }

    //Wypełniamy tablicę z odległościami, nieskończoność oznaczamy wartością INT_MAX
    std::vector<int> distance = std::vector<int>(vertex_amount, INT_MAX);

    //Wypełniamy tablicę poprzednikow wartosciami -1
    std::vector<int> previous = std::vector<int>(vertex_amount, -1);

    //Ustawiamy odległość startowego wierzchołka na 0
    distance[starting_vertex] = 0;

    bool changed;

    for(int i = 1; i < vertex_amount; i++) {
        changed = false;
        for(int j = 0; j < vertex_amount; j++) {
            for(int k = 0; k < vertex_amount; k++) {
                if(matrix[j][k] != INT_MAX) {
                    if(distance[j] != INT_MAX && distance[k] > distance[j] + matrix[j][k]) {
                        distance[k] = distance[j] + matrix[j][k];
                        previous[k] = j;
                        changed = true;
                    }
                }
            }
        }
        if (!changed)
            break;
    }
    //Sprawdzamy czy jest ujemny cykl
    bool negative_cycle = false;
    for(int j = 0; j < vertex_amount; j++) {
        for (int k = 0; k < vertex_amount; k++) {
            if (matrix[j][k] != INT_MAX) {
                if (distance[j] != INT_MAX && distance[k] > distance[j] + matrix[j][k]) {
                    negative_cycle = true;
                    break;
                }
            }
        }
    }

    if(show_results) {

        if(negative_cycle) {
            std::cout << "Graf zawiera cykl ujemny" << std::endl;
            return;
        }

        std::cout << std::endl << "Start = " << starting_vertex << std::endl;
        std::cout << "End  Dist  Path" << std::endl;

        for(int i = 0; i < vertex_amount; i++) {
            std::cout << std::setw(3) << i << " | "
                      << std::setw(3) << distance[i] << " | ";

            int j = i;
            std::stack<int> path;
            while(j != -1) {
                path.push(j);
                j = previous[j];
            }

            while(!path.empty()) {
                std::cout << path.top() << " ";
                path.pop();
            }
            std::cout << std::endl;
        }
    }
}

void Matrix_Graph::prim_algorithm(bool show_results) {

    int p[vertex_amount]; // Tablica rodzicow (previous) uzywana do wyswietlenia struktury MST
    int key[vertex_amount];    // Tablica uzywana do kolejki pokazuje najmniejsza krawedz wychodzaca z danego wierzcholka
    bool visited[vertex_amount]; // Tablica, ktora pozwala stwierdzic czy wierzcholek nalezy do mst

    // Wszystkie klucze maja wartosc INT_MAX (nieskonczonosc) i kazdy nalezy do zbioru nie odwiedzonych
    for (int i = 0; i < vertex_amount; i++) {
        key[i] = INT_MAX;
        visited[i] = false;
    }

    // Poczatkowy wierzcholek ma wartosc 0 i nie ma poprzednika (rodzica)
    key[starting_vertex] = 0;
    p[starting_vertex] = -1;

    //Tworzymy swój własny comparator
    auto comparator = [](std::pair<int, int> left, std::pair<int, int> right) { return left.second > right.second; };

    typedef std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, decltype(comparator)> Min_heap;
    //Tworzymy kolejkę priorytetową z wierzchołkami (kopiec min)
    Min_heap vertex_queue(comparator);

    // Dodajemy tylko pierwszy wierzcholek, a reszte będziemy dodawac podczas aktualizowania wynikow
    vertex_queue.emplace(starting_vertex, key[starting_vertex]);

    while (!vertex_queue.empty()) {
        // Pobieramy wierzcholek, ktory ma krawedz o najmniejszej wadze
        int i = vertex_queue.top().first;
        vertex_queue.pop();

        //Jesli dany wierzcholek byl juz odwidzany to przechodzimy dalej
        if (visited[i]) {
            continue;
        } else
            visited[i] = true; //Dodajemy ten wierzcholek do listy tworzacej MST (odwiedzone wierzcholki)


        for (int j = 0; j < vertex_amount; j++) {
            //Szukamy sąsiadów danego wierzcholka, ktorzy nie tworza jeszcze MST
            if (matrix[i][j] != INT_MAX && !visited[j]) {
                //Jesli dana krawedz jest lepsza niz poprzednia to aktualizujemy zawartosc p i key oraz wstawiamy do kolejki
                if (matrix[i][j] < key[j]) {
                    p[j] = i;
                    key[j] = matrix[i][j];
                    vertex_queue.emplace(j, key[j]);
                }
            }
        }
    }
    if(show_results) {
        for (int i = 0; i < vertex_amount; i++) {
            if (!visited[i]) {
                std::cout << "Graf nie jest spojny, nie odwiedzono wierzcholka " << i << std::endl;
                return;
            }
        }

        int MST_sum = 0;
        std::cout << "Krawedzie MST Prim macierzowo:\n";
        std::cout << "Edge    Weight\n";
        for (int i = 0; i < vertex_amount; i++) {
            //nie wypisujemy dla poczatkowego, poniewaz nie ma poprzednika (wartosc -1)
            if(i == starting_vertex)
                continue;

            std::cout << "(" << p[i] << ", " << i << ")    " << key[i] << "\n";
            MST_sum += key[i];
        }
        std::cout << "\nMST = " << MST_sum << std::endl;
    }
}

void Matrix_Graph::kruskal_algorithm(bool show_results) {

    // ilość krawędzi w MST
    int mst_edges = 0;
    // zbiór krawędzi tworzących MST
    std::vector<std::tuple<int, int, int>> MST;


    int parent[vertex_amount]; //Tablica pozwalająca stwierdzić do jakiej grupy należy wierzchołek
    int rank[vertex_amount]; //Tablica zawierająca informację o randze danego wierzcholka

    auto comparator = [](std::tuple<int, int, int> left, std::tuple<int, int, int> right) { return std::get<2>(left) < std::get<2>(right); };
    //Zbiór wszystkich krawędzi
    std::vector<std::tuple<int, int, int>> allEdges;


    // Lista wszystkich krawedzi: początkowy wierzcholek, koncowy wierzcholek, waga.
    for (int i = 0; i < vertex_amount; i++) {
        for (int j = 0; j < vertex_amount; j++) {
            //Czytamy tylko jedną połowę macierzy, poniewaz graf jest nieskierowany
            if (i <= j) {
                if (matrix[i][j] != INT_MAX) {
                    allEdges.push_back(std::make_tuple(i, j, matrix[i][j]));
                }
            }

        }
    }

    //MakeSet - Tworzymy tyle grup ile jest wierzcholkow i kazdej przypisujemy wartosc rowna danemu wierzcholkowi
    //ustawiamy dla każdej rank na 0
    for(int i = 0; i < vertex_amount; i++) {
        parent[i] = i;
        rank[i] = 0;
    }

    //Sortujemy krawędzie niemalejąco
    std::sort(allEdges.begin(), allEdges.end(), comparator);

    for(int i = 0; i < allEdges.size(); i++) {
        auto current_edge = allEdges[i];
        //wierzcholek pierwszy
        int u = std::get<0>(current_edge);
        //wierzcholek drugi
        int v = std::get<1>(current_edge);
        //Sprawdzamy do jakiej grupy nalezy wierzcholek u
        while(u != parent[u])
            u = parent[u];
        //Sprawdzamy do jakiej grupy nalezy wierzcholek v
        while(v != parent[v])
            v = parent[v];

        //Jeśli wierzchołki należą do różnych grup
        if(u != v) {
            //Dodajemy krawędz do listy krawedzi tworzacych MST
            MST.push_back(current_edge);
            mst_edges++;
            //Union - łączymy ze sobą zbiory patrząc na rank
            if(rank[u] < rank[v])
                parent[u] = v;
            else
                parent[v] = u;
            //Jeśli są takie same to zwiększamy rangę lewej krawędzi o 1
            if(rank[u] == rank[v])
                rank[u] = rank[v] + 1;
        }
        //Jak algorytm stworzy spojny graf to przerywamy
        if (mst_edges == vertex_amount - 1) {
            break;
        }
    }

    if(show_results) {

        //Sortujemy krawędzie po wierzchołku początkowym i końcowym
        std::sort(MST.begin(), MST.end(), [](std::tuple<int, int, int> left, std::tuple<int, int, int> right) {
             if (std::get<0>(left) != std::get<0>(right)) {
             return std::get<0>(left) < std::get<0>(right);
         } else {
             return std::get<1>(left) < std::get<1>(right);
         }});

        int MST_sum = 0;
        std::cout << "Krawedzie MST Kruskal macierzowo:\n";
        std::cout << "Edge    Weight\n";
        for (int i = 0; i < MST.size(); i++) {

            std::cout << "(" << std::get<0>(MST[i]) << ", " << std::get<1>(MST[i]) << ")    " << std::get<2>(MST[i]) << "\n";
            MST_sum += std::get<2>(MST[i]);
        }
        std::cout << "\nMST = " << MST_sum << std::endl;
    }
}



