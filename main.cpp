#include <iostream>
#include <thread>
#include <vector>

// to store user input, carry overs while adding, and sum of both num1 and num2 (global since we have to pass into thread functions)
std::vector<short int> sum, number1, number2;

const unsigned int maxThreads = 8;            // max number of threads to the power of 2 (12 threads in total)
unsigned short int carryOver[maxThreads + 1]; // keeps track which thread had a carry

void longAddition(int, int, int);

int main()
{
    // creating a list of threads
    std::vector<std::thread> threadPool;

    // used to get user input
    std::string num1, num2 = "";

    std::cout << std::endl;
    std::cout << "Enter a number: ";
    std::cin >> num1;
    std::cout << std::endl;

    std::cout << "Enter a number: ";
    std::cin >> num2;
    std::cout << std::endl;

    // LOOP TO TAKE USER INPUT, CONVERT TO INT, AND STORE IN VECTOR //
    unsigned int trueNum = 0; // to store number after conversion
    for (int i = 0; i < num1.size(); i++)
    {
        trueNum = num1[i] - '0'; // convert char to int
        number1.push_back(trueNum);

        trueNum = num2[i] - '0';
        number2.push_back(trueNum);
    }

    unsigned int threadWork = num1.size() / maxThreads; // getting amount of digits each thread gets
    sum.resize(num1.size() + 1);                        // allocate space needed (+1 just in case a carry happens)

    // initializing array
    for (int i = 0; i <= maxThreads; i++)
    {
        carryOver[i] = 0;
    }

    // LOOP THAT CREATES INDIVIDUAL THREADS //
    unsigned int start = 0;            // starting index for thread
    unsigned int end = threadWork - 1; // end index for thread
    unsigned int carryAmount = 0;      // keeps track of carries
    for (int i = 0; i < maxThreads; i++)
    {
        threadPool.push_back(std::thread(longAddition, start, end, carryAmount));

        // updating parameters
        start = end + 1;
        end += threadWork;
        carryAmount++;
    }

    // synchronizing threads
    for (auto &t : threadPool)
        t.join();

    // CALCULATING SUM //
    unsigned int result = 0;       // stores result of each index
    unsigned int carry = 0;        // stores whether or not there is a carry
    unsigned int threadDigits = 0; // counts the individual digits in a thread
    carryAmount = maxThreads;      // the amount of carries for each thread

    for (int i = sum.size() - 2; i >= 0; i--)
    {
        result = sum[i] + carryOver[carryAmount] + carry; // we add the 2 carries because carryOver[carryCount] keeps track of the thread carries
                                                          //  and carry keeps track of the index carries

        if (carryOver[carryAmount] == 1) // since we added the carry to the "thread," set it to 0
            carryOver[carryAmount] = 0;

        sum[i] = result % 10; // placing digits
        carry = result / 10;  // getting the carry
        threadDigits++;

        if (threadDigits == threadWork) // checks if we need to "go to next thread"
        {
            carryAmount--; // "goes" to the other thread's carry
            threadDigits = 0;
        }
    }

    if ((carry || carryOver[0]) > 0) // if the MSB is a carry, add carry to the front
    {
        sum.insert(sum.begin(), 1);
    }

    // PRINTING RESULTS //
    std::cout << "Result = ";

    for (int i = 0; i < sum.size() - 1; i++)
    {
        std::cout << sum[i];
    }

    threadPool.clear(); // clearing threads

    return 0;
}

/*
FUNCTION_IDENTIFIER: thread function
parameters: starting index, end index
return value: void
description: this function is a threaded function that adds up a
             64 (or larger) digit number in parallel. 
*/
void longAddition(int start, int end, int carryAmount)
{
    unsigned int threadSum = 0; // result of each digit in thread
    unsigned int carry = 0;

    for (int i = end; i >= start; i--)
    {
        threadSum = number1[i] + number2[i] + carry;
        sum[i] = threadSum % 10; // placing digits
        carry = threadSum / 10;  // getting carry
    }

    carryOver[carryAmount] = carry; // placing each thread's carry
}
