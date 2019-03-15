#pragma once
class Point2D {
public:
	double x;
	double y;

	Point2D() {
		x = 0;
		y = 0;
	}

	Point2D(double x, double y) {
		this->x = x;
		this->y = y;
	}
};

class Point3D {
public:
	double x;
	double y;
	double z;

	Point3D() {
		x = 0;
		y = 0;
		z = 0;
	}

	Point3D(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

class Color {
public:
	int r;
	int g;
	int b;

	Color() {

	}

	Color(int r, int g, int b) {
		this->r = r;
		this->g = g;
		this->b = b;
	}
};


std::vector<std::string> wordsInLine(std::string line, char separator=' ') {
	std::vector<std::string> words;

	std::string word = "";

	for (auto c : line)
	{
		if (c == separator)
		{
			if (word != "") {
				words.push_back(word);
			}
			word = "";
		}
		else
		{
			word = word + c;
		}
	}

	if (word != "") {
		words.push_back(word);
	}

	return words;
}