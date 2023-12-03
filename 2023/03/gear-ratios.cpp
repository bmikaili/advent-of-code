#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <queue>

int main()
{
    std::ifstream myfile("input.txt");
    if (!myfile)
    {
        throw std::runtime_error("Unable to open file");
    }

    int sum = 0;
    std::string line;
    std::vector<std::vector<char>> gears;

    std::queue<std::pair<int, int>> q;

    // Create a 2d matrix of gears

    int row = 0;
    while (std::getline(myfile, line))
    {
        int col = 0;
        std::istringstream iss(line);
        std::vector<char> gear;
        char gear_type;
        while (iss >> gear_type)
        {
            gear.push_back(gear_type);
            if (gear_type != '.' && !std::isdigit(gear_type))
            {
                // push pair of row and col
                q.push({row, col});
            }
            col++;
        }
        gears.push_back(gear);
        row++;
    }

    /**
     * do a bfs starting from all the symbols
     * if a neighbor is a digit, note its position
     * don't add neighbors we just do bfs for one layer
     * then we have a set of all the digits
     * next we need a set of ranges where the digits are
     * we need to expand them so go left and right until we have all the digits of the number
     * set will make sure no duplicates
     * then sum all the numbers in the set given by range
     */

    // Range set
    std::set<std::pair<std::pair<int, int>, std::pair<int, int>>> range_set;
    std::set<std::pair<int, int>> digit_set;

    // Get reachable digits
    while (!q.empty())
    {
        auto current = q.front();
        q.pop();

        // Check if neighbor is a digit, 8 directions
        std::vector<std::vector<int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

        for (auto dir : directions)
        {
            int new_row = current.first + dir[0];
            int new_col = current.second + dir[1];

            if (new_row < 0 || new_row >= gears.size() || new_col < 0 || new_col >= gears[0].size())
            {
                continue;
            }

            if (std::isdigit(gears[new_row][new_col]))
            {
                // Add to digit set
                digit_set.insert({new_row, new_col});
            }
        }
    }

    // Now we have a set of all digit locations reachable from symbols
    // Now we need to expand the ranges
    for (auto digit : digit_set)
    {
        int row = digit.first;
        int col = digit.second;

        // Go left
        while (col - 1 >= 0 && std::isdigit(gears[row][col - 1]))
        {
            col--;
        }

        // Left is now a symbol or out of bounds
        // Go right
        int right_col = digit.second;
        while (right_col + 1 < gears[0].size() && std::isdigit(gears[row][right_col + 1]))
        {
            right_col++;
        }

        // Right is now a symbol or out of bounds
        // Add to range set
        range_set.insert({{row, col}, {row, right_col}});
    }

    // Now we have a set of ranges
    // Sum all the numbers given by the digits in the ranges
    for (auto range : range_set)
    {
        int row = range.first.first;
        int col = range.first.second;
        int right_col = range.second.second;

        // construct number from digits
        std::string number = "";
        while (col <= right_col)
        {
            number += gears[row][col];
            col++;
        }

        sum += std::stoi(number);
    }

    std::cout << "Sum: " << sum << std::endl;

    return 0;
}
