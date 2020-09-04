/* Author: Brian Sun 
*  Date: September 1, 2020
*  UBC UAS Software Systems Coding Challenge - Line Intersection Challenge
*/

#include <iostream>
using namespace std;
#include <cmath>
#include <string>

class Line {
public:
	double x1, y1, x2, y2;

	Line(double x1, double y1, double x2, double y2) {
		this->x1 = x1;
		this->y1 = y1;
		this->x2 = x2;
		this->y2 = y2;
	}

public:
	double getSlope() {
		return (y2 - y1) / (x2 - x1);
	}

	string toString() {
		return "(" + to_string(x1) + ", " + to_string(y1) + "), (" + to_string(x2) + ", " + to_string(y2) + ") ";
	}
};

class Circle {
public:
	double x, y, radius;

	Circle(double x, double y, double radius) {
		this->x = x;
		this->y = y;
		this->radius = radius;
	}
};

class Binomial {
public:
	double xCoeff;
	double constant;

	Binomial(double xCoeff, double constant) {
		this->xCoeff = xCoeff;
		this->constant = constant;
	}

	double getXSquaredCoeff() {
		return xCoeff * xCoeff;
	}

	double getXCoeff() {
		return 2 * xCoeff * constant;
	}

	double getConst() {
		return constant * constant;
	}
};

// returns the amount of intersections (0, 1, or 2)
int howManyIntersections(Line line, Circle circle) {

	double numerator = abs((line.y2 - line.y1) * circle.x - (line.x2 - line.x1) * circle.y + (line.x2 * line.y1) - (line.y2 * line.x1));
	double denominator = sqrt(pow(line.y2 - line.y1, 2) + pow(line.x2 - line.x1, 2));
	double distance = numerator / denominator;

	if (distance > circle.radius) {
		return 0;
	}
	else if (distance == circle.radius) {
		return 1;
	}
	else {
		return 2;
	}
}

