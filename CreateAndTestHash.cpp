// Yi Zong Kuang
// Create and Test Hash main

#include "QuadraticProbing.h"
#include "DoubleProbing.h"
#include "LinearProbing.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;



template <typename HashTableType>
void
TestFunctionForHashTable(HashTableType &hash_table, const string &words_filename, const string &query_filename) {
  cout << "TestFunctionForHashTables..." << endl;
  cout << "Words filename: " << words_filename << endl;
  cout << "Query filename: " << query_filename << endl;
  hash_table.MakeEmpty();  

  //..Insert your own code													
  ifstream input_file;														
  input_file.open( words_filename.c_str() );											
  if( input_file.fail() ) {													
    cout << "File doesn't exists." << endl;											
    cout << "Terminating routine..." << endl;											
    return;															
  }																
																
  string input_line = "";													
  while( std::getline( input_file, input_line ) ) {											
    hash_table.Insert( input_line );												
  }																
																
  cout << "Number of items (N): " << hash_table.size() << endl;									
  cout << "Size of hash table (T): " << hash_table.arraySize() << endl;								
  cout << "Load factor (N/T): " << float( hash_table.size() )/float( hash_table.arraySize() ) << endl;				
  cout << "Collisions (C): " << hash_table.collision() << endl;									
  cout << "Avg. number of collisions (C/N): " << float( hash_table.collision() )/float( hash_table.size() ) << endl;		
  																
  input_file.close();														
																
  ifstream query_file;														
  query_file.open( query_filename.c_str() );											
  if( query_file.fail() ) {
    cout << "File doesn't exist." << endl;
    cout << "Terminating routine..." << endl;
    return;
  }

  string query_word = "";
  while( getline( query_file, query_word ) ) {
    unsigned int probes = 1;
    if( hash_table.Contains( query_word, probes ) ) 
      cout << setw(15) << query_word << setw(15) << "Found. " << setw(20) << "Number of Probes: " << probes << endl;
    else 
      cout << setw(15) << query_word << setw(15) << "Not Found. " << setw(20) << "Number of Probes: " << probes << endl;
  }
}

// Sample main for program CreateAndTestHash
int
main(int argc, char **argv) {
  if (argc != 4) {
    cout << "Usage: " << argv[0] << " <wordsfilename> <queryfilename> <flag>" << endl;
    return 0;
  }
  
  const string words_filename(argv[1]);
  const string query_filename(argv[2]);
  const string param_flag(argv[3]);

  if (param_flag == "linear") {
     HashTableLinear<string> linear_probing_table;
     TestFunctionForHashTable(linear_probing_table, words_filename, query_filename);    
  } else if (param_flag == "quadratic") {
    HashTable<string> quadratic_probing_table;
    TestFunctionForHashTable(quadratic_probing_table, words_filename, query_filename);    
  } else if (param_flag == "double") {
     HashTableDouble<string> double_probing_table;
     TestFunctionForHashTable(double_probing_table, words_filename, query_filename);    
  } else {
    cout << "Uknown tree type " << param_flag << " (User should provide linear, quadratic, or double)" << endl;
  }
  return 0;
}
