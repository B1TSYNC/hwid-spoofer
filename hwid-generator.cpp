#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <chrono>
#include <thread>
#include <limits>

class HWIDGenerator {
private:
    const int MAX_HWIDS_PER_TIME_LIMIT = 1;
    const int TIME_LIMIT_IN_SECONDS = 5;
    std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
    int numCalls = 0;

public:
    std::string generateHWID(int length) {
        char alphabet[36] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 
                            'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
                            'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
        std::string hwid;
        std::srand(std::time(nullptr));
        for(int i = 0; i < length; i++)
        {
            hwid += alphabet[std::rand() % 36];
        }
        return hwid;
    }
    
    void generateHWIDs(int numHWIDs, int length) {
        while (numHWIDs > 0) {
            std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
            int elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastTime).count();

            if (elapsedSeconds >= TIME_LIMIT_IN_SECONDS) {
                numCalls = 0;
                lastTime = currentTime;
            }

            if (numCalls >= MAX_HWIDS_PER_TIME_LIMIT) {
                int remainingTime = TIME_LIMIT_IN_SECONDS - elapsedSeconds;
                std::cout << "Rate limit exceeded. Trying again in " << remainingTime << " seconds." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(remainingTime));
                numCalls = 0;
                lastTime = std::chrono::steady_clock::now();
            }

            std::string hwid = generateHWID(length);
            std::cout << "Generated HWID: " << hwid << std::endl;
            numCalls++;
            numHWIDs--;
        }
    }
};

int main()
{
    HWIDGenerator hwidGenerator;

    int length;
    do {
        std::cout << "> Length of generated Hardware ID, since it varies (between 8 and 32): ";
        std::cin >> length;

        if (std::cin.fail() || length < 8 || length > 32) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number between 8 and 32." << std::endl;
        }
    } while (length < 8 || length > 32);

    hwidGenerator.generateHWIDs(1, length);
    
    return 0;
}
