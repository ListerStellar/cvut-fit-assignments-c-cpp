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
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <random>
#include <type_traits>
#include <compare>
#include <ranges>
#include <optional>
#include <variant>

struct Item {
  enum Type : uint8_t {
    Weapon = 0,
    Armor = 1,
    RubberDuck = 2,
    TYPE_COUNT = 3,
  };

  std::string name;
  Type type;
  int hp = 0, off = 0, def = 0;
  int stacking_off = 0, stacking_def = 0;
  bool first_attack = false; // Hero attacks first.
  bool stealth = false; // Hero can sneak past monsters (but cannot loot items while sneaking).

  friend auto operator <=> (const Item&, const Item&) = default;
};

struct Monster {
  int hp = 0, off = 0, def = 0;
  int stacking_off = 0, stacking_def = 0;
};

using RoomId = size_t;
using ItemId = size_t;

struct Room {
  std::vector<RoomId> neighbors;
  std::optional<Monster> monster;
  std::vector<Item> items;
};

struct Move { RoomId room; };
struct Pickup { ItemId item; };
struct Drop { Item::Type type; };
using Action = std::variant<Move, Pickup, Drop>;

namespace student_namespace {
#endif

  std::optional<int> turns_to_kill(int hp, int dmg, int stacking_dmg) {
    assert(hp > 0);

    if (stacking_dmg == 0) {
      if (dmg <= 0) return {};
      return (hp + dmg - 1) / dmg;
    }

    int i = 0;
    for (; hp > 0; i++) {
      if (dmg <= 0 && stacking_dmg < 0) return {};
      hp -= std::max(dmg, 0);
      dmg += stacking_dmg;
    }

    return i;
  }

  enum CombatResult {
    A_WINS, B_WINS, TIE
  };

  // Monster `a` attacks first
  CombatResult simulate_combat(Monster a, Monster b) {
    a.def += a.stacking_def;

    auto a_turns = turns_to_kill(b.hp, a.off - b.def, a.stacking_off - b.stacking_def);
    auto b_turns = turns_to_kill(a.hp, b.off - a.def, b.stacking_off - a.stacking_def);

    if (!a_turns && !b_turns) return TIE;
    if (!a_turns) return B_WINS;
    if (!b_turns) return A_WINS;
    return *a_turns <= *b_turns ? A_WINS : B_WINS;
  }

  bool contains(const auto &vec, const auto &x) {
    return std::ranges::find(vec, x) != vec.end();
  };

  struct State {
    //Room room;
    RoomId roomID;
    bool winnedMonster = true;
    bool collectedTreasure = false;

    // 0 - weapon, 1 - armour, 2 - duck
    std::vector<std::optional<Item>> items = std::vector<std::optional<Item>>(Item::Type::TYPE_COUNT);

    Monster stats = {.hp = 10000, .off = 3, .def = 2};
    bool attacksFirst = false;
    bool canSneak = false;

    State() = default;

    void updateStats() {
      stats = {.hp = 10000, .off = 3, .def = 2};
      attacksFirst = false;
      canSneak = false;
      for (auto item : items) {
        if (item.has_value()) {
          stats.hp = stats.hp + item.value().hp;
          stats.off += item.value().off;
          stats.def += item.value().def;
          stats.stacking_off += item.value().stacking_off;
          stats.stacking_def += item.value().stacking_def;

          if (item.value().first_attack) attacksFirst = true;
          if (item.value().stealth) canSneak = true;
        }
      }
      stats.hp = std::max(1, stats.hp);
    }

    bool operator<(const State& other) const {
      return std::tie(roomID, items, collectedTreasure, winnedMonster, attacksFirst, canSneak, stats.hp, stats.off, stats.def, stats.stacking_off, stats.stacking_def) < std::tie(other.roomID, other.items, other.collectedTreasure, other.winnedMonster, other.attacksFirst, other.canSneak, other.stats.hp, other.stats.off, other.stats.def, other.stats.stacking_off, other.stats.stacking_def);
    }

    bool operator==(const State& other) const {
      return roomID == other.roomID && winnedMonster == other.winnedMonster && collectedTreasure == other.collectedTreasure && attacksFirst == other.attacksFirst && canSneak == other.canSneak && items == other.items && stats.hp == other.stats.hp && stats.off == other.stats.off && stats.def == other.stats.def && stats.stacking_off == other.stats.stacking_off && stats.stacking_def == other.stats.stacking_def;
    }

  };

