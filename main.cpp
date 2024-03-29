/* 
 * File:   main.cpp
 * Author: palmyman
 *
 * Created on May 5, 2015, 1:54 PM
 */

#ifndef __PROGTEST__
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <list>
#include <algorithm>
using namespace std;
#endif /* __PROGTEST__ */

class NoRouteException {
};

template <typename _T, typename _E>
class CRoute {
    map<_T, multimap<_T, _E>> mainRoute;
public:
    //CRoute();
    //~CRoute();
    CRoute & Add(const _T & from, const _T & to, const _E & value) {
        typename map<_T, multimap<_T, _E>>::iterator mainRouteIT;
        //from
        mainRouteIT = mainRoute.find(from);
        if(mainRouteIT == mainRoute.end()) {
            //new node
            multimap <_T,_E> newNode;
            newNode.insert(make_pair(from, value));
            mainRoute.insert(make_pair(from, newNode));
        } else {
            //adding to an existing node
            mainRouteIT->second.insert(make_pair(to, value));
        }
        
        //to
        mainRouteIT = mainRoute.find(to);
        if(mainRouteIT == mainRoute.end()) {
            //new node
            multimap <_T,_E> newNode;
            newNode.insert(make_pair(to, value));
            mainRoute.insert(make_pair(to, newNode));
        } else {
            //adding to an existing node
            mainRouteIT->second.insert(make_pair(from, value));
        }
        return * this;
    }
    list<_T> Find(const _T & from, const _T & to) {
        typename map<_T, multimap<_T, _E>>::iterator mainRouteIT;        
        list<_T> retunRoute;
        
        map<_T, _T> route;
        typename map<_T, _T>::iterator routeIT;
        
        deque<_T> linear;
        typename deque<_T>::iterator linearIT;
        
        set<_T> beenTo;
        typename set<_T>::iterator beenToIT;
        
        //no route exception
        mainRouteIT = mainRoute.find(from);
        if(mainRouteIT == mainRoute.end()) {
            throw NoRouteException();
        }
        
        linear.push_back(mainRouteIT->first);
        
        _T current(from);
        
        while(!linear.empty()) {
            current = linear.at(0);
        }
        return retunRoute;
    }
    template<typename _F>
    list<_T> Find(const _T &, const _T &, const _F) {
        list<_T> retunRoute;
        return retunRoute;
    };
};

#ifndef __PROGTEST__
//=================================================================================================

struct CTrain {

    CTrain(const string & company, int speed)
    : m_Company(company), m_Speed(speed) {
    }

    string m_Company;
    int m_Speed;
};

struct TrainFilterCompany {

    TrainFilterCompany(const set<string> & companies)
    : m_Companies(companies) {
    }

    bool operator()(const CTrain & train) const {
        return m_Companies . find(train . m_Company) != m_Companies . end();
    }

    set <string> m_Companies;
};

struct TrainFilterSpeed {

    TrainFilterSpeed(int min, int max)
    : m_Min(min), m_Max(max) {
    }

    bool operator()(const CTrain & train) const {
        return train . m_Speed >= m_Min && train . m_Speed <= m_Max;
    }

    int m_Min, m_Max;
};

bool NurSchnellzug(const CTrain & zug) {
    return ( zug . m_Company == "OBB" || zug . m_Company == "DB") && zug . m_Speed > 100;
}
//=================================================================================================

static string toText(const list<string> & l) {
    ostringstream oss;

    auto it = l . cbegin();
    oss << *it;
    for (++it; it != l . cend(); ++it)
        oss << " > " << *it;
    return oss . str();
}
//=================================================================================================

int main(void) {
    CRoute<string, CTrain> lines;

    lines . Add("Berlin", "Prague", CTrain("DB", 120))
            . Add("Berlin", "Prague", CTrain("CD", 80))
            . Add("Berlin", "Dresden", CTrain("DB", 160))
            . Add("Dresden", "Munchen", CTrain("DB", 160))
            . Add("Munchen", "Prague", CTrain("CD", 90))
            . Add("Munchen", "Linz", CTrain("DB", 200))
            . Add("Munchen", "Linz", CTrain("OBB", 90))
            . Add("Linz", "Prague", CTrain("CD", 50))
            . Add("Prague", "Wien", CTrain("CD", 100))
            . Add("Linz", "Wien", CTrain("OBB", 160))
            . Add("Paris", "Marseille", CTrain("SNCF", 300))
            . Add("Paris", "Dresden", CTrain("SNCF", 250));

    list<string> r1 = lines . Find("Berlin", "Linz");
    assert(toText(r1) == "Berlin > Prague > Linz");

    list<string> r2 = lines . Find("Linz", "Berlin");
    assert(toText(r2) == "Linz > Prague > Berlin");

    list<string> r3 = lines . Find("Wien", "Berlin");
    assert(toText(r3) == "Wien > Prague > Berlin");

    list<string> r4 = lines . Find("Wien", "Berlin", NurSchnellzug);
    assert(toText(r4) == "Wien > Linz > Munchen > Dresden > Berlin");

    list<string> r5 = lines . Find("Wien", "Munchen", TrainFilterCompany(set<string>{"CD", "DB"}));
    assert(toText(r5) == "Wien > Prague > Munchen");

    list<string> r6 = lines . Find("Wien", "Munchen", TrainFilterSpeed(120, 200));
    assert(toText(r6) == "Wien > Linz > Munchen");

    list<string> r7 = lines . Find("Wien", "Munchen", [] (const CTrain & x) {
        return x . m_Company == "CD"; });
    assert(toText(r7) == "Wien > Prague > Munchen");

    list<string> r8 = lines . Find("Munchen", "Munchen");
    assert(toText(r8) == "Munchen");

    list<string> r9 = lines . Find("Marseille", "Prague");
    assert(toText(r9) == "Marseille > Paris > Dresden > Berlin > Prague"
            || toText(r9) == "Marseille > Paris > Dresden > Munchen > Prague");

    try {
        list<string> r10 = lines . Find("Marseille", "Prague", NurSchnellzug);
        assert("Marseille > Prague connection does not exist!!" == NULL);
    } catch (const NoRouteException & e) {
    }
    try {
        list<string> r11 = lines . Find("London", "Oxford");
        assert("London > Oxford connection does not exist!!" == NULL);
    } catch (const NoRouteException & e) {
    }
}
#endif  /* __PROGTEST__ */