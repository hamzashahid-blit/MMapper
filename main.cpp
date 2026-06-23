#include "state.h"
#include<raylib.h>

void render(const App_state& app)
{
  	BeginDrawing();
	ClearBackground(app.config.bg_color);
	for (const auto pixel : app.get_path()) {
          DrawRectangle(static_cast<int>(pixel.first),
                        static_cast<int>(pixel.second),
                        app.config.pixel_width,
                        app.config.pixel_height,
                        app.config.draw_color);
	}
	EndDrawing();
}

void update(App_state& app)
{
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
	app.draw(GetMouseX(), GetMouseY());
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
