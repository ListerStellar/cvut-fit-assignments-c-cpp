#ifndef __PROGTEST__
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

#include "sample.h"
#endif


struct RegexNodeInfo {
    bool nullable;
    std::vector<size_t> firstPositions;
    std::vector<size_t> lastPositions;
};

class RegexAutomatonMaker {
public:
    std::vector<uint8_t> nodeSymbols;
    std::vector<std::vector<size_t> > followTable;
    RegexNodeInfo finalNodeInfo;
    
    void buildAutomaton(const regexp::RegExp &rootNode) {
        nodeSymbols.clear();
        followTable.clear();
        finalNodeInfo = std::visit([this](auto &&n) { return analyzeNode(n); }, rootNode);
    }

private:
    void mergePositions(std::vector<size_t> &dst, const std::vector<size_t> &src) {
        dst.insert(dst.end(), src.begin(), src.end());
        std::sort(dst.begin(), dst.end());
        dst.erase(std::unique(dst.begin(), dst.end()), dst.end());
    }

    RegexNodeInfo analyzeNode(const std::unique_ptr<regexp::Empty> &) {
        return {false, {}, {}};
    }

    RegexNodeInfo analyzeNode(const std::unique_ptr<regexp::Epsilon> &) {
        return {true, {}, {}};
    }

    RegexNodeInfo analyzeNode(const std::unique_ptr<regexp::Symbol> &symbolNode) {
        size_t pos = nodeSymbols.size();
        nodeSymbols.push_back(symbolNode->m_symbol);
        followTable.emplace_back();
        return {false, {pos}, {pos}};
    }

    RegexNodeInfo analyzeNode(const std::unique_ptr<regexp::Alternation> &altNode) {
        auto leftInfo = std::visit([this](auto &&n) { return analyzeNode(n); }, altNode->m_left);
        auto rightInfo = std::visit([this](auto &&n) { return analyzeNode(n); }, altNode->m_right);

        RegexNodeInfo result;
        result.nullable = leftInfo.nullable || rightInfo.nullable;

        result.firstPositions = leftInfo.firstPositions;
        mergePositions(result.firstPositions, rightInfo.firstPositions);

        result.lastPositions = leftInfo.lastPositions;
        mergePositions(result.lastPositions, rightInfo.lastPositions);

        return result;
    }

    RegexNodeInfo analyzeNode(const std::unique_ptr<regexp::Concatenation> &concatNode) {
        auto leftInfo = std::visit([this](auto &&n) { return analyzeNode(n); }, concatNode->m_left);
        auto rightInfo = std::visit([this](auto &&n) { return analyzeNode(n); }, concatNode->m_right);

        for (size_t pos: leftInfo.lastPositions) {
            auto &row = followTable[pos];
            row.insert(row.end(), rightInfo.firstPositions.begin(), rightInfo.firstPositions.end());
            std::sort(row.begin(), row.end());
            row.erase(std::unique(row.begin(), row.end()), row.end());
        }

        RegexNodeInfo result;
        result.nullable = leftInfo.nullable && rightInfo.nullable;

        result.firstPositions = leftInfo.firstPositions;
        if (leftInfo.nullable) mergePositions(result.firstPositions, rightInfo.firstPositions);

        result.lastPositions = rightInfo.lastPositions;
        if (rightInfo.nullable) mergePositions(result.lastPositions, leftInfo.lastPositions);

        return result;
    }

    RegexNodeInfo analyzeNode(const std::unique_ptr<regexp::Iteration> &loopNode) {
        auto info = std::visit([this](auto &&n) { return analyzeNode(n); }, loopNode->m_node);

        for (size_t pos: info.lastPositions) {
            auto &row = followTable[pos];
            row.insert(row.end(), info.firstPositions.begin(), info.firstPositions.end());
            std::sort(row.begin(), row.end());
            row.erase(std::unique(row.begin(), row.end()), row.end());
        }

        return {true, info.firstPositions, info.lastPositions};
    }
};

