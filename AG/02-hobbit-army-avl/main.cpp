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
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <random>
#include <type_traits>
#include <utility>

struct Hobbit {
    std::string name;
    int hp, off, def;

    friend bool operator ==(const Hobbit &, const Hobbit &) = default;
};

std::ostream &operator <<(std::ostream &out, const Hobbit &h) {
    return out
           << "Hobbit{\"" << h.name << "\", "
           << ".hp=" << h.hp << ", "
           << ".off=" << h.off << ", "
           << ".def=" << h.def << "}";
}

template<typename T>
std::ostream &operator <<(std::ostream &out, const std::optional<T> &x) {
    if (!x) return out << "EMPTY_OPTIONAL";
    return out << "Optional{" << *x << "}";
}

#endif

struct Node {
    //std::string key;
    Hobbit value;
    Node *parent;
    Node *left;
    Node *right;

    int pending_update_hp;
    int pending_update_off;
    int pending_update_def;

    int height;
    std::string minSubTreeKey;
    std::string maxSubTreeKey;

    int minSubTreeHobbitHP;

    void updateData() {
        int leftHeight = left ? left->height : 0;
        int rightHeight = right ? right->height : 0;
        height = std::max(leftHeight, rightHeight) + 1;

        minSubTreeKey = left ? left->minSubTreeKey : value.name;
        maxSubTreeKey = right ? right->maxSubTreeKey : value.name;

        minSubTreeHobbitHP = value.hp;
        minSubTreeHobbitHP = left ? std::min(minSubTreeHobbitHP, left->minSubTreeHobbitHP + left->pending_update_hp) : minSubTreeHobbitHP;
        minSubTreeHobbitHP = right ? std::min(minSubTreeHobbitHP, right->minSubTreeHobbitHP + right->pending_update_hp) : minSubTreeHobbitHP;
    }

    int sign() {
        int leftHeight = left ? left->height : 0;
        int rightHeight = right ? right->height : 0;
        return rightHeight - leftHeight;
    }

    //Node(Hobbit value, Node *p) : key(value.name), value(value), parent(p), left(nullptr), right(nullptr), pending_update_hp(0), pending_update_off(0), pending_update_def(0), height(1), minSubTreeKey(value.name), maxSubTreeKey(value.name), minSubTreeHobbitHP(value.hp) {}
    Node(Hobbit value, Node *p) : value(std::move(value)), parent(p), left(nullptr), right(nullptr), pending_update_hp(0), pending_update_off(0), pending_update_def(0), height(1), minSubTreeKey(this->value.name), maxSubTreeKey(this->value.name), minSubTreeHobbitHP(this->value.hp) {}
};

struct HobbitArmy {
    static constexpr bool CHECK_NEGATIVE_HP = true;

    ~HobbitArmy() {
        deleteTree(root);
    }

    size_t size() const { return tsize; }

    bool add(const Hobbit &hobbit) {
        if (hobbit.hp <= 0) return false;
        size_t nowSize = size();
        root = BVSInsert(root, nullptr, hobbit.name, hobbit);
        return nowSize != size();
    }

    std::optional<Hobbit> erase(const std::string &hobbit_name) {
        Hobbit result;
        size_t nowSize = size();
        root = BVSErase(root, hobbit_name, &result, false);
        if (nowSize != size()) {
            return {result};
        }
        return std::nullopt;
    }

    std::optional<Hobbit> stats(const std::string &hobbit_name) const {
        Node *node = BVSFind(root, hobbit_name);
        if (!node) {
            return std::nullopt;
        }
        return {node->value};
    }

    bool enchant(
        const std::string &first,
        const std::string &last,
        int hp_diff,
        int off_diff,
        int def_diff
    ) {
        if (last < first) return true;

        if (hp_diff < 0) {
            int minhp = BVSFindMinHP(root, first, last);
            if (minhp != std::numeric_limits<int>::max() && minhp + hp_diff <= 0) return false;
        }

        root = BVSUpdate(root, first, last, hp_diff, off_diff, def_diff);
        return true;
    }

