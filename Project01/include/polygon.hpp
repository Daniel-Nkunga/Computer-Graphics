#include <iostream>
#include <tuple>
#include <vector>

struct Polygon
{
    private:
        std::vector<std::tuple<float, float, float>> coordinates; // Vector of 3D coordinates for the polygon
        std::tuple<float, float, float> color; //RGB value for the color of the polygon
    public:
        Polygon(std::vector<std::tuple<float, float, float>>, std::tuple<float, float, float>);
        ~Polygon() {};
        void set(std::vector<std::tuple<float, float, float>>, std::tuple<float, float, float>); // Set both coordinates and color
        void set(std::vector<std::tuple<float, float, float>>); // Set coordinates; must stake in a vector
        void set(std::tuple<float, float, float>); // Set color
        void add(std::tuple<float, float, float>); // Add singular point to coordinate vector
        void debug(); // Reworked dispaly function
        void display();
};