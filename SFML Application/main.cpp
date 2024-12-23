#include "Controler.h"

int main()
{
    Controler& game = Controler::get();
    game.run();
    return 0;
}