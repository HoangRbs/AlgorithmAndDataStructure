#include <iostream>
#include <chrono>

using namespace std;

struct Timer {
	chrono::time_point<chrono::high_resolution_clock> start, end;
	chrono::duration<double> m_duration;

	Timer() {
		start = chrono::high_resolution_clock::now();
	}

	~Timer(){
		end = chrono::high_resolution_clock::now();
		m_duration = end - start;
		double ms = m_duration.count() * 1000.0f;
		double micros = m_duration.count() * 1000000.0f;

		cout << endl << endl << "executed in : " << ms << " mili_s" << endl;
		cout << "executed in : " << micros << " micro_s" << endl;
	}
};