    void for_each(auto &&fun) const {
        for_each_impl(root, fun);
    }

private:
    Node *root = nullptr;
    size_t tsize = 0;

    void deleteTree(Node *v) {
        if (!v) return;
        deleteTree(v->left);
        deleteTree(v->right);
        delete v;
    }

    static void for_each_impl(Node *node, auto &fun) {
        if (!node) return;
        lazyPush(node);
        for_each_impl(node->left, fun);
        fun(node->value);
        for_each_impl(node->right, fun);
    }

    static void lazyPush(Node *v) {
        if (!v || (v->pending_update_hp == 0 && v->pending_update_def == 0 && v->pending_update_off == 0)) return;
        v->value.hp += v->pending_update_hp;
        v->value.off += v->pending_update_off;
        v->value.def += v->pending_update_def;
        if (v->left) {
            v->left->pending_update_hp += v->pending_update_hp;
            v->left->pending_update_off += v->pending_update_off;
            v->left->pending_update_def += v->pending_update_def;
        }
        if (v->right) {
            v->right->pending_update_hp += v->pending_update_hp;
            v->right->pending_update_off += v->pending_update_off;
            v->right->pending_update_def += v->pending_update_def;
        }
        v->pending_update_hp = 0;
        v->pending_update_off = 0;
        v->pending_update_def = 0;
        v->updateData();
    }

    int BVSFindMinHP(Node* v, const std::string& first, const std::string& last) {
        if (!v || v->maxSubTreeKey < first || last < v->minSubTreeKey) return std::numeric_limits<int>::max();
        lazyPush(v);
        if (first <= v->minSubTreeKey && v->maxSubTreeKey <= last) {
            return v->minSubTreeHobbitHP;
        }
        return std::min({first <= v->value.name && v->value.name <= last ? v->value.hp : std::numeric_limits<int>::max(), BVSFindMinHP(v->left, first, last), BVSFindMinHP(v->right, first, last)});
    }

    Node *BVSUpdate(Node* v, const std::string &first, const std::string &last, int hp_diff, int off_diff, int def_diff) {
        if (!v) return nullptr;
        if (v->maxSubTreeKey < first || last < v->minSubTreeKey) return v;
        if (first <= v->minSubTreeKey && v->maxSubTreeKey <= last) {
            v->pending_update_hp += hp_diff;
            v->pending_update_off += off_diff;
            v->pending_update_def += def_diff;
            return v;
        }
        lazyPush(v);
        if (first <= v->value.name && v->value.name <= last) {
            v->value.hp += hp_diff;
            v->value.off += off_diff;
            v->value.def += def_diff;
        }
        v->left = BVSUpdate(v->left, first, last, hp_diff, off_diff, def_diff);
        v->right = BVSUpdate(v->right, first, last, hp_diff, off_diff, def_diff);
        v->updateData();
        return v;
    }

    Node *BVSFind(Node *v, const std::string &x) const {
        if (!v) return nullptr;
        lazyPush(v);
        auto res = x <=> v->value.name;
        if (res < 0) return BVSFind(v->left, x);
        if (res > 0) return BVSFind(v->right, x);
        return v;
    }

    Node *rotateLeft(Node *x) {
        lazyPush(x);
        Node *y = x->right;
        lazyPush(y);
        x->right = y->left;
        y->left = x;
        if (x->right)
            x->right->parent = x;
        y->parent = x->parent;
        x->parent = y;
        x->updateData();
        y->updateData();
        return y;
    }

    Node *rotateRight(Node *x) {
        lazyPush(x);
        Node *y = x->left;
        lazyPush(y);
        x->left = y->right;
        y->right = x;
        if (x->left)
            x->left->parent = x;
        y->parent = x->parent;
        x->parent = y;
        x->updateData();
        y->updateData();
        return y;
    }

