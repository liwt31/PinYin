//
// Created by Weitang Li on 2017/10/19.
//

#ifndef PROJECT_IO_H
#define PROJECT_IO_H

#include <fstream>
#include "utils.h"

#define DISSI_PATH "../data/dissipation_map.txt"
#define PINYIN_PATH "../data/pinyin_map.txt"


int loadDissipationMap(dissipation_map_t &dissipation_map);

int loadPinyinMap(pinyin_map_t &pinyin_map);

int loadInput(const char * input_file_path, input_vector_t &input_vectors);

int saveOutput(const char * output_file_path, result_vector_t &output_vectors);

#endif //PROJECT_IO_H
