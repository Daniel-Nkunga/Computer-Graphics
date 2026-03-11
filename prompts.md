# Prompt 1

I'm trying to practice better CPP coding practice. For another class, a teacher was able to reference their local .hpp files in their main function using <> and I'm not sure why I can't do the same. I would typically using an explicit filepath and "", but I want to try to use <> if possible. Can you help. My program is the one trying to reference "polygon.hpp"

[daniel_n@akihayakawa Project01]$ ls
include  Makefile  src
[daniel_n@akihayakawa Project01]$ ls include/
polygon.hpp
[daniel_n@akihayakawa Project01]$ ls src/
main.cpp
[daniel_n@akihayakawa Project01]$ make
g++ -Wall -Wno-deprecated-declarations -o polygon src/main.cpp -lGL -lGLU -lglut
src/main.cpp:5:10: fatal error: polygon.hpp: No such file or directory
    5 | #include <polygon.hpp>
      |          ^~~~~~~~~~~~~
compilation terminated.
make: *** [Makefile:11: polygons] Error 1
[daniel_n@akihayakawa Project01]$ 


```cpp
#include <iostream>
using namespace std;

// Custom classes
#include <polygon.hpp>

int main()
{
    cout << "Hello, World! :D" << endl;
    return 0;
}
```

Working code from different class/teacher:

[daniel_n@akihayakawa HW01]$ ls
assets  bin  include  makefile  objs  src
[daniel_n@akihayakawa HW01]$ ls src/
graph.cpp  linked_list.cpp  main.cpp  queue.cpp
[daniel_n@akihayakawa HW01]$ ls include/
graph.hpp  linked_list.hpp  queue.hpp
[daniel_n@akihayakawa HW01]$


```cpp
#include <iostream>
#include <vector>
#include <linked_list.hpp>
#include <graph.hpp>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <limits>

. . .
```

The key difference is in how the **include path** is configured in your compilation command!

When you use angle brackets `<>`, the compiler only searches in **system include directories** and directories you've explicitly told it to search with the `-I` flag.

## The Problem

Your current `Makefile` compilation command:
```bash
g++ -Wall -Wno-deprecated-declarations -o polygon src/main.cpp -lGL -lGLU -lglut
```

This doesn't tell the compiler where to find your `include/` directory, so it can't find `polygon.hpp`.

## The Solution

Add `-I include` to your compilation command to tell the compiler to search in your `include/` directory:

```bash
g++ -Wall -Wno-deprecated-declarations -I include -o polygon src/main.cpp -lGL -lGLU -lglut
```

The `-I` flag adds a directory to the compiler's include search path.

## Why Your Other Class Works

Your other teacher's `Makefile` almost certainly has something like `-I include` in their compilation command. That's why `#include <linked_list.hpp>` works there.

## Quick Reference

- **`#include <file.hpp>`** - Searches system directories + directories specified with `-I`
- **`#include "file.hpp"`** - Searches current directory first, then falls back to system directories

## Best Practice

Using `<>` for your own headers with `-I include` is actually a **good practice** because:
1. It clearly separates your header location from your source location
2. It's more consistent with how libraries are included
3. It makes your code more portable (the include path is in the build system, not hardcoded in source files)

Just make sure your `Makefile` includes the `-I include` flag!

# Prompt 2

Can you help me test out my polygon class? Don't complete any functions if possible


```cpp
#include <iostream>
#include <tuple>
#include <vector>

struct polygon
{
    private:
        std::vector<std::tuple<float, float, float>> coordinates; // Vector of 3D coordinates for the polygon
        std::tuple<int, int, int> color; //RGB value for the color of the polygon
    public:
        polygon(std::vector<std::tuple<float, float, float>>, std::tuple<int, int, int>);
        ~polygon();
        void set(std::vector<std::tuple<float, float, float>>, std::tuple<int, int, int>);
        void display();
};
```


```cpp
#include <iostream>
#include <tuple>
#include <vector>
using namespace std;

// Custom classes
#include <polygon.hpp>

int main()
{
    polygon test(make_tuple(0, 1, 2), make_tuple(3, 4, 5));
    cout << "Hello, World! :D" << endl;
    // cout << test.coordinates;
    return 0;
}
```

I can help you test your polygon class! I notice there's a mismatch between your constructor signature and how you're calling it in `main()`.

Your constructor expects:
- `std::vector<std::tuple<float, float, float>>` (a vector of coordinates)
- `std::tuple<int, int, int>` (RGB color)

But you're calling it with:
- `make_tuple(0, 1, 2)` (single tuple)
- `make_tuple(3, 4, 5)` (single tuple)

Here's a test file that should work with your current class definition:

