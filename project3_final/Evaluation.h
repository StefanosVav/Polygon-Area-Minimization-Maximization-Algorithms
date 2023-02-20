//
// Created by nikos on 1/13/23.
//

#include <string>
#include "incremental.h"
#include "localSearch.h"
#include "SimulatedAnnealing.h"
#include "ConvexHullToSimplePolygon.h"

#ifndef TOPOLYGON_EVALUATION_H
#define TOPOLYGON_EVALUATION_H

#endif //TOPOLYGON_EVALUATION_H

vector<long double> evaluateLocalForMinimization(std::string inputFile);
vector<long double> evaluateLocalForMaximization(std::string inputFile);
vector<long double> evaluateSimulatedForMinimization(std::string inputFile);
vector<long double> evaluateSimulatedForMaximization(std::string inputFile);
Polygon_2 getPolygonFromConvexHull(std::string inputFile, std::string edgeSelection);
void getPrintScores(vector<long double>, vector<long double>,vector<long double>,vector<long double>, string);