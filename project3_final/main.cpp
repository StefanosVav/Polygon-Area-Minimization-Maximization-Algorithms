//
// Created by nikos on 10/31/22.
//
#include "utils.h"
#include "incremental.h"
#include "localSearch.h"
#include "ConvexHullToSimplePolygon.h"
#include "SimulatedAnnealing.h"
#include "Evaluation.h"
int main(int argc, char* argv[]) {

    /**
     * Utils Assignment 3
     * Parameters[0]:point set path
     * Parameters[1]:output file
     * Parameters[2]:preprocessing setting
     */
    std::vector<std::string> parameters = validateInput(argc,argv);
    std::vector<std::string> inputFiles;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (parameters[0].c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            if (!(!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) )
                inputFiles.push_back(parameters[0]+"/"+ent->d_name);
        }
        closedir (dir);
    } else {
        perror ("");
        return EXIT_FAILURE;
    }
    cout << "In directory " << parameters[0] << " found " << inputFiles.size() << " files:" << endl;
    for(auto i: inputFiles)
        cout << i << endl;
    cout << endl;

    ofstream myfile;
    myfile.open (parameters[1]);
    myfile << std::setprecision(6) << std::fixed;
    myfile << "      ||                Local Search                   ||               Simulated Annealing             ||" << endl;
    myfile << "size  || min score | max score | min bound | max bound || min score | max score | min bound | max bound ||" << endl;

    int number_of_lines;
    string line;
    vector<long double> ret;
    vector <long double> individualScoresMax, individualScoresMin, simIndividualScoresMax, simIndividualScoresMin;
    for(auto i: inputFiles) {
        cout <<  "Running algorithm combinations for file: " << i << "..." << endl;
        std::ifstream infile(i);
        number_of_lines = 0;
        while (std::getline(infile, line))
            ++number_of_lines;
        number_of_lines = number_of_lines - 2;

        individualScoresMin = evaluateLocalForMinimization(i);
        simIndividualScoresMin = evaluateSimulatedForMinimization(i);
        individualScoresMax = evaluateLocalForMaximization(i);
        simIndividualScoresMax = evaluateSimulatedForMaximization(i);

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
        if(number_of_lines < 100)
            myfile << number_of_lines << "    || " << minScore << "  | " << maxScore << "  | " << minBound << "  | " << maxBound  << "  || "  << simMinScore << "  | " << simMaxScore << "  | " << simMinBound << "  | " << simMaxBound  << "  ||" <<  endl;
        else if( number_of_lines < 1000)
            myfile << number_of_lines << "   || " << minScore << "  | " << maxScore << "  | " << minBound << "  | " << maxBound  << "  || "  << simMinScore << "  | " << simMaxScore << "  | " << simMinBound << "  | " << simMaxBound  << "  ||" <<  endl;
        else
            myfile << number_of_lines << "  || " << minScore << "  | " << maxScore << "  | " << minBound << "  | " << maxBound  << "  || "  << simMinScore << "  | " << simMaxScore << "  | " << simMinBound << "  | " << simMaxBound  << "  ||" <<  endl;
    }

    myfile.close();
    std::cout << "Finished! Results in " << parameters[1] << std::endl;

}