    Node *BVSInsert(Node *v, Node *p, const std::string &x, const Hobbit &val) {
        if (!v) {
            ++tsize;
            return new Node(val, p);
        }
        lazyPush(v);
        auto res = x <=> v->value.name;
        if (res < 0) {
            int prevLeftHeight = v->left ? v->left->height : 0;

            v->left = BVSInsert(v->left, v, x, val);
            v->updateData();

            if (prevLeftHeight != (v->left ? v->left->height : 0)) {
                if (v->sign() < -1) {
                    if (v->left->sign() > 0)
                        v->left = rotateLeft(v->left);
                    v = rotateRight(v);
                }
            }
        }
        if (res > 0) {
            int prevRightHeight = v->right ? v->right->height : 0;

            v->right = BVSInsert(v->right, v, x, val);
            v->updateData();

            if (prevRightHeight != (v->right ? v->right->height : 0)) {
                if (v->sign() > 1) {
                    if (v->right->sign() < 0)
                        v->right = rotateRight(v->right);
                    v = rotateLeft(v);
                }
            }
        }

        return v;
    }

    Node *BVSMin(Node *v) {
        if (!v) return nullptr;
        lazyPush(v);
        if (!v->left) return v;
        return BVSMin(v->left);
    }

    Node *BVSErase(Node *v, const std::string &x, Hobbit *result, bool resultSet) {
        if (!v) return nullptr;
        lazyPush(v);
        auto res = x <=> v->value.name;
        if (res < 0) {
            int prevLeftHeight = v->left ? v->left->height : 0;

            v->left = BVSErase(v->left, x, result, resultSet);
            v->updateData();

            if (prevLeftHeight != (v->left ? v->left->height : 0)) {
                if (v->sign() > 1) {
                    if (v->right->sign() == -1) {
                        v->right = rotateRight(v->right);
                    }
                    Node *parent = v->parent;
                    if (parent) {
                        if (parent->left == v) { parent->left = rotateLeft(v); return parent->left; }
                        if (parent->right == v) { parent->right = rotateLeft(v); return parent->right; }
                    } else { root = rotateLeft(v); return root; }
                }
            }
        }
        else if (res > 0) {
            int prevRightHeight = v->right ? v->right->height : 0;

            v->right = BVSErase(v->right, x, result, resultSet);
            v->updateData();

            if (prevRightHeight != (v->right ? v->right->height : 0)) {
                if (v->sign() < -1) {
                    if (v->left->sign() == 1) {
                        v->left = rotateLeft(v->left);
                    }
                    Node *parent = v->parent;
                    if (parent) {
                        if (parent->left == v) { parent->left = rotateRight(v); return parent->left; }
                        if (parent->right == v) { parent->right = rotateRight(v); return parent->right; }
                    } else { root = rotateRight(v); return root; }
                }
            }
        }
        else {
            if (!resultSet) {
                resultSet = true;
                *result = v->value;
            }
            if (!v->left && !v->right) { --tsize; delete v; return nullptr; }
            if (!v->left) { --tsize; v->right->parent = v->parent; Node *tmp = v->right; delete v; return tmp; }
            if (!v->right) { --tsize; v->left->parent = v->parent; Node *tmp = v->left; delete v; return tmp; }
            Node *w = BVSMin(v->right);
            v->value = w->value;
            v->updateData();

            int prevRightHeight = v->right ? v->right->height : 0;

            v->right = BVSErase(v->right, w->value.name, result, resultSet);
            v->updateData();

            if (prevRightHeight != (v->right ? v->right->height : 0)) {
                if (v->sign() < -1) {
                    if (v->left->sign() == 1) {
                        v->left = rotateLeft(v->left);
                    }
                    Node *parent = v->parent;
                    if (parent) {
                        if (parent->left == v) { parent->left = rotateRight(v); return parent->left; }
                        if (parent->right == v) { parent->right = rotateRight(v); return parent->right; }
                    } else { root = rotateRight(v); return root; }
                }
            }
        }
        return v;
    }
};

