//
// Created by nikos on 10/31/22.
//


#include "SimpleClosedPolygonalChain.h"
SimpleClosedPolygonalChain::SimpleClosedPolygonalChain() {}


void SimpleClosedPolygonalChain::initializeInnerPoints(const Points& points) {
    for (auto point : points) {
        if (this->bounded_side(point) == CGAL::ON_BOUNDED_SIDE) {
            //std::cout << point << " is INSIDE the polygon" << std::endl;
            innerPoints.push_back(point);
        }
    }
}

void SimpleClosedPolygonalChain::updateEdgesToClosestPoint() {
    Points toCheck(innerPoints);
    //const_iterator index;
    for (const Segment_2& e : edges()) {
        long double minDistance = DBL_MAX;
        Point_2 closestPoint;
        long double distance;
        for (const_iterator it = toCheck.begin(); it < toCheck.end(); it++) {
            distance = CGAL::squared_distance(e,*it);
            if (distance < minDistance) {
                minDistance = distance;
                closestPoint = *it;
                //index = it;
            }
        }
        EdgeToClosestPoint entry(e.source(),e.target(),closestPoint.x(),closestPoint.y());
        double area = (double)fabs(CGAL::area(e.source(),e.target(),closestPoint));
        edgeToClosestPointWithArea.push_back(std::make_pair(area,entry));
    }
}

void SimpleClosedPolygonalChain::printInnerPoints() {
    //std::cout << "!!!! Inner Points" << std::endl;
//    for (auto inner = innerPoints.begin(); inner < innerPoints.end(); inner++) {
//        std::cout << *inner << std::endl;
//    }
}

void SimpleClosedPolygonalChain::writeToFile(std::string outputFile) {
    std::ofstream output(outputFile);
    const Polygon_2::Vertices& range = this->vertices();

    for(auto it = range.begin(); it!= range.end(); ++it){
        output << *it << std::endl;
    }
    for(const Segment_2& e  : edges()){
        output << e << std::endl;
    }

    output << "Start Area: " << Area << std::endl;
    output << "Ratio of starting polygon from greedy algorithm area with starting area of convex hull" << std::endl;
    output << computeRatio() << std::endl;

    output << "Area after simulated annealing" << optimizedArea << std::endl;
    output << "Ratio of polygon area after simulated annealing with starting area of convex hull" << std::endl;
    output << computeOptRatio() << std::endl;

    output << "Polygon is simple: " << is_simple() << std::endl;

    output.close();
}

void SimpleClosedPolygonalChain::initializeArea() {
    this->startingConvexArea = fabs(area());
    this->Area = startingConvexArea;
}

void SimpleClosedPolygonalChain::printVertices() {

}

const EdgeToClosestPoint SimpleClosedPolygonalChain::getEdgeToMaximizeArea() {
    // Couldn't find time to modify std::sort accordingly
    // to work with the pair<double,EdgeToClosestPoint> --> modify it to pair<double,*EdgeToClosestPoint*>
    // still after std::sort in the new pair, vertices of the edges turn into double (if you find time debug it)
    //std::sort(edgeToClosestPointWithArea.begin(),edgeToClosestPointWithArea.end(),[](const std::pair<double,EdgeToClosestPoint>&x, const std::pair<double,EdgeToClosestPoint>&y){return x.first < y.first;});


    while (edgeToClosestPointWithArea.size() > 0) {
        Segment_2 edge;
        Point_2 point;
        double minArea = std::numeric_limits<double>::max();
        auto index = edgeToClosestPointWithArea.begin();
        for (auto it = edgeToClosestPointWithArea.begin(); it < edgeToClosestPointWithArea.end(); it++) {
            if (it->first < minArea) {
                minArea = it->first;
                edge = it->second.getSegment();
                point = it->second.getClosestPoint();
                index = it;
            }
        }
        if (isEdgeVisibleFromPoint(edge,point)) {
            Area -= minArea;
            return EdgeToClosestPoint(edge.source(),edge.target(),point.x(),point.y());
        }
        else {
            edgeToClosestPointWithArea.erase(index);
        }
    }
    // handle special case
    std::cout << "Special Case where the last remained point isn't visible from it's closest edge, exiting.." << std::endl;
    exit(EXIT_FAILURE);
}

