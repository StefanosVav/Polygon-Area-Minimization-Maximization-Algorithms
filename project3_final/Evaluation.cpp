//
// Created by nikos on 1/13/23.
//

#include "Evaluation.h"

vector<long double> evaluateLocalForMinimization(std::string inputFile) {
    long double minScore;
    int number_of_lines = 0;
    std::string line;
    std::ifstream myfile(inputFile);

    while (std::getline(myfile, line))
        ++number_of_lines;

    int cutOff = 500*(number_of_lines - 2);

    auto start = chrono::steady_clock::now();
    // Incremental + Local Search
    Polygon_2 pol = Incremental(inputFile,"2");                           // Edge selection 2 for Min Polygonization
    minScore = localSearch(pol, "-min");

    auto stop = chrono::steady_clock::now();
    auto diff = stop - start;
    auto diffInMilli = chrono::duration<double, milli>(diff).count();
    //std::cout << "Incremental + Local Search -Minimization score: " << minScore << " in time: " << diffInMilli << " ms" << std::endl;

    double diffInMilli1;
    long double minScore1;
    if(number_of_lines>500)
        minScore1 = 1;
    else {
        start = chrono::steady_clock::now();
        // Convex Hull + Local Search
        Polygon_2 pol1 = getPolygonFromConvexHull(inputFile,"2");                           // Edge selection 2 for Min Polygonization
        minScore1 = localSearch(pol1, "-min");

        stop = chrono::steady_clock::now();
        diff = stop - start;
        diffInMilli1 = chrono::duration<double, milli>(diff).count();
        //std::cout << "Convex Hull + Local Search -Minimization score: " << minScore1 << " in time: " << diffInMilli1 << " ms" << std::endl;
    }

    return { minScore, minScore1 };
}

vector<long double> evaluateLocalForMaximization(std::string inputFile) {
    long double maxScore;
    int number_of_lines = 0;
    std::string line;
    std::ifstream myfile(inputFile);

    while (std::getline(myfile, line))
        ++number_of_lines;
    int cutOff = 500*(number_of_lines - 2);

    auto start = chrono::steady_clock::now();
    // Incremental + Local Search
    Polygon_2 pol = Incremental(inputFile,"3");                           // Edge selection 2 for Min Polygonization
    maxScore = localSearch(pol, "-max");

    auto stop = chrono::steady_clock::now();
    auto diff = stop - start;
    auto diffInMilli = chrono::duration<double, milli>(diff).count();
    //std::cout << "Incremental + Local Search -Maximization score: " << maxScore << " in time: " << diffInMilli << " ms" << std::endl;

    start = chrono::steady_clock::now();
    // Convex Hull + Local Search
    Polygon_2 pol1 = getPolygonFromConvexHull(inputFile,"3");
    long double maxScore1 = localSearch(pol1, "-max");

    stop = chrono::steady_clock::now();
    diff = stop - start;
    auto diffInMilli1 = chrono::duration<double, milli>(diff).count();
    //std::cout << "Convex Hull + Local Search -Maximization score: " << maxScore1 << " in time: " << diffInMilli1 << " ms" << std::endl;

    return { maxScore, maxScore1 };
}

vector<long double> evaluateSimulatedForMinimization(std::string inputFile) {
    long double minScore, minScore1, minScore2, minScore3;
    vector <long double> scores;
    int number_of_lines = 0;
    std::string line;
    std::ifstream myfile(inputFile);

    while (std::getline(myfile, line))
        ++number_of_lines;
    int cutOff = 500*(number_of_lines - 2);


    auto start = chrono::steady_clock::now();
    // Convex Hull + Sim annealing local
    minScore = SimulatedAnnealing(inputFile, "convex_hull", "2", 1000, "-min", "local").solve();

    auto stop = chrono::steady_clock::now();
    auto diff = stop - start;
    auto diffInMilli = chrono::duration<double, milli>(diff).count();
    //std::cout << "Convex Hull + Simulated annealing local -Minimization score: " << minScore << " in time: " << diffInMilli << " ms" << std::endl;
    if(diffInMilli > cutOff)
        minScore = 1;

    start = chrono::steady_clock::now();
    // Convex Hull + Sim annealing global
    minScore1 = SimulatedAnnealing(inputFile, "convex_hull", "2", 1000, "-min", "global").solve();

    stop = chrono::steady_clock::now();
    diff = stop - start;
    auto diffInMilli1 = chrono::duration<double, milli>(diff).count();
    //std::cout << "Convex Hull + Simulated annealing global -Minimization score: " << minScore1 << " in time: " << diffInMilli1 << " ms" << std::endl;
    if(diffInMilli1 > cutOff)
        minScore1 = 1;

    start = chrono::steady_clock::now();

    // SIM ANNEALING WITH INCREMENTAL -- local annealing
    minScore2 = SimulatedAnnealing(inputFile, "incremental", "2", 1000, "-min", "local").solve();

    stop = chrono::steady_clock::now();
    diff = stop - start;
    auto diffInMilli2 = chrono::duration<double, milli>(diff).count();
    //std::cout << "Incremental + Simulated annealing local -Minimization score: " << minScore2 << " in time: " << diffInMilli2 << " ms" << std::endl;
    if(diffInMilli2 > cutOff)
        minScore2 = 1;

    start = chrono::steady_clock::now();

    // SIM ANNEALING WITH INCREMENTAL -- global annealing
    minScore3 = SimulatedAnnealing(inputFile, "incremental", "2", 1000, "-min", "global").solve();

    stop = chrono::steady_clock::now();
    diff = stop - start;
    auto diffInMilli3 = chrono::duration<double, milli>(diff).count();
    //std::cout << "Incremental + Simulated annealing global -Minimization score: " << minScore3 << " in time: " << diffInMilli3 << " ms" << std::endl;
    if(diffInMilli3 > cutOff)
        minScore3 = 1;

    /*sort(scores.begin(), scores.end());             // Sorting scores ascending, so that the min score is at scores[0]

    if(!scores.empty())
        return scores[0];
    else
        return 1;*/

    return {minScore, minScore1, minScore2, minScore3};

}