  struct SHash {
    size_t operator()(const State& state) const {
      size_t hash = 31;
      hash = hash * 29 + std::hash<size_t>()(state.roomID);
      hash = hash * 29 + std::hash<bool>()(state.winnedMonster);
      hash = hash * 29 + std::hash<bool>()(state.collectedTreasure);
      hash = hash * 29 + std::hash<bool>()(state.attacksFirst);
      hash = hash * 29 + std::hash<bool>()(state.canSneak);
      hash = hash * 29 + std::hash<int>()(state.stats.hp);
      hash = hash * 29 + std::hash<int>()(state.stats.off);
      hash = hash * 29 + std::hash<int>()(state.stats.def);
      hash = hash * 29 + std::hash<int>()(state.stats.stacking_off);
      hash = hash * 29 + std::hash<int>()(state.stats.stacking_def);
      for (auto& it : state.items) {
        if (it) {
          hash = hash * 29 + std::hash<int>()(it->hp);
          hash = hash * 29 + std::hash<int>()(it->off);
          hash = hash * 29 + std::hash<int>()(it->def);
          hash = hash * 29 + std::hash<int>()(it->stacking_off);
          hash = hash * 29 + std::hash<int>()(it->stacking_def);
          hash = hash * 29 + std::hash<bool>()(it->first_attack);
          hash = hash * 29 + std::hash<bool>()(it->stealth);
        }
      }
      return hash;
    }
  };

  State getNextState(const std::vector<Room>& rooms, RoomId treasure, State& nowState, Action& action) {
    State newState = nowState;
    if (auto a = std::get_if<Move>(&action)) {
      //newState.room = rooms[a->room];
      newState.roomID = a->room;

      bool canWin;
      if (rooms[newState.roomID].monster.has_value()) {
        if (newState.attacksFirst) {
          canWin = simulate_combat(newState.stats, rooms[newState.roomID].monster.value()) == A_WINS;
        } else {
          canWin = simulate_combat(rooms[newState.roomID].monster.value(), newState.stats) == B_WINS;
        }
      } else canWin = true;
      newState.winnedMonster = canWin;

      if (newState.roomID == treasure) {
        if (newState.winnedMonster) {
          newState.collectedTreasure = true;
        }
      }
    }
    if (auto p = std::get_if<Pickup>(&action)) {
      Item it = rooms[newState.roomID].items[p->item];
      newState.items.at(it.type).emplace(it);
      newState.updateStats();
    }
    if (auto d = std::get_if<Drop>(&action)) {
      newState.items.at(d->type).reset();
      newState.updateStats();
    }
    return newState;
  }

  std::vector<Action> getPossibleActions(const std::vector<Room>& rooms, const State &state) {
    std::vector<Action> actions;

    if (state.winnedMonster) {
      for (ItemId i = 0; i < rooms[state.roomID].items.size(); i++) {
        actions.emplace_back(Pickup{i});
      }
    }
    for (auto it : state.items) {
      if (it.has_value()) {
        actions.emplace_back(Drop{it->type});
      }
    }
    if (state.winnedMonster || state.canSneak) {
      for (auto it : rooms[state.roomID].neighbors) {
        actions.emplace_back(Move{it});
      }
    }

    return actions;
  }

  static constexpr RoomId START_UNREACHABLE_ROOM = std::numeric_limits<RoomId>::max();

