//
// Created by nikos on 10/31/22.
//

#include "ConvexHullToSimplePolygon.h"

ConvexHullToSimplePolygon::ConvexHullToSimplePolygon() {}

ConvexHullToSimplePolygon::ConvexHullToSimplePolygon(Points points, std::string crit):criteria(crit) {

    Points result;
    CGAL::convex_hull_2( points.begin(), points.end(), std::back_inserter(result) );
    for(Points::iterator it = result.begin(); it < result.end(); it++) {
        polygonChain.push_back(*it);
    }
    polygonChain.initializeArea();
    polygonChain.initializeInnerPoints(points);

}
ConvexHullToSimplePolygon::ConvexHullToSimplePolygon(Points points, std::string crit, Segment_2 left, Segment_2 right):criteria(crit) {

    Points result;
    CGAL::convex_hull_2( points.begin(), points.end(), std::back_inserter(result) );
    for(Points::iterator it = result.begin(); it < result.end(); it++) {
        polygonChain.push_back(*it);
    }
    polygonChain.leftEdgeNotTouch = left;
    polygonChain.rightEdgeNotTouch = right;
    polygonChain.bothAreUsed = true;
    polygonChain.leftIsUsed = false;
    polygonChain.rightIsUsed = false;
    polygonChain.SpatialIsUsed = true;
    polygonChain.initializeArea();
    polygonChain.initializeInnerPoints(points);

}

ConvexHullToSimplePolygon::ConvexHullToSimplePolygon(Points points, std::string crit, Segment_2 noTouch, bool isFirst):criteria(crit) {

    Points result;
    CGAL::convex_hull_2( points.begin(), points.end(), std::back_inserter(result) );
    for(Points::iterator it = result.begin(); it < result.end(); it++) {
        polygonChain.push_back(*it);
    }
    if (isFirst) {
        polygonChain.rightEdgeNotTouch = noTouch;
        polygonChain.SpatialIsUsed = true;
        polygonChain.bothAreUsed = false;
        polygonChain.leftIsUsed = false;
        polygonChain.rightIsUsed = true;
    }
    else {
        polygonChain.SpatialIsUsed = true;
        polygonChain.rightIsUsed = false;
        polygonChain.bothAreUsed = false;
        polygonChain.leftIsUsed = true;
        polygonChain.leftEdgeNotTouch = noTouch;
    }

    polygonChain.initializeArea();
    polygonChain.initializeInnerPoints(points);

}



SimpleClosedPolygonalChain ConvexHullToSimplePolygon::ConstructSimplePolygonalChain() {

    while (!polygonChain.getInnerPoints().empty()) {
        polygonChain.updateEdgesToClosestPoint();
        const EdgeToClosestPoint& toRemove = pickEdgeToRemove();
        reConstructPolygon(toRemove);
    }
    //polygonChain.writeToFile(outputFile);
    //std::cout << "Is polygon simple: " << polygonChain.is_simple() << std::endl;
    return polygonChain;
}

// Call the appropriate method according to the given
// criteria from the input
// 1) random edge selection
// 2) edge that minimizes area of the polygon(local search)
// 3) edge that maximizes area of the polygon(local search)
const EdgeToClosestPoint ConvexHullToSimplePolygon::pickEdgeToRemove() {
    if (criteria == "1") {
        if (polygonChain.SpatialIsUsed) {
            return polygonChain.getRandomEdgeSpatial();
        }
        return polygonChain.getRandomEdge();
    }
    else if (criteria == "2") {
        return polygonChain.getEdgeToMinimizeArea();
    }
    else if (criteria == "3") {
        return polygonChain.getEdgeToMaximizeArea();
    }
    else {
        std::cout << "Edge selection criteria must be 1 or 2 or 3" << std::endl;
        exit(EXIT_FAILURE);
    }


}

void ConvexHullToSimplePolygon::reConstructPolygon(const EdgeToClosestPoint & edgeToClosestPoint) {
    polygonChain.includePickedPointToPolygon(edgeToClosestPoint);
}

void ConvexHullToSimplePolygon::PrintConvexHull() {


    std::cout << "Printing convex hull" << std::endl;
    const Polygon_2::Vertices& range = polygonChain.vertices();
    for(auto it = range.begin(); it!= range.end(); ++it){
        std::cout << *it << std::endl;
    }

}


ConvexHullToSimplePolygon::~ConvexHullToSimplePolygon() {

}
