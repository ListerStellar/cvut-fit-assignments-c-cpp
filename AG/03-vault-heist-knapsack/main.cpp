#ifndef __PROGTEST__
#include <cassert>
#include <iomanip>
#include <cstdint>
#include <iostream>
#include <memory>
#include <limits>
#include <optional>
#include <algorithm>
#include <functional>
#include <bitset>
#include <list>
#include <array>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <random>
#include <type_traits>
#include <compare>

struct Vault {
    unsigned value;
    unsigned pebbles;
    std::vector<std::pair<unsigned, unsigned> > missing_connections;
};

struct UnlockingSequence {
    unsigned vault_id;
    std::vector<bool> moved_pebbles;
};

#endif

struct Item {
    unsigned weight;
    unsigned value;
};

size_t getPos(const size_t x, const size_t y, const size_t width) {
    return x*width + y;
}

std::vector<bool> knapsack(const std::vector<Item>& items, unsigned max_weight) {
    std::vector<bool> result; result.resize(items.size());
    std::vector<unsigned> memory; memory.resize(max_weight + 1);
    std::vector<bool> pickedUp((max_weight + 1) * items.size());

    for (size_t i = 0; i < items.size(); ++i) {
        for (size_t weight = max_weight; weight >= items[i].weight; weight--) {
            if (memory[weight] < memory[weight - items[i].weight] + items[i].value) {
                memory[weight] = memory[weight - items[i].weight] + items[i].value;
                pickedUp[getPos(i, weight, max_weight + 1)] = true;
            }
        }
    }

    size_t weight = max_weight;
    for (int i = static_cast<int>(items.size()) - 1; i >= 0; i--) {
        if (pickedUp[getPos(i, weight, max_weight + 1)] == true) {
            weight = weight - items[i].weight;
            result[i] = true;
        }
    }

    return result;
}

std::vector<UnlockingSequence> plan_heist(
    const std::vector<Vault> &vaults,
    unsigned transition_time,
    unsigned max_time
) {
    std::vector<Item> itemVector; itemVector.reserve(vaults.size());
    std::vector<std::pair<unsigned, std::vector<bool>>> possibleResults; possibleResults.reserve(vaults.size());

    for (unsigned i = 0; i < vaults.size(); i++) {
        const auto&[value, pebbles, missing_connections] = vaults[i];
        if (pebbles % 2 == 0) {
            std::vector<std::vector<unsigned>> neighbors(pebbles);
            for (const auto&[e1, e2] : missing_connections) {
                neighbors[e1].emplace_back(e2);
                neighbors[e2].emplace_back(e1);
            }

            std::vector<std::vector<unsigned>> parts;
            std::vector<bool> visited(pebbles, false);

            for (unsigned i = 0; i < pebbles; ++i) {
                if (!visited[i]) {
                    std::vector<unsigned> tmp;
                    std::queue<unsigned> toVisit;
                    toVisit.emplace(i); visited[i] = true;
                    while(!toVisit.empty()) {
                        auto current = toVisit.front(); toVisit.pop();
                        tmp.push_back(current);
                        for (auto neighbor : neighbors[current]) {
                            if (!visited[neighbor]) {
                                toVisit.emplace(neighbor);
                                visited[neighbor].flip();
                            }
                        }
                    }
                    parts.push_back(std::move(tmp));
                }
            }

            std::vector<int> memory(pebbles / 2 + 1, -1337);
            memory[0] = -228;

            for (size_t i = 0; i < parts.size(); ++i) {
                for (unsigned j = pebbles / 2; j >= parts[i].size(); --j) {
                    if (memory[j] == -1337 && memory[j - parts[i].size()] != -1337) {
                        memory[j] = static_cast<int>(i);
                    }
                }
            }

            if (memory[pebbles / 2] != -1337) {
                std::vector<bool> tmp; tmp.resize(pebbles);
                unsigned nowWeight = pebbles / 2;
                while (nowWeight > 0) {
                    int memi = memory[nowWeight];
                    for (const auto it : parts[memi]) {
                        tmp[it] = true;
                    }
                    nowWeight = nowWeight - parts[memi].size();
                }

                if (pebbles / 2 <= max_time) {
                    itemVector.emplace_back(pebbles / 2 + transition_time, value);
                    possibleResults.emplace_back(i, std::move(tmp));
                }
            }
        }
    }

    std::vector<bool> resParts = knapsack(itemVector, max_time + transition_time);

    std::vector<UnlockingSequence> result;
    for (size_t i = 0; i < resParts.size(); ++i) {
        if (resParts[i]) {
            result.emplace_back(possibleResults[i].first, std::move(possibleResults[i].second));
        }
    }

    return result;
}

#ifndef __PROGTEST__

struct TestFailed : std::runtime_error {
    using std::runtime_error::runtime_error;
};

#define CHECK(cond, msg) do { \
    if (!(cond)) throw TestFailed(msg); \
  } while (0)

void check_unlocking_sequence(
    unsigned pebbles,
    const std::vector<std::pair<unsigned, unsigned> > &missing_connections,
    const std::vector<bool> &moved
) {
    CHECK(moved.size() == pebbles, "Solution has wrong size.\n");

    size_t moved_cnt = 0;
    for (bool p: moved) moved_cnt += p;
    CHECK(2*moved_cnt == pebbles,
          "Exactly half of the pebbles must be moved.\n");

    for (auto [u, v]: missing_connections)
        CHECK(moved[u] == moved[v],
          "Pebble not connected with all on other side.\n");
}

