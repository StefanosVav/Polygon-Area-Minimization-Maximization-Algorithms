//
// Created by nikos on 10/31/22.
//

#ifndef TOPOLYGON_CONVEXHULLTOSIMPLEPOLYGON_H
#define TOPOLYGON_CONVEXHULLTOSIMPLEPOLYGON_H

#include "SimpleClosedPolygonalChain.h"


class ConvexHullToSimplePolygon {
public:
    ConvexHullToSimplePolygon(Points,std::string);
    ConvexHullToSimplePolygon(Points,std::string, Segment_2, Segment_2);
    ConvexHullToSimplePolygon(Points,std::string, Segment_2, bool);
    ConvexHullToSimplePolygon();
    void PrintConvexHull();

    const EdgeToClosestPoint pickEdgeToRemove();
    void reConstructPolygon(const EdgeToClosestPoint&);
    SimpleClosedPolygonalChain ConstructSimplePolygonalChain();
    virtual ~ConvexHullToSimplePolygon();

private:
    SimpleClosedPolygonalChain polygonChain;
    std::string outputFile;
    std::string criteria;

};


#endif //TOPOLYGON_CONVEXHULLTOSIMPLEPOLYGON_H
