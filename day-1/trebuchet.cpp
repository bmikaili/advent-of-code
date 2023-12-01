#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::ifstream myfile("input.txt");
    if (!myfile)
    {
        throw std::runtime_error("Unable to open file");
    }

    int sum = 0;
    std::string line;
    while (std::getline(myfile, line))
    {
        auto firstDigitPos = line.find_first_of("0123456789");
        auto lastDigitPos = line.find_last_of("0123456789");

        if (firstDigitPos != std::string::npos && lastDigitPos != std::string::npos)
        {
            int firstDigitValue = line[firstDigitPos] - '0';
            int lastDigitValue = line[lastDigitPos] - '0';

            sum += firstDigitValue * 10 + lastDigitValue;
        }
    }

    std::cout << "Result: " << sum << "\n";
    return 0;
}