void check_solution(
    const std::vector<UnlockingSequence> &solution,
    unsigned expected_value,
    const std::vector<Vault> &vaults,
    unsigned transition_time,
    unsigned max_time
) {
    unsigned time = 0, value = 0;
    std::vector<bool> robbed(vaults.size(), false);

    for (size_t i = 0; i < solution.size(); i++) {
        const auto &[id, moved] = solution[i];

        CHECK(id < vaults.size(), "Id is out of range.\n");
        CHECK(!robbed[id], "Robbed same vault twice.\n");
        robbed[id] = true;

        const auto &vault = vaults[id];
        value += vault.value;

        if (i != 0) time += transition_time;
        time += vault.pebbles / 2;
        CHECK(time <= max_time, "Run out of time.\n");

        check_unlocking_sequence(vault.pebbles, vault.missing_connections, moved);
    }

    CHECK(value == expected_value, "Total value mismatch.\n");
}


struct Test {
    unsigned expected_value;
    unsigned max_time;
    unsigned transition_time;
    std::vector<Vault> vaults;
};

inline const std::vector<Test> TESTS = {
    Test{
        .expected_value = 3010, .max_time = 3, .transition_time = 8,
        .vaults = {
            {.value = 3010, .pebbles = 6, .missing_connections = {{3, 4}, {0, 1}, {4, 5}, {5, 3},}},
            {.value = 3072, .pebbles = 6, .missing_connections = {{2, 1}, {1, 3}, {0, 1}, {0, 3}, {4, 5}, {2, 3},}},
            {
                .value = 5069, .pebbles = 10,
                .missing_connections = {{7, 2}, {3, 4}, {0, 1}, {8, 4}, {1, 2}, {8, 3}, {7, 0}, {5, 6}, {9, 5}, {9, 6},}
            },
            {.value = 2061, .pebbles = 4, .missing_connections = {{3, 0}, {2, 1}, {0, 2}, {1, 3},}},
        }
    },
    Test{
        .expected_value = 6208, .max_time = 13, .transition_time = 12,
        .vaults = {
            {
                .value = 6011, .pebbles = 12,
                .missing_connections = {
                    {1, 5}, {2, 4}, {5, 10}, {1, 10}, {0, 3}, {8, 3}, {8, 0}, {9, 8}, {2, 6}, {3, 9}, {0, 9}, {4, 6},
                    {11, 7},
                }
            },
            {.value = 2056, .pebbles = 4, .missing_connections = {{1, 0}, {2, 0}, {2, 1},}},
            {
                .value = 5885, .pebbles = 12,
                .missing_connections = {
                    {1, 6}, {3, 7}, {1, 0}, {2, 9}, {9, 8}, {2, 8}, {5, 7}, {11, 4}, {10, 1}, {5, 3}, {0, 10},
                }
            },
            {
                .value = 5818, .pebbles = 12,
                .missing_connections = {
                    {9, 0}, {7, 1}, {6, 4}, {8, 6}, {4, 2}, {11, 5}, {5, 3}, {9, 7}, {8, 4}, {2, 8}, {10, 11}, {5, 10},
                    {10, 3}, {9, 1},
                }
            },
            {
                .value = 4880, .pebbles = 10,
                .missing_connections = {{7, 3}, {4, 1}, {9, 2}, {6, 9}, {2, 6}, {5, 0}, {8, 4},}
            },
            {
                .value = 5233, .pebbles = 10,
                .missing_connections = {{0, 2}, {4, 5}, {8, 3}, {9, 7}, {7, 1}, {6, 3}, {6, 8},}
            },
            {
                .value = 6208, .pebbles = 12,
                .missing_connections = {
                    {1, 7}, {3, 4}, {10, 7}, {0, 3}, {8, 2}, {5, 1}, {9, 11}, {0, 6}, {6, 3}, {10, 1}, {0, 4},
                }
            },
            {
                .value = 4182, .pebbles = 8,
                .missing_connections = {{5, 7}, {7, 4}, {4, 5}, {1, 0}, {5, 6}, {3, 1}, {6, 4}, {0, 3}, {6, 7},}
            },
        }
    },
    Test{
        .expected_value = 1, .max_time = 100, .transition_time = 8,
        .vaults = {
            {
                .value = 1, .pebbles = 14, .missing_connections = {
                    {0, 1}, {0, 2}, {3, 4}, {4, 5}, {6, 7}, {8, 9}, {10, 11}, {12, 13},
                }
            },
        }
    },
};

int main() {
    int ok = 0, fail = 0;

    for (auto t: TESTS) {
        try {
            auto sol = plan_heist(t.vaults, t.transition_time, t.max_time);
            check_solution(sol, t.expected_value, t.vaults, t.transition_time, t.max_time);
            ok++;
        } catch (const TestFailed &) {
            fail++;
        }
    }

    if (!fail) std::cout << "Passed all " << ok << " tests!" << std::endl;
    else std::cout << "Failed " << fail << " of " << (ok + fail) << " tests." << std::endl;
}

#endif
