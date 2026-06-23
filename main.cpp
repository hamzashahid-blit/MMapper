#include "state.h"
#include<raylib.h>

void render(const App_state& app)
{
  BeginDrawing();
  ClearBackground(app.config.bg_color);
  for (const auto& stroke : app.get_strokes()) {
	for (const auto& anchor : stroke.get_anchors()) {
          DrawCircle(static_cast<int>(anchor.x), static_cast<int>(anchor.y),
                     app.config.brush_size,
                     stroke.get_color());
	}
  }
  EndDrawing();
}

void update(App_state& app)
{ 
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    app.add_anchor({
        static_cast<size_t>(GetMouseX()),
        static_cast<size_t>(GetMouseY())});
  } else if (IsMouseButtonUp(MOUSE_BUTTON_LEFT)) {
	app.stop_stroke();
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
