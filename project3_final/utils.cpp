//
// Created by nikos on 11/4/22.
//

#include "utils.h"

std::vector<std::string> validateInput(int argc, char* argv[]) {
    std::vector<std::string> parameters;
    const char* invalidInput = "usage: ./evaluate –i <point set path> –ο <output file> -preprocess <optional>\n";


    if (argc != 5 && argc != 7) {
        perror(invalidInput);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1],"-i") != 0) {
        perror(invalidInput);
        exit(EXIT_FAILURE);
    }
    else {
        parameters.push_back(argv[2]);
    }
    if (strcmp(argv[3],"-o") != 0) {
        perror(invalidInput);
        exit(EXIT_FAILURE);
    }
    else {
        parameters.push_back(argv[4]);
    }
    if(argc == 7) {
        if (strcmp(argv[5], "-preprocess") != 0) {
            perror(invalidInput);
            exit(EXIT_FAILURE);
        } else {
            parameters.push_back(argv[6]);
        }
    }
    return parameters;
}


// Sorting parameter for initialization
bool points_by_x_asc(Point_2 p1, Point_2 p2){
    if(p1.x() < p2.x())
        return true;
    if(p1.x() == p2.x() && p1.y() < p2.y())
        return true;
    return false;
}

bool points_by_x_desc(Point_2 p1, Point_2 p2){
    if(p1.x() > p2.x())
        return true;
    if(p1.x() == p2.x() && p1.y() > p2.y())
        return true;
    return false;
}

bool points_by_y_asc(Point_2 p1, Point_2 p2){
    if(p1.y() < p2.y())
        return true;
    if(p1.y() == p2.y() && p1.x() < p2.x())
        return true;
    return false;
}

bool points_by_y_desc(Point_2 p1, Point_2 p2){
    if(p1.y() > p2.y())
        return true;
    if(p1.y() == p2.y() && p1.x() > p2.x())
        return true;
    return false;
}

long double areaOfTriangle(long double dist1, long double dist2, long double dist3) {
    long double s = (dist1 + dist2 + dist3) / 2.0;
    long double area = sqrt((s*(s-dist1)*(s-dist2)*(s-dist3)));
    return area;
}

Points readFromFile(std::string inputFile) {
    Points inputPoints;
    std::ifstream input(inputFile);
    std::string lineData;

    while (getline(input, lineData)) {
        int index;
        int xCord,yCord;

        // Ignore first 2 lines with comments
        if (lineData[0] == '#') {
            continue;
        }
        std::stringstream lineStream(lineData);
        if (lineStream >> index >> xCord >> yCord) {
            inputPoints.push_back(Point_2(xCord,yCord));
        }
    }
    return inputPoints;
}

bool check_collinear(int x1, int y1, int x2, int y2, int x3, int y3){
    int a = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
    if (a == 0)
        return true;
    else
        return false;
}