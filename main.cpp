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
#include <chrono>
using namespace std;

#include "utilities.h"
#include "movies.h"

bool parseLine(string &line, string &movieName, double &movieRating);
bool compareMovies(const pair<string, double>& a, const pair<string, double>& b);
bool hasPrefix(const string& str, const string& prefix);
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

        auto start = chrono::high_resolution_clock::now();



        for (const string& prefix : prefixes) {
                vector<pair<string, double>> match_movies;

                int n = movies.size();
                int left = 0;
                int right = n;
                int start = -1;


                while (left <= right) {
                    int mid = left + (right - left) / 2;
                    if (movies[mid].first >= prefix) {
                        start = mid;
                        right = mid - 1;
                    } else {
                        left = mid + 1;
                    }
                }

                if (start != -1) {
                    for (int i = start; i < n; ++i) {
                        if (hasPrefix(movies[i].first, prefix)) {
                            match_movies.push_back(movies[i]);
                        } else {
                            break;
                        }
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
        //      cout << endl;
        }

        auto end = chrono::high_resolution_clock::now();
        double time_ms = chrono::duration_cast<chrono::microseconds>(end - start).count() / 1000.0;

        cerr << "Prefix search took " << fixed << setprecision(3) << time_ms << " ms" << endl;

    //print for best movies only
        for (const auto& entry : bestMovies) {
                cout << "Best movie with prefix " << entry.first << " is: " << entry.second.first << " with rating " << fixed << setprecision(1) << entry.second.second << endl;
        }








    return 0;

}

/* Add your run time analysis for part 3 of the assignment here as commented block*/

/*  3a: Analyze time complexity (Worst Case)
 *
 * Sorting movies by name
 * O(nlog n)
 *
 * for m prefizes:
 * - binary search O(log n)
 * - get n matching movies O(n)
 * - sort movies with matching prefixes (k movies) = O(klog k)
 * so prefix search comes to be O(nlog n + mlog n +  mklog k)
 * in the worst case where every movie matches with every prefix too, then 
 * k becomes equal to n.
 *
 * worst case time complexity = O(nlog n + mlog n + mnlog n) => O(mnlog n)
 *
 * runtime:
 * 20 random: 65.59 ms
 * 100 random:   66.44 ms
 * 1000 random:  54.16 ms
 * 76920 random: 360.2886 ms
 *
 * 3b:
 * n movies in a vector, m prefixes in a vector, k matching movies per prefix
 * = O(n + m + k)
 *
 *
 * 3c
 * 1. I tried to design my algorithm for a lower space complexity.
 *    I think I was able to design a program with fairly low time complexity as well. 
 *     However, that was not the primary goal. I originally started with using a vector
 *     of pairs to essentially have a prefix key for movies. By having contiguous memory
 *     memory usage is kept low as everything is stored without the use of pointers.
 *     At first, due to the linear search my algorithm was quite slow, but with the 
 *     implementation of binary search through the sorted vector, I was able to essentially
 *     take on the time complexity of something similar to a tree structure while 
 *     maintaining the low space complexity of a vector. 
 *
 *
 */




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

bool hasPrefix(const string& str, const string& prefix) {
    return str.compare(0, prefix.size(), prefix) == 0;
}
