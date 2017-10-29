//
// Created by Weitang Li on 2017/10/18.
//

#ifndef PROJECT_CHARCANDIDATE_H
#define PROJECT_CHARCANDIDATE_H


#include <string>
#include "utils.h"

#define START_SYM "##"

class CharCandidate {

    gbk_char_t gbk_char;
    int level;
    result_t prev_chars;
    double overall_dissipation;


public:
    CharCandidate();

    CharCandidate(const CharCandidate & prev_candidate, const gbk_char_t & new_char, double new_dissipation);

    bool operator<(const CharCandidate &rhs) const;

    bool operator>(const CharCandidate &rhs) const;

    bool operator<=(const CharCandidate &rhs) const;

    bool operator>=(const CharCandidate &rhs) const;

    int getLevel() const;

    const gbk_char_t &getChar() const;

    void build_result(result_t & result);
};


#endif //PROJECT_CHARCANDIDATE_H
