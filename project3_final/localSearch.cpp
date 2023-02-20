//
// Created by stefanos on 12/10/22.
//

#include "localSearch.h"

double long localSearch(Polygon_2 startingPoly, const string& AP) {                 //AP: Area polygonization max/min

    vector <Segment_2> segmentsPL;
    vector <Point_2> pointsPL;

    for(Point_2 vertex: startingPoly)
        pointsPL.push_back(vertex);

    for(int i = 0; i < pointsPL.size(); i++){
        if(i == pointsPL.size()-1)
            segmentsPL.push_back(Segment_2 (pointsPL[i], pointsPL[0]));
        else
            segmentsPL.push_back(Segment_2 (pointsPL[i], pointsPL[i+1]));
    }
    // Preprocess based on images
    int k = 4;                                          // Variable k in papers = assignment parameter L
    double threshold = 0.0001;
    if(pointsPL.size()>100) {
        k = 1;
        threshold = 0.001;
    }
    if(pointsPL.size()>200)
        threshold = 0.01;

    long double MAD;                                                            // MAD: Max Area Difference out of all possible changes in 1 iteration of the algorithm. If MAD < threshold, stop the algorithm.
    vector <int> path;
    vector <vector<int>> allPaths, segPaths;                            // Vectors of paths. allPaths: all possible paths, segPaths: paths that don't include points of segment/edge e
    Polygon_2 polygTemp, myPolyg;
    long double tempArea, AreaP, areaDif;
    vector <Point_2> tempPoints, chosenChange = pointsPL;
    bool simple;
    int firstPointIndex, en1, en2;

    if(pointsPL.size() - 3 < k)                         // If the number of points in the polygon - 3 is less than k, then set that value as k, instead of 10 ****EXPLANATION*****
        k = pointsPL.size() - 3;

    do {
        MAD = 0;
        for(auto pts: pointsPL)                                    // Create temp polygon from tempPoints
            myPolyg.push_back(pts);
        AreaP = myPolyg.area();
        //cout << "Starting Iteration with Area: " << (long int)AreaP << endl;

        for(int l = 1; l <= k; l++) {                           // Finding all paths with each length l <= k
            for (int j = 0; j < pointsPL.size(); j++) {         // Parse through the points
                for (int m = 0; m < l; m++) {                   // For each point, get a path of length l
                    if(j+m >= pointsPL.size()){
                        path.clear();
                        break;
                    }
                    path.push_back(j+m);
                }
                if(!path.empty()) {
                    allPaths.push_back(path);                       // Add to vector all paths
                    path.clear();
                }
            }
        }

        for (int i = 0; i < segmentsPL.size(); i++) {                       // For each edge e
            segPaths.clear();
            for (auto j: allPaths){
                if(!(find(j.begin(), j.end(), i) != j.end() || find(j.begin(), j.end(), (i+1) % pointsPL.size()) != j.end() )) {
                    segPaths.push_back(j);                                  // Get the paths that don't include the segment's source/target
                }
            }

            for (auto p: segPaths){                                             // For each path, move path V to edge e

                tempPoints = pointsPL;
                firstPointIndex = p[0];
                move_V_to_e(firstPointIndex, p.size(), i+1, tempPoints);        // Move path to edge

                if(p[0] == 0)
                    en1 = tempPoints.size()-1;
                else
                    en1 = p[0] - 1;
                if(p[0] + p.size() > tempPoints.size()-1)
                    en2 = 0;
                else
                    en2 = p[0] + p.size();
                Segment_2 paths_ends(pointsPL[en1], pointsPL[en2]);
                Segment_2 newSegment1(pointsPL[i],pointsPL[p[0]]);
                Segment_2 newSegment2(pointsPL[(i+1)%pointsPL.size()], pointsPL[p[0]+p.size()-1]);
                const auto result1 = intersection(paths_ends, newSegment1);
                const auto result2 = intersection(paths_ends, newSegment2);
                if (result1 || result2 || segIntPol(Segment_2(pointsPL[i], pointsPL[p[0]]), segmentsPL) || segIntPol(Segment_2(pointsPL[(i+1)%pointsPL.size()], pointsPL[p[0]+p.size()-1]), segmentsPL))
                    simple = 0;
                else {
                    for(auto tp: tempPoints)                                    //Create temp polygon from tempPoints
                        polygTemp.push_back(tp);
                    simple = polygTemp.is_simple();
                }

                if (simple){                                             // If new polygon is simple
                    tempArea = polygTemp.area();
                    if(AP == "-max") {
                        if (tempArea > AreaP) {                             // And area of new polygon > area of my polygon
                            areaDif = tempArea - AreaP;
                            if (areaDif > MAD) {                     // Find and save the V,e change that creates a polygon of MAX area
                                MAD = areaDif;
                                chosenChange = tempPoints;
                            }
                        }
                    }
                    else if(AP == "-min"){
                        if (tempArea < AreaP) {                             // And area of new polygon < area of my polygon
                            areaDif = AreaP - tempArea;
                            if (areaDif > MAD) {                     // Find and save the V,e change that creates a polygon of MAX area
                                MAD = areaDif;
                                chosenChange = tempPoints;
                            }
                        }
                    }
                }
                polygTemp.clear();
            }
        }
        pointsPL = chosenChange;

        segmentsPL.clear();
        for(int i = 0; i < pointsPL.size(); i++){
            if(i == pointsPL.size()-1)
                segmentsPL.push_back(Segment_2 (pointsPL[i], pointsPL[0]));
            else
                segmentsPL.push_back(Segment_2 (pointsPL[i], pointsPL[i+1]));
        }
        myPolyg.clear();
        allPaths.clear();

        //cout << "MAD: " << MAD << " Threshold: " << (long double)threshold*AreaP << endl;
    } while (MAD > threshold*AreaP);

    Polygon_2 p, ch;
    for (auto i: pointsPL)
        p.push_back(i);

    vector <Point_2> pointsCH;
    CGAL::convex_hull_2(startingPoly.begin(), startingPoly.end(), back_inserter(pointsCH));
    for (auto i: pointsCH)
        ch.push_back(i);

    long double Area = p.area();
    long double ratio = Area/ch.area();
    return ratio;
}


void move_V_to_e(size_t start, size_t length, size_t dst, std::vector<Point_2> & v) {
    vector<Point_2>::iterator first, middle, last;
    if (start < dst)
    {
        first  = v.begin() + start;
        middle = first + length;
        last   = v.begin() + dst;
    }
    else
    {
        first  = v.begin() + dst;
        middle = v.begin() + start;
        last   = middle + length;
    }
    std::rotate(first, middle, last);
}


bool segIntPol(Segment_2 seg, vector<Segment_2> segmentsPL) {

    bool sip = 0;
    for (auto i: segmentsPL) {

        const auto result1 = intersection(i, seg);
        if (result1) {                                                                  // If segment intersects the polygon
            if (const Segment_2 *s = boost::get<Segment_2>(&*result1)) {
                sip = 1;
                break;
            } else {
                const Point_2 *pts = boost::get<Point_2>(&*result1);
                if ((pts[0] != seg.source()) && (pts[0] != seg.target())){
                    sip = 1;
                    break;
                }
            }
        }

    }
    return sip;
}