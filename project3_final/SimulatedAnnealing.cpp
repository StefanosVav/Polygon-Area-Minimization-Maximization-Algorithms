//
// Created by nikos on 12/4/22.
//

#include "SimulatedAnnealing.h"
#include "incremental.h"

SimulatedAnnealing::SimulatedAnnealing(std::string inputFile, std::string init, std::string edgeSelection, int L, std::string crit, std::string method)
        : L(L), initializationAlgorithm(init), edgeSelection(edgeSelection), criteria(crit), methodOfSimulatedAnnealing(method) {
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
    this->T = 1;

    // Use Convex hull algorithm to generate the initial state
    if (initializationAlgorithm == "convex_hull" && (methodOfSimulatedAnnealing == "global" || methodOfSimulatedAnnealing == "local") || methodOfSimulatedAnnealing == "subdivision") {
        simplePolygon = ConvexHullToSimplePolygon(inputPoints, edgeSelection).ConstructSimplePolygonalChain();
    }

    if (initializationAlgorithm == "incremental") {
        Polygon_2 returnedPolygon= Incremental(inputFile, edgeSelection);

        Points points = returnedPolygon.container();
        for (auto p : points) {
            simplePolygon.push_back(p);
        }
        Points result;
        Polygon_2 convex;
        CGAL::convex_hull_2( points.begin(), points.end(), std::back_inserter(result) );
        for(Points::iterator it = result.begin(); it < result.end(); it++) {
            convex.push_back(*it);
        }

        simplePolygon.setArea(simplePolygon.area());
        simplePolygon.setStartingConvexArea(convex.area());

    }


//    for (auto e:simplePolygon.edges()) {
//        std::cout << e << std::endl;
//    }
    simplePolygon.setOptimizedArea(simplePolygon.getArea());
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);//uniform distribution between 0 and 1
    this->R = dis(gen);
    this->n = inputPoints.size();

}

long double SimulatedAnnealing::solve() {
    if (methodOfSimulatedAnnealing == "global") {
        optimizeWithGlobalStep();
        return simplePolygon.computeOptRatio();
    }

    else if (methodOfSimulatedAnnealing == "local") {
        optimizeWithLocalStep();
        return simplePolygon.computeOptRatio();
    }
    else if (methodOfSimulatedAnnealing == "subdivision"){
        createSubsets();
        optimizeWithSpatial();
        mergePolygons();

    }
    else {

        std::cout << "Wrong Input" << std::endl;
        exit(1);
    }
    return 0;

}

