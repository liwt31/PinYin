//
// Created by Weitang Li on 2017/10/18.
//
//
#include <ctime>
#include "io.h"
#include "search.h"

int main(int argc, char * argv[]){
    if (argc != 3){
        printf("usage: pinyin input.txt output.txt");
        return -1;
    }
    printf("Loading dissipation map...\n");
    dissipation_map_t dissipation_map;
    if (-1 == loadDissipationMap(dissipation_map)){
        printf("Open dissipation map file failed, please make sure dissipation_map is in %s\n", DISSI_PATH);
        return -1;
    }
    printf("Loading pinyin map...\n");
    pinyin_map_t pinyin_map;
    if (-1 == loadPinyinMap(pinyin_map)){
        printf("Open pinyin map file failed, please make sure pinyin map is in %s\n", PINYIN_PATH);
        return -1;
    }
    printf("Loading input...\n");
    input_vector_t input_vector;
    if (-1 == loadInput(argv[1],input_vector)){
        printf("Open input file failed, please make sure input file is in %s\n", argv[1]);
        return -1;
    }
    result_vector_t result_vector;
    for (auto &input:input_vector){
        time_t timep1 = clock();
        result_t new_result;
        int leaves_searched = search(input, new_result, pinyin_map, dissipation_map);
        time_t timep2 = clock();
        printf("%d leaves are searched for \"", leaves_searched);
        for (auto &pinyin:input){
            printf("%s ", pinyin.c_str());
        }
        printf("\", time cost:%ld\n", timep2 - timep1);
        result_vector.push_back(new_result);
    }
    if (-1 == saveOutput(argv[2], result_vector)){
        printf("Saving output file to %s failed", argv[2]);
        return -1;
    }
    return 0;
}
