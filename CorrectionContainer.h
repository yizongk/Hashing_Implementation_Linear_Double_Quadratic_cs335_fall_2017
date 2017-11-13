// Yi Zong Kuang
// Container holding a mistake and its candidate corrections.

#ifndef CORRECTION_CONTAINER_H
#define CORRECTION_CONTAINER_H

#include <iomanip>
#include <string>
#include <iostream>
#include <vector>

class CorrectionContainer {
 public:
  CorrectionContainer( std::string & mistake, unsigned int line = 0, unsigned nth = 0 ) {
    mistake_ = mistake;
    line_num = line;
    nth_word = nth;
  }

  void insert( std::string & a ) {
    candidates_.push_back( a );
  }

  const std::string & getMistake() const {
    return mistake_;
  }

  const std::vector<std::string> & getCandidates() const {
    return candidates_;
  }

  void setCandidates( std::vector<std::string> good_corrections ) {
    candidates_.swap( good_corrections );
  }

  friend std::ostream & operator<< ( std::ostream &out, const CorrectionContainer & a_correction ) {
    out << "_____________________" << std::endl;
    out << std::setw(6) << "Line: " << a_correction.getLineNum() << ", " << a_correction.getNthWord();
     // Decides st, nd, or th.
    if( a_correction.getNthWord() == 1 ) 
      out << "st word.|" << std::endl;
    else if ( a_correction.getNthWord() == 2 ) 
      out << "nd word.|" << std::endl;
    else 
      out << "th word.|" << std::endl;

    out << std::setw(20) << std::left << a_correction.getMistake() << "|" << std::endl;
    out << "---------------------" << std::endl;
 
    if( a_correction.getCandidates().size()  == 0 ) 
      out << "  No corrections found..." << std::endl;
    else 
      for( unsigned int i = 0; i < a_correction.getCandidates().size(); ++i ) 
        out << "  * " << std::setw(16) << std::left << a_correction.getCandidates()[i] << "|" << std::endl;

    out << "_____________________" << std::endl;

    return out;
  } 

  const unsigned int & getLineNum() const {
    return line_num;
  }

  const unsigned int & getNthWord() const {
    return nth_word;
  }

 private:
  std::string mistake_;
  std::vector<std::string> candidates_;
  unsigned int line_num;
  unsigned int nth_word;
};

#endif