void SimulatedAnnealing::createSubsets() {
    sort(inputPoints.begin(), inputPoints.end(), points_by_x_asc);
    int numberOfSubsets = ceil((inputPoints.size() - 1) /(m -1) );
    int subsetsCreated = 0;

    std::vector<Point_2>::const_iterator leftMostCurrent = inputPoints.begin();
    std::vector<Point_2>::const_iterator rightMostCurrent = inputPoints.begin() + m;
    int pointsIncluded = 0;
    while (subsetsCreated != numberOfSubsets) {
        if (inputPoints.size() - pointsIncluded < m) {
            for (auto it = leftMostCurrent; it < inputPoints.end(); it++) {
                subSets[subsetsCreated - 1].push_back(*it);
            }
            break;
        }
        std::vector<Point_2> currentSubset(leftMostCurrent, rightMostCurrent);
        std::vector<Point_2>::const_iterator leftMostAdj = rightMostCurrent - 1;
        std::vector<Point_2>::const_iterator rightMostAdjacent = rightMostCurrent + m - 1;
        std::vector<Point_2> adjSubset(leftMostAdj,rightMostAdjacent);

        /**TODO: maybe move it to a separate method
         * TODO: include the equivalent case of decrement/increment upper hull
         * if condition for spatial subdivision holds create the subset, else add a point to the potential subset
         */
        Points lowerHullCurrent;
        Points lowerHullAdjacent;
        CGAL::lower_hull_points_2(currentSubset.begin(), currentSubset.end(), std::back_inserter(lowerHullCurrent));
        CGAL::lower_hull_points_2(adjSubset.begin(), adjSubset.end(), std::back_inserter(lowerHullAdjacent));
        std::vector<Point_2>::const_iterator lowStartCurr = lowerHullCurrent.begin() + 1;
        std::vector<Point_2>::const_iterator lowCurr = lowerHullCurrent.end() - 1;
        std::vector<Point_2>::const_iterator lowAdj = lowerHullAdjacent.begin() + 1;
        if (lowCurr->y() < leftMostAdj->y() && lowAdj->y() < leftMostAdj->y()) {
            subSets.push_back(currentSubset);
            pointsIncluded += currentSubset.size();
            leftMostCurrent = leftMostAdj;
            rightMostCurrent = rightMostAdjacent;
            subsetsCreated++;
        }
        else {
            rightMostCurrent = rightMostCurrent + 1;
            continue;
        }

    }




//    Points lowerHull;
//    Points upperHull;
//    Points convexHull;
//    CGAL::lower_hull_points_2(subset.begin(), subset.end(), std::back_inserter(lowerHull));
//    CGAL::convex_hull_2(subset.begin(), subset.end(),std::back_inserter(convexHull));
//    CGAL::upper_hull_points_2(subset.begin(), subset.end(),std::back_inserter(upperHull));

//    std::cout << "----- Lower Hull ------" << std::endl;
//    for (int i = 0; i < lowerHull.size(); i++) {
//        std::cout << lowerHull[i] << std::endl;
//    }
//    std::cout << "----- Lower Hull ------" << std::endl;
//
//
//    std::cout << "----- Convex Hull ------" << std::endl;
//    for (int i = 0; i < convexHull.size(); i++) {
//        std::cout << convexHull[i] << std::endl;
//    }
//    std::cout << "----- Convex Hull ------" << std::endl;
//
//    std::cout << "----- Upper Hull ------" << std::endl;
//    for (int i = 0; i < upperHull.size(); i++) {
//        std::cout << upperHull[i] << std::endl;
//    }
//    std::cout << "----- Upper Hull ------" << std::endl;
// std::cout << "I am here" << std::endl;
    for (int i = 0; i < subSets.size(); i++) {
        Points lowerHullCurrent;
        CGAL::lower_hull_points_2(subSets[i].begin(), subSets[i].end(), std::back_inserter(lowerHullCurrent));
        std::vector<Point_2>::const_iterator lowStartCurr = lowerHullCurrent.begin() + 1;
        std::vector<Point_2>::const_iterator lowCurr = lowerHullCurrent.end() - 1;
        Segment_2 leftNoTouch(*subSets[i].begin(), *lowStartCurr);
        Segment_2 rightNoTouch(*lowCurr, *(subSets[i].end() - 1));

        if (i == 0) {
            polygons.push_back(ConvexHullToSimplePolygon(subSets[i],edgeSelection, rightNoTouch,true).ConstructSimplePolygonalChain());
        }
        else if (i == (subSets.size() - 1)) {
            polygons.push_back(ConvexHullToSimplePolygon(subSets[i],edgeSelection, leftNoTouch, false).ConstructSimplePolygonalChain());
        }
        else {
            polygons.push_back(ConvexHullToSimplePolygon(subSets[i],edgeSelection, leftNoTouch, rightNoTouch).ConstructSimplePolygonalChain());
        }

    }
}

Points SimulatedAnnealing::rangeQuery(Points& points) {

    Tree tree;
    for (auto it = inputPoints.begin(); it < inputPoints.end(); it++) {
        tree.insert(*it);
    }

    auto bbox = CGAL::bbox_2(points.begin(), points.end());

    Points res;
    auto smallest = Point_2(bbox.xmin(), bbox.ymin());
    auto largest = Point_2(bbox.xmax(), bbox.ymax());
    Fuzzy_iso_box rec(smallest, largest);
    tree.search(std::back_inserter(res), rec);

//    std::cout << "The points in the box are: " << std::endl;
//    std::copy (res.begin(), res.end(), std::ostream_iterator<Point_2>(std::cout,"\n") );
//    std::cout << std::endl;

    // Remove the points that we don't need
//    Points toReturn;
//    for (auto point:res) {
//        if (point == points[0] || point == points[1] || point == points[2] || point == points[3]) {
//            continue;
//        }
//        else {
//            toReturn.push_back(point);
//        }
//    }

    //return toReturn;
    return res;
}

