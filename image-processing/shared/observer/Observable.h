//============================================================================
// Name        : Observable.h
// Author      : ITM13
// Version     : 1.0

// Description :
//============================================================================
#pragma once
#include "IObserver.h"

#include <vector>

using namespace std;

template<class T>
class Observable
{
public:
	Observable() {
	}

	virtual ~Observable() {
		observers.clear();
	}

	bool AddObserver(IObserver<T> *observer) {
		vector<IObserver<T> *>::iterator iter = find(observers.begin(), observers.end(), observer);
		// observer exists already
		if (iter != observers.end())
			return false;

		observers.push_back(observer);
		return true;
	}

	bool RemoveObserver(IObserver<T> *observer) {
		vector<IObserver<T> *>::iterator iter = find(observers.begin(), observers.end(), observer);

		// observer could not be found
		if (iter == observers.end()) {
			return false;
		}

		observers.erase(remove(observers.begin(), observers.end(), observer));
		return true;
	}

	void Notify() {
		if (observers.size() == 0) {
			return;
		}

		vector<IObserver<T> *>::iterator iter;
		for (iter = observers.begin(); iter != observers.end(); iter++) {
			(*iter)->Update(static_cast<T *>(this));
		}
	}

private:
	vector<IObserver<T> *> observers;
};
