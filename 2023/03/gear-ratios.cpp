#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <queue>
#include <map>
#include <array>
#include <optional>

// Alias for range
using Range = std::pair<std::pair<int, int>, std::pair<int, int>>;

// Expand a range
// Return start and end coordinates of range
Range expandRange(const std::vector<std::vector<char>> &gears, std::pair<int, int> digit)
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
    return {{row, col}, {row, right_col}};
}

int getNumberFromRange(Range range, std::vector<std::vector<char>> &gears)
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

    return std::stoi(number);
}

int main()
{
    std::ifstream myfile("input.txt");
    if (!myfile)
    {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }

    int sum = 0;        // part 1
    int gear_count = 0; // part 2
    std::string line;
    std::vector<std::vector<char>> gears;

    std::queue<std::pair<int, int>> q;

    // Create a 2d matrix of gears

    int row = 0;
    while (std::getline(myfile, line))
    {
        std::string_view sv(line);
        std::vector<char> gear;
        int col = 0;
        for (char gear_type : sv)
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

    std::set<Range> range_set;
    std::map<std::pair<int, int>, std::set<Range>> gear_map;

    /** PART 2
     * Gear map has all neghbors of a *
     * If a * has more EXACTLY 2 neighbor, then it is a gear
     * In the end, go thorugh all gears and multiply the numbers and return the sum
     */

    // Get reachable digits
    while (!q.empty())
    {
        auto current = q.front();
        q.pop();

        // Check if neighbor is a digit, 8 directions
        std::array<std::vector<int>, 8> directions = {{{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}}};

        for (const auto &dir : directions)
        {
            int new_row = current.first + dir[0];
            int new_col = current.second + dir[1];

            if (new_row < 0 || new_row >= gears.size() || new_col < 0 || new_col >= gears[0].size())
            {
                continue;
            }

            if (std::isdigit(gears[new_row][new_col]))
            {
                // Expand and add to range_set
                Range range = expandRange(gears, {new_row, new_col});
                range_set.insert(range);

                if (gears[current.first][current.second] == '*')
                {
                    gear_map[{current.first, current.second}].insert(range);
                }
            }
        }
    }

    // This is for part 1
    // Now we have a set of ranges
    // Sum all the numbers given by the digits in the ranges
    for (const auto &range : range_set)
    {
        sum += getNumberFromRange(range, gears);
    }

    // Part 2
    // Go through all the gears and multiply the numbers
    for (const auto &gear : gear_map)
    {
        if (gear.second.size() == 2)
        {
            int gear_product = 1;
            // Get the range
            for (const auto &range : gear.second)
            {
                gear_product *= getNumberFromRange(range, gears);
            }
            gear_count += gear_product;
        }
    }

    std::cout << "Sum: " << sum << std::endl;
    std::cout << "Gear count: " << gear_count << std::endl;

    return 0;
}
