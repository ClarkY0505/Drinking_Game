#include "widgets/button.h"

bool Button::is_clicked(int x, int y) {
    return x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h;
}