void SimulatedAnnealing::optimizeWithLocalStep() {
    double diffArea;

    for (int i = 0; i < getL(); i++) {
        Points randomPoints = getRandomPointForLocalStep();
        if (localStepValid(randomPoints)) {
            diffArea = calculateDiffArea(randomPoints);

            if (criteria == "-max") {
                if (maxMetropolisCriteria(diffArea)) {

                    updatePolygon(randomPoints);

                }
            }
            else {
                if (minMetropolisCriteria(diffArea)) {
                    updatePolygon(randomPoints);
//                    if (!simplePolygon.is_simple()) {
//                        std::cout << "broke" << std::endl;
//                    }
                }
            }

        }
        this->setT(this->getT() - (long double)(1.0/this->getL()));
    }

    // simplePolygon.writeToFile(outputFile);

}

void SimulatedAnnealing::optimizeWithGlobalStepSpatial(int polygonIndex) {
    this->T = 1;
    long double diffArea;


    for (int i = 0; i < L; i++) {

        Points randomPoints = getRandomPointsForGlobalStepSpatial(polygonIndex);
        Segment_2 s1(randomPoints[0], randomPoints[1]);
        Segment_2 s2(randomPoints[1], randomPoints[2]);
        Segment_2 s3(randomPoints[3], randomPoints[4]);
        if (polygons[polygonIndex].rightIsUsed) {
            if (s1 == polygons[polygonIndex].rightEdgeNotTouch || s2 == polygons[polygonIndex].rightEdgeNotTouch ||
                s3 == polygons[polygonIndex].rightEdgeNotTouch) {
                continue;
            }
        }
        else if (polygons[polygonIndex].leftIsUsed) {
            if (s1 == polygons[polygonIndex].leftEdgeNotTouch || s2 == polygons[polygonIndex].leftEdgeNotTouch || s3 == polygons[polygonIndex].leftEdgeNotTouch) {
                continue;
            }
        }
        else {
            if (s1 == polygons[polygonIndex].rightEdgeNotTouch || s1 == polygons[polygonIndex].leftEdgeNotTouch || s2 == polygons[polygonIndex].rightEdgeNotTouch
                || s2 == polygons[polygonIndex].leftEdgeNotTouch || s3 == polygons[polygonIndex].rightEdgeNotTouch || s3 == polygons[polygonIndex].leftEdgeNotTouch) {
                continue;
            }
        }


        if (globalStepValidSpatial(randomPoints, polygonIndex)) {
            diffArea = calculateDiffAreaGlob(randomPoints);
            if (criteria == "-max") {
                if (maxMetropolisCriteriaSpatial(diffArea, polygonIndex)) {
                    updatePolygonGlob(randomPoints);
                }
            }
            else {
                if (minMetropolisCriteriaSpatial(diffArea, polygonIndex)) {
                    updatePolygonGlob(randomPoints);
                }
            }

        }
        this->T = this->T - (1.0/ (long double)this->L);
        //std::cout << "T = " << this->T << std::endl;
    }

}
void SimulatedAnnealing::mergePolygons() {
    long double optArea = 0;

    for (auto pol:polygons) {
        optArea += pol.area();
    }
    /*
    int index = 0;
    Points mergedPoints;
    while (index < polygons.size()) {
        Points points = polygons[index].container();
        for (auto p : points) {
            if (polygons[index].rightIsUsed && polygons[index].rightEdgeNotTouch.target() == p) {
                continue;
            }
            if (polygons[index].leftIsUsed && polygons[index].leftEdgeNotTouch.source() == p) {
                continue;
            }
            if (polygons[index].bothAreUsed && (polygons[index].rightEdgeNotTouch.target() == p || polygons[index].leftEdgeNotTouch.source() == p)) {
                continue;
            }
            mergedPoints.push_back(p);
        }
        index++;
    }

    for (auto p : mergedPoints) {
        merged.push_back(p);
    }
     */
    std::ofstream output(outputFile);



    output << "Start Area: " << simplePolygon.Area << std::endl;
    output << "Ratio of starting polygon from greedy algorithm area with starting area of convex hull" << std::endl;
    output << simplePolygon.computeRatio() << std::endl;

    output << "Area after simulated annealing" << optArea << std::endl;
    output << "Ratio of polygon area after simulated annealing with starting area of convex hull" << std::endl;
    output << (long double)optArea/simplePolygon.getStartingConvexArea() << std::endl;

    output.close();
}

