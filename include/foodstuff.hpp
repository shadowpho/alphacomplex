#pragma once

#include "entity.hpp"

struct Foodstuff : AspectStatic<Aspect::Foodstuff, Foodstuff> {
  Foodstuff(int amt) : amount(amt) { }

  int amount;
};

Ent* make_bread(const Position& p);
