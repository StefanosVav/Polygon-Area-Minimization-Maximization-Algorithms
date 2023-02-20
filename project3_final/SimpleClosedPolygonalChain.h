//
// Created by nikos on 10/31/22.
//

#ifndef TOPOLYGON_SIMPLECLOSEDPOLYGONALCHAIN_H
#define TOPOLYGON_SIMPLECLOSEDPOLYGONALCHAIN_H

#include "EdgeToClosestPoint.h"

class SimpleClosedPolygonalChain: public Polygon_2 {
private:
    Points innerPoints;
    double startingConvexArea;
    double optimizedArea;
    double ratio;
    std::vector<std::pair<double,EdgeToClosestPoint>> edgeToClosestPointWithArea;
public:
    bool SpatialIsUsed = false;
    bool leftIsUsed = false;
    bool rightIsUsed = false;
    bool bothAreUsed = false;
    Segment_2 leftEdgeNotTouch;
    Segment_2 rightEdgeNotTouch;
    SimpleClosedPolygonalChain(const Points &, const Polygon_2 &);
    SimpleClosedPolygonalChain();

    void printInnerPoints();
    void writeToFile(std::string);
    void printVertices();
    void printEdgesToClosestPoint();

    void initializeInnerPoints(const Points &);
    void initializeArea();
    Points getRandomPoints();
    void updateEdgesToClosestPoint();
    void updateInnerPoints(const Point_2 &);
    long double computeRatio();
    long double computeOptRatio();

    const EdgeToClosestPoint getEdgeToMaximizeArea();
    const EdgeToClosestPoint getEdgeToMaximizeAreaSpatial();
    const EdgeToClosestPoint getEdgeToMinimizeArea();
    const EdgeToClosestPoint getEdgeToMinimizeAreaSpatial();
    const EdgeToClosestPoint getRandomEdge();
    const EdgeToClosestPoint getRandomEdgeSpatial();

    bool myComparison(const std::pair<double,EdgeToClosestPoint> &a,const std::pair<double,EdgeToClosestPoint> &b);

    bool isEdgeVisibleFromPoint(const Segment_2&, const Point_2&);

    bool doIntersect(const Segment_2&, const Segment_2&);

    bool onSegment(const Point_2& , const Point_2& , const Point_2& );

    void includePickedPointToPolygon(const EdgeToClosestPoint &);

    // Getters, Setters
    const Points &getInnerPoints() const;

    void setInnerPoints(const Points &innerPoints);

    double getArea() const;

    void setArea(double area);

    double getOptimizedArea() const;

    void setOptimizedArea(double optimizedArea);

    double getStartingConvexArea() const;

    void setStartingConvexArea(double startingConvexArea);


    SimpleClosedPolygonalChain(Polygon_2<K> polygon2);


    long double Area;
};

#endif //TOPOLYGON_SIMPLECLOSEDPOLYGONALCHAIN_H