#ifndef __PROGTEST__

////////////////// Dark magic, ignore ////////////////////////

template<typename T>
auto quote(const T &t) { return t; }

std::string quote(const std::string &s) {
    std::string ret = "\"";
    for (char c: s) if (c != '\n') ret += c;
    else ret += "\\n";
    return ret + "\"";
}

#define STR_(a) #a
#define STR(a) STR_(a)

#define CHECK_(a, b, a_str, b_str) do { \
    auto _a = (a); \
    decltype(a) _b = (b); \
    if (_a != _b) { \
      std::cout << "Line " << __LINE__ << ": Assertion " \
        << a_str << " == " << b_str << " failed!" \
        << " (lhs: " << quote(_a) << ")" << std::endl; \
      fail++; \
    } else ok++; \
  } while (0)

#define CHECK(a, b) CHECK_(a, b, #a, #b)


////////////////// End of dark magic ////////////////////////


void check_army(const HobbitArmy &A, const std::vector<Hobbit> &ref, int &ok, int &fail) {
    size_t i = 0;

    A.for_each([&](const Hobbit &h) {
        CHECK(i < ref.size(), true);
        CHECK(h, ref[i]);
        i++;
    });

    CHECK(i, ref.size());
}

void test1(int &ok, int &fail) {
    HobbitArmy A;
    check_army(A, {}, ok, fail);

    CHECK(A.add({"Frodo", 100, 10, 3}), true);
    CHECK(A.add({"Frodo", 200, 10, 3}), false);
    CHECK(A.erase("Frodo"), std::optional(Hobbit("Frodo", 100, 10, 3)));
    CHECK(A.add({"Frodo", 200, 10, 3}), true);

    CHECK(A.add({"Sam", 80, 10, 4}), true);
    CHECK(A.add({"Pippin", 60, 12, 2}), true);
    CHECK(A.add({"Merry", 60, 15, -3}), true);
    CHECK(A.add({"Smeagol", 0, 100, 100}), false);

    if constexpr (HobbitArmy::CHECK_NEGATIVE_HP)
        CHECK(A.add({"Smeagol", -100, 100, 100}), false);

    CHECK(A.add({"Smeagol", 200, 100, 100}), true);

    CHECK(A.enchant("Frodo", "Frodo", 10, 1, 1), true);
    CHECK(A.enchant("Sam", "Frodo", -1000, 1, 1), true); // empty range
    CHECK(A.enchant("Bilbo", "Bungo", 1000, 0, 0), true); // empty range

    if constexpr (HobbitArmy::CHECK_NEGATIVE_HP)
        CHECK(A.enchant("Frodo", "Sam", -60, 1, 1), false);

    CHECK(A.enchant("Frodo", "Sam", 1, 0, 0), true);
    CHECK(A.enchant("Frodo", "Sam", -60, 1, 1), true);

    CHECK(A.stats("Gandalf"), std::optional<Hobbit>{});
    CHECK(A.stats("Frodo"), std::optional(Hobbit("Frodo", 151, 12, 5)));
    CHECK(A.stats("Merry"), std::optional(Hobbit("Merry", 1, 16, -2)));

    check_army(A, {
                   {"Frodo", 151, 12, 5},
                   {"Merry", 1, 16, -2},
                   {"Pippin", 1, 13, 3},
                   {"Sam", 21, 11, 5},
                   {"Smeagol", 200, 100, 100},
               }, ok, fail);
}

int main() {
    int ok = 0, fail = 0;
    test1(ok, fail);

    if (!fail) std::cout << "Passed all " << ok << " tests!" << std::endl;
    else std::cout << "Failed " << fail << " of " << (ok + fail) << " tests." << std::endl;
}

#endif
