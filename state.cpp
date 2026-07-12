#include "state.h"
#include <cmath>
#include <functional>
#include <optional>
#include <raylib.h>
#include <vector>
#include <iostream>

Stroke::Stroke() : m_id{m_index++} { std::cout << "Stroke()"; }
Stroke::Stroke(const Anchor &first_anchor)
    : m_id{m_index++},
      m_color(Color{static_cast<unsigned char>(std::rand() % 255),
                    static_cast<unsigned char>(std::rand() % 255),
                    static_cast<unsigned char>(std::rand() % 255), 255}),
      m_bounding_box{0, 0, 0, 0} {
  std::cout << "Stroke(const Anchor&)";
  add_anchor(first_anchor, true);
}

StrokeId Stroke::get_id() const { return m_id; }
Color Stroke::get_color() const { return m_color; }
const std::vector<Anchor> Stroke::get_anchors() const { return m_anchors; }
const StrokeBoundingBox& Stroke::get_bounding_box() const { return m_bounding_box; }
void Stroke::add_anchor(const Anchor &anchor, bool should_force = false) {
  if (should_force) {
    m_anchors.emplace_back(anchor);
    update_bounding_box(anchor);
  } else {
	const Anchor &latest_anchor = m_anchors.back();
	auto dist{std::hypotf(anchor.x - latest_anchor.x, anchor.y - latest_anchor.y)};
	if (dist > anchor.size/2) {
	  m_anchors.emplace_back(anchor);
	  update_bounding_box(anchor);
	}
  }
}
void Stroke::update_bounding_box(const Anchor &anchor) {
  bool is_colliding {
    ((anchor.x - anchor.size/2 >= m_bounding_box.x) &&
     (anchor.x + anchor.size/2 < (m_bounding_box.x + m_bounding_box.width)) &&
     (anchor.y - anchor.size/2 >= m_bounding_box.y) &&
     (anchor.y + anchor.size/2 < (m_bounding_box.y + m_bounding_box.height)))};
  if (m_bounding_box.width == 0) {
    m_bounding_box = {
      .x = anchor.x - anchor.size/2,
      .y = anchor.y - anchor.size/2,
      .width = anchor.size,
	  .height = anchor.size
	};
  } else if (!is_colliding) {
    if (anchor.x + anchor.size/2 > m_bounding_box.x + m_bounding_box.width) {
	  m_bounding_box.width += (anchor.x + anchor.size/2) - (m_bounding_box.x + m_bounding_box.width);
    }
    if (anchor.y + anchor.size/2 > m_bounding_box.y + m_bounding_box.height) {
	  m_bounding_box.height += (anchor.y + anchor.size/2) - (m_bounding_box.y + m_bounding_box.height);
    }
    if (anchor.x - anchor.size/2 < m_bounding_box.x) {
	  const float difference{m_bounding_box.x - (anchor.x - anchor.size/2)};
	  m_bounding_box.width += difference;
      m_bounding_box.x -= difference;
    }
    if (anchor.y - anchor.size/2 < m_bounding_box.y) {
	  const float difference{m_bounding_box.y - (anchor.y - anchor.size/2)};
      m_bounding_box.height += difference;
	  m_bounding_box.y -= difference;
	}
  }
}

size_t Stroke::m_index{0};

App_state::App_state()
    : m_is_drawing(false),
	  m_tool(Tool::Brush),
	  m_brush_size(40.0f) {}
const std::vector<Stroke>& App_state::get_strokes() const { return m_strokes; }
const Stroke& App_state::get_stroke(size_t index) const {
  return m_strokes.at(index);
}
std::optional<const std::reference_wrapper<Stroke>> App_state::get_current_stroke() {
  if (m_strokes.empty()) return std::nullopt;
  return std::ref(m_strokes.back());
}
const std::vector<StrokeId>& App_state::get_selections() const { return m_selections; }
Tool App_state::get_current_tool() const { return m_tool; }
float App_state::get_brush_size() const { return m_brush_size; }

void App_state::set_current_tool(Tool new_tool) {
  m_tool = new_tool;
}
void App_state::create_stroke(const Anchor &first_anchor) {
  m_strokes.emplace_back(Stroke{first_anchor});
  m_is_drawing = true;
}
void App_state::add_anchor(const Anchor &anchor) {
  if (!get_current_stroke() || !m_is_drawing) {
	create_stroke(anchor);
	return;
  }
  get_current_stroke().value().get().add_anchor(anchor);
  m_is_drawing = true;
}
void App_state::stop_stroke(const Anchor &anchor) {
  if (get_current_stroke()) {
	get_current_stroke().value().get().add_anchor(anchor, true);
	m_is_drawing = false;
  }
}

void App_state::set_selection(const Stroke &stroke) {
  m_selections.clear();
  m_selections.push_back(stroke.get_id());
}
void App_state::add_to_selection(const Stroke &stroke) {
  m_selections.push_back(stroke.get_id());
}
