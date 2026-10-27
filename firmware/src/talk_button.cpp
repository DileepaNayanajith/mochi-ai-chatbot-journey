#include "talk_button.h"
#include "board_pins.h"
namespace TalkButton {
namespace { bool prior = HIGH, stable = HIGH; unsigned long edge = 0; }
void begin() { pinMode(Pins::TALK_BUTTON, INPUT_PULLUP); }
Event poll() {
    const bool current = digitalRead(Pins::TALK_BUTTON);
    const unsigned long now = millis();
    if (current != prior) edge = now;
    prior = current;
    if (now - edge < 40 || current == stable) return Event::NONE;
    stable = current;
    return stable == LOW ? Event::PRESSED : Event::RELEASED;
}
}