void SimulatedAnnealing::optimizeWithGlobalStep() {
    this->T = 1;

    double diffArea;


    for (int i = 0; i < L; i++) {
        Points randomPoints = getRandomPointsForGlobalStep();

        if (globalStepValid(randomPoints)) {
            diffArea = calculateDiffAreaGlob(randomPoints);
            if (criteria == "-max") {
                if (maxMetropolisCriteria(diffArea)) {
                    updatePolygonGlob(randomPoints);
                }
            }
            else {
                if (minMetropolisCriteria(diffArea)) {
                    updatePolygonGlob(randomPoints);
                }
            }

        }
        this->T = this->T - (1.0/ (long double)this->L);
        //std::cout << "T = " << this->T << std::endl;
    }

    //   simplePolygon.writeToFile(outputFile);

}

void SimulatedAnnealing::optimizeWithSpatial() {
    for (int i = 0; i < polygons.size(); i++) {
        optimizeWithGlobalStepSpatial(i);
    }

}
long double SimulatedAnnealing::calculateDiffAreaGlob(Points& points) {
//    long double dist1 = sqrt(CGAL::squared_distance(points[1], points[3]));
//    long double dist2 = sqrt(CGAL::squared_distance(points[1], points[4]));
//    long double dist3 = sqrt(CGAL::squared_distance(points[4], points[3]));
    long double areaOfTriangle1 = (long double) fabs(CGAL::area(points[1],points[3],points[4]));
    //long double areaOfTriangleToBeAdded = areaOfTriangle(dist1, dist2, dist3);

//    long double dist4 = sqrt(CGAL::squared_distance(points[0], points[1]));
//    long double dist5 = sqrt(CGAL::squared_distance(points[1], points[2]));
//    long double dist6 = sqrt(CGAL::squared_distance(points[0], points[2]));

    long double areaOfTriangle2 = (long double) fabs(CGAL::area(points[0],points[1],points[2]));
    // long double areaOfTriangleToBeRemoved = areaOfTriangle(dist4, dist5, dist6);

    if (CGAL::orientation(points[0], points[2], points[1]) == CGAL::CLOCKWISE) {
        return areaOfTriangle1 - areaOfTriangle2;
    }
    return areaOfTriangle2 - areaOfTriangle1;



}

void SimulatedAnnealing::updatePolygonGlobSpatial(Points & points, int index) {

    //std::cout << "Before update polygon is simple? " << simplePolygon.is_simple() << std::endl;
    for (auto it = polygons[index].vertices_begin(); it < polygons[index].vertices_end(); it++) {
        if (it->x() == points[1].x() && it->y() == points[1].y()) {
            this->polygons[index].erase(it);
            break;
        }

    }
    for (auto it = polygons[index].vertices_begin(); it < polygons[index].vertices_end(); it++) {
        if (it->x() == points[4].x() && it->y() == points[4].y()) {
            this->polygons[index].insert(it, points[1]);
            break;
        }
    }
    // std::cout << "After update polygon is simple? " << simplePolygon.is_simple() << std::endl;

}

void SimulatedAnnealing::updatePolygonGlob(Points & points) {

    //std::cout << "Before update polygon is simple? " << simplePolygon.is_simple() << std::endl;
    for (auto it = simplePolygon.vertices_begin(); it < simplePolygon.vertices_end(); it++) {
        if (it->x() == points[1].x() && it->y() == points[1].y()) {
            this->simplePolygon.erase(it);
            break;
        }

    }
    for (auto it = simplePolygon.vertices_begin(); it < simplePolygon.vertices_end(); it++) {
        if (it->x() == points[4].x() && it->y() == points[4].y()) {
            this->simplePolygon.insert(it, points[1]);
            break;
        }
    }
    //simplePolygon.setOptimizedArea(simplePolygon.area());
    // std::cout << "After update polygon is simple? " << simplePolygon.is_simple() << std::endl;

}