```cpp
#include <iostream>
#include <tuple>
#include <vector>
using namespace std;

// Custom classes
#include <polygon.hpp>

int main()
{
    // Create a vector of 3D coordinates (for a triangle, for example)
    vector<tuple<float, float, float>> coords = {
        make_tuple(0.0f, 0.0f, 0.0f),
        make_tuple(1.0f, 0.0f, 0.0f),
        make_tuple(0.5f, 1.0f, 0.0f)
    };

    // Create RGB color tuple
    tuple<int, int, int> color = make_tuple(255, 0, 0); // Red

    // Create polygon
    polygon test(coords, color);

    cout << "Polygon created successfully!" << endl;

    // Test the display function
    test.display();

    // Test the set function with new values
    vector<tuple<float, float, float>> new_coords = {
        make_tuple(0.0f, 0.0f, 1.0f),
        make_tuple(1.0f, 1.0f, 1.0f),
        make_tuple(1.0f, 0.0f, 1.0f),
        make_tuple(0.0f, 1.0f, 1.0f)
    };
    tuple<int, int, int> new_color = make_tuple(0, 255, 0); // Green

    test.set(new_coords, new_color);
    cout << "\nAfter set() call:" << endl;
    test.display();

    return 0;
}
```

This test creates a polygon with proper vector of coordinates and tests both the constructor and the `set()` method. The `display()` function should output the coordinates and color (once you implement it).

Is there any way this can be condensed/more elegant? Its okay if not

Yes! You can make it more elegant by constructing the vectors and tuples inline:

```cpp
#include <iostream>
#include <tuple>
#include <vector>
using namespace std;

// Custom classes
#include <polygon.hpp>

int main()
{
    // Create polygon directly with inline initialization
    polygon test(
        {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.5f, 1.0f, 0.0f}},
        {255, 0, 0}
    );

    cout << "Polygon created successfully!" << endl;
    test.display();

    // Test set() with inline initialization
    test.set(
        {{0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
        {0, 255, 0}
    );

    cout << "\nAfter set() call:" << endl;
    test.display();

    return 0;
}
```

This uses brace initialization which is much cleaner! The compiler automatically converts the nested braces into the appropriate `vector<tuple<...>>` and `tuple<...>` types.

# Prompt 3

Can you help me complete my constructor/destructor functions? 


```cpp
#include <iostream>
#include <tuple>
#include <vector>

struct Polygon
{
    private:
        std::vector<std::tuple<float, float, float>> coordinates; // Vector of 3D coordinates for the polygon
        std::tuple<int, int, int> color; //RGB value for the color of the polygon
    public:
        Polygon(std::vector<std::tuple<float, float, float>>, std::tuple<int, int, int>);
        ~Polygon();
        void set(std::vector<std::tuple<float, float, float>>, std::tuple<int, int, int>);
        void display();
};

#include <polygon.hpp>
#include <iostream>
#include <tuple>
#include <vector>

Polygon::Polygon(std::vector<std::tuple<float, float, float>> _coordinates, std::tuple<int, int, int> _color)
{
    this->coordinates = _coordinates;
    this->color = _color;
    cout << "Polygon made" << endl;
}

Polygon::~Polygon()
{
    
}
```

I can help you complete your constructor and destructor! Here's the completed code with explanations:

```cpp
#include "polygon.hpp"
#include <iostream>
#include <tuple>
#include <vector>

Polygon::Polygon(std::vector<std::tuple<float, float, float>> _coordinates, std::tuple<int, int, int> _color)
{
    this->coordinates = _coordinates;
    this->color = _color;
    std::cout << "Polygon made" << std::endl;
}

Polygon::~Polygon()
{
    std::cout << "Polygon destroyed" << std::endl;
}
```

**Key points:**

1. **Constructor**: Your constructor is already correct! It initializes the member variables with the passed parameters.

