#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <ranges>
#include <algorithm>
#include <unordered_map>

std::vector<std::string> splitString(const std::string &str, char delimiter)
{
    std::vector<std::string> result;
    std::istringstream stream(str);
    std::string token;
    while (std::getline(stream, token, delimiter))
    {
        result.push_back(token);
    }
    return result;
}

int main()
{
    std::ifstream myfile("input.txt");
    if (!myfile)
    {
        throw std::runtime_error("Unable to open file");
    }

    int sum = 0;
    int powerSum = 0;
    std::unordered_map<std::string, int> maxCounts{{"red", 12}, {"blue", 14}, {"green", 13}};
    std::string line;

    // Each game
    while (std::getline(myfile, line))
    {
        auto gamePos = line.find("Game");
        auto colonPos = line.find(":");
        if (gamePos == std::string::npos || colonPos == std::string::npos)
            continue;

        std::string gameId = line.substr(gamePos + 5, colonPos - gamePos - 5);
        std::vector<std::string> sets = splitString(line.substr(colonPos + 1), ';');

        bool gameValid = true;
        int power = 1;

        // Keep track of maximum counts for each color
        std::unordered_map<std::string, int> gameMaxColorCount{{"red", 0}, {"blue", 0}, {"green", 0}};
        // Each set
        for (const auto &set : sets)
        {
            std::vector<std::string> colors = splitString(set, ',');
            std::unordered_map<std::string, int> colorCounts;

            // Each color
            for (const auto &color : colors)
            {
                auto firstDigitPos = color.find_first_of("0123456789");
                auto lastDigitPos = color.find_last_of("0123456789");
                int colorCount = std::stoi(color.substr(firstDigitPos, lastDigitPos - firstDigitPos + 1));
                std::string colorName = color.substr(lastDigitPos + 1);
                std::erase(colorName, ' ');
                colorCounts[colorName] += colorCount;
                gameMaxColorCount[colorName] = std::max(gameMaxColorCount[colorName], colorCounts[colorName]);
            }

            for (const auto &[colorName, count] : colorCounts)
            {
                if (count > maxCounts[colorName])
                {
                    gameValid = false;
                    break;
                }
            }
        }

        for (const auto &[colorName, count] : gameMaxColorCount)
        {
            power *= count;
        }

        powerSum += power;
        if (gameValid)
        {
            sum += std::stoi(gameId);
        }
    }

    std::cout << "Result Sum: " << sum << "\n";
    std::cout << "Result Power Sum: " << powerSum << "\n";
    return 0;
}