void SimulatedAnnealing::updatePolygon(Points & points) {


//    SimpleClosedPolygonalChain::Vertex_iterator posToReEnter;
//
//    Points points2 = simplePolygon.container();
//
//    for (auto it = points2.begin(); it < points2.end(); it++) {
//        if (*it == points[1]) {
//            std::cout << "I am here" << std::endl;
//            iter_swap(it, it + 1);
//            break;
//        }
//    }
//    Polygon_2 pgn(points2.begin(), points2.end());
    // setSimplePolygon(pgn);

//    for (auto e : simplePolygon.edges()) {
//        std::cout << e << std::endl;
//    }
//     std::cout << "******************************************** BEFORE ERASING ********************" << std::endl;


    for (auto it = simplePolygon.vertices_begin(); it < simplePolygon.vertices_end(); it++) {
        if (it->x() == points[2].x() && it->y() == points[2].y()) {
            this->simplePolygon.erase(it);
        }
    }
//    for (auto e : simplePolygon.edges()) {
//        std::cout << e << std::endl;
//    }
//    std::cout << "******************************************** AFTER ERASING ********************" << std::endl;


    for (auto it = simplePolygon.vertices_begin(); it < simplePolygon.vertices_end(); it++) {
        if (it->x() == points[1].x() && it->y() == points[1].y()) {
            this->simplePolygon.insert(it, points[2]);
            break;
        }
    }
//    for (auto e : simplePolygon.edges()) {
//        std::cout << e << std::endl;
//    }
//
//    std::cout << "******************************************** AFTER INSERTING ********************" << std::endl;

//    if (!simplePolygon.is_simple()) {
//        std::cout<< "Broke simplicity after update" << std::endl;
//    }

    //simplePolygon.setOptimizedArea(simplePolygon.area());


}

double SimulatedAnnealing::calculateDiffArea(Points& points) {
//    long double dist1 = sqrt(CGAL::squared_distance(points[1], points[3]));
//    long double dist2 = sqrt(CGAL::squared_distance(points[1], points[2]));
//    long double dist3 = sqrt(CGAL::squared_distance(points[2], points[3]));
    double areaOfTriangle1 = (double)fabs(CGAL::area(points[1],points[2],points[3]));
    //double areaOfTriangleToBeAdded = areaOfTriangle(dist1, dist2, dist3);

//    long double dist4 = sqrt(CGAL::squared_distance(points[0], points[1]));
//    long double dist5 = sqrt(CGAL::squared_distance(points[1], points[2]));
//    long double dist6 = sqrt(CGAL::squared_distance(points[0], points[2]));

    double areaOfTriangle2 = (double)fabs(CGAL::area(points[0],points[1],points[2]));

    if (CGAL::orientation(points[1], points[2], points[3]) == CGAL::COUNTERCLOCKWISE) {
        return areaOfTriangle2 - areaOfTriangle1;
    }
    else {
        return areaOfTriangle1 - areaOfTriangle2;
    }
    //double areaOfTriangleToBeRemoved = areaOfTriangle(dist4, dist5, dist6);
    //return areaOfTriangleToBeAdded - areaOfTriangleToBeRemoved;

}

bool SimulatedAnnealing::maxMetropolisCriteriaSpatial(long double & diffArea, int index) {
    long double EnergyOld = n * (1 - (polygons[index].area()/polygons[index].getStartingConvexArea()));
    long double EnergyNew = n * (1 - ((polygons[index].area() + diffArea)/polygons[index].getStartingConvexArea()));
    long double DE = EnergyNew - EnergyOld;

    if (DE < 0 || exp(-DE/ this->T) >= R) {
        polygons[index].setOptimizedArea(polygons[index].area() + diffArea);
        return true;
    }
    return false;
}

