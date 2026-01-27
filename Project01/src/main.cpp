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
    cout << "Hello, World! :D" << endl;
    test.display();
    return 0;
}