  std::vector<Action> find_shortest_path(
    const std::vector<Room>& rooms,
    const std::vector<RoomId>& entrances,
    RoomId treasure
  ) {
    std::unordered_set<State, SHash> V;
    std::unordered_map<State, std::pair<State, Action>, SHash> P;
    std::deque<State> Q;

    //Room initRoom = {.neighbors = entrances};

    State initState;
    //initState.room = initRoom;
    initState.roomID = START_UNREACHABLE_ROOM;
    initState.winnedMonster = true;
    initState.collectedTreasure = false;
    initState.items = std::vector<std::optional<Item>>(Item::Type::TYPE_COUNT);
    initState.updateStats();

    std::vector<Action> possibleActions;
    for (auto it : entrances) {
      possibleActions.emplace_back(Move{it});
    }


    Q.emplace_back(initState);

    State v; bool found = false;
    while (!Q.empty()) {
      v = Q.front(); Q.pop_front();

      if (v.collectedTreasure && contains(entrances, v.roomID) && (v.winnedMonster || v.canSneak)) {
        found = true;
        break;
      }

      V.emplace(v);
      if (v.roomID != START_UNREACHABLE_ROOM) {
        possibleActions = getPossibleActions(rooms, v);
      }
      for (Action pA : possibleActions) {
        State w = getNextState(rooms, treasure, v, pA);
        if (!P.contains(w)) {
          P[w] = std::make_pair(v, pA);
          V.emplace(w);
          if (std::holds_alternative<Move>(pA)) {
            Q.emplace_back(w);
          } else {
            Q.emplace_front(w);
          }
        }
      }
    }

    std::vector<Action> path;
    if (found) {
      while (v.roomID != START_UNREACHABLE_ROOM) {
        path.emplace_back(P[v].second);
        v = P[v].first;
      }
      std::ranges::reverse(path);
    }

    return path;
  }

#ifndef __PROGTEST__
}

bool contains(const auto& vec, const auto& x) {
  return std::ranges::find(vec, x) != vec.end();
};

#define CHECK(cond, ...) do { \
    if (!(cond)) { fprintf(stderr, __VA_ARGS__); assert(0); } \
  } while (0)
void check_solution(
  const std::vector<Room>& rooms,
  const std::vector<RoomId>& entrances,
  RoomId treasure,
  size_t expected_rooms,
  bool print = false
) {
  // TODO check if hero survives combat
  // TODO check if treasure was collected

  using student_namespace::find_shortest_path;
  const std::vector<Action> solution = find_shortest_path(rooms, entrances, treasure);

  if (expected_rooms == 0) {
    CHECK(solution.size() == 0, "No solution should exist but got some.\n");
    return;
  }

  CHECK(solution.size() != 0, "Expected solution but got none.\n");

  try {
    CHECK(contains(entrances, std::get<Move>(solution.front()).room),
      "Path must start at entrance.\n");
    CHECK(contains(entrances, std::get<Move>(solution.back()).room),
      "Path must end at entrance.\n");
  } catch (const std::bad_variant_access&) {
    CHECK(false, "Path must start and end with Move.\n");
  }

  std::vector<Item> equip;
  RoomId cur = std::get<Move>(solution.front()).room;
  CHECK(cur < rooms.size(), "Room index out of range.\n");
  size_t room_count = 1;
  if (print) printf("Move(%zu)", cur);

  auto drop_items = [&](Item::Type type) {
    std::erase_if(equip, [&](const Item& i) { return i.type == type; });
  };

  for (size_t i = 1; i < solution.size(); i++) {
    if (auto m = std::get_if<Move>(&solution[i])) {
      CHECK(m->room < rooms.size(), "Next room index out of range.\n");
      CHECK(contains(rooms[cur].neighbors, m->room),
        "Next room is not a neighbor of the current one.\n");
      cur = m->room;
      room_count++;

      if (print) printf(", Move(%zu)", cur);
    } else if (auto p = std::get_if<Pickup>(&solution[i])) {
      CHECK(p->item < rooms[cur].items.size(), "Picked up item out of range.\n");
      const Item& item = rooms[cur].items[p->item];
      drop_items(item.type);
      equip.push_back(item);

      if (print) printf(", Pickup(%zu, %s)", p->item, item.name.c_str());
    } else {
      auto t = std::get<Drop>(solution[i]).type;
      drop_items(t);

      if (print) printf(", Drop(%s)",
        t == Item::Armor ? "Armor" :
        t == Item::Weapon ? "Weapon" :
        t == Item::RubberDuck ? "Duck" :
        "ERROR");
    }
  }

  if (print) printf("\n");

  CHECK(room_count == expected_rooms,
    "Expected %zu rooms but got %zu.\n", expected_rooms, room_count);
}
#undef CHECK