bool SimulatedAnnealing::maxMetropolisCriteria(double & diffArea) {
    long double EnergyOld = n * (1 - (simplePolygon.area()/simplePolygon.getStartingConvexArea()));
    long double EnergyNew = n * (1 - ((simplePolygon.area() + diffArea)/simplePolygon.getStartingConvexArea()));
    long double DE = EnergyNew - EnergyOld;

    if (exp(-DE/ this->T) >= R) {
        simplePolygon.setOptimizedArea(simplePolygon.area() + diffArea);
        return true;
    }
    return false;
}
bool SimulatedAnnealing::minMetropolisCriteriaSpatial(long double & diffArea, int index) {
    long double EnergyOld = n * (polygons[index].area()/polygons[index].getStartingConvexArea());
    long double EnergyNew = n * ((polygons[index].area() + diffArea)/polygons[index].getStartingConvexArea());
    long double DE = EnergyNew - EnergyOld;

    if (DE < 0 || exp(-DE/ this->T) >= R) {
        polygons[index].setOptimizedArea(polygons[index].area() + diffArea);
        return true;
    }
    return false;
}
bool SimulatedAnnealing::minMetropolisCriteria(double & diffArea) {
    long double EnergyOld = n * (simplePolygon.getOptimizedArea()/simplePolygon.getStartingConvexArea());
    long double EnergyNew = n * ((simplePolygon.getOptimizedArea() + diffArea)/simplePolygon.getStartingConvexArea());
    long double DE = EnergyNew - EnergyOld;

    if (DE < 0 || exp(-DE/ getT()) >= getR()) {
        simplePolygon.setOptimizedArea(simplePolygon.getOptimizedArea() + diffArea);
        return true;
    }
    return false;
}
bool SimulatedAnnealing::globalStepValidSpatial(Points& points, int index) {
    Segment_2 predToSucc(points[0], points[2]);
    Segment_2 sourceToPick(points[3], points[1]);
    Segment_2 PickToTarget(points[1], points[4]);
    if (polygons[index].doIntersect(predToSucc,sourceToPick) || polygons[index].doIntersect(predToSucc,PickToTarget)) {
        //std::cout << "Invalid Step" << std::endl;
        return false;
    }
    for (auto edge : polygons[index].edges()) {

        if (polygons[index].doIntersect(edge, predToSucc) && edge.source() != predToSucc.target()) {
            return false;
        }
        if (edge.source() != sourceToPick.target() && polygons[index].doIntersect(edge, sourceToPick)) {
            return false;
        }

        if (edge.source() != PickToTarget.target() && polygons[index].doIntersect(edge, PickToTarget)) {
            return false;
        }

    }
    return true;


}
bool SimulatedAnnealing::globalStepValid(Points& points) {
    Segment_2 predToSucc(points[0], points[2]);
    Segment_2 sourceToPick(points[3], points[1]);
    Segment_2 PickToTarget(points[1], points[4]);
    if (simplePolygon.doIntersect(predToSucc,sourceToPick) || simplePolygon.doIntersect(predToSucc,PickToTarget)) {
        //std::cout << "Invalid Step" << std::endl;
        return false;
    }
    for (auto edge : simplePolygon.edges()) {

        if (simplePolygon.doIntersect(edge, predToSucc) && edge.source() != predToSucc.target()) {
            return false;
        }
        if (edge.source() != sourceToPick.target() && simplePolygon.doIntersect(edge, sourceToPick)) {
            return false;
        }

        if (edge.source() != PickToTarget.target() && simplePolygon.doIntersect(edge, PickToTarget)) {
            return false;
        }

    }
    return true;

}

bool SimulatedAnnealing::localStepValid(Points& points) {
    Segment_2 newSegment1(points[0],points[2]);
    Segment_2 newSegment2(points[1], points[3]);

    Segment_2 toRemove1(points[0], points[1]);
    Segment_2 toRemove2(points[2], points[3]);
    Segment_2 toStay(points[1], points[2]);

    /**
     * Check if the newly introduced segments intersect each other
     */

    if (CGAL::intersection(newSegment1, newSegment2)) {

        return false;
    }


    /**
     * Check if the newly introduced segments intersect with one of the
     * edge that start or ends from a point within the rectangle obtained
     * from the range query
     */
    Points inRectangle = rangeQuery(points);

    for (auto point : inRectangle) {
        for (auto edge:simplePolygon.edges()) {
            if (edge.source() == point || edge.target() == point) {
                if(edge == toRemove1 || edge == toRemove2 || edge == toStay) {
                    continue;
                }
                if (edge.source() == toRemove1.target() || edge.source() == toRemove2.target()) {
                    if (simplePolygon.doIntersect(edge,newSegment1)) {
                        return false;
                    }
                    continue;
                }
                if (simplePolygon.doIntersect(edge,newSegment1) || simplePolygon.doIntersect(edge, newSegment2)) {
//                    std::cout << "Invalid local step " << std::endl;
//                    for (auto p:points) {
//                        std::cout << p << std::endl;
//                    }
                    return false;
                }
            }
        }
    }

    return true;

}

