#!/usr/bin/env python3

 

# Define a class named "Rectangle"
 class Rectangle:
 # Constructor to initialize the rectangle with width and height
 def __init__(self, width, height):
     self.width = width
     self.height = height

 

# Method to calculate the area of the rectangle
 def area(self):
     return self.width * self.height

 

# Method to calculate the perimeter of the rectangle
 def perimeter(self):
     return 2 * (self.width + self.height)

 

# Method to display the dimensions
 def display(self):
     print(f"Width: {self.width}, Height: {self.height}")

 

# Main program execution block
 if __name__ == "__main__":
 # Create an object of the class Rectangle
 rect1 = Rectangle(5.0, 3.0)

 

# Display the dimensions
 rect1.display()

 

# Calculate and display the area and perimeter
 print(f"Area: {rect1.area()}")
 print(f"Perimeter: {rect1.perimeter()}")