#include "views/hud.hpp"
#include "graphics.hpp"
#include "clock.hpp"

#include <sstream>
#include <deque>

using namespace std;

int influence = 0;

void Hud::render(Graphics& g, render_box const& pos) {
  stringstream out;
  out << "Influence: " << influence;
  
  g.drawString(pos.x, pos.y + pos.h - 1, out.str(), Graphics::DEFAULT);
  g.drawString(pos.x + pos.w - 25, pos.y + pos.h - 1, format_time(gametime), Graphics::DEFAULT);

  int sz = A11s::instance.msgs.size();

  for (int x=1;x < pos.h && x <= sz;++x)
    g.drawString(pos.x, pos.y + pos.h - x - 1, A11s::instance.msgs[sz - x], Graphics::DEFAULT);

}

void A11s::announce(const string& s) {
  msgs.push_back(s);
}

void announce(const string& s) { A11s::instance.announce(s); }

A11s A11s::instance;
Hud Hud::instance;
