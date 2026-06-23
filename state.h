#pragma once
#include <optional>
#include <raylib.h>
#include <cstdlib>
#include <vector>

struct Config {
  constexpr static Color bg_color{26, 26, 26, 255};
  constexpr static Color draw_color{245, 245, 245, 255};
  constexpr static size_t brush_size{40}; // diameter
};

// struct Anchor {
//   float x;
//   float y;
// };

using Anchor = Vector2;

class Stroke {
public:
  Stroke();
  explicit Stroke(const Anchor &first_anchor);
  Stroke(const Stroke &stroke) = default;
  Stroke(Stroke &&stroke) = default;
  Stroke &operator=(const Stroke &stroke) = default;
  Stroke &operator=(Stroke &&stroke) = default;
  
  size_t get_id() const;
  Color get_color() const;
  const std::vector<Anchor> get_anchors() const;
  // const Vector2* get_anchors_vec2() const;

  void add_anchor(const Anchor &anchor);
private:
  static size_t m_index;
  size_t m_id;
  std::vector<Anchor> m_anchors;
  Color m_color;
};

class App_state {
public:
  App_state();
  constexpr static Config config{};
  std::vector<Stroke> get_strokes() const;
  std::optional<Stroke> get_stroke(size_t index) const;
  void create_stroke(const Anchor &first_anchor);
  void add_anchor(const Anchor &anchor);
  void stop_stroke();
private:
  std::vector<Stroke> m_strokes;
  Stroke* m_current_stroke;
};
