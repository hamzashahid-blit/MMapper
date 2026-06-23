#include "state.h"
#include <cmath>
#include <iterator>
#include <optional>
#include <raylib.h>
#include <vector>
#include <iostream>

Stroke::Stroke() : m_id{m_index++} { std::cout << "Stroke()"; }
Stroke::Stroke(const Anchor &first_anchor)
    : m_id{m_index++}, m_anchors({first_anchor}),
      m_color(Color{static_cast<unsigned char>(std::rand() % 255),
                    static_cast<unsigned char>(std::rand() % 255),
                    static_cast<unsigned char>(std::rand() % 255), 255}) {
  std::cout << "Stroke(const Anchor&)";
}

size_t Stroke::get_id() const { return m_id; }
Color Stroke::get_color() const { return m_color; }
const std::vector<Anchor> Stroke::get_anchors() const { return m_anchors; }
void Stroke::add_anchor(const Anchor &anchor) {
  const Anchor &latest_anchor = m_anchors.back();
  auto dist{std::hypotf(anchor.x - latest_anchor.x, anchor.y - latest_anchor.y)};
  if (dist > App_state::config.brush_size/2)
	m_anchors.emplace_back(anchor);
}
void Stroke::add_anchor_force(const Anchor &anchor) {
  m_anchors.emplace_back(anchor);
}
size_t Stroke::m_index{0};

App_state::App_state() : m_current_stroke(nullptr) {}
const std::vector<Stroke> App_state::get_strokes() const { return m_strokes; }
const std::optional<Stroke> App_state::get_stroke(size_t index) const {
  if (index < m_strokes.size())
	return m_strokes.at(index);
  return {};
}
void App_state::create_stroke(const Anchor &first_anchor) {
  m_current_stroke = &m_strokes.emplace_back(Stroke{first_anchor});
}
void App_state::add_anchor(const Anchor &anchor) {
  if (!m_current_stroke) {
	create_stroke(anchor);
	return;
  }
  m_current_stroke->add_anchor(anchor);
}
void App_state::stop_stroke(const Anchor &anchor) {
  m_current_stroke->add_anchor_force(anchor);
  m_current_stroke = nullptr;
}
