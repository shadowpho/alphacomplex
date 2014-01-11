#include "views/hud.hpp"
#include "graphics.hpp"
#include "clock.hpp"

#include <sstream>
#include <deque>

using namespace std;

int influence = 0;

void Hud::render(Graphics& g) {
  stringstream out;
  out << "Influence: " << influence;
  
  g.drawString(1, g.getHeight()-2, out.str(), Graphics::DEFAULT);
  g.drawString(g.getWidth() - 25, g.getHeight()-2, format_time(gametime), Graphics::DEFAULT);

  auto sz = a11s.msgs.size();

  for (int x=1;x<4;++x)
    g.drawString(1, g.getHeight()-2-x, a11s.msgs[sz-x], Graphics::DEFAULT);

}

void A11s::announce(const string& s) {
  msgs.push_back(s);
}

void announce(const string& s) { a11s.announce(s); }

A11s a11s;
Hud hud;
