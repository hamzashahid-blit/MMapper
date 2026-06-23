#pragma once
#include <optional>
#include <raylib.h>
#include <cstdlib>
#include <vector>

struct Config {
  constexpr static int window_width{800};
  constexpr static int window_height{600};
  constexpr static Color bg_color{26, 26, 26, 255};
  constexpr static Color draw_color{245, 245, 245, 255};
  constexpr static size_t brush_size{40}; // diameter
};

using Anchor = Vector2;
using StrokeId = size_t;

class Stroke {
public:
  Stroke();
  explicit Stroke(const Anchor &first_anchor);
  Stroke(const Stroke &stroke) = default;
  Stroke(Stroke &&stroke) = default;
  Stroke &operator=(const Stroke &stroke) = default;
  Stroke &operator=(Stroke &&stroke) = default;
  
  [[nodiscard]] StrokeId get_id() const;
  [[nodiscard]] Color get_color() const;
  [[nodiscard]] const std::vector<Anchor> get_anchors() const;

  void add_anchor(const Anchor &anchor);
  void add_anchor_force(const Anchor &anchor);
private:
  static size_t m_index;
  StrokeId m_id;
  std::vector<Anchor> m_anchors;
  Color m_color;
};

enum class Tool {
  Brush,
  Selection,
};

class App_state {
public:
  App_state();
  constexpr static Config config{};
  [[nodiscard]] const std::vector<Stroke> get_strokes() const;
  [[nodiscard]] std::optional<Stroke> get_stroke(size_t index) const;
  [[nodiscard]] std::optional<std::reference_wrapper<Stroke>> get_current_stroke();
  [[nodiscard]] Tool get_current_tool() const;

  void set_current_tool(Tool new_tool);
  void create_stroke(const Anchor &first_anchor);
  void add_anchor(const Anchor &anchor);
  void stop_stroke(const Anchor &anchor);

private:
  bool m_is_drawing;
  std::vector<Stroke> m_strokes;
  std::vector<StrokeId> m_selections;
  Tool m_tool;
};
