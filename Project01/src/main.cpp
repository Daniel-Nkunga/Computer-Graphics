#include <iostream>
#include <tuple>
#include <vector>
using namespace std;

// Custom classes
#include <polygon.hpp>

int main()
{
    // srand(time(0));
    srand(88);

    std::vector<Polygon> polygons;

    for(int i = 0; i < 25; i++) // TODO: change to random number to generate
    {
        std::vector<std::tuple<float, float, float>> coordinates;
        for(int j = 0; j < (rand() % 4) + 3; j++)
        {
            float x = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
            float y = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
            float z = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
            coordinates.push_back({x, y, z});
        }
        // cout << x << " " << y << " " << z << endl;
        int color1 = rand() % 255;
        int color2 = rand() % 255;
        int color3 = rand() % 255;
        // cout << color1 << " " << color2 << " " << color3 << endl;
        polygons.push_back(Polygon(coordinates, {color1, color2, color3}));
        polygons[i].display();
    }
    return 0;
}