void combat_examples() {
  const Item defensive_duck = {
    .name = "Defensive Duck", .type = Item::RubberDuck,
    .off = -2, .def = 8,
  };

  const Item invincible_duck = {
    .name = "Invincible Duck", .type = Item::RubberDuck,
    .hp = -20'000, .def = 1'000,
  };

  const Item fast_duck = {
    .name = "Fast Duck", .type = Item::RubberDuck,
    .first_attack = true,
  };

  const Item offensive_duck = {
    .name = "Offensive Duck", .type = Item::RubberDuck,
    .stacking_off = 100,
  };

  std::vector<Room> rooms(2);
  rooms[0].neighbors.push_back(1);
  rooms[1].neighbors.push_back(0);

  check_solution(rooms, { 0 }, 1, 3);

  rooms[1].monster = Monster{ .hp = 9'999, .off = 3, .def = 2 };
  check_solution(rooms, { 0 }, 1, 3);

  rooms[1].monster->hp += 1;
  check_solution(rooms, { 0 }, 1, 0);

  rooms[1].monster = Monster{ .hp = 100'000, .off = 10 };
  check_solution(rooms, { 0 }, 1, 0);

  rooms[0].items = { defensive_duck };
  check_solution(rooms, { 0 }, 1, 3);

  rooms[0].items = { invincible_duck };
  check_solution(rooms, { 0 }, 1, 3);

  rooms[0].items = {};
  rooms[1].monster = Monster{ .hp=1, .off=3, .def=0, .stacking_def=100 };
  check_solution(rooms, { 0 }, 1, 0);

  rooms[0].items.push_back(offensive_duck);
  check_solution(rooms, { 0 }, 1, 0);

  rooms[0].items.push_back(fast_duck);
  check_solution(rooms, { 0 }, 1, 3);
}

void stealth_examples() {
  const Item stealth_duck = {
    .name = "Stealth Duck", .type = Item::RubberDuck,
    .stealth = true,
  };

  const Item sword = {
    .name = "Sword", .type = Item::Weapon,
    .off = 10,
  };

  const Monster m = { .hp = 10'000, .off=10, .def=2 };

  std::vector<Room> rooms(4);

  for (size_t i = 1; i < rooms.size(); i++) {
    rooms[i].neighbors.push_back(i - 1);
    rooms[i - 1].neighbors.push_back(i);
  }

  rooms[0].items = { stealth_duck };
  rooms[2].monster = m;

  check_solution(rooms, { 0 }, 2, 0); // Cannot stealth steal treasure

  rooms[3].items = { sword };
  // Stealth to 3, grab sword & kill monster
  check_solution(rooms, { 0 }, 2, 7);

  rooms[3].items = {};
  rooms[1].items = { sword };
  check_solution(rooms, { 0 }, 2, 5);

  rooms[1].monster = m;
  check_solution(rooms, { 0 }, 2, 0); // Cannot pickup while stealthing
}

void example_tests() {
  const Item sword = {
    .name = "Sword", .type = Item::Weapon,
    .off = 10, .def = -1,
  };

  const Item berserker_sword = {
    .name = "Berserker's Sword", .type = Item::Weapon,
    .hp = -1'000, .off = 10'000, .def = 0,
    .stacking_off = 1'000, .stacking_def = -500,
    .first_attack = true
  };

  const Item heavy_armor = {
    .name = "Heavy Armor", .type = Item::Armor,
    .hp = 5'000, .off = -10, .def = 300,
  };

  const Item debugging_duck = {
    .name = "Debugging Duck", .type = Item::RubberDuck,
    .stacking_off = 1,
    .stealth = true
  };

  std::vector<Room> rooms(14);
  enum : RoomId {
    no_monster = 10,
    weak,
    strong,
    durable
  };

  rooms[no_monster] = { {}, {}, { heavy_armor } };
  rooms[weak] = { {}, Monster{ .hp = 1000, .off = 10 }, { debugging_duck, sword } };
  rooms[strong] = { {}, Monster{ .hp = 10, .off = 10'000, .def = 1'000'000 },
    { berserker_sword } };
  rooms[durable] =  { {}, Monster{ .hp = 100'000, .off = 10, .stacking_def = 1 },
    { berserker_sword } };

  auto link = [&](RoomId a, RoomId b) {
    rooms[a].neighbors.push_back(b);
    rooms[b].neighbors.push_back(a);
  };

  link(0, no_monster);
  link(0, weak);
  link(weak, 7);
  link(0, strong);
  link(strong, 8);
  link(0, 1);
  link(1, 2);
  link(2, durable);
  link(durable, 6);

  check_solution(rooms, { 0 }, 0, 1); // Treasure at entrance
  check_solution(rooms, { 9 }, 0, 0); // No path to treasure
  check_solution(rooms, { 8 }, 0, 0); // Blocked by monster
  check_solution(rooms, { durable }, durable, 0); // Killed on spot
  check_solution(rooms, { 7 }, 0, 5); // Kills weak monster
  check_solution(rooms, { 6, 7 }, 2, 7); // Sneaks around durable
  check_solution(rooms, { 6, 7 }, durable, 9); // Kills durable
}

void example_tests2() {
  const Item duck_of_power = {
    .name = "Duck of Power", .type = Item::RubberDuck,
    .hp = 10'000'000, .off = 10'000'000, .def = 10'000'000,
  };

  const Item dull_sword = {
    .name = "Dull Sword", .type = Item::Weapon,
    .off = -10, .def = -5,
  };

  const Item sword = {
    .name = "Sword", .type = Item::Weapon,
    .off = 5, .def = -1,
  };

  const Item leather_pants = {
    .name = "Leather pants", .type = Item::Armor,
    .off = -3, .def = 1,
    .first_attack = true
  };

  const Item defensive_duck = {
    .name = "Defensive Duck", .type = Item::RubberDuck,
    .off = -2, .def = 8,
  };

  const Item stealth_duck = {
    .name = "Stealth Duck", .type = Item::RubberDuck,
    .off = -100, .def = -100,
    .stealth = true,
  };

  const Item slow_sword = {
    .name = "Slow Sword", .type = Item::Weapon,
    .off = -10'000,
    .stacking_off = 1,
  };

  constexpr int CYCLE_LEN = 100;
  enum : RoomId {
    impossible = CYCLE_LEN,
    r1, r2, r3, r4, r4a, r4b, ROOM_COUNT
  };
  std::vector<Room> rooms(ROOM_COUNT);

  auto link = [&](RoomId a, RoomId b) {
    rooms[a].neighbors.push_back(b);
    rooms[b].neighbors.push_back(a);
  };

  for (int i = 1; i < CYCLE_LEN; i++) link(i - 1, i);
  rooms[CYCLE_LEN-1].neighbors.push_back(0);

  rooms[impossible] = { {}, {{ .hp = 1'000'000, .off = 1'000'000 }}, { duck_of_power } };
  link(impossible, 0);

  rooms[r1] = { {}, {{ .hp = 9'999, .off = 3, .def = 2 }}, { defensive_duck, dull_sword } };
  link(r1, 1);

  rooms[r2] = { {}, {{ .hp = 100'000, .off = 10 }}, { sword, leather_pants } };
  link(r2, CYCLE_LEN - 3);

  rooms[r3] = { {}, {{ .hp = 100'000, .off = 10, .def = 1 }}, { stealth_duck, slow_sword } };
  link(r3, 2);

  rooms[r4] = { { r4a }, {{ .hp = 10'000, .off = 10'000 }}, {} };
  rooms[r4a] = { { r4b } };
  rooms[r4b] = { {}, {{ .hp = 10'000, .off = 1 }}, {} };
  link(r4, CYCLE_LEN - 4);
  link(r4b, CYCLE_LEN - 4);

  // r1 (loots duck) -> r2 (loots pants & sword) -> r3
  check_solution(rooms, { 0 }, r3, CYCLE_LEN + 11);
  // r1 (loots duck) -> r2 (loots pants & sword) -> r3 (loots stealth duck) -> r4a
  check_solution(rooms, { 0 }, r4a, 2*CYCLE_LEN + 11);
}

void example_tests3() {
  const Item sword = {
    .name = "Sword", .type = Item::Weapon,
    .off = 10,
  };

  const Item stacking_duck = {
    .name = "Stacking Duck", .type = Item::RubberDuck,
    .hp = -9'999, .stacking_off = 100,
  };

  const Item heavy_armor = {
    .name = "Heavy Armor", .type = Item::Armor,
    .off = -1'000, .def = 1'000,
  };

  enum : RoomId {
    start, treasure, short_path, long_path = short_path + 3,
    COUNT = long_path + 4
  };

  std::vector<Room> rooms(COUNT);

  auto link = [&](RoomId a, RoomId b) {
    rooms[a].neighbors.push_back(b);
    rooms[b].neighbors.push_back(a);
  };

  rooms[treasure].neighbors.push_back(start); // one-way back to start

  link(start, long_path + 0);
  link(long_path + 0, long_path + 1);
  link(long_path + 1, long_path + 2);
  link(long_path + 2, long_path + 3);
  link(long_path + 3, treasure);

  link(start, short_path + 0);
  link(short_path + 0, short_path + 1);
  link(short_path + 1, short_path + 2);
  link(short_path + 2, treasure);

  rooms[short_path + 0].items = { sword };
  rooms[short_path + 1].monster = Monster{ .hp=10'000, .off=5, .def=3 };
  rooms[short_path + 1].items = { stacking_duck, heavy_armor };
  rooms[short_path + 2].monster = Monster{ .hp=100'000, .off=5, .def=3 };

  check_solution(rooms, { start }, treasure, 6);
}

void example_tests4() {
  const Item sword = {
    .name = "Sword", .type = Item::Weapon,
    .off = 3, .def = -1
  };

  enum : RoomId {
    start, treasure, short_path, long_path = short_path + 3,
    COUNT = long_path + 4
  };

  std::vector<Room> rooms(COUNT);

  auto link = [&](RoomId a, RoomId b) {
    rooms[a].neighbors.push_back(b);
    rooms[b].neighbors.push_back(a);
  };

  rooms[treasure].neighbors.push_back(start); // one-way back to start

  link(start, long_path + 0);
  link(long_path + 0, long_path + 1);
  link(long_path + 1, long_path + 2);
  link(long_path + 2, long_path + 3);
  link(long_path + 3, treasure);

  link(start, short_path + 0);
  link(short_path + 0, short_path + 1);
  link(short_path + 1, short_path + 2);
  link(short_path + 2, treasure);

  Monster needs_sword = Monster{ .hp=10'000, .off=6, .def=3 };
  Monster no_sword = Monster{ .hp=100'000, .off=3 };
  rooms[short_path + 0].monster = needs_sword;
  rooms[short_path + 1].monster = no_sword;
  rooms[short_path + 0].monster = needs_sword;

  check_solution(rooms, { start }, treasure, 7);
}

void example_tests5() {
  const Item sword = {
    .name = "Sword", .type = Item::Weapon,
    .off = 5, .def = -1,
  };

  constexpr int LEN = 300;
  std::vector<Room> rooms(LEN);

  auto link = [&](RoomId a, RoomId b) {
    rooms[a].neighbors.push_back(b);
    rooms[b].neighbors.push_back(a);
  };

  for (int i = 1; i < LEN; i++) {
    rooms[i].items = { sword, sword, sword };
    link(i - 1, i);
  }

  rooms[LEN - 1].monster = Monster{ .hp = 1'000'000, .off = 1'000'000 };

  check_solution(rooms, { 0 }, LEN - 1, 0);
}

void example_tests6() {
  const Item defensive_duck = {
    .name = "Defensive Duck", .type = Item::RubberDuck,
    .off = -100, .def = 100,
  };

  constexpr int LEN = 31;
  std::vector<Room> rooms(LEN + LEN + 10);

  auto link = [&](RoomId a, RoomId b) {
    rooms[a].neighbors.push_back(b);
    rooms[b].neighbors.push_back(a);
  };

  rooms[0].items = { defensive_duck };

  assert(LEN % 2 == 1);
  for (int i = 1; i + 1 < LEN; i += 2) {
    link(i - 1, i);
    link(i, i + 1);

    rooms[i+1].items = { defensive_duck };

    rooms[i].monster = Monster{ .hp = 10'000'000, .off = 50, .def = -120 };
    rooms[i+1].monster = Monster{ .hp = 10'000, .off = 1, .def = 1 };
  }

  for (int i = 1; i < LEN + 10; i++)
    link(LEN + i - 1, LEN + i);

  link(0, LEN);
  link(LEN - 1, 2*LEN + 10 - 1);

  check_solution(rooms, { 0 }, LEN - 1, 2*LEN - 1);
}


int main() {
  combat_examples();
  stealth_examples();
  example_tests();
  example_tests2();
  example_tests3();
  example_tests4();
  example_tests5();
  example_tests6();
}

#endif


