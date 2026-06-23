#include "state.h"
#include <raylib.h>
#include <iostream>

void render(const App_state& app)
{
  BeginDrawing();
  ClearBackground(app.config.bg_color);
  const auto &strokes{app.get_strokes()};
  for (const auto &stroke : strokes) {
    const auto &anchors{stroke.get_anchors()};
	DrawCircle(static_cast<int>(anchors.at(0).x), static_cast<int>(anchors.at(0).y),
			   app.config.brush_size/2, stroke.get_color());
    for (auto i{0uz}; i < anchors.size() - 1; i++) {
	  DrawLineEx(anchors[i], anchors[i + 1],
				 app.config.brush_size,
				 stroke.get_color());
	  DrawCircle(static_cast<int>(anchors[i+1].x), static_cast<int>(anchors[i+1].y),
				 app.config.brush_size/2, stroke.get_color());
    }
  }

  if (!strokes.empty() && !strokes.back().get_anchors().empty()) {
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
	  DrawLineEx(strokes.back().get_anchors().back(), GetMousePosition(),
				 app.config.brush_size, strokes.back().get_color());
	}
	DrawCircle(GetMouseX(), GetMouseY(),
			   app.config.brush_size/2, strokes.back().get_color());
  }
  
  EndDrawing();
}

void update(App_state& app)
{
  const Anchor& mouse_anchor{GetMousePosition()};
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    app.add_anchor(mouse_anchor);
  } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
	app.stop_stroke(mouse_anchor);
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
