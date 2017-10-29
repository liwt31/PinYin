//
// Created by Weitang Li on 2017/10/18.
//

#include "CharCandidate.h"

CharCandidate::CharCandidate():
    gbk_char(START_SYM),
    prev_chars(result_t()),
    overall_dissipation(0),
    level(-1)
{}


CharCandidate::CharCandidate(const CharCandidate & prev_candidate, const gbk_char_t & new_char, double new_dissipation):
        gbk_char(new_char),
        prev_chars(prev_candidate.prev_chars + prev_candidate.gbk_char + " "),
        overall_dissipation(prev_candidate.overall_dissipation + new_dissipation),
        level(prev_candidate.level + 1)
{}

bool CharCandidate::operator<(const CharCandidate &rhs) const {
    // The higher dissipation, the lower the priority
    return overall_dissipation > rhs.overall_dissipation;
}

bool CharCandidate::operator>(const CharCandidate &rhs) const {
    return rhs < *this;
}

bool CharCandidate::operator<=(const CharCandidate &rhs) const {
    return !(rhs < *this);
}

bool CharCandidate::operator>=(const CharCandidate &rhs) const {
    return !(*this < rhs);
}

int CharCandidate::getLevel() const {
    return level;
}

const gbk_char_t &CharCandidate::getChar() const {
    return gbk_char;
}

void CharCandidate::build_result(result_t & result){
    if (prev_chars.size() < 2){
        result = gbk_char;
    } else {
        result = prev_chars.substr(3) + gbk_char;
    }
}