Points SimulatedAnnealing::getRandomPointForLocalStep() {

    /**
     * TODO: refactor it and use simplePolygon.vertex(index)
     */
    bool found = false;
    Points pointsToReturn;
    while(!found) {
        int polygonSize = simplePolygon.size();
        Point_2 pick;
        Point_2 predPick;
        Point_2 succPick;
        Point_2 SuccSuccPick;
        int indexToPick = rand() % polygonSize;
        int count = 0;

        // handle special cases
        if (indexToPick == 0) {
//             pick = *(simplePolygon.vertices_begin());
//             predPick = *(simplePolygon.vertices().end() - 1);
//             succPick = *(simplePolygon.vertices_begin() + 1);
//             SuccSuccPick = *(simplePolygon.vertices_begin() + 2);
            pick = simplePolygon.vertex(0);
            predPick = simplePolygon.vertex(polygonSize - 1);
            succPick = simplePolygon.vertex(1);
            SuccSuccPick = simplePolygon.vertex(2);
        } else if (indexToPick == polygonSize - 1) {
//             pick = *(simplePolygon.vertices().end() - 1);
//             predPick = *(simplePolygon.vertices().end() - 2);
//             succPick = *(simplePolygon.vertices_begin());
//             SuccSuccPick = *(simplePolygon.vertices_begin() + 1);
            pick = simplePolygon.vertex(indexToPick);
            predPick = simplePolygon.vertex(indexToPick - 1);
            succPick = simplePolygon.vertex(0);
            SuccSuccPick = simplePolygon.vertex(1);

        } else if (indexToPick == polygonSize - 2) {
//             pick = *(simplePolygon.vertices_end() - 2);
//             predPick = *(simplePolygon.vertices_end() - 3);
//             succPick = *(simplePolygon.vertices_end() - 1);
//             SuccSuccPick = *(simplePolygon.vertices_begin());
            pick = simplePolygon.vertex(indexToPick);
            predPick = simplePolygon.vertex(indexToPick - 1);
            succPick = simplePolygon.vertex(indexToPick + 1);
            SuccSuccPick = simplePolygon.vertex(0);
        } else {
//             for (auto v:simplePolygon.vertices()) {
//                 if (count == indexToPick - 1) {
//                     predPick = v;
//                     count++;
//                 }
//                 else if (count == indexToPick) {
//                     pick = v;
//                     count++;
//                 }
//                 else if (count == indexToPick + 1) {
//                     succPick = v;
//                     count++;
//                 }
//                 else if (count == indexToPick + 2) {
//                     SuccSuccPick = v;
//                     break;
//                 }
//                 else {
//                     count++;
//                 }
            pick = simplePolygon.vertex(indexToPick);
            predPick = simplePolygon.vertex(indexToPick - 1);
            succPick = simplePolygon.vertex(indexToPick + 1);
            SuccSuccPick = simplePolygon.vertex((indexToPick + 2));
        }

        if (check_collinear(predPick.x(), predPick.y(), pick.x(), pick.y(), succPick.x(), succPick.y()) ||
            check_collinear(pick.x(), pick.y(), succPick.x(), succPick.y(), SuccSuccPick.x(), SuccSuccPick.y())
            || check_collinear(predPick.x(), predPick.y(), pick.x(), pick.y(), SuccSuccPick.x(), SuccSuccPick.y() ||
                                                                                                 check_collinear(
                                                                                                         predPick.x(),
                                                                                                         predPick.y(),
                                                                                                         succPick.x(),
                                                                                                         succPick.y(),
                                                                                                         SuccSuccPick.x(),
                                                                                                         SuccSuccPick.y()))) {
            continue;
        } else {
            found = true;
        }

        pointsToReturn.push_back(predPick);
        pointsToReturn.push_back(pick);
        pointsToReturn.push_back(succPick);
        pointsToReturn.push_back(SuccSuccPick);
    }


    return pointsToReturn;
}


