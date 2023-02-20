//
// Created by nikos on 11/4/22.
//

#ifndef TOPOLYGON_UTILS_H
#define TOPOLYGON_UTILS_H

#include "iostream"
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_2_algorithms.h>
#include <algorithm>
#include<bits/stdc++.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <vector>
#include <CGAL/squared_distance_2.h> //for 2D functions
#include <iostream>
#include <fstream>
#include <string>
#include <CGAL/Kd_tree.h>
#include <CGAL/Fuzzy_iso_box.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/algorithm.h>
#include <CGAL/Search_traits_2.h>
#include <CGAL/Iso_rectangle_2.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <dirent.h>
#include <iomanip>


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Polygon_with_holes_2<K>     Polygon_with_holes_2;

typedef K::Point_2 Point_2;
typedef std::vector<Point_2> Points;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef K::Segment_2  Segment_2;
typedef CGAL::Search_traits_2<K> Traits;
typedef CGAL::Kd_tree<Traits> Tree;

typedef CGAL::Fuzzy_iso_box<Traits> Fuzzy_iso_box;

bool check_collinear(int x1, int y1, int x2, int y2, int x3, int y3);
std::vector<std::string> validateInput(int ,char* [] );

// Defining sorting parameters for initialization
bool points_by_x_asc(Point_2 p1, Point_2 p2);
bool points_by_x_desc(Point_2 p1, Point_2 p2);
bool points_by_y_asc(Point_2 p1, Point_2 p2);
bool points_by_y_desc(Point_2 p1, Point_2 p2);
long double areaOfTriangle(long double dist1, long double dist2, long double dist3);
Points readFromFile(std::string inputFile);

#endif //TOPOLYGON_UTILS_H