// outputs the coordinates of intersections
void getIntersections(Line line, Circle circle) {

	// different formula for a vertical line
	bool verticalLine;

	if (line.x1 == line.x2) {
		verticalLine = true;
	}
	else {
		verticalLine = false;
	}

	if (!verticalLine) {

		// solving for intersection points 
		// solving for b in (y = mx + b)
		double lineConst = line.y1 - line.getSlope() * line.x1;

		// general quadratic equation of form (mx + b - y1)^2 + (x-x1)^2 - r^2 = 0

		Binomial binomial1(line.getSlope(), lineConst - circle.y);
		Binomial binomial2(1, -circle.x);

		// essentially adding like-terms of a quadratic together
		// I am using generic variables like a,b,c because they are often used in quadratic formula
		// a is coeff of X^2, b is coeff of X, and c is the constant

		double a = binomial1.getXSquaredCoeff() + binomial2.getXSquaredCoeff();
		double b = binomial1.getXCoeff() + binomial2.getXCoeff();
		double c = binomial1.getConst() + binomial2.getConst() - (circle.radius * circle.radius);

		// solving for roots using quadratic formula
		double root1 = (-b - sqrt((b * b) - (4 * a * c))) / (2 * a);
		double root2 = (-b + sqrt((b * b) - (4 * a * c))) / (2 * a);

		double root1YValue = line.getSlope() * root1 + lineConst;
		double root2YValue = line.getSlope() * root2 + lineConst;

		// to help with line segmentation feature, finds the left and right boundary of line
		double leftX, leftY, rightX, rightY;

		if (line.x1 < line.x2) {
			leftX = line.x1;
			leftY = line.y1;
			rightX = line.x2;
			rightY = line.y2;
		}
		else {
			leftX = line.x2;
			leftY = line.y2;
			rightX = line.x1;
			rightY = line.y1;
		}

		// only need to check one root if it's imaginary
		if (isnan(root1)) {

			cout << "The segment is: " << endl;
			cout << line.toString() << endl;
		}
		else if (root1 != root2) { // the roots are different

			cout << "The points of intersection: (" << root1 << ", " << root1YValue << ") and (" << root2 << ", " << root2YValue << ")" << endl;

			Line segment1(leftX, leftY, root1, root1YValue);
			Line segment2(root1, root1YValue, root2, root2YValue);
			Line segment3(root2, root2YValue, rightX, rightY);

			cout << "The segments are: " << endl;
			cout << segment1.toString() << endl;
			cout << segment2.toString() << endl;
			cout << segment3.toString() << endl;
		}
		else { // the roots are equal

			cout << "The point of intersection: (" << root1 << ", " << root1YValue << ")" << endl;

			Line segment1(leftX, leftY, root1, root1YValue);
			Line segment2(root1, root1YValue, rightX, rightY);

			cout << "The segments are: " << endl;
			cout << segment1.toString() << endl;
			cout << segment2.toString() << endl;
		}
		cout << endl;

	}
	else {
		// we solve by y instead of x

		Binomial binomial1(1, -circle.y);
		Binomial binomial2(0, line.x1 - circle.x);

		// essentially adding like-terms of a quadratic together
		// I am using generic variables like a,b,c because they are often used in quadratic formula
		// a is coeff of Y^2, b is coeff of Y, and c is the constant

		double a = binomial1.getXSquaredCoeff() + binomial2.getXSquaredCoeff();
		double b = binomial1.getXCoeff() + binomial2.getXCoeff();
		double c = binomial1.getConst() + binomial2.getConst() - (circle.radius * circle.radius);

		// these roots are y roots
		double root1 = (-b - sqrt((b * b) - (4 * a * c))) / (2 * a);
		double root2 = (-b + sqrt((b * b) - (4 * a * c))) / (2 * a);

		// the intersection x values are all going to be the same (vertical line)
		double lowerY, higherY;

		if (line.y1 < line.y2) {
			lowerY = line.y1;
		    higherY = line.y2;
		}
		else {
			lowerY = line.y2;
			higherY = line.y1;
		}

		if (isnan(root1)) {

			cout << "There are no points of intersection" << endl;
			cout << "The segment is: " << endl;
			cout << line.toString() << endl;
		}
		else if (root1 != root2) { // the roots are different

			cout << "The points of intersection: (" << line.x1 << ", " << root1 << ") and (" << line.x1 << ", " << root2 << ")" << endl;

			Line segment1(line.x1, lowerY, line.x1, root1);
			Line segment2(line.x1, root1, line.x1, root2);
			Line segment3(line.x1, root2, line.x1, higherY);

			cout << "The segments are: " << endl;
			cout << segment1.toString() << endl;
			cout << segment2.toString() << endl;
			cout << segment3.toString() << endl;
		}
		else { // the roots are equal

			cout << "The point of intersection: (" << line.x1 << ", " << root1 << ")" << endl;

			Line segment1(line.x1, lowerY, line.x1, root1);
			Line segment2(line.x1, root1, line.x1, higherY);

			cout << "The segments are: " << endl;
			cout << segment1.toString() << endl;
			cout << segment2.toString() << endl;
		}
	}
}

int main() {

	// Testing

	// Expected output: 2 intersections 
	Line line(-2, -2, 2, 2);
	Circle circle(0, 0, 1);

	// Expected output: 1 intersection
	Line line1(0, 10, 30, 10);
	Circle circle1(12, 0, 10);

	// Expected: 2 intersections
	Line line2(0, -10, 15, 15);
	Circle circle2(9, 3, 5);

	// Expected: 0 intersections
	Line line3(0, 10, 15, 15);
	Circle circle3(10, -5, 4);

	// Expected: 1 intersection
	Line line4(-10, 0, 15, 0);
	Circle circle4(0, -1, 1);

	// Expected: 2 intersections
	Line verticalLine(10, -15, 10, 15);
	Circle circle5(10, 0, 10);

	cout << howManyIntersections(line, circle) << endl;
	getIntersections(line, circle);
	
	cout << howManyIntersections(line1, circle1) << endl;
	getIntersections(line1, circle1);

	cout << howManyIntersections(line2, circle2) << endl;
	getIntersections(line2, circle2);

	cout << howManyIntersections(line3, circle3) << endl;
	getIntersections(line3, circle3);

	cout << howManyIntersections(line4, circle4) << endl;
	getIntersections(line4, circle4);

	cout << howManyIntersections(verticalLine, circle5) << endl;
	getIntersections(verticalLine, circle5);

}
