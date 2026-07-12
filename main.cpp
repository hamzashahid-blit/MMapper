#include "state.h"
#include <cmath>
#include <raylib.h>
#include <algorithm>

void render(const App_state& app)
{
  BeginDrawing();
  ClearBackground(app.config.bg_color);
  const auto &strokes{app.get_strokes()};
  for (const auto &stroke : strokes) {
    const auto &anchors{stroke.get_anchors()};
	DrawCircle(static_cast<int>(anchors.at(0).x), static_cast<int>(anchors.at(0).y),
			   app.get_brush_size()/2, Color{255, 255, 255, 255});
    for (auto i{0uz}; i < anchors.size() - 1; i++) {
	  DrawLineEx(anchors[i].as_vec2(), anchors[i + 1].as_vec2(),
				 app.get_brush_size(),
				 stroke.get_color());
	  DrawCircle(static_cast<int>(anchors[i+1].x), static_cast<int>(anchors[i+1].y),
				 app.get_brush_size()/2, Color{255, 255, 255, 255});
    }
  }

  // Phantom stroke
  switch (app.get_current_tool()) {
  case Tool::Brush:
	if (!strokes.empty() && !strokes.back().get_anchors().empty()) {
	  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		DrawLineEx(strokes.back().get_anchors().back().as_vec2(), GetMousePosition(),
				   app.get_brush_size(), strokes.back().get_color());
	  }
	  DrawCircle(GetMouseX(), GetMouseY(),
				 app.get_brush_size()/2, strokes.back().get_color());
        }
        break;
  case Tool::Selection:
	const auto& selections{app.get_selections()};
    for (const auto &stroke : strokes) {
      if (std::ranges::contains(selections, stroke.get_id())) {
        DrawRectangleLinesEx(stroke.get_bounding_box(),
                             App_state::config.selection_thick,
                             App_state::config.selection_color);
	  }
	}
  }
  
  EndDrawing();
}

void update(App_state& app)
{
  const Vector2 &mouse_pos{GetMousePosition()};
  const Anchor &mouse_anchor{
	.x = mouse_pos.x,
	.y = mouse_pos.y,
	.size = app.get_brush_size()
  };
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    switch (app.get_current_tool()) {
	case Tool::Brush:
	  app.add_anchor(mouse_anchor);
	  break;
	case Tool::Selection:
	  const auto &strokes{app.get_strokes()};
	  for (auto it{strokes.cbegin()}; it != strokes.cend(); it++) {
		const auto &anchors{it->get_anchors()};
		for (const Anchor &anchor : anchors) {
		  if (std::hypot(mouse_anchor.x - anchor.x, mouse_anchor.y - anchor.y) < anchor.size/2) {
			app.set_selection(*it);
			break;
		  }
		}
	  }
	  break;
	}
  } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
	switch (app.get_current_tool()) {
	case Tool::Brush:
	  app.stop_stroke(mouse_anchor);
	  break;
	case Tool::Selection:
	  break;
	}
  }

  if (IsKeyPressed(KEY_ONE)) {
	app.set_current_tool(Tool::Selection);
  } else if (IsKeyPressed(KEY_TWO)) {
	app.set_current_tool(Tool::Brush);
  }
}

int main(int argc, const char *argv[])
{
  App_state app;

  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(app.config.window_width, app.config.window_height, "MMapper");
  ToggleBorderlessWindowed();
  while (!WindowShouldClose()) {
	update(app);
	render(app);
  }
}
