#include "state.h"
#include <raylib.h>
#include <iostream>

void render(const App_state& app)
{
  BeginDrawing();
  ClearBackground(app.config.bg_color);
  for (const auto &stroke : app.get_strokes()) {
    auto &anchors{stroke.get_anchors()};
	std::cout << "Anchor count: " << anchors.size() << '\n';
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

  DrawCircle(10, 10, app.config.brush_size/2, app.config.draw_color);
  DrawLineEx(Vector2{10, 10}, Vector2{100, 100}, app.config.brush_size,
             app.config.draw_color);
  DrawCircle(100, 100, app.config.brush_size/2, app.config.draw_color);
  DrawLineEx(Vector2{100, 100}, Vector2{230, 80}, app.config.brush_size,
             app.config.draw_color);
  DrawCircle(230, 80, app.config.brush_size/2, app.config.draw_color);
  EndDrawing();
}

void update(App_state& app)
{
  const Anchor& mouse_anchor{
        static_cast<float>(GetMouseX()),
        static_cast<float>(GetMouseY())};
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    app.add_anchor(mouse_anchor);
  } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
	app.add_anchor(mouse_anchor);
	app.stop_stroke(mouse_anchor);
  }
}

int main(int argc, const char *argv[])
{
  App_state app;

  InitWindow(600, 800, "MMapper");
  while (!WindowShouldClose()) {
	update(app);
	render(app);
  }
}
