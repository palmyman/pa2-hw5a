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

class NoRouteException { };

template <typename _T, typename _E>
class CRoute
{
  public:
    // default constructor
    // destructor
    CRoute<_T, _E>& Add (const _T & odkad, const _T & kam, const _E & balast){
        spojeniMestIT=spojeniMest.find(odkad);
        if (spojeniMestIT == spojeniMest.end()){    // nove mesto
            multimap <_T,_E> mp;
            mp.insert(make_pair(kam, balast));
            spojeniMest.insert(make_pair(odkad, mp));
        } else {                                    // pridavam uz k mestu v mape
            spojeniMestIT->second.insert(make_pair(kam, balast));
        }

        spojeniMestIT=spojeniMest.find(kam);
        if (spojeniMestIT == spojeniMest.end()){    // nove mesto
            multimap <_T,_E> mp;
            mp.insert(make_pair(odkad, balast));
            spojeniMest.insert(make_pair(kam, mp));
        } else {                                    // pridavam uz k mestu v mape
            spojeniMestIT->second.insert(make_pair(odkad, balast));
        }
        return *this;
    }
    
    list<_T> Find (const _T & odkad, const _T & kam){
                 map <_T,  _T> cestickoMa;
        typename map <_T,  _T>::iterator cestickoMaIT;
                 deque <_T> expandnute; 
        typename deque <_T>::iterator expandnuteIT;
                 set < _T > kdeJsemByl; 
        typename set < _T >::iterator kdeJsemBylIT;
        list <_T> cestulenka;

        
        spojeniMestIT=spojeniMest.find(odkad);
        if(spojeniMestIT == spojeniMest.end())
            throw NoRouteException();
        expandnute.push_back(spojeniMestIT->first); // pridam prvni
        
        _T  current(odkad);
        
        while (!expandnute.empty()){
           current = expandnute.at(0);
           expandnute.pop_front();        // vezmu si ho
           kdeJsemByl.insert(current);
           
           if ( current == kam ) {
               _T tmp = kam;
               cestulenka.push_back(tmp);
               while ( tmp != odkad){
                   cestickoMaIT = cestickoMa.find(tmp);
                   tmp = cestickoMaIT->second;
                   cestulenka.push_back(tmp);
               }
               cestulenka.reverse();
               return cestulenka;
           }
           spojeniMestIT=spojeniMest.find(current);

            for (auto it: spojeniMestIT->second){
                kdeJsemBylIT=kdeJsemByl.find(it.first);
                if(kdeJsemBylIT == kdeJsemByl.end()){
                   expandnute.push_back(it.first);
                   cestickoMa.insert(make_pair(it.first, current));
                }
            }
        } // pro while
        throw NoRouteException();
    }

    template<typename _F>

    list<_T> Find (const _T & odkad, const _T & kam, _F f){
                 map <_T,  _T> cestickoMa;
        typename map <_T,  _T>::iterator cestickoMaIT;
                 deque <_T> expandnute; 
        typename deque <_T>::iterator expandnuteIT;
                 set < _T > kdeJsemByl; 
        typename set < _T >::iterator kdeJsemBylIT;
        list <_T> cestulenka;

        
        spojeniMestIT=spojeniMest.find(odkad);
        if(spojeniMestIT == spojeniMest.end())
            throw NoRouteException();
        expandnute.push_back(spojeniMestIT->first); // pridam prvni
        
        _T  current(odkad);
        
        while (!expandnute.empty()){
           current = expandnute.at(0);
           expandnute.pop_front();        // vezmu si ho
           kdeJsemByl.insert(current);
           
           if ( current == kam ) {
               _T tmp = kam;
               cestulenka.push_back(tmp);
               while ( tmp != odkad){
                   cestickoMaIT = cestickoMa.find(tmp);
                   tmp = cestickoMaIT->second;
                   cestulenka.push_back(tmp);
               }
               cestulenka.reverse();
               return cestulenka;
           }
           spojeniMestIT=spojeniMest.find(current);

            for (auto it: spojeniMestIT->second){
                kdeJsemBylIT=kdeJsemByl.find(it.first);
                if( (kdeJsemBylIT == kdeJsemByl.end()) && f(it.second) ){
                   expandnute.push_back(it.first);
                   cestickoMa.insert(make_pair(it.first, current));
                }
            }
        } // pro while
        throw NoRouteException();
    }
    
     
     
void    vypis(void){
    for (auto it: spojeniMest){
        cout << it.first << "   DO: ";
        for (auto it2: it.second)
            cout << it2.first << " ";
        cout << endl;
    }
}
// _E nema << pozor osle

private: // jeden uzel jde do mnoziny uzlu, pozor obousmerne
    map <_T, multimap <_T,  _E> > spojeniMest;    
    typename map <_T, multimap <_T,  _E> >::iterator spojeniMestIT; 

};

