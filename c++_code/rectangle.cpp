#include <iostream>
using namespace std;

// Define a class named "Rectangle"
class Rectangle {
    // Private members (cannot be accessed directly outside the class)
private:
    double width;
    double height;

    // Public members (can be accessed outside the class)
public:
    // Constructor to initialize the rectangle with width and height
    Rectangle(double w, double h) {
        width = w;
        height = h;
    }

    // Member function to calculate the area of the rectangle
    double area() {
        return width * height;
    }

    // Member function to calculate the perimeter of the rectangle
    double perimeter() {
        return 2 * (width + height);
    }

    // Function to display the dimensions
    void display() {
        cout << "Width: " << width << ", Height: " << height << endl;
    }
};

int main() {
    // Create an object of the class Rectangle
    Rectangle rect1(5.0, 3.0);

    // Display the dimensions
    rect1.display();

    // Calculate and display the area and perimeter
    cout << "Area: " << rect1.area() << endl;
    cout << "Perimeter: " << rect1.perimeter() << endl;

    return 0;
}

