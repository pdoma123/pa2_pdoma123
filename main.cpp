// Winter'24
// Instructor: Diba Mirza
// Student name: Prabhav Doma
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cstring>
#include <algorithm>
#include <limits.h>
#include <iomanip>
#include <set>
#include <queue>
#include <sstream>
using namespace std;

#include "utilities.h"
#include "movies.h"

bool parseLine(string &line, string &movieName, double &movieRating);
bool compareMovies(const pair<string, double>& a, const pair<string, double>& b);

int main(int argc, char** argv){
    if (argc < 2){
        cerr << "Not enough arguments provided (need at least 1 argument)." << endl;
        cerr << "Usage: " << argv[ 0 ] << " moviesFilename prefixFilename " << endl;
        exit(1);
    }
    ifstream movieFile (argv[1]);

    if (movieFile.fail()){
        cerr << "Could not open file " << argv[1];
        exit(1);
    }

    vector <pair<string, double>> movies;

    string line, movieName;
    double movieRating;
    // Read each file and store the name and rating
    while (getline (movieFile, line) && parseLine(line, movieName, movieRating)){
            // Use std::string movieName and double movieRating
            // to construct your Movie objects
            // cout << movieName << " has rating " << movieRating << endl;
            // insert elements into your data structure

            movies.push_back({movieName, movieRating});

    }

    movieFile.close();

    sort(movies.begin(), movies.end());
    if (argc == 2){
            //print all the movies in ascending alphabetical order of movie names

            for (size_t i = 0; i < movies.size(); ++i) {
                    cout << movies[i].first << ", " << movies[i].second << endl;
            }
            return 0;

    }

    ifstream prefixFile (argv[2]);

    if (prefixFile.fail()) {
        cerr << "Could not open file " << argv[2];
        exit(1);
    }

    vector<string> prefixes;
    while (getline (prefixFile, line)) {
        if (!line.empty()) {
            prefixes.push_back(line);
        }
    }

    prefixFile.close();

    //  For each prefix,
    //  Find all movies that have that prefix and store them in an appropriate data structure
    //  If no movie with that prefix exists print the following message
    // cout << "No movies found with prefix "<<"<replace with prefix>" << endl;

    //  For each prefix,
    //  Print the highest rated movie with that prefix if it exists.
    //cout << "Best movie with prefix " << "<replace with prefix>" << " is: " << "replace with movie name" << " with rating " << std::fixed << std::setprecision(1) << "replace with movie rating" << endl;


        vector<pair<string, pair<string, double>>> bestMovies; // prefix, (movie_name, rating)

        for (const string& prefix : prefixes) {
                vector<pair<string, double>> match_movies;

                for (const auto& movie : movies) {
                        if (movie.first.compare(0, prefix.size(), prefix) == 0) {
                                match_movies.push_back(movie);
                        }
                        else if (movie.first > prefix) {
                                break;
                        }
                }

                if (match_movies.empty()) {
                        cout << "No movies found with prefix " << prefix << endl;
                }
                else {
                        sort(match_movies.begin(), match_movies.end(), compareMovies);

                        for (const auto& movie : match_movies) {
                                cout << movie.first << ", " << movie.second << endl;
                        }

                        //best movies per prefix
                        bestMovies.push_back({prefix, {match_movies[0].first, match_movies[0].second}});
                        cout << endl;
                }
                
        }

    //print for best movies only
        for (const auto& entry : bestMovies) {
                cout << "Best movie with prefix " << entry.first << " is: " << entry.second.first << " with rating " << fixed << setprecision(1) << entry.second.second << endl;
        }








    return 0;

}

/* Add your run time analysis for part 3 of the assignment here as commented block*/

bool parseLine(string &line, string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex+1));
    if (movieName[0] == '\"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}

bool compareMovies(const pair<string, double>& a, const pair<string, double>& b) {
        if (a.second != b.second) {
                return a.second > b.second;
        }
        return a.first < b.first;
}
