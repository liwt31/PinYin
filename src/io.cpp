//
// Created by Weitang Li on 2017/10/19.
//

#include <sstream>
#include "io.h"


int loadDissipationMap(dissipation_map_t &dissipation_map){
    FILE * fp = fopen(DISSI_PATH, "r");
    if (fp == nullptr){
        return -1;
    }
    fseek(fp, 0L, SEEK_END);
    auto size = (size_t)ftell(fp);

    auto buf = new uint8_t[size];

    fseek(fp, 0L, SEEK_SET);
    size_t len = fread(buf, 1, size, fp);
    uint8_t * pt = buf;
    while((pt - buf) < len){
        gbk_char_t chi_char1(pt, pt+2);
        pt += 3;
        gbk_char_t chi_char2(pt, pt+2);
        pt += 3;
        uint8_t i, float_buf[20];
        for(i = 0; *pt != '\n'; i++, pt++){
            float_buf[i] = *pt;
        }
        pt ++;
        float_buf[i] = 0;
        double lncond_prob = strtod((const char *)float_buf, nullptr);
        dissipation_map[chi_char1][chi_char2] = lncond_prob;
    }
    delete buf;
    return 0;
}

int loadPinyinMap(pinyin_map_t &pinyin_map){
    std::ifstream fin(PINYIN_PATH);
    if (!fin){
        return -1;
    }
    std::string line;
    while (std::getline(fin, line)){
        auto space_posi = line.find_first_of(' ');
        std::string pinyin(line, 0, space_posi);
        pinyin_map[pinyin] = std::vector<std::string>();
        std::istringstream istring(line);
        istring >> pinyin;
        std::string chi_char;
        while(istring >> chi_char){
            pinyin_map[pinyin].push_back(chi_char);
        }
    }
    return 0;
}

int loadInput(const char * input_file_path, input_vector_t &input_vectors){
    std::fstream fin(input_file_path);
    std::string line;
    if (!fin){
        return -1;
    }
    while(std::getline(fin, line)){
        std::istringstream istring(line);
        std::vector<std::string> input;
        std::string pinyin;
        while (istring >> pinyin){
            input.push_back(pinyin);
        }
        input_vectors.push_back(input);
    }
    return 0;
}

int saveOutput(const char * output_file_path, result_vector_t &output_vectors){
    std::ofstream fout(output_file_path);
    if (!fout){
        return -1;
    }
    for (auto &result:output_vectors){
        fout << result << std::endl;
    }
    return 0;
}