const EdgeToClosestPoint SimpleClosedPolygonalChain::getEdgeToMinimizeArea() {
    // Couldn't find time to modify std::sort accordingly
    // to work with the pair<double,EdgeToClosestPoint> --> modify it to pair<double,*EdgeToClosestPoint*>
    // still after std::sort in the new pair, vertices of the edges turn into double (if you find time debug it)
    //std::sort(edgeToClosestPointWithArea.begin(),edgeToClosestPointWithArea.end(),[](const std::pair<double,EdgeToClosestPoint>&x, const std::pair<double,EdgeToClosestPoint>&y){return x.first < y.first;});


    while (edgeToClosestPointWithArea.size() > 0) {
        Segment_2 edge;
        Point_2 point;
        double maxArea = std::numeric_limits<double>::min();
        auto index = edgeToClosestPointWithArea.begin();
        for (auto it = edgeToClosestPointWithArea.begin(); it < edgeToClosestPointWithArea.end(); it++) {
            if (it->first > maxArea) {
                maxArea = it->first;
                edge = it->second.getSegment();
                point = it->second.getClosestPoint();
                index = it;
            }
        }
        if (isEdgeVisibleFromPoint(edge,point)) {
            Area -= maxArea;
            return EdgeToClosestPoint(edge.source(),edge.target(),point.x(),point.y());
        }
        else {
            edgeToClosestPointWithArea.erase(index);
        }
    }
    // handle special case
    std::cout << "Special Case where the last remained point isn't visible from it's closest edge, exiting.." << std::endl;
    exit(EXIT_FAILURE);
}

const EdgeToClosestPoint SimpleClosedPolygonalChain::getRandomEdge(){
    srand((unsigned) time(NULL));

    while (edgeToClosestPointWithArea.size() > 0) {
        Segment_2 edge;
        Point_2 point;
        double areaToRemove;
        int indexToRemove = rand() % edgeToClosestPointWithArea.size();
        auto index = edgeToClosestPointWithArea.begin();
        int count = 0;
        for (auto it = edgeToClosestPointWithArea.begin(); it < edgeToClosestPointWithArea.end(); it++) {
            if (count == indexToRemove) {
                areaToRemove = it->first;
                index = it;
                edge = it->second.getSegment();
                point = it->second.getClosestPoint();
                break;
            }
            count++;
        }
        if (isEdgeVisibleFromPoint(edge,point)) {
            Area -= areaToRemove;
            return EdgeToClosestPoint(edge.source(),edge.target(),point.x(),point.y());
        }
        else {
            edgeToClosestPointWithArea.erase(index);
        }
    }
    // handle special case
    std::cout << "Special Case where the last remained point isn't visible from it's closest edge, exiting.." << std::endl;
    exit(EXIT_FAILURE);
}

const EdgeToClosestPoint SimpleClosedPolygonalChain::getRandomEdgeSpatial(){
    srand((unsigned) time(NULL));

    while (edgeToClosestPointWithArea.size() > 0) {
        Segment_2 edge;
        Point_2 point;
        double areaToRemove;
        bool found = false;
        auto index = edgeToClosestPointWithArea.begin();
        while (!found) {
            int indexToRemove = rand() % edgeToClosestPointWithArea.size();
            index = edgeToClosestPointWithArea.begin();
            int count = 0;
            for (auto it = edgeToClosestPointWithArea.begin(); it < edgeToClosestPointWithArea.end(); it++) {
                if (count == indexToRemove) {
                    areaToRemove = it->first;
                    index = it;
                    edge = it->second.getSegment();
                    point = it->second.getClosestPoint();
                    if (leftIsUsed) {
                        if (edge == leftEdgeNotTouch) {
                            break;
                        }
                    }
                    else if (rightIsUsed) {
                        if (edge == rightEdgeNotTouch) {
                            break;
                        }
                    }
                    else {
                        if (edge == rightEdgeNotTouch || edge == leftEdgeNotTouch) {
                            break;
                        }
                    }
                    found = true;
                    break;
                }
                count++;
            }
        }

        if (isEdgeVisibleFromPoint(edge,point)) {
            Area -= areaToRemove;
            return EdgeToClosestPoint(edge.source(),edge.target(),point.x(),point.y());
        }
        else {
            edgeToClosestPointWithArea.erase(index);
        }
    }
    // handle special case
    std::cout << "Special Case where the last remained point isn't visible from it's closest edge, exiting.." << std::endl;
    exit(EXIT_FAILURE);
}


