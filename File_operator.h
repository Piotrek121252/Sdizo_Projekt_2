#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>

class File_operator {
public:
    std::fstream file;

    File_operator();

    ~File_operator();

    //metoda służy do zapisywania wyników pomiarów do pliku
    void save_results_to_file(const std::string &path, const std::vector<long long int> &data);

    std::vector<std::vector<int>> load_graph_from_file(const std::string &path);

private:
    bool file_read_line(std::fstream &file_stream, std::vector<int> &tab, int size);

};