std::set<size_t> wordsMatch(const regexp::RegExp &regexp, const std::vector<Word> &words) {
    RegexAutomatonMaker maker;
    maker.buildAutomaton(regexp);

    std::set<size_t> matchedWordIndexes;

    auto &initialPositions = maker.finalNodeInfo.firstPositions;
    auto &finalPositions = maker.finalNodeInfo.lastPositions;

    std::vector<bool> isFinal(maker.nodeSymbols.size(), false);
    for (auto p: finalPositions) isFinal[p] = true;

    std::vector<size_t> currentState;
    std::vector<size_t> nextState;
    currentState.reserve(maker.nodeSymbols.size());
    nextState.reserve(maker.nodeSymbols.size());

    for (size_t wordIndex = 0; wordIndex < words.size(); ++wordIndex) {
        const auto &w = words[wordIndex];

        if (w.empty()) {
            if (maker.finalNodeInfo.nullable) matchedWordIndexes.insert(wordIndex);
            continue;
        }

        currentState.clear();

        uint8_t ch0 = w[0];
        for (size_t pos: initialPositions) {
            if (maker.nodeSymbols[pos] == ch0) currentState.push_back(pos);
        }

        if (currentState.empty()) continue;

        bool valid = true;

        for (size_t ci = 1; ci < w.size(); ++ci) {
            nextState.clear();
            uint8_t ch = w[ci];

            for (size_t state: currentState) {
                const auto &row = maker.followTable[state];
                for (size_t nextPos: row) {
                    if (maker.nodeSymbols[nextPos] == ch) nextState.push_back(nextPos);
                }
            }

            if (nextState.empty()) {
                valid = false;
                break;
            }

            std::sort(nextState.begin(), nextState.end());
            nextState.erase(std::unique(nextState.begin(), nextState.end()), nextState.end());

            currentState = std::move(nextState);
        }

        if (valid) {
            for (size_t p: currentState) {
                if (isFinal[p]) {
                    matchedWordIndexes.insert(wordIndex);
                    break;
                }
            }
        }
    }

    return matchedWordIndexes;
}


