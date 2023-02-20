//
// Created by stefanos on 12/10/22.
//

#include "incremental.h"

Polygon_2 Incremental(string inputFile, string edge_selection) {
    std::ifstream input(inputFile);
    std::string lineData;
    std::vector<Point_2> inputPoints;

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
    Polygon_2 pol = executeIncremental(inputPoints, edge_selection);
    return pol;
}

Polygon_2 executeIncremental(vector<Point_2> p, string edge_selection) {

    if(edge_selection != "1" && edge_selection != "2" && edge_selection != "3"){
        cout << "Invalid edge selection, going with default edge selection no1: Random" << endl;
        edge_selection = "1";
    }

    vector <Point_2> allPoints = p;
    sort(allPoints.begin(), allPoints.end(), points_by_x_asc);       // Sort points in vector by x ascending

    vector <Point_2> pointsPL;
    vector <Segment_2> segmentsPL;
    vector <Point_2> pointsUn;

    Polygon_2 tempPol;
    for(int i=0; i<3; i++){                             // Pushing the first 3 points (and segments) to my polygonal line
        tempPol.push_back(allPoints[i]);
        pointsPL.push_back(allPoints[i]);
    }
    if (tempPol.orientation() == CGAL::CLOCKWISE)                                 // Checking if they've been added in CounterClockWise orientation
        reverse(pointsPL.begin(), pointsPL.end());                                  // If not, then reverse the vector so that they are now in CCW

    segmentsPL.push_back(Segment_2(pointsPL[0], pointsPL[1]));
    segmentsPL.push_back(Segment_2(pointsPL[1], pointsPL[2]));
    segmentsPL.push_back(Segment_2(pointsPL[2], pointsPL[0]));

    vector<Point_2> pointsCH, pointsCH_new, pointsPL_plusnew;
    vector<Segment_2> segmentsCH, segmentsCH_new, redEdges, visibleEdges;
    for(int k = 3; k < allPoints.size(); k++){
        // Create current convex hull and new convex hull after adding new point
        pointsPL_plusnew = pointsPL;
        pointsPL_plusnew.push_back(allPoints[k]);
        CGAL::convex_hull_2(pointsPL.begin(), pointsPL.end(), back_inserter(pointsCH));
        CGAL::convex_hull_2(pointsPL_plusnew.begin(), pointsPL_plusnew.end(), std::back_inserter(pointsCH_new));

        for(int i = 0; i < pointsCH.size(); i++){                   // Get convex hull segments from points
            if(i == pointsCH.size() - 1)
                segmentsCH.push_back(Segment_2 (pointsCH[i], pointsCH[0]));
            else
                segmentsCH.push_back(Segment_2 (pointsCH[i], pointsCH[i+1]));
        }

        for(int i = 0; i < pointsCH_new.size(); i++){
            if(i == pointsCH_new.size() - 1)
                segmentsCH_new.push_back(Segment_2 (pointsCH_new[i], pointsCH_new[0]));
            else
                segmentsCH_new.push_back(Segment_2 (pointsCH_new[i], pointsCH_new[i+1]));
        }

        // Lemma: Edges that are in CURRENT convex hull, but NOT in the convex hull after adding new point, are the red edges
        for(auto i: segmentsCH){
            if (!(find(segmentsCH_new.begin(), segmentsCH_new.end(), i) != segmentsCH_new.end()))
                redEdges.push_back(i);
        }

        int fs = 0;             //found source flag
        for(auto i: redEdges) {
            for(auto j: segmentsPL){
                if(i == j) {                                    //If the red edge is an edge of my polygonal line
                    if(isVisible(j, allPoints[k], segmentsPL))      // Checking because there is a case where a red edge is an edge of my polygon, but it's not visible from new point (linear exception)
                        visibleEdges.push_back(j);
                    break;
                }
                else if(i.source() == j.source()){              // Find the edge of my polygonal line whose source is the same as the red edge's
                    fs = 1;
                    if(isVisible(j, allPoints[k], segmentsPL))
                        visibleEdges.push_back(j);
                }
                else if(fs==1){                                 // If we've already found the edge of polygline that shares the same source with the red edge
                    if(isVisible(j, allPoints[k], segmentsPL))
                        visibleEdges.push_back(j);
                    if(j.target() == i.target()){               // Parse through the edges of polygline until you find an edge with the same target as the red edge
                        fs = 0;
                        break;
                    }
                }
            }

        }

        if( visibleEdges.size() == 0){
            pointsUn.push_back(allPoints[k]);
            pointsCH.clear();                   // Clear the vectors
            pointsCH_new.clear();
            segmentsCH.clear();
            segmentsCH_new.clear();
            redEdges.clear();
            continue;
        }

        // Select edge based on edge_selection
        Segment_2 chosenEdge;
        vector <Point_2> pointsTemp;
        Polygon_2 temp;
        if(edge_selection == "1"){                      // For edge selection 1: random selection
            int random = rand() % visibleEdges.size();      // Get random_integer MOD number_of_visible_edges
            chosenEdge = visibleEdges[random];
        }
        else if(edge_selection == "2"){                                 // For edge selection 2: minimum area added
            long int minArea = numeric_limits<long int>::max();                   //set min area as infinite
            for (auto j : visibleEdges) {                           // Parse through the visible edges
                pointsTemp = pointsPL;
                for (int i = 0; i < segmentsPL.size(); i++) {
                    if (segmentsPL[i] == j){
                        pointsTemp.insert(pointsTemp.begin() + i + 1, allPoints[k]);        // For each visible edge, add the point between the edge's ends
                        for(auto p: pointsTemp)
                            temp.push_back(p);
                        long int tmpArea = temp.area();
                        if(tmpArea < minArea) {                             // If the area of the newly created polygon is smaller than what we have so far
                            chosenEdge = j;                                     // Choose current edge
                            minArea = tmpArea;                              // Change min area to this one
                        }
                        temp.clear();
                        break;
                    }
                }
            }
        }
        else if(edge_selection == "3"){                             // For edge selection 3: maximum area added
            long int maxArea = 0;                                        // Set max area as 0
            for (auto j : visibleEdges) {                   // Parse through the visible edges
                pointsTemp = pointsPL;
                for (int i = 0; i < segmentsPL.size(); i++) {
                    if (segmentsPL[i] == j){
                        pointsTemp.insert(pointsTemp.begin() + i + 1, allPoints[k]);        // For each visible edge, add the point between the edge's ends
                        for(auto p: pointsTemp)
                            temp.push_back(p);
                        long int tmpArea = temp.area();
                        if(tmpArea > maxArea) {                         // If the area of the newly created polygon is larger than what we have so far
                            chosenEdge = j;                                 // Choose current edge
                            maxArea = tmpArea;                          // Change max area to this one
                        }
                        temp.clear();
                        break;
                    }
                }
            }
        }


        for (int i  = 0; i < segmentsPL.size(); i++){                   // Parse through the segments vector, remove the chosen Edge and add the 2 new ones, connecting with the new point
            if (chosenEdge == segmentsPL[i]){
                segmentsPL[i] = Segment_2 (chosenEdge.source(), allPoints[k]);
                segmentsPL.insert(segmentsPL.begin() + i + 1, Segment_2(allPoints[k], chosenEdge.target()));
                pointsPL.insert(pointsPL.begin() + i + 1, allPoints[k]);                // And add the new point to the points vector
                break;
            }
        }

        pointsCH.clear();                   // Clear the vectors
        pointsCH_new.clear();
        segmentsCH.clear();
        segmentsCH_new.clear();
        redEdges.clear();
        visibleEdges.clear();

    }
    if(pointsUn.size() > 0)
        cout << "-" << pointsUn.size() << endl;

    Polygon_2 pol;
    for (auto i: pointsPL) {
        pol.push_back(i);
    }
    return pol;
}


