#include <iostream>
#include <tuple>
#include <vector>
using namespace std;

// Custom classes
#include <polygon.hpp>

int main()
{
    Polygon test(
        {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.5f, 1.0f, 0.0f}},
        {255, 0, 0}
    );
    test.display();
    test.set({{-0.8f, 0.0f, 0.0f}, {1.0f, 0.0f, -0.8f}, {0.5f, 1.0f, 0.0f}}, {0, 0, 255});
    test.display();
    test.set({{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}});
    test.display();
    test.set({255, 255, 255});
    test.display();
    return 0;
}