#ifndef __PROGTEST__
int main() {
    // basic test 1
    regexp::RegExp re1 = std::make_unique<regexp::Iteration>(
        std::make_unique<regexp::Concatenation>(
            std::make_unique<regexp::Concatenation>(
                std::make_unique<regexp::Concatenation>(
                    std::make_unique<regexp::Iteration>(
                        std::make_unique<regexp::Alternation>(
                            std::make_unique<regexp::Symbol>('a'),
                            std::make_unique<regexp::Symbol>('b'))),
                    std::make_unique<regexp::Symbol>('a')),
                std::make_unique<regexp::Symbol>('b')),
            std::make_unique<regexp::Iteration>(
                std::make_unique<regexp::Alternation>(
                    std::make_unique<regexp::Symbol>('a'),
                    std::make_unique<regexp::Symbol>('b')))));
    assert(wordsMatch(re1, {Word{}}) == std::set<size_t>{0});
    assert(wordsMatch(re1, {Word{'a', 'b'}}) == std::set<size_t>{0});
    assert(wordsMatch(re1, {Word{'a'}}) == std::set<size_t>{});
    assert(wordsMatch(re1, {Word{'a', 'a', 'a', 'a'}}) == std::set<size_t>{});
    assert(wordsMatch(re1, {Word{'a', 'a', 'a', 'c'}}) == std::set<size_t>{});
    assert(wordsMatch(re1, {Word{'a', 'a', 0x07, 'c'}}) == std::set<size_t>{});
    assert(wordsMatch(re1, {Word{'a', 'a', 'b'}}) == std::set<size_t>{0});
    assert(
        wordsMatch(re1, {Word{'a', 'a', 'b', 'a', 'a', 'b', 'a', 'a', 'b', 'a', 'a', 'b', 'a', 'a', 'b', 'a', 'a', 'b'}}
        ) == std::set<size_t>{0});
    assert(
        (wordsMatch(re1, {Word{}, Word{'a', 'b'}, Word{'a'}, Word{'a', 'a', 'a', 'a'}, Word{'a', 'a', 'a', 'c'}, Word{
            'a', 'a', 0x07, 'c'}, Word{'a', 'a', 'b'}, Word{'a', 'a', 'b', 'a', 'a', 'b', 'a', 'a', 'b', 'a', 'a', 'b',
            'a', 'a', 'b', 'a', 'a', 'b'}}) == std::set<size_t>{0, 1, 6, 7}));

    // basic test 2
    regexp::RegExp re2 = std::make_unique<regexp::Concatenation>(
        std::make_unique<regexp::Concatenation>(
            std::make_unique<regexp::Iteration>(
                std::make_unique<regexp::Concatenation>(
                    std::make_unique<regexp::Concatenation>(
                        std::make_unique<regexp::Iteration>(
                            std::make_unique<regexp::Alternation>(
                                std::make_unique<regexp::Symbol>('a'),
                                std::make_unique<regexp::Symbol>('b'))),
                        std::make_unique<regexp::Iteration>(
                            std::make_unique<regexp::Alternation>(
                                std::make_unique<regexp::Symbol>('c'),
                                std::make_unique<regexp::Symbol>('d')))),
                    std::make_unique<regexp::Iteration>(
                        std::make_unique<regexp::Alternation>(
                            std::make_unique<regexp::Symbol>('e'),
                            std::make_unique<regexp::Symbol>('f'))))),
            std::make_unique<regexp::Empty>()),
        std::make_unique<regexp::Iteration>(
            std::make_unique<regexp::Alternation>(
                std::make_unique<regexp::Symbol>('a'),
                std::make_unique<regexp::Symbol>('b'))));
    assert(wordsMatch(re2, {Word{}}) == std::set<size_t>{});
    assert(wordsMatch(re2, {Word{'a', 'b'}}) == std::set<size_t>{});
    assert(wordsMatch(re2, {Word{'a', 'b', 'c', 'd'}}) == std::set<size_t>{});
    assert(wordsMatch(re2, {Word{'a', 'b', 'c', 'd', 'e', 'f'}}) == std::set<size_t>{});
    assert(wordsMatch(re2, {Word{'a', 'b', 'c', 'd', 'e', 'f', 'a', 'b'}}) == std::set<size_t>{});
    assert(
        (wordsMatch(re2, {Word{}, Word{'a', 'b'}, Word{'a', 'b', 'c', 'd'}, Word{'a', 'b', 'c', 'd', 'e', 'f'}, Word{'a'
            , 'b', 'c', 'd', 'e', 'f', 'a', 'b'}}) == std::set<size_t>{}));

    // basic test 3
    regexp::RegExp re3 = std::make_unique<regexp::Concatenation>(
        std::make_unique<regexp::Concatenation>(
            std::make_unique<regexp::Concatenation>(
                std::make_unique<regexp::Symbol>('0'),
                std::make_unique<regexp::Symbol>('1')),
            std::make_unique<regexp::Symbol>('1')),
        std::make_unique<regexp::Iteration>(
            std::make_unique<regexp::Alternation>(
                std::make_unique<regexp::Alternation>(
                    std::make_unique<regexp::Concatenation>(
                        std::make_unique<regexp::Concatenation>(
                            std::make_unique<regexp::Symbol>('0'),
                            std::make_unique<regexp::Symbol>('1')),
                        std::make_unique<regexp::Symbol>('1')),
                    std::make_unique<regexp::Concatenation>(
                        std::make_unique<regexp::Concatenation>(
                            std::make_unique<regexp::Symbol>('1'),
                            std::make_unique<regexp::Iteration>(
                                std::make_unique<regexp::Symbol>('0'))),
                        std::make_unique<regexp::Symbol>('1'))),
                std::make_unique<regexp::Symbol>('0'))));
    assert(wordsMatch(re3, {Word{'0', '1'}}) == std::set<size_t>{});
    assert(wordsMatch(re3, {Word{'0', '1', '1'}}) == std::set<size_t>{0});
    assert(wordsMatch(re3, {Word{'0', '1', '1', '0'}}) == std::set<size_t>{0});
    assert(wordsMatch(re3, {Word{'0', '1', '1', '0', '1', '1', '1', '0', '0', '0'}}) == std::set<size_t>{});
    assert(wordsMatch(re3, {Word{'0', '1', '1', '0', '1', '1', '1', '0', '0', '1'}}) == std::set<size_t>{0});
    assert(wordsMatch(re3, {Word{'0', '1', '1', '0', '1', '1', '1', '0', '0', '1', '0'}}) == std::set<size_t>{0});
    assert(
        (wordsMatch(re3, {Word{'0', '1'}, Word{'0', '1', '1'}, Word{'0', '1', '1', '0'}, Word{'0', '1', '1', '0', '1',
            '1', '1', '0', '0', '0'}, Word{'0', '1', '1', '0', '1', '1', '1', '0', '0', '1'}, Word{'0', '1', '1', '0',
            '1', '1', '1', '0', '0', '1', '0'}}) == std::set<size_t>{1, 2, 4, 5}));
}
#endif
