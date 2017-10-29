//
// Created by Weitang Li on 2017/10/19.
//

#ifndef PROJECT_UTILS_H
#define PROJECT_UTILS_H

#include <utility>
#include <unordered_map>
#include <vector>

typedef std::string gbk_char_t;
typedef std::unordered_map<gbk_char_t, std::unordered_map<gbk_char_t, double> > dissipation_map_t;
typedef std::unordered_map<std::string, std::vector<gbk_char_t> > pinyin_map_t;
typedef std::vector<std::string> input_t;
typedef std::vector<input_t> input_vector_t;
typedef std::string result_t;
typedef std::vector<result_t> result_vector_t;



#endif //PROJECT_UTILS_H
