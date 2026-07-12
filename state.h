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
  constexpr static Color selection_color{87, 148, 247, 255};
  constexpr static float selection_thick{5.0f};
  constexpr static float selection_padding{10.0f};
};

using StrokeId = size_t;
using StrokeBoundingBox = Rectangle;

struct Anchor {
  float x;
  float y;
  float size; // diameter

  [[nodiscard]] const Vector2 as_vec2() const { return {x, y}; }
};

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
  [[nodiscard]] const StrokeBoundingBox &get_bounding_box() const;

  void add_anchor(const Anchor &anchor, bool should_force);
  void update_bounding_box(const Anchor &anchor);
private:
  static size_t m_index;
  StrokeId m_id;
  std::vector<Anchor> m_anchors;
  Color m_color;
  StrokeBoundingBox m_bounding_box;
};

enum class Tool {
  Brush,
  Selection,
};

class App_state {
public:
  App_state();
  constexpr static Config config{};
  [[nodiscard]] const std::vector<Stroke>& get_strokes() const;
  [[nodiscard]] const Stroke& get_stroke(size_t index) const;
  [[nodiscard]] std::optional<const std::reference_wrapper<Stroke>> get_current_stroke();
  [[nodiscard]] const std::vector<StrokeId>& get_selections() const;
  [[nodiscard]] Tool get_current_tool() const;
  [[nodiscard]] float get_brush_size() const;

  void create_stroke(const Anchor &first_anchor);
  void add_anchor(const Anchor &anchor);
  void stop_stroke(const Anchor &anchor);
  void set_current_tool(Tool new_tool);
  void set_selection(const Stroke &stroke);
  void add_to_selection(const Stroke &stroke);

private:
  bool m_is_drawing;
  std::vector<Stroke> m_strokes;
  std::vector<StrokeId> m_selections;
  Tool m_tool;
  float m_brush_size; // diameter
};
