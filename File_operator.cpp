#include "File_operator.h"

File_operator::File_operator() = default;

File_operator::~File_operator() = default;

void File_operator::save_results_to_file(const std::string &path, const std::vector<long long int> &data) {
    file.open(path, std::ios::app);
    if (file.good()) {
        std::cout << "File was opened successfully" << std::endl;
    } else {
        std::cout << "File not found" << std::endl;
    }

    if (file.is_open()) {
        int size = static_cast<int>(data.size());
        int k = 0;
        for (long long int i : data) {
            file << i;
            if (k < size - 1) {
                file << ",";
            }
            k++;
        }
        file << std::endl;
        file.close();
    } else {
        std::cout << "File error - OPEN" << std::endl;
    }
}

std::vector<std::vector<int>> File_operator::load_graph_from_file(const std::string &path) {

    std::vector<int> temp_tab(4);

    std::vector<std::vector<int>> graph_data;

    int graph_edges;

    file.open(path, std::ios::in);
    if(file.is_open()) {
        if (file_read_line(file, temp_tab, 4)) {
            graph_edges = temp_tab[0];
            graph_data.push_back(std::vector<int> (temp_tab.begin(), temp_tab.end()));

            for (int i = 0; i < graph_edges; i++) {
                if (file_read_line(file, temp_tab, 3)) {
                    //temp_tab[0] - edge_start, temp_tab[1] - edge_end, temp_tab[2] - edge_weight
                    graph_data.push_back(std::vector<int> {temp_tab[0], temp_tab[1], temp_tab[2]});
                } else {
                    std::cout << "File error - READ EDGE" << std::endl;
                    graph_data = { };
                    break;
                }
            }
        } else {
            std::cout << "File error - READ INFO" << std::endl;
            graph_data = { };
        }

        file.close();

    } else {
        std::cout << "File error - OPEN" << std::endl;
        graph_data = { };
    }

    return graph_data;
}

bool File_operator::file_read_line(std::fstream &file_stream, std::vector<int> &tab, int size) {
    std::string s;

    std::getline(file_stream, s);

    if (file_stream.fail() || s.empty()) {
        return false;
    }

    std::istringstream in_ss(s);

    for (int i = 0; i < size; i++) {
        in_ss >> tab[i];
        if (in_ss.fail()) {
            return false;
        }
    }

    return true;
}
