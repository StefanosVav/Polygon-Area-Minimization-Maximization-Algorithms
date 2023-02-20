//
// Created by nikos on 12/4/22.
//

#ifndef TOPOLYGON_SIMULATEDANNEALING_H
#define TOPOLYGON_SIMULATEDANNEALING_H

#include "utils.h"
#include "SimpleClosedPolygonalChain.h"
#include "ConvexHullToSimplePolygon.h"

#define m 15
class SimulatedAnnealing {
public:
    SimulatedAnnealing(std::string, std::string, std::string, int, std::string, std::string);
    void createSubsets();
    Points rangeQuery(Points& );
    Points getRandomPointForLocalStep();
    Points getRandomPointsForGlobalStep();
    void optimizeWithLocalStep();
    void optimizeWithGlobalStep();
    void optimizeWithGlobalStepSpatial(int);
    void optimizeWithSpatial();
    bool localStepValid(Points&);
    bool globalStepValid(Points&);
    bool maxMetropolisCriteria(double& );
    bool minMetropolisCriteria(double&);
    Points getRandomPointsForGlobalStepSpatial(int index);
    bool globalStepValidSpatial(Points& points, int index);
    bool maxMetropolisCriteriaSpatial(long double & diffArea, int index);
    bool minMetropolisCriteriaSpatial(long double & diffArea, int index);
    void updatePolygonGlobSpatial(Points & points, int index);
    void mergePolygons();
    long double solve();


private:
    std::vector<Points> subSets;
    std::string outputFile;
    SimpleClosedPolygonalChain simplePolygon;
    Polygon_with_holes_2  merged;
    Points inputPoints;
    int L;
    int n;
    long double T;
    long double R;
    std::string criteria;
    std::string edgeSelection;
    std::string methodOfSimulatedAnnealing;
    std::string initializationAlgorithm;
    double calculateDiffArea(Points&);
    long double calculateDiffAreaGlob(Points&);
    void updatePolygon(Points&);
    void updatePolygonGlob(Points&);
    std::vector<SimpleClosedPolygonalChain> polygons;

public:
    long double getT() const;

    void setT(long double t);

    int getN() const;

    void setN(int n);

    int getL() const;

    void setL(int l);

    long double getR() const;

    void setR(long double r);

    const SimpleClosedPolygonalChain &getSimplePolygon() const;

    void setSimplePolygon(const SimpleClosedPolygonalChain &simplePolygon);

};


#endif //TOPOLYGON_SIMULATEDANNEALING_H