Points SimulatedAnnealing::getRandomPointsForGlobalStepSpatial(int index) {
    Points pointsToReturn;
    Point_2 pick;
    Point_2 predPick;
    Point_2 succPick;
    Point_2 edgeSource;
    Point_2 edgeTarget;
    int size = polygons[index].size();

    int indexToPick;
    int indexToEdge;

    while (true) {
        indexToPick = rand() % size;
        indexToEdge = rand() % size;
        if (indexToEdge != indexToPick) {
            // handle special cases
            if (indexToPick == 0) {
                pick = polygons[index].vertex(0);
                predPick = polygons[index].vertex(size - 1);
                succPick = polygons[index].vertex(1);
            } else if (indexToPick == (size - 1)) {
                pick = polygons[index].vertex(size - 1);
                predPick = polygons[index].vertex(size - 2);
                succPick = polygons[index].vertex(0);
            } else {
                pick = polygons[index].vertex(indexToPick);
                predPick = polygons[index].vertex(indexToPick - 1);
                succPick = polygons[index].vertex(indexToPick + 1);
            }
            if (indexToEdge == (size - 1)) {
                edgeSource = polygons[index].vertex(size - 1);
                edgeTarget = polygons[index].vertex(0);
            } else {
                edgeSource = polygons[index].vertex(indexToEdge);
                edgeTarget = polygons[index].vertex(indexToEdge + 1);
            }

            if (predPick != edgeSource && predPick != edgeTarget && succPick != edgeSource && succPick != edgeTarget) {
                break;
            }
        }
    }

    pointsToReturn.push_back(predPick);
    pointsToReturn.push_back(pick);
    pointsToReturn.push_back(succPick);
    pointsToReturn.push_back(edgeSource);
    pointsToReturn.push_back(edgeTarget);

    return pointsToReturn;
}
Points SimulatedAnnealing::getRandomPointsForGlobalStep() {
    Points pointsToReturn;
    Point_2 pick;
    Point_2 predPick;
    Point_2 succPick;
    Point_2 edgeSource;
    Point_2 edgeTarget;
    int size = simplePolygon.size();

    int indexToPick;
    int indexToEdge;

    while (true) {
        indexToPick = rand() % size;
        indexToEdge = rand() % size;
        if (indexToEdge != indexToPick) {
            // handle special cases
            if (indexToPick == 0) {
                pick = simplePolygon.vertex(0);
                predPick = simplePolygon.vertex(size - 1);
                succPick = simplePolygon.vertex(1);
            } else if (indexToPick == size - 1) {
                pick = simplePolygon.vertex(indexToPick);
                predPick = simplePolygon.vertex(indexToPick - 1);
                succPick = simplePolygon.vertex(0);
            } else {
                pick = simplePolygon.vertex(indexToPick);
                predPick = simplePolygon.vertex(indexToPick - 1);
                succPick = simplePolygon.vertex(indexToPick + 1);
            }
            if (indexToEdge == size - 1) {
                edgeSource = simplePolygon.vertex(indexToEdge);
                edgeTarget = simplePolygon.vertex(0);
            } else {
                edgeSource = simplePolygon.vertex(indexToEdge);
                edgeTarget = simplePolygon.vertex(indexToEdge + 1);
            }

            if (predPick != edgeSource && predPick != edgeTarget && succPick != edgeSource && succPick != edgeTarget) {
                break;
            }
        }
    }


    pointsToReturn.push_back(predPick);
    pointsToReturn.push_back(pick);
    pointsToReturn.push_back(succPick);
    pointsToReturn.push_back(edgeSource);
    pointsToReturn.push_back(edgeTarget);

    return pointsToReturn;

}

long double SimulatedAnnealing::getT() const {
    return T;
}

void SimulatedAnnealing::setT(long double t) {
    T = t;
}

const SimpleClosedPolygonalChain &SimulatedAnnealing::getSimplePolygon() const {
    return simplePolygon;
}

void SimulatedAnnealing::setSimplePolygon(const SimpleClosedPolygonalChain &simplePolygon) {
    SimulatedAnnealing::simplePolygon = simplePolygon;
}

int SimulatedAnnealing::getL() const {
    return L;
}

void SimulatedAnnealing::setL(int l) {
    L = l;
}

long double SimulatedAnnealing::getR() const {
    return R;
}

void SimulatedAnnealing::setR(long double r) {
    R = r;
}

int SimulatedAnnealing::getN() const {
    return n;
}

void SimulatedAnnealing::setN(int n) {
    SimulatedAnnealing::n = n;
}