vector<long double> evaluateSimulatedForMaximization(std::string inputFile) {
    long double maxScore, maxScore1, maxScore2, maxScore3;
    vector <long double> scores;
    int number_of_lines = 0;
    std::string line;
    std::ifstream myfile(inputFile);

    while (std::getline(myfile, line))
        ++number_of_lines;
    int cutOff = 500*(number_of_lines - 2);

    auto start = chrono::steady_clock::now();
    // Convex Hull + Sim annealing local
    maxScore = SimulatedAnnealing(inputFile, "convex_hull", "3", 1000, "-max", "local").solve();

    auto stop = chrono::steady_clock::now();
    auto diff = stop - start;
    auto diffInMilli = chrono::duration<double, milli>(diff).count();
    //std::cout << "Convex Hull + Simulated annealing local -Maximization score: " << maxScore << " in time: " << diffInMilli << " ms" << std::endl;
    if(diffInMilli > cutOff)
        maxScore = 0;

    start = chrono::steady_clock::now();
    // Convex Hull + Sim annealing global
    maxScore1 = SimulatedAnnealing(inputFile, "convex_hull", "3", 1000, "-max", "global").solve();

    stop = chrono::steady_clock::now();
    diff = stop - start;
    auto diffInMilli1 = chrono::duration<double, milli>(diff).count();
    //std::cout << "Convex Hull + Simulated annealing global -Maximization score: " << maxScore1 << " in time: " << diffInMilli1 << " ms" << std::endl;
    if(diffInMilli1 > cutOff)
        maxScore1 = 0;

    start = chrono::steady_clock::now();

    // SIM ANNEALING WITH INCREMENTAL -- local annealing
    maxScore2 = SimulatedAnnealing(inputFile, "incremental", "3", 1000, "-max", "local").solve();

    stop = chrono::steady_clock::now();
    diff = stop - start;
    auto diffInMilli2 = chrono::duration<double, milli>(diff).count();
    //std::cout << "Incremental + Simulated annealing local -Maximization score: " << maxScore2 << " in time: " << diffInMilli2 << " ms" << std::endl;
    if(diffInMilli2 > cutOff)
        maxScore2 = 0;

    start = chrono::steady_clock::now();

    // SIM ANNEALING WITH INCREMENTAL -- global annealing
    maxScore3 = SimulatedAnnealing(inputFile, "incremental", "3", 1000, "-max", "global").solve();

    stop = chrono::steady_clock::now();
    diff = stop - start;
    auto diffInMilli3 = chrono::duration<double, milli>(diff).count();
    //std::cout << "Incremental + Simulated annealing global -Maximization score: " << maxScore3 << " in time: " << diffInMilli3 << " ms" << std::endl;
    if(diffInMilli3 > cutOff)
        maxScore3 = 0;

    /*sort(scores.begin(), scores.end(), greater <>());       // Sorting scores descending, so that the max score is at scores[0]

    if(!scores.empty())
        return scores[0];
    else
        return 1;*/
    return {maxScore, maxScore1, maxScore2, maxScore3};


}

Polygon_2 getPolygonFromConvexHull(std::string inputFile, std::string edgeSelection) {

    std::ifstream input(inputFile);
    std::string lineData;

    Points inputPoints;

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
    return ConvexHullToSimplePolygon(inputPoints, edgeSelection).ConstructSimplePolygonalChain();

}
/*
void getPrintScores(vector<long double> individualScoresMin, vector<long double> individualScoresMax, vector<long double>simIndividualScoresMin, vector<long double> simIndividualScoresMax, string output){
    long double minBound = individualScoresMin[0], minScore = 0;
    for (auto score: individualScoresMin) {
        if (score > minBound)
            minBound = score;
        minScore += score;
    }

    long double maxBound = individualScoresMax[0], maxScore = 0;
    for (auto score: individualScoresMax) {
        if (score < maxBound)
            maxBound = score;
        maxScore += score;
    }

    long double simMinBound = simIndividualScoresMin[0], simMinScore = 0;
    for (auto score: simIndividualScoresMin) {
        if (score > simMinBound)
            simMinBound = score;
        simMinScore += score;
    }

    long double simMaxBound = simIndividualScoresMax[0], simMaxScore = 0;
    for (auto score: simIndividualScoresMax) {
        if (score < simMaxBound)
            simMaxBound = score;
        simMaxScore += score;
    }

    ofstream myfile;
    myfile.open (output);
    myfile << std::setprecision(6) << std::fixed;
    myfile << "||                Local Search                   ||               Simulated Annealing             ||" << endl;
    myfile << "|| min score | max score | min bound | max bound || min score | max score | min bound | max bound ||" << endl;
    myfile << "|| " << minScore << "  | " << maxScore << "  | " << minBound << "  | " << maxBound  << "  || "  << simMinScore << "  | " << simMaxScore << "  | " << simMinBound << "  | " << simMaxBound  << "  ||" <<  endl;
    myfile.close();
}*/