2. **Destructor**: Since you're using `std::vector` and `std::tuple`, which manage their own memory automatically, your destructor doesn't need to do any manual cleanup. The default destructor would work fine, but I added a message to show when the object is destroyed (matching your constructor's output).

3. **Fixed**: Changed `cout` to `std::cout` and `endl` to `std::endl` for consistency (or add `using namespace std;` if you prefer).

**Optional improvements:**

If you want to make your code more efficient, consider using an initializer list and pass-by-reference:

```cpp
Polygon::Polygon(const std::vector<std::tuple<float, float, float>>& _coordinates, 
                 const std::tuple<int, int, int>& _color)
    : coordinates(_coordinates), color(_color)
{
    std::cout << "Polygon made" << std::endl;
}
```

This avoids unnecessary copies of the vectors and tuples. You could also use move semantics if you don't need the original data after construction.

Can you help me define the destructor inline then?

Sure! To define the destructor inline, you can do it directly in the header file. Here's how:

```cpp
#include <iostream>
#include <tuple>
#include <vector>

struct Polygon
{
    private:
        std::vector<std::tuple<float, float, float>> coordinates; // Vector of 3D coordinates for the polygon
        std::tuple<int, int, int> color; //RGB value for the color of the polygon
    public:
        Polygon(std::vector<std::tuple<float, float, float>>, std::tuple<int, int, int>);
        ~Polygon() { std::cout << "Polygon destroyed" << std::endl; }
        void set(std::vector<std::tuple<float, float, float>>, std::tuple<int, int, int>);
        void display();
};
```

Now you can remove the destructor definition from your `.cpp` file entirely. The destructor is defined inline in the class definition itself.

Alternatively, if you want an even simpler destructor that doesn't print anything (since cleanup is automatic):

```cpp
~Polygon() = default;
```

or just

```cpp
~Polygon() {}
```

Can you explain this error?

[daniel_n@akihayakawa Project01]$ make
g++ -Wall -Wno-deprecated-declarations -o polygon -I include src/main.cpp -lGL -lGLU -lglut
/usr/bin/ld: /tmp/ccBZ5NON.o: in function `main':
main.cpp:(.text+0xad): undefined reference to `Polygon::Polygon(std::vector<std::tuple<float, float, float>, std::allocator<std::tuple<float, float, float> > >, std::tuple<int, int, int>)'
collect2: error: ld returned 1 exit status
make: *** [Makefile:11: polygons] Error 1
[daniel_n@akihayakawa Project01]$ 


```cpp
#include "polygon.hpp"
#include <iostream>
#include <tuple>
#include <vector>

Polygon::Polygon(std::vector<std::tuple<float, float, float>> coordinates, std::tuple<int, int, int> color)
{
    this->coordinates = coordinates;
    this->color = color;
}

// Setting functions
void Polygon::set(std::vector<std::tuple<float, float, float>> coordinates, std::tuple<int, int, int> color)
{
    this->coordinates = coordinates;
    this->color = color;
}

void Polygon::set(std::vector<std::tuple<float, float, float>> coordinates) // Set coordinates
{
    this->coordinates = coordinates;
}

void Polygon::set(std::tuple<int, int, int> color) // Set color
{
    this->color = color;
}
```

The error is a **linker error** - it means the linker can't find the implementation of your `Polygon` constructor. Looking at your `make` command, I can see the problem:

```bash
g++ -Wall -Wno-deprecated-declarations -o polygon -I include src/main.cpp -lGL -lGLU -lglut
```

**The issue**: You're only compiling `src/main.cpp`, but **not** the file containing your `Polygon` implementation (which appears to be `polygon.cpp` or `src/polygon.cpp`).

**The fix**: You need to include the polygon implementation file in your compilation. Your `make` command should be:

```bash
g++ -Wall -Wno-deprecated-declarations -o polygon -I include src/main.cpp src/polygon.cpp -lGL -lGLU -lglut
```

Or update your `Makefile` to include all source files. A typical Makefile pattern would be:

```makefile
CXX = g++
CXXFLAGS = -Wall -Wno-deprecated-declarations -I include
LDFLAGS = -lGL -lGLU -lglut
SOURCES = src/main.cpp src/polygon.cpp
TARGET = polygon

polygons: $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS)
```

The linker is looking for the compiled code of `Polygon::Polygon(...)` but can't find it because you never compiled the `.cpp` file that contains it!

Can you help me with my functions?


```cpp
#include "polygon.hpp"
#include <iostream>
#include <tuple>
#include <vector>

Polygon::Polygon(std::vector<std::tuple<float, float, float>> coordinates, std::tuple<int, int, int> color)
{
    this->coordinates = coordinates;
    this->color = color;
}

// Setting functions
void Polygon::set(std::vector<std::tuple<float, float, float>> coordinates, std::tuple<int, int, int> color)
{
    this->coordinates = coordinates;
    this->color = color;
}

void Polygon::set(std::vector<std::tuple<float, float, float>> coordinates) // Set coordinates
{
    this->coordinates = coordinates;
}

void Polygon::set(std::tuple<int, int, int> color) // Set color
{
    this->color = color;
}

void Polygon::display()
{
    std::cout << "Test" << std::endl;
}

```

The error message is telling you that `display()` returns `void`, but you're trying to use it with the `<<` operator on line 16 of main.cpp:

```cpp
cout << test.display() << endl;  // ❌ Wrong - display() returns void
```

The problem is that `display()` doesn't return anything - it just prints to the console. You can't chain a `void` return value with `<<`.

**Fix option 1** - Just call `display()` without using `<<`:

```cpp
test.display();  // ✅ Correct - just call the function
```

**Fix option 2** - If you want `display()` to be chainable, make it return a reference to `std::ostream`:

```cpp
// In polygon.hpp
std::ostream& display(std::ostream& os = std::cout);

// In polygon.cpp
std::ostream& Polygon::display(std::ostream& os)
{
    os << "Test" << std::endl;
    return os;
}

// Then in main.cpp you can do:
test.display() << endl;  // Now this works
```

**Fix option 3** - If you want to print information about the polygon, you could also implement the `<<` operator overload:

```cpp
// In polygon.hpp (or as a friend inside the class)
std::ostream& operator<<(std::ostream& os, const Polygon& p);

// In polygon.cpp
std::ostream& operator<<(std::ostream& os, const Polygon& p)
{
    os << "Polygon with " << p.coordinates.size() << " vertices";
    return os;
}

// Then in main.cpp:
cout << test << endl;  // Now this works naturally
```

The simplest fix is **option 1** - just call `test.display();` without trying to use it with `<<`.

Can you help now with the modified display?


```cpp
void Polygon::display()
{
    std::cout << this->coordinates << " " << this->color;
}
```

The issue is that `std::vector` and `std::tuple` don't have built-in `<<` operators, so you can't print them directly. You need to iterate through them and print each element.

Here's a corrected version:

```cpp
void Polygon::display()
{
    // Display coordinates
    std::cout << "Coordinates: ";
    for (size_t i = 0; i < this->coordinates.size(); i++)
    {
        auto& coord = this->coordinates[i];
        std::cout << "(" << std::get<0>(coord) << ", " 
                  << std::get<1>(coord) << ", " 
                  << std::get<2>(coord) << ")";
        
        if (i < this->coordinates.size() - 1)
            std::cout << ", ";
    }
    
    // Display color
    std::cout << " | Color: RGB(" 
              << std::get<0>(this->color) << ", " 
              << std::get<1>(this->color) << ", " 
              << std::get<2>(this->color) << ")" << std::endl;
}
```

**Alternative with range-based for loop** (cleaner):

```cpp
void Polygon::display()
{
    std::cout << "Coordinates: ";
    bool first = true;
    for (const auto& coord : this->coordinates)
    {
        if (!first) std::cout << ", ";
        std::cout << "(" << std::get<0>(coord) << ", " 
                  << std::get<1>(coord) << ", " 
                  << std::get<2>(coord) << ")";
        first = false;
    }
    
    std::cout << " | Color: RGB(" 
              << std::get<0>(this->color) << ", " 
              << std::get<1>(this->color) << ", " 
              << std::get<2>(this->color) << ")" << std::endl;
}
```

This will print something like:
```
Coordinates: (1.0, 2.0, 3.0), (4.0, 5.0, 6.0) | Color: RGB(255, 0, 128)
```

# Prompt 4

In CPP, can you make a random number generator that generates floats between [-1,1]

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
    Polygon test(
        {{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
        {255, 0, 0}
    );
    test.display();

    for(int i = 0; i < 25; i++) // TODO: change to random number to generate
    {
        // Generate three random number generators here for coordinates?
    }
    return 0;
}

Here's a random number generator for floats between [-1, 1] in C++:

```cpp
#include <iostream>
#include <tuple>
#include <vector>
using namespace std;

// Custom classes
#include <polygon.hpp>

// Random float generator between [-1, 1]
float randomFloat() {
    return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
}

int main()
{
    // srand(time(0));
    srand(88);
    Polygon test(
        {{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
        {255, 0, 0}
    );
    test.display();

    for(int i = 0; i < 25; i++)
    {
        // Generate three random coordinates
        float x = randomFloat();
        float y = randomFloat();
        float z = randomFloat();
        
        cout << "Point " << i << ": (" << x << ", " << y << ", " << z << ")" << endl;
    }
    return 0;
}
```

**How it works:**
1. `rand()` generates a random integer between 0 and `RAND_MAX`
2. Divide by `RAND_MAX` to get a float in range [0, 1]
3. Multiply by 2 to get range [0, 2]
4. Subtract 1 to get range [-1, 1]

**Modern C++ alternative (C++11+):**
If you want better quality random numbers, use `<random>`:

```cpp
#include <random>

std::random_device rd;
std::mt19937 gen(88); // or gen(rd()) for truly random seed
std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

// Then use:
float x = dist(gen);
```

# Prompt 5

Can you help me with testing this display function

[daniel_n@akihayakawa Project01]$ make
g++ -Wall -Wno-deprecated-declarations -o polygon -I include src/* -lGL -lGLU -lglut
src/polygon.cpp: In member function ‘void Polygon::display()’:
src/polygon.cpp:57:22: warning: comparison of integer expressions of different signedness: ‘int’ and ‘std::vector<std::tuple<float, float, float> >::size_type’ {aka ‘long unsigned int’} [-Wsign-compare]
   57 |     for(int i = 0; i < this->coordinates.size(); i++)
      |                    ~~^~~~~~~~~~~~~~~~~~~~~~~~~~
[daniel_n@akihayakawa Project01]$ 


```cpp
void Polygon::display()
{
    glClear(GL_COLOR_BUFFER_BIT);
3
    glBegin(GL_POLYGON);
    for(int i = 0; i < this->coordinates.size(); i++)
    {
        std::cout << i;
    }
    // glVertex3f();
    glEnd();
    glFlush();
    return;
}
```

I can help you fix both the warning and complete the display function. Here are the issues:

## Problems

1. **Sign comparison warning**: You're comparing a signed `int` with an unsigned `size_t` (from `.size()`)
2. **Incomplete implementation**: You're not actually drawing the vertices—just printing indices

## Solution

```cpp
void Polygon::display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    glBegin(GL_POLYGON);
    for(size_t i = 0; i < this->coordinates.size(); i++)
    {
        // Extract x, y, z from the tuple
        float x = std::get<0>(this->coordinates[i]);
        float y = std::get<1>(this->coordinates[i]);
        float z = std::get<2>(this->coordinates[i]);
        
        glVertex3f(x, y, z);
    }
    glEnd();
    glFlush();
}
```

## Key changes:

1. **`size_t i`** instead of `int i` - fixes the warning by matching types
2. **Extract tuple values** using `std::get<>()` to get the x, y, z coordinates
3. **Call `glVertex3f()`** to actually draw the vertex

Alternatively, you could use a range-based for loop (cleaner C++11+ style):

```cpp
void Polygon::display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    glBegin(GL_POLYGON);
    for(const auto& coord : this->coordinates)
    {
        glVertex3f(std::get<0>(coord), std::get<1>(coord), std::get<2>(coord));
    }
    glEnd();
    glFlush();
}
```

This should eliminate the warning and actually render your polygon!

# Prompt 6

I'm trying to change the way I'm generating my random coordinates so that they'll always be near each other. Can you fix those lines to allow me to reference the previous coordinates?


```cpp
for(int i = 0; i < 3; i++) // TODO: change to random number to generate
    {
        std::vector<std::tuple<float, float, float>> coordinates;
        for(int j = 0; j < (rand() % 4) + 3; j++)
        {
            float x, y, z;
            if (coordinates.empty())
            {
                x = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
                y = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
                z = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
            } else {
                x = coordinates[i-1].std::get<1> + .1; // Help here!
                y = coordinates[i-1].std::get<2> + .1;
                z = coordinates[i-1].std::get<3> + .1;
            }
            coordinates.push_back({x, y, z});
        }
        int color1 = rand() % 255;
        int color2 = rand() % 255;
        int color3 = rand() % 255;
        polygons.push_back(Polygon(coordinates, {color1, color2, color3}));
        polygons[i].debug();
    }
```

I can help you fix those lines! There are a couple of issues:

1. You should use `j-1` instead of `i-1` since you're iterating with `j` through the coordinates
2. The syntax for accessing tuple elements is `std::get<0>`, `std::get<1>`, `std::get<2>` (indexed from 0)
3. You can also add some randomness to the offset so they're "near" but not in a straight line

Here's the corrected version:

```cpp
for(int i = 0; i < 3; i++)
{
    std::vector<std::tuple<float, float, float>> coordinates;
    for(int j = 0; j < (rand() % 4) + 3; j++)
    {
        float x, y, z;
        if (coordinates.empty())
        {
            x = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
            y = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
            z = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
        } else {
            x = std::get<0>(coordinates[j-1]) + ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 0.2f - 0.1f);
            y = std::get<1>(coordinates[j-1]) + ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 0.2f - 0.1f);
            z = std::get<2>(coordinates[j-1]) + ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 0.2f - 0.1f);
        }
        coordinates.push_back({x, y, z});
    }
    int color1 = rand() % 255;
    int color2 = rand() % 255;
    int color3 = rand() % 255;
    polygons.push_back(Polygon(coordinates, {color1, color2, color3}));
    polygons[i].debug();
}
```

The key fixes:
- Changed `coordinates[i-1]` to `coordinates[j-1]`
- Fixed tuple access: `std::get<0>`, `std::get<1>`, `std::get<2>` (tuples are 0-indexed)
- Added random offsets in range [-0.1, 0.1] instead of a fixed +0.1 to make it more natural

If you want the coordinates even closer together, you can adjust the 0.2f multiplier to a smaller value!

# Prompt 7

I'm trying to change the way I'm generating my random coordinates so that they'll always be near each other. Can you fix those lines to allow me to reference the previous coordinates?


```cpp
for(int i = 0; i < 3; i++) // TODO: change to random number to generate
    {
        std::vector<std::tuple<float, float, float>> coordinates;
        for(int j = 0; j < (rand() % 4) + 3; j++)
        {
            float x, y, z;
            if (coordinates.empty())
            {
                x = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
                y = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
                z = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
            } else {
                x = coordinates[i-1].std::get<1> + .1; // Help here!
                y = coordinates[i-1].std::get<2> + .1;
                z = coordinates[i-1].std::get<3> + .1;
            }
            coordinates.push_back({x, y, z});
        }
        int color1 = rand() % 255;
        int color2 = rand() % 255;
        int color3 = rand() % 255;
        polygons.push_back(Polygon(coordinates, {color1, color2, color3}));
        polygons[i].debug();
    }
```

I can help you fix those lines! There are a couple of issues:

1. You should use `j-1` instead of `i-1` since you're iterating with `j` through the coordinates
2. The syntax for accessing tuple elements is `std::get<0>`, `std::get<1>`, `std::get<2>` (indexed from 0)
3. You can also add some randomness to the offset so they're "near" but not in a straight line

Here's the corrected version:

```cpp
for(int i = 0; i < 3; i++)
{
    std::vector<std::tuple<float, float, float>> coordinates;
    for(int j = 0; j < (rand() % 4) + 3; j++)
    {
        float x, y, z;
        if (coordinates.empty())
        {
            x = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
            y = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
            z = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
        } else {
            x = std::get<0>(coordinates[j-1]) + ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 0.2f - 0.1f);
            y = std::get<1>(coordinates[j-1]) + ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 0.2f - 0.1f);
            z = std::get<2>(coordinates[j-1]) + ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 0.2f - 0.1f);
        }
        coordinates.push_back({x, y, z});
    }
    int color1 = rand() % 255;
    int color2 = rand() % 255;
    int color3 = rand() % 255;
    polygons.push_back(Polygon(coordinates, {color1, color2, color3}));
    polygons[i].debug();
}
```

The key fixes:
- Changed `coordinates[i-1]` to `coordinates[j-1]`
- Fixed tuple access: `std::get<0>`, `std::get<1>`, `std::get<2>` (tuples are 0-indexed)
- Added random offsets in range [-0.1, 0.1] instead of a fixed +0.1 to make it more natural

If you want the coordinates even closer together, you can adjust the 0.2f multiplier to a smaller value!

# Prompt 8

Can you help me rotate my polygons around 
Your final task is to modify the “display” callback function so all the polygons arerotated by an angle “theta” around a pre-defined axis. This will require you toadd calls to glMatrixMode, glLoadIdentity and glRotatef somewhere in theprogram. If you increase the angle “theta” by a small amount each time the“display” callback is executed your random polygons will spin around therotation axis as you manually adjust the size of the display window.


```cpp
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <iostream>
#include <tuple>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
using namespace std;

// Custom classes
#include <polygon.hpp>

std::vector<Polygon> polygons;

void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(size_t i = 0; i < polygons.size(); i++)
    {
        polygons[i].display();
    }

    glFlush();
}

int main(int argc, char *argv[])
{
    // srand(time(0));
    srand(88);

    for(int i = 0; i < 50; i++)
    {
        std::vector<std::tuple<float, float, float>> coordinates;
        for(int j = 0; j < (rand() % 4) + 3; j++)
        {
            float x, y, z;
            if (coordinates.empty())
            {
                x = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
                y = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
                z = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
            } else {
                x = std::get<0>(coordinates[j-1]) + ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 0.2f - 0.1f);
                y = std::get<1>(coordinates[j-1]) + ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 0.2f - 0.1f);
                z = std::get<2>(coordinates[j-1]) + ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 0.2f - 0.1f);
            }
            coordinates.push_back({x, y, z});
        }
        int color1 = rand() % 255;
        int color2 = rand() % 255;
        int color3 = rand() % 255;
        polygons.push_back(Polygon(coordinates, {color1, color2, color3}));
        polygons[i].debug();
    }

    // Display function
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(250, 250);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE |GLUT_DEPTH);
    glutCreateWindow("Polygons - Nkunga");
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}

//TODO:
// GL_ENABLE DEPTH BUFFER, DEPTH BUFFER CLEAR

#include <GL/gl.h>
#include <polygon.hpp>
#include <iostream>
#include <tuple>
#include <vector>

Polygon::Polygon(std::vector<std::tuple<float, float, float>> coordinates, std::tuple<int, int, int> color)
{
    this->coordinates = coordinates;
    this->color = color;
}

// Setting functions
void Polygon::set(std::vector<std::tuple<float, float, float>> coordinates, std::tuple<int, int, int> color)
{
    this->coordinates = coordinates;
    this->color = color;
}

void Polygon::set(std::vector<std::tuple<float, float, float>> coordinates) // Set coordinates
{
    this->coordinates = coordinates;
}

void Polygon::set(std::tuple<int, int, int> color) // Set color
{
    this->color = color;
}

void Polygon::debug()
{
    // Display coordinates
    std::cout << "\033[1;4;33mCoordinates:\033[0m ";
    for (size_t i = 0; i < this->coordinates.size(); i++)
    {
        auto& coord = this->coordinates[i];
        std::cout << "(" << std::get<0>(coord) << ", "
                  << std::get<1>(coord) << ", "
                  << std::get<2>(coord) << ")";

        if (i < this->coordinates.size() - 1)
            std::cout << ",\033[0m ";
    }

    // Display color
    std::cout << " | \033[1;4;34mColor:\033[0m ("
              << std::get<0>(this->color) << ", "
              << std::get<1>(this->color) << ", "
              << std::get<2>(this->color) << ")\033[0m" << std::endl;
}

void Polygon::display()
{
    glColor3f(std::get<0>(this->color) / 255.0f,
              std::get<1>(this->color) / 255.0f,
              std::get<2>(this->color) / 255.0f);  // Set the polygon's color!

    glBegin(GL_POLYGON);
    for(size_t i = 0; i < this->coordinates.size(); i++)
    {
        float x = std::get<0>(this->coordinates[i]);
        float y = std::get<1>(this->coordinates[i]);
        float z = std::get<2>(this->coordinates[i]);

        glVertex3f(x, y, z);
    }
    glEnd();
    glRotatef(30.0, 1.0, 1.0, 1.0);
}
```

Looking at your code, I can see you already have a `glRotatef` call, but it's in the wrong place — it's inside `Polygon::display()` after `glEnd()`, which means each polygon gets an *additional* rotation applied, causing them to accumulate rotations unpredictably rather than all spinning together around a shared axis.

Here's what needs to happen:

1. Add a global `theta` variable that increments each frame.
2. In the `display()` callback, set up the modelview matrix with `glLoadIdentity()` and then apply `glRotatef` using `theta` *before* drawing any polygons. This way every polygon shares the same rotation.
3. Remove the stray `glRotatef` from `Polygon::display()`.

**In your main `.cpp` file**, add `theta` as a global and update `display()`**:**

```cpp
float theta = 0.0f; // global rotation angle

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(theta, 1.0f, 1.0f, 1.0f); // rotate all polygons around axis (1,1,1)

    for(size_t i = 0; i < polygons.size(); i++)
    {
        polygons[i].display();
    }

    theta += 0.5f; // increment angle each frame (adjust speed as you like)
    if(theta >= 360.0f) theta -= 360.0f; // keep it in [0, 360)

    glFlush();
}
```

**In `polygon.cpp`**, remove the `glRotatef` line at the end of `Polygon::display()`**:**

```cpp
void Polygon::display()
{
    glColor3f(std::get<0>(this->color) / 255.0f,
              std::get<1>(this->color) / 255.0f,
              std::get<2>(this->color) / 255.0f);

    glBegin(GL_POLYGON);
    for(size_t i = 0; i < this->coordinates.size(); i++)
    {
        float x = std::get<0>(this->coordinates[i]);
        float y = std::get<1>(this->coordinates[i]);
        float z = std::get<2>(this->coordinates[i]);

        glVertex3f(x, y, z);
    }
    glEnd();
    // REMOVED: glRotatef(30.0, 1.0, 1.0, 1.0);
}
```

**Why this works:** OpenGL's matrix stack is cumulative. By calling `glLoadIdentity()` at the start of each frame you reset the modelview matrix to the identity, then `glRotatef(theta, ...)` applies a single rotation that every subsequent `glVertex3f` call (across all polygons) is transformed by. The axis `(1, 1, 1)` gives a nice diagonal spin — you can change it to `(1, 0, 0)`, `(0, 1, 0)`, or `(0, 0, 1)` to rotate around X, Y, or Z instead.

One note: since you're using `GLUT_SINGLE` buffering, the animation will only update when the window is resized (as your task description mentions). If you want continuous spinning, you'd add a `glutTimerFunc` or `glutIdleFunc` to trigger redraws automatically.

The polygons still aren't rotating

Can you share your `polygon.hpp` header file? That way I can see the full picture. In the meantime, the most likely culprit is that the rotation isn't actually triggering a redraw.

With `GLUT_SINGLE` buffering and no idle or timer function registered, `display()` only gets called when something forces a redraw (like resizing the window). So `theta` never actually increments unless you manually resize.

Add this to your main `.cpp` file to force continuous redraws:

```cpp
void idle()
{
    glutPostRedisplay(); // forces display() to be called again
}
```

Then register it in `main()` right after `glutDisplayFunc`:

```cpp
glutDisplayFunc(display);
glutIdleFunc(idle);     // <-- add this line
```

Your `main()` should look like this now:

```cpp
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(250, 250);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
    glutCreateWindow("Polygons - Nkunga");
    glutDisplayFunc(display);
    glutIdleFunc(idle);  // <-- here
    init();
    glutMainLoop();
```

Also double-check that you actually removed the `glRotatef` from the end of `Polygon::display()` in `polygon.cpp` — if that's still there it will fight against the rotation in your `display()` callback and cause weird behavior.

If it's still not working after both of those changes, send over the `.hpp` file and I'll dig deeper.

```cpp
#include <iostream>
#include <tuple>
#include <vector>

struct Polygon
{
    private:
        std::vector<std::tuple<float, float, float>> coordinates; // Vector of 3D coordinates for the polygon
        std::tuple<int, int, int> color; //RGB value for the color of the polygon // TODO: Make these into floats
    public:
        Polygon(std::vector<std::tuple<float, float, float>>, std::tuple<int, int, int>);
        ~Polygon() {};
        void set(std::vector<std::tuple<float, float, float>>, std::tuple<int, int, int>); // Set both coordinates and color
        void set(std::vector<std::tuple<float, float, float>>); // Set coordinates
        void set(std::tuple<int, int, int>); // Set color
        void debug(); // Reworked dispaly function
        void display();
};
```

The header looks fine — nothing wrong there. Can you paste your current full `main.cpp` and `polygon.cpp` exactly as they are right now? I want to see the actual state of both files, because the issue is likely one of these:

1. The `glRotatef` inside `Polygon::display()` wasn't actually removed, so each polygon is still accumulating its own rotation and overriding the one you set up in the `display()` callback.

2. The `glutIdleFunc(idle)` or the `idle()` function itself wasn't added, so `display()` is never being called again after the first frame and `theta` never changes.

3. The `glMatrixMode(GL_MODELVIEW)` / `glLoadIdentity()` / `glRotatef(theta, ...)` block wasn't added to `display()`, or is in the wrong place.

Paste both files as they currently stand and I'll pinpoint exactly what's off.

```cpp
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <iostream>
#include <tuple>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
using namespace std;

// Custom classes
#include <polygon.hpp>

std::vector<Polygon> polygons;
float theta = 0.0f;

void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(theta, 1.0f, 1.0f, 1.0f); // rotate all polygons around axis (1,1,1)

    for(size_t i = 0; i < polygons.size(); i++)
    {
        polygons[i].display();
    }

    theta += 0.5f; // increment angle each frame (adjust speed as you like)
    if(theta >= 360.0f) theta -= 360.0f; // keep it in [0, 360)

    glFlush();
}

int main(int argc, char *argv[])
{
    // srand(time(0));
    srand(88);

    for(int i = 0; i < 50; i++)
    {
        std::vector<std::tuple<float, float, float>> coordinates;
        for(int j = 0; j < (rand() % 4) + 3; j++)
        {
            float x, y, z;
            if (coordinates.empty())
            {
                x = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
                y = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
                z = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
            } else {
                x = std::get<0>(coordinates[j-1]) + ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 0.2f - 0.1f);
                y = std::get<1>(coordinates[j-1]) + ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 0.2f - 0.1f);
                z = std::get<2>(coordinates[j-1]) + ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 0.2f - 0.1f);
            }
            coordinates.push_back({x, y, z});
        }
        int color1 = rand() % 255;
        int color2 = rand() % 255;
        int color3 = rand() % 255;
        polygons.push_back(Polygon(coordinates, {color1, color2, color3}));
        polygons[i].debug();
    }

    // Display function
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(250, 250);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE |GLUT_DEPTH);
    glutCreateWindow("Polygons - Nkunga");
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}

//TODO:
// GL_ENABLE DEPTH BUFFER, DEPTH BUFFER CLEAR

#include <GL/gl.h>
#include <polygon.hpp>
#include <iostream>
#include <tuple>
#include <vector>

Polygon::Polygon(std::vector<std::tuple<float, float, float>> coordinates, std::tuple<int, int, int> color)
{
    this->coordinates = coordinates;
    this->color = color;
}

// Setting functions
void Polygon::set(std::vector<std::tuple<float, float, float>> coordinates, std::tuple<int, int, int> color)
{
    this->coordinates = coordinates;
    this->color = color;
}

void Polygon::set(std::vector<std::tuple<float, float, float>> coordinates) // Set coordinates
{
    this->coordinates = coordinates;
}

void Polygon::set(std::tuple<int, int, int> color) // Set color
{
    this->color = color;
}

void Polygon::debug()
{
    // Display coordinates
    std::cout << "\033[1;4;33mCoordinates:\033[0m ";
    for (size_t i = 0; i < this->coordinates.size(); i++)
    {
        auto& coord = this->coordinates[i];
        std::cout << "(" << std::get<0>(coord) << ", "
                  << std::get<1>(coord) << ", "
                  << std::get<2>(coord) << ")";

        if (i < this->coordinates.size() - 1)
            std::cout << ",\033[0m ";
    }

    // Display color
    std::cout << " | \033[1;4;34mColor:\033[0m ("
              << std::get<0>(this->color) << ", "
              << std::get<1>(this->color) << ", "
              << std::get<2>(this->color) << ")\033[0m" << std::endl;
}

void Polygon::display()
{
    glColor3f(std::get<0>(this->color) / 255.0f,
              std::get<1>(this->color) / 255.0f,
              std::get<2>(this->color) / 255.0f);  // Set the polygon's color!

    glBegin(GL_POLYGON);
    for(size_t i = 0; i < this->coordinates.size(); i++)
    {
        float x = std::get<0>(this->coordinates[i]);
        float y = std::get<1>(this->coordinates[i]);
        float z = std::get<2>(this->coordinates[i]);

        glVertex3f(x, y, z);
    }
    glEnd();
    // glRotatef(30.0, 1.0, 1.0, 1.0);
}
```

Found it. You have two problems:

**Problem 1:** The `idle()` function and `glutIdleFunc` call are missing — you never actually added them. Without that, `display()` only runs once and `theta` never changes.

**Problem 2:** Your near/far clipping planes in `glOrtho` are too tight. You have `-1.0` to `1.0`, but your polygons have z-coordinates spread across that entire range. When you rotate them, vertices move outside that range and get clipped (invisible). Widen it out.

Here are the two changes to `main.cpp`:

Add the `idle` function before `display()`:

```cpp
void idle()
{
    glutPostRedisplay();
}
```

Register it in `main()`:

```cpp
    glutDisplayFunc(display);
    glutIdleFunc(idle);     // <-- add this
    init();
    glutMainLoop();
```

And fix the clipping range in `init()`:

```cpp
    glOrtho(-1.0, 1.0, -1.0, 1.0, -10.0, 10.0);
//                                  ^^^^  ^^^^  widened from -1/1
```

The rotation around `(1,1,1)` swings vertices well beyond z = ±1, so anything that lands outside that original narrow range just disappears. Widening to ±10 gives plenty of room.