#ifndef __PROGTEST__
//=================================================================================================
struct CTrain
{
  CTrain ( const string & company, int speed )
    : m_Company ( company ), m_Speed ( speed ) { }

  string m_Company;
  int    m_Speed; 
};

struct TrainFilterCompany
{
  TrainFilterCompany ( const set<string> & companies ) 
    : m_Companies ( companies ) { }

  bool operator () ( const CTrain & train ) const
    { return m_Companies . find ( train . m_Company ) != m_Companies . end (); }

  set <string> m_Companies;    
};
 
struct TrainFilterSpeed
{
  TrainFilterSpeed ( int min, int max )
    : m_Min ( min ), m_Max ( max ) { }
    
  bool operator () ( const CTrain & train ) const
    { return train . m_Speed >= m_Min && train . m_Speed <= m_Max; }

  int m_Min, m_Max; 
};

bool NurSchnellzug ( const CTrain & zug )
 {
   return ( zug . m_Company == "OBB" || zug . m_Company == "DB" ) && zug . m_Speed > 100;
 }
//=================================================================================================
static string      toText                                  ( const list<string> & l )
 {
   ostringstream oss;
   
   auto it = l . cbegin();
   oss << *it;
   for ( ++it; it != l . cend (); ++it )
    oss << " > " << *it;
   return oss . str ();
 }
//=================================================================================================
int main ( void )
 {
   CRoute<string,CTrain> lines;
   
   lines . Add ( "Berlin", "Prague", CTrain ( "DB", 120 ) )
         . Add ( "Berlin", "Prague", CTrain ( "CD",  80 ) )
         . Add ( "Berlin", "Dresden", CTrain ( "DB", 160 ) )
         . Add ( "Dresden", "Munchen", CTrain ( "DB", 160 ) )
         . Add ( "Munchen", "Prague", CTrain ( "CD",  90 ) )
         . Add ( "Munchen", "Linz", CTrain ( "DB", 200 ) )
         . Add ( "Munchen", "Linz", CTrain ( "OBB", 90 ) )
         . Add ( "Linz", "Prague", CTrain ( "CD", 50 ) )
         . Add ( "Prague", "Wien", CTrain ( "CD", 100 ) )
         . Add ( "Linz", "Wien", CTrain ( "OBB", 160 ) )
         . Add ( "Paris", "Marseille", CTrain ( "SNCF", 300 ))
         . Add ( "Paris", "Dresden",  CTrain ( "SNCF", 250 ) );
       
  list<string> r1 = lines . Find ( "Berlin", "Linz" );
   assert ( toText ( r1 ) == "Berlin > Prague > Linz" );
   
   list<string> r2 = lines . Find ( "Linz", "Berlin" );
   assert ( toText ( r2 ) == "Linz > Prague > Berlin" );

   list<string> r3 = lines . Find ( "Wien", "Berlin" );
   assert ( toText ( r3 ) == "Wien > Prague > Berlin" );

   list<string> r4 = lines . Find ( "Wien", "Berlin", NurSchnellzug );
   assert ( toText ( r4 ) == "Wien > Linz > Munchen > Dresden > Berlin" );

   list<string> r5 = lines . Find ( "Wien", "Munchen", TrainFilterCompany ( set<string> { "CD", "DB" } ) );
   assert ( toText ( r5 ) == "Wien > Prague > Munchen" );

   list<string> r6 = lines . Find ( "Wien", "Munchen", TrainFilterSpeed ( 120, 200 ) );
   assert ( toText ( r6 ) == "Wien > Linz > Munchen" );

   list<string> r7 = lines . Find ( "Wien", "Munchen", [] ( const CTrain & x ) { return x . m_Company == "CD"; } );
   assert ( toText ( r7 ) == "Wien > Prague > Munchen" );

   list<string> r8 = lines . Find ( "Munchen", "Munchen" );
   assert ( toText ( r8 ) == "Munchen" );
   
   list<string> r9 = lines . Find ( "Marseille", "Prague" );
   assert ( toText ( r9 ) == "Marseille > Paris > Dresden > Berlin > Prague" 
            || toText ( r9 ) == "Marseille > Paris > Dresden > Munchen > Prague" );
  
   try 
    { 
      list<string> r10 = lines . Find ( "Marseille", "Prague", NurSchnellzug ); 
      assert ( "Marseille > Prague connection does not exist!!" == NULL );
    }
   catch ( const NoRouteException & e ) { }
   try 
    { 
      list<string> r11 = lines . Find ( "London", "Oxford" ); 
      assert ( "London > Oxford connection does not exist!!" == NULL );
    }
   catch ( const NoRouteException & e ) { }
 }
#endif  /* __PROGTEST__ */