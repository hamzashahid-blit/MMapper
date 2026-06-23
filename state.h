#pragma once
#include <memory>
#include <optional>
#include <raylib.h>
#include <stdexcept>
#include <vector>
#include <format>

struct Config {
  constexpr static Color bg_color{26, 26, 26, 255};
  constexpr static Color draw_color{245, 245, 245, 255};
  constexpr static size_t brush_size{10}; // diameter
};

struct Anchor {
  size_t x;
  size_t y;
};

class Stroke {
public:
  Stroke() : m_id{m_index++} {}
  explicit Stroke(const Anchor &first_anchor)
      : m_id{m_index++}, m_anchors({first_anchor}),
  m_color(Color{ static_cast<unsigned char>(std::rand() % 255), static_cast<unsigned char>(std::rand() % 255), static_cast<unsigned char>(std::rand() % 255), 255}) {}
  size_t get_id() const { return m_id; }
  Color get_color() const { return m_color; }
  const auto get_anchors() const { return m_anchors; }

  void add_anchor(const Anchor &anchor) { m_anchors.emplace_back(anchor); }
private:
  inline static size_t m_index{0};
  size_t m_id;
  std::vector<Anchor> m_anchors;
  Color m_color;
};

class App_state {
public:
  App_state() : m_current_stroke(nullptr) {}
  Config config;
  auto get_strokes() const { return m_strokes; }
  std::optional<Stroke> get_stroke(size_t index) const {
	if (index < m_strokes.size())
	  return m_strokes.at(index);
	return {};
  }
  void create_stroke(const Anchor &first_anchor) {
    m_current_stroke = &m_strokes.emplace_back(Stroke{first_anchor});
  }
  void add_anchor(const Anchor &anchor) {
    if (!m_current_stroke) {
      create_stroke(anchor);
      return;
	}
    m_current_stroke->add_anchor(anchor);
  }
  void stop_stroke() {
	m_current_stroke = nullptr;
  }
private:
  std::vector<Stroke> m_strokes;
  Stroke* m_current_stroke;
};

// strokes -> anchors
