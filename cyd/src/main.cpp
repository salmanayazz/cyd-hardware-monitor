#include <Display.h>
#include <ClientHandler.h>

Display* display;
ClientHandler* clientHandler;

void setup() {
    display = new Display();
    clientHandler = new ClientHandler();
}

void loop() {
    while (true) {
        display->print(clientHandler->getHardwareData());
        delay(1000);
    }
}
