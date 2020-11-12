
#include <iostream>       
#include <thread>     
#include <chrono>
#include <vector>
#include <algorithm>

using namespace std;

// Returns difference between a given start and end time value
static int calcTime(int s, int e)
{
    return (e - s) / double(CLOCKS_PER_SEC) * 1000;
}

// Fibonacci class recursively calculates the fibonacci sequence from a given value and number of iterations
class Fibonacci
{
private:
    int num;
    int count;
    int start;
    int end;
    int result;
    int index;
	
    int fib(int n)
    {
        if (n <= 1)
            return n;
        return fib(n - 1) + fib(n - 2);
    }

    void run()
    {
        start = clock();
    	
        for (int i = 0; i < count; i++)
        {
            result = fib(num);
        }
        end = clock();
    }
public:
    Fibonacci(int _num, int _count, int _index)
    {
        num = _num;
        count = _count;
        index = _index;
    }

	int GetTime()
	{
        return calcTime(start, end);
	}

	int GetIndex()
    {
        return index;
    }

	int GetNumber()
    {
        return num;
    }

	int GetResult()
    {
        return result;
    }

	void RunProcess()
    {
        run();
    }

	void Reset()
    {
        result = 0;
        start = 0;
        end = 0;
    }

    std::thread th()
	{
        return std::thread([=] { run(); });
    }
};

// Constants
const int MIN = 5;
const int MAX = 20;
const int PROCESS_MIN = 1;
const int PROCESS_MAX = 10000;
const int ITERATIONS_MIN = 1;
const int ITERATION_MAX = 1000000;

// Utilities

int GetRandom()
{
	return rand() % (MAX - MIN + 1) + MIN;
}

void DisplayResults(vector<Fibonacci>* fibonaccis, int time)
{
    cout << "\nProcess\t" << "fib(n)\t" << "Result\t" << "Time(ms)\t" << endl;
    for (Fibonacci f : *fibonaccis)
    {
        cout << f.GetIndex() << "\t" << f.GetNumber() << "\t" << f.GetResult() << "\t" << f.GetTime() << endl;
    }
}

int main(int argc, char* argv[])
{
	// Validate arguments
    if (argc <  3)
    {
        cerr << "Usage: " << endl <<
            argv[0] << "<number of processes> <number of iterations>" << endl
            << "Example: 100 1000" << endl;
        return -1;
    }

	// Get and validate argument values
	unsigned int process_count = atoi(argv[1]);
	unsigned int process_iterations = atoi(argv[2]);
	
    if (process_count < PROCESS_MIN)
        process_count = PROCESS_MIN;
    else if (process_count > PROCESS_MAX)
        process_count = PROCESS_MAX;
    if (process_iterations < ITERATIONS_MIN)
        process_iterations = ITERATIONS_MIN;
    else if (process_iterations > ITERATION_MAX)
        process_iterations = ITERATION_MAX;

	// Init vars
    srand((unsigned)time(nullptr));
    thread* threads = new thread[process_count];
	
    vector<Fibonacci>* fibonaccis = new vector<Fibonacci>();
    int start, end, nothreading, threading;

	// Init fibonacci objects and add to array
    cout << "Initializing fib(n) using " << process_count << " processes and " << process_iterations << " iterations each."  << endl;
    for (int i = 0; i < process_count; i++)
    {
        Fibonacci* f = new Fibonacci(GetRandom(), process_iterations, i + 1);
        fibonaccis->push_back(*f);
    }

	// Iterate and run fib(n) processes with a single thread, display results
    cout << "\nRunning processes single threaded ...";
	start = clock();
	
    for (int i=0;i<process_count;i++)
    {
        fibonaccis->at(i).RunProcess();
    }
	
    end = clock();
    nothreading = calcTime(start, end);
    DisplayResults(fibonaccis, nothreading);

    // Iterate and run fib(n) processes utilizing multithreading, display results
    cout << "\nRunning processes multithreaded ...";
    start = clock();
	
    for (unsigned int i = 0; i < process_count; i++)
    {
        fibonaccis->at(i).Reset();
        threads[i] = fibonaccis->at(i).th();
    }
    
    for (int pcount = 0; pcount < process_count; pcount++)
    {
        threads[pcount].join();
    }

    end = clock();
    threading = calcTime(start, end);
    DisplayResults(fibonaccis, threading);

	// Display time results
    cout << "\nResult Type\tTime(ms)" << endl;
    cout << "Singlethread\t" << nothreading << "(ms)" << endl;
    cout << "Multithread\t" << threading << "(ms)" << endl;
    cout << "Difference\t" << nothreading - threading << "(ms)" << endl;

    return 0;
}