#pragma once
#include <raylib.h>
#include <vector>

struct Config {
  constexpr static Color bg_color{26, 26, 26, 255};
  constexpr static Color draw_color{245, 245, 245, 255};
  constexpr static size_t pixel_width{2};
  constexpr static size_t pixel_height{2};
};

class App_state {
public:
  Config config;
  auto get_path() const { return m_path; }
  void draw(size_t x, size_t y)
  { 
	m_path.emplace_back(x, y);
  }
private:
  std::vector<std::pair<size_t, size_t>> m_path;
};
