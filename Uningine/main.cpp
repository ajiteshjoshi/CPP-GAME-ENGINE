#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <fstream>
#include "Vec2.h"
#include "EntityManager.h"
#include "Assets.h"
#include "GameEngine.h"
#include <iostream>

int main()
{
    GameEngine g("assets.txt");
    g.run();
    return 0;
}