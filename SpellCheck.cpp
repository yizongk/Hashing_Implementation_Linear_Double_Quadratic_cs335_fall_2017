// Yi Zong Kuang
// Spell Check Main
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include "LinearProbing.h"
#include "QuadraticProbing.h"
#include "DoubleProbing.h"
#include "CorrectionContainer.h"

using namespace std;

 // Chks if argument is a letter or number or single dash.
bool chkForGrammarPoint( const char & a ) {
  return ( ( '0' <= a and a <= '9' ) or ( 'a' <= a and a <= 'z' ) or ( 'A' <= a and a <= 'Z' ) or a == '-' );
}

 // Change string argument to all lower case, doesn't affect grammar points.
void toLower( string & word ) {
  std::transform( word.begin(), word.end(), word.begin(), ::tolower );
  return;
}

 // Method a to modifiy word. Returns a vector of such modification, no correction checks have been done on this vector.
vector<string> addOneCharInEachPos( const string & mistake ) {
  vector<string> mixed;
  for( unsigned int i = 0; i <= mistake.size(); ++i ) 
    for( char alphabet = 'a'; alphabet <= 'z'; ++alphabet ) {
      string mix = mistake;
      mix.insert( i, 1, alphabet );
      mixed.push_back( mix );
    }

  return mixed;
}
 // Method b to modifiy word. Returns a vector of such modification, no correction checks have been done on this vector.
vector<string> rmOneCharInEachPos( const string & mistake ) {
  vector<string> mixed;
  for( unsigned int i = 0; i < mistake.size(); ++i ) {
    string mix = mistake;
    mix.erase( i, 1 );
    mixed.push_back( mix );
  }

  return mixed;
}
 // Method c to modifiy word. Returns a vector of such modification, no correction checks have been done on this vector.
vector<string> swapAdjChar( const string & mistake ) {
  vector<string> mixed;
  for( unsigned int i = 0; i < mistake.size() - 1; ++i ) {
    string mix = mistake;
    std::swap( mix[i], mix[i+1] );
    mixed.push_back( mix );
  }
  
  return mixed;
}

 // My own little method to apply a,b,c method together.
vector<string> swapAddRm( const string & mistake ) {
  vector<string> all_mixed = addOneCharInEachPos( mistake );
  vector<string> b = rmOneCharInEachPos( mistake );
  vector<string> c = swapAdjChar( mistake );

  for( unsigned int i = 0; i < b.size(); ++i )
    all_mixed.push_back( b[i] ); 
  for( unsigned int i = 0; i < c.size(); ++i )  
    all_mixed.push_back( c[i] );  

  return all_mixed;
}

 // Takes in a vector of string as argument, returns a vector of strings that exist in the table.
template <typename HashTableType>
vector<string> probeTheTable( HashTableType & hash_table, const vector<string> & pos_correction ) {
  vector<string> corrections;
  for( unsigned int i = 0; i < pos_correction.size(); ++i ) {
    if( hash_table.Contains( pos_correction[i] ) ) 
      corrections.push_back( pos_correction[i] );
  }

  return corrections;
}

template <typename HashTableType>
bool
HashInputFile( HashTableType &hash_table, const string & dictionary_filename ) {
  cout << "Hashing file into table..." << endl;
  cout << "Dictionary filename: " << dictionary_filename << endl;
  hash_table.MakeEmpty();  

  ifstream input_file;														
  input_file.open( dictionary_filename.c_str() );									
  if( input_file.fail() ) {												
    cout << "File doesn't exists." << endl;
    cout << "Terminating routine..." << endl;										
    return false;													
  }																
																
  string input_line = "";													
  while( std::getline( input_file, input_line ) ) {
    toLower( input_line );											
    hash_table.Insert( input_line );												
  }																
																
  input_file.close();	

  return true;													
}

template <typename HashTableType>
bool
SpellCheck( HashTableType & hash_table, const string & document_filename ) {
  cout << "Starting Spell Checking Routine..." << endl;
  cout << "Document filename: " << document_filename << endl << "____________________________________________" <<endl << endl;
  vector<CorrectionContainer> list_of_mistakes;

  ifstream input_file;
  input_file.open( document_filename.c_str() );
  if( input_file.fail() ) {
    cout << "File doesn't exists." << endl;
    cout << "Terminating routine..." << endl;
    return false;
  }

  string input_line = "";
  unsigned int line_num = 0;
  while( std::getline( input_file, input_line ) ) {
     // Read word by word in the line.
    ++line_num;
    string word = "";
    unsigned int nth_word = 0;
    for( string::iterator itr = input_line.begin(); itr != input_line.end(); ++itr ) {
         // Only collect letter and numbers into our word. Will not take in grammar points (such as '?' and '!') With the exception of single quotation mark.
        if( chkForGrammarPoint( *itr ) ) 
          word += *itr;

      if( *itr == ' ' or ( itr + 1) == input_line.end() ) {
        if( word != " " and word != "" ) {
          ++nth_word;
          toLower( word );

          if( !hash_table.Contains( word ) ) {
             // CorrectionContainer Stores the mistake word, and a vector of possible correction.
            CorrectionContainer current_container( word, line_num, nth_word );

             // Find corrections, insert into current_mistake. Choose one method to run.
            //vector<string> pos_correction = addOneCharInEachPos( word );
            //vector<string> pos_correction = rmOneCharInEachPos( word );
            //vector<string> pos_correction = swapAdjChar( word );
            vector<string> pos_correction = swapAddRm( word );

             // Find a vector of good correction from pos_correction.
            vector<string> good_corrections = probeTheTable( hash_table, pos_correction );
             // setCandidates() use swap, so good_corrections will now be empty after being called.
            current_container.setCandidates( good_corrections );
            list_of_mistakes.push_back( current_container );
          }
          word = "";
        }
      }
    }
  }
  input_file.close();

   // Prints out corrections.
  for( unsigned int i = 0; i < list_of_mistakes.size(); ++i ) 
    cout << list_of_mistakes[i] << endl;

  return true;
}

int main( int argc, char **argv ) {
  if( argc != 3) {
    cout << "Usage: " << argv[0] << " <documentfile> <dictionaryfile>" << endl;
    return 0;
  }

  const string document_filename( argv[1] );
  const string dictionary_filename( argv[2] );

  //HashTableLinear<string> probing_table;
   // Quadratic.
  //HashTable<string> probing_table; 
  HashTableDouble<string> probing_table;

  if( !HashInputFile( probing_table, dictionary_filename ) )
    cout << "Table is empty, dictionary file doesn't exist." << endl;

  if( !SpellCheck( probing_table, document_filename ) )
    cout << "No spell check were done, document file doesn't exist." << endl;  

  return 0;
}