bool isVisible(Segment_2 seg, Point_2 point, vector<Segment_2> segmentsPL) {

    int isVis = 1;
    Segment_2 s1(point, seg.source()), s2(point, seg.target());     // Get the 2 segments connecting the point to the edge's ends
    for (auto i: segmentsPL) {                                      // Check if they intersect the polygon
        if(i == seg)
            continue;

        const auto result1 = intersection(i, s1);
        if (result1) {                                                                  // If they intersect the polygon
            if (const Segment_2 *s = boost::get<Segment_2>(&*result1)) {
                isVis = 0;
                break;
            } else {
                const Point_2 *pts = boost::get<Point_2>(&*result1);
                if ((pts[0] != seg.source()) && (pts[0] != seg.target())) {               // If the point of intersection is one of the edge's ends, do nothing
                    isVis = 0;
                    break;
                }
            }
        }

        const auto result2 = intersection(i, s2);
        if (result2) {
            if (const Segment_2* s = boost::get<Segment_2>(&*result2)) {
                isVis = 0;
                break;
            } else {
                const Point_2 *pts = boost::get<Point_2>(&*result2);
                if( (pts[0] != seg.source()) && (pts[0] != seg.target()) ) {
                    isVis = 0;
                    break;
                }
            }
        }
    }

    return isVis;
}