bool SimpleClosedPolygonalChain::myComparison(const std::pair<double,EdgeToClosestPoint> &a,const std::pair<double,EdgeToClosestPoint> &b){
    return a.first < b.first;
}

bool SimpleClosedPolygonalChain::isEdgeVisibleFromPoint(const Segment_2 & edgeToRemove, const Point_2 & pointToAdd) {
    Segment_2 s1(pointToAdd,edgeToRemove.source());
    Segment_2 s2(pointToAdd,edgeToRemove.target());
    for (auto edge:edges()) {
        if (edge == edgeToRemove) {
            continue;
        }
        else {
            Segment_2 edgeToCheck(edge.source(), edge.target());
            if (doIntersect(s1,edgeToCheck) || doIntersect(s2,edgeToCheck)) {
                return false;
            }
        }
    }
    return true;
}

// Source: https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
bool SimpleClosedPolygonalChain::doIntersect(const Segment_2 & segment1, const Segment_2 & segment2) {

    // Segment 1
    Point_2 p1(segment1.source());
    Point_2 q1(segment1.target());

    // Segment 2
    Point_2 p2(segment2.source());
    Point_2 q2(segment2.target());

    if (q1 == p2 || q1 == q2 || p1 == p2) {
        return false;
    }

//    if (q1 == p2 || q1 == q2) {
//        return false;
//    }

    int o1 = CGAL::orientation(p1, q1, p2);
    int o2 = CGAL::orientation(p1, q1, q2);
    int o3 = CGAL::orientation(p2, q2, p1);
    int o4 = CGAL::orientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4) {
        return true;
    }
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;

    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

    if (o3 == 0 && onSegment(p2, p1, q2)) return true;

    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false;


}

bool SimpleClosedPolygonalChain::onSegment(const Point_2& p, const Point_2& q, const Point_2& r)
{
    if (q.x() <= std::max(p.x(), r.x()) && q.x() >= std::min(p.x(), r.x()) &&
        q.y() <= std::max(p.y(), r.y()) && q.y() >= std::min(p.y(), r.y()))
        return true;

    return false;
}


void SimpleClosedPolygonalChain::includePickedPointToPolygon(const EdgeToClosestPoint & edgeToPoint) {
    for (auto it = vertices_begin(); it < vertices_end(); it++) {
        if (it->x() == edgeToPoint.getSegment().target().x() && it->y() == edgeToPoint.getSegment().target().y()) {
            this->insert(it,edgeToPoint.getClosestPoint());
            break;
        }
    }
    updateInnerPoints(edgeToPoint.getClosestPoint());

}

void SimpleClosedPolygonalChain::updateInnerPoints(const Point_2 & point) {
    for (auto it = innerPoints.begin(); it < innerPoints.end(); it++) {
        if (*it == point) {
            innerPoints.erase(it);
            break;
        }
    }
    printInnerPoints();
}

const Points &SimpleClosedPolygonalChain::getInnerPoints() const {
    return innerPoints;
}

void SimpleClosedPolygonalChain::setInnerPoints(const Points &innerPoints) {
    SimpleClosedPolygonalChain::innerPoints = innerPoints;
}

long double SimpleClosedPolygonalChain::computeRatio() {
    return (long double)Area/startingConvexArea;
}

long double SimpleClosedPolygonalChain::computeOptRatio() {
    return (long double)optimizedArea/startingConvexArea;
}

double SimpleClosedPolygonalChain::getArea() const {
    return Area;
}

void SimpleClosedPolygonalChain::setArea(double area) {
    Area = area;
}

double SimpleClosedPolygonalChain::getOptimizedArea() const {
    return optimizedArea;
}

void SimpleClosedPolygonalChain::setOptimizedArea(double optimizedArea) {
    SimpleClosedPolygonalChain::optimizedArea = optimizedArea;
}

double SimpleClosedPolygonalChain::getStartingConvexArea() const {
    return startingConvexArea;
}

void SimpleClosedPolygonalChain::setStartingConvexArea(double startingConvexArea) {
    SimpleClosedPolygonalChain::startingConvexArea = startingConvexArea;
}

SimpleClosedPolygonalChain::SimpleClosedPolygonalChain(Polygon_2<K> polygon2) {
    this->Area = polygon2.area();

}