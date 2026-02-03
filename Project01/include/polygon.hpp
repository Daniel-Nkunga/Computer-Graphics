#include <iostream>
#include <tuple>
#include <vector>

struct Polygon
{
    private:
        std::vector<std::tuple<float, float, float>> coordinates; // Vector of 3D coordinates for the polygon
        std::tuple<float, float, float> color; //RGB value for the color of the polygon // TODO: Make these into floats
    public:
        Polygon(std::vector<std::tuple<float, float, float>>, std::tuple<int, int, int>);
        ~Polygon() {};
        void set(std::vector<std::tuple<float, float, float>>, std::tuple<int, int, int>); // Set both coordinates and color
        void set(std::vector<std::tuple<float, float, float>>); // Set coordinates
        void set(std::tuple<int, int, int>); // Set color
        void debug(); // Reworked dispaly function
        void display();
};