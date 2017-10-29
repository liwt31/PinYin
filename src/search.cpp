//
// Created by weitang on 17-10-20.
//

#include <queue>
#include "search.h"
#include "CharCandidate.h"


double get_max_dissipation(const dissipation_map_t &dissipation_map){
    static double max_dissipation = -1;
    if (max_dissipation < 0){
        for (auto &sub_map:dissipation_map){
            for (auto &subsub_map:sub_map.second){
                max_dissipation = subsub_map.second < max_dissipation? max_dissipation:subsub_map.second;
            }
        }
    }
    return max_dissipation;
}


double query_default_c2(const dissipation_map_t & dissipation_map, const gbk_char_t & c2){
    auto submap = dissipation_map.find(START_SYM);
    auto dissipation = submap->second.find(c2);
    if (dissipation == submap->second.end()){
        return get_max_dissipation(dissipation_map);
    }
    return dissipation->second;
}


double query_dissipation_map(const dissipation_map_t & dissipation_map, const gbk_char_t & c1, const gbk_char_t & c2){
    auto submap = dissipation_map.find(c1);
    if (submap == dissipation_map.end()){
        return query_default_c2(dissipation_map, c2);
    }
    auto dissipation = submap->second.find(c2);
    if (dissipation == submap->second.end()){
        return query_default_c2(dissipation_map, c2);
    }
    return dissipation->second;
}


int search(const input_t & input,
           result_t & result,
           const pinyin_map_t & pinyin_map,
           const dissipation_map_t & dissipation_map)
{
    std::priority_queue<CharCandidate> candidate_queue;
    size_t target_level = input.size();
    candidate_queue.emplace();
    int leaves_searched = 0;
    while(true){
        CharCandidate candidate = candidate_queue.top();
        if (candidate_queue.empty()){
            return -1;
        }
        candidate_queue.pop();
        leaves_searched ++;
        int current_level = candidate.getLevel();
        int next_level = current_level + 1;
        if (target_level == next_level){
            candidate.build_result(result);
            return leaves_searched;
        }
        std::string next_pinyin = input[next_level];
        for (auto &gbk_char:pinyin_map.at(next_pinyin)){
            gbk_char_t last_char = candidate.getChar();
            double dissipation = query_dissipation_map(dissipation_map, last_char, gbk_char);
            candidate_queue.emplace(candidate, gbk_char, dissipation);
        }
    }
}
