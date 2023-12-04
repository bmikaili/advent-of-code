#include <fstream>
#include <iostream>
#include <set>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath> // Add missing import for std::pow

std::set<int> split_into_digits(const std::string &str, char delim)
{
    std::set<int> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delim))
    {
        // Strip whitespace and after filter out empty strings
        token.erase(std::remove_if(token.begin(), token.end(), isspace), token.end());
        if (!token.empty())
        {
            tokens.insert(std::stoi(token));
        }
    }
    return tokens;
}

int main()
{
    std::ifstream myfile("input.txt");
    if (!myfile)
    {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }

    int points = 0;

    /**
     * In each line
     * 1. Get rid of the Card N: part and the space after it
     * Then split by | to get my numbers and the winning numbers
     * Then split by space to get each number put in 2 sets
     * Then compare the 2 sets to see how many numbers match
     * Add 2 to the power of the number of matches to the points
     */
    std::string line;

    while (std::getline(myfile, line))
    {
        // Get rid of the Card N: part and the space after it do not use string view
        auto colon_pos = line.find(':');
        if (colon_pos == std::string::npos)
        {
            std::cerr << "Unable to find colon" << std::endl;
        }

        line.erase(0, colon_pos + 2);

        // Split by | to get my numbers and the winning numbers
        auto pipe_pos = line.find('|');
        if (pipe_pos == std::string::npos)
        {
            std::cerr << "Unable to find pipe" << std::endl;
        }

        std::string my_numbers = line.substr(0, pipe_pos);
        std::string winning_numbers = line.substr(pipe_pos + 1);

        // Split by space to get each number put in 2 sets
        std::set<int> my_numbers_set = split_into_digits(my_numbers, ' ');
        std::set<int> winning_numbers_set = split_into_digits(winning_numbers, ' ');

        std::vector<int> intersection;
        std::set_intersection(my_numbers_set.begin(), my_numbers_set.end(),
                              winning_numbers_set.begin(), winning_numbers_set.end(),
                              std::back_inserter(intersection));

        if (intersection.size() > 0)
            points += std::pow(2, intersection.size() - 1);
    }

    std::cout << "Result: " << points << std::endl;

    return 0;
}
