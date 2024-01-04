#ifndef menu_handler_h
#define menu_handler_h

#include <SharedData.h>

class MenuHandler {
private:
    SharedData* sharedData;
    char *menu[4];
    int currentlySelected = 0;
public:
    MenuHandler(SharedData* sharedData);
    ~MenuHandler();
    void handleMenuUpdate();
    void handleMenuEnter();
    void handleInputChange();
    void handleEnterPressed();
    void tick();
};

#endif