#include<iostream>
#include<vector>
#include<sstream>
#include<string>
#include<cmath>
#include<iomanip>

#include "char_heap.h"

// http://stackoverflow.com/questions/994593/how-to-do-an-integer-log2-in-c
// will be used to compute height of the heap
size_t IntegerLogarithm2(size_t arg) {
  size_t logarithm = 0;
  while (arg >>= 1) ++logarithm;
  return logarithm;
}

// will be used to compute number of elements at the level i
size_t IntegerPower2(size_t arg) {
  if(arg)
    return (size_t)2 << (arg-1);
  else
    return 1;
}

// returns total line length for the level
size_t LineLength(size_t level, size_t item_width, size_t spaces_between) {
  return IntegerPower2(level) * (item_width + spaces_between) - spaces_between;
}

int main()
{

  Heap h = new_heap();
  Heap e = new_heap();
  int i;
  for(i = 0; i < 64; i++){
    heap_insert(h,'0'+i, i, i%2);
  }
  for(i = 0; i < 26; i++){
    heap_insert(e,'A'+i, i, (i+1)%2);
  }

  std::vector<char> H;
  std::vector<char> E;
  std::vector<std::vector<char> > H_levels;
  std::vector<std::vector<char> > E_levels;

  char *vh = array(h);
  char *ve = array(e);

  for(int i = 0; i < 64; i++){
      H.push_back(vh[i]);
      E.push_back(ve[i]);
  }

  // The input heap array
  std::vector<int> A = {15, 6, 8, 2, 4, 3, 6, 0, 1, 3, 2, 2, 1, 0, 2};
  // The heap array split by levels
  std::vector<std::vector<int> > levels;

  // Height of the heap
  size_t levels_number = IntegerLogarithm2(A.size() + 1);
  levels.resize(levels_number);

  size_t H_levels_number = IntegerLogarithm2(H.size() + 1);
  H_levels.resize(H_levels_number);
  size_t E_levels_number = IntegerLogarithm2(E.size() + 1);
  E_levels.resize(E_levels_number);

  // Now fill the levels
  for (size_t i = 0; i < levels.size(); ++i) {
    size_t elements_number = IntegerPower2(i);
    levels[i].resize(elements_number);
    for (size_t j = elements_number - 1, p = 0; p < elements_number; ++j, ++p)
      levels[i][p] = A[j];
  }

  for (size_t i = 0; i < H_levels.size(); ++i) {
    size_t elements_number = IntegerPower2(i);
    H_levels[i].resize(elements_number);
    for (size_t j = elements_number - 1, p = 0; p < elements_number; ++j, ++p)
      H_levels[i][p] = H[j];
  }

  for (size_t i = 0; i < E_levels.size(); ++i) {
    size_t elements_number = IntegerPower2(i);
    E_levels[i].resize(elements_number);
    for (size_t j = elements_number - 1, p = 0; p < elements_number; ++j, ++p)
      E_levels[i][p] = E[j];
  }

  if (levels_number < 1) return 0;
  if (H_levels_number < 1) return 0;
  if (E_levels_number < 1) return 0;

  int magnitude = (abs(A[0]) <= 1 ? 1 : abs(A[0]));
  size_t tab_width = (size_t)floor(log(double(magnitude)) / log(10.0)) + 1;
  // size_t longest_line = LineLength(levels_number - 1, tab_width, tab_width);
  std::vector<std::string> text;
  text.reserve(levels_number * 2 - 1);

  // Do the aligned output to the strings array
  for (size_t i = 0; i < levels_number; ++i) {
    size_t outer_space_width = IntegerPower2(levels_number - 1 - i) - 1;
    size_t inner_space_width = outer_space_width * 2 + 1;
    std::string outer_space(outer_space_width * tab_width, ' ');
    std::string inner_space(inner_space_width * tab_width, ' ');

    std::ostringstream line;
    line << outer_space;

    if (i > 0) {
      std::ostringstream branchline;
      std::string joint(tab_width, '|');
      std::string branch(inner_space_width * tab_width, '-');

      branchline << outer_space;

      if (levels[i].size() > 0) {
        branchline << joint;
      }

      bool isline = true;
      for (size_t j = 1; j < levels[i].size(); ++j, isline = !isline) {
        if(isline)
          branchline << branch << joint;
        else
          branchline << inner_space << std::setfill(' ') <<
              std::setw(tab_width) << joint;
      }

      branchline << outer_space;
      text.push_back(branchline.str());
    }

    if (levels[i].size() > 0) {
      line << std::setfill(' ') << std::setw(tab_width) << levels[i][0];
    }

    for (size_t j = 1; j < levels[i].size(); ++j) {
      line << inner_space << std::setfill(' ') <<
          std::setw(tab_width) << levels[i][j];
    }

    line << outer_space;
    text.push_back(line.str());
  }


  //HHHHHHHHHHHHHHHHHHHHHHHHHHHHH
  int H_magnitude = (abs(H[0]) <= 1 ? 1 : abs(H[0]));
  size_t H_tab_width = (size_t)floor(log(double(H_magnitude)) / log(10.0)) + 1;
  // size_t longest_line = LineLength(levels_number - 1, tab_width, tab_width);
  std::vector<std::string> H_text;
  text.reserve(H_levels_number * 2 - 1);

  // Do the aligned output to the strings array
  for (size_t i = 0; i < H_levels_number; ++i) {
    size_t outer_space_width = IntegerPower2(H_levels_number - 1 - i) - 1;
    size_t inner_space_width = outer_space_width * 2 + 1;
    std::string outer_space(outer_space_width * tab_width, ' ');
    std::string inner_space(inner_space_width * tab_width, ' ');

    std::ostringstream line;
    line << outer_space;

    if (i > 0) {
      std::ostringstream branchline;
      std::string joint(tab_width, '|');
      std::string branch(inner_space_width * tab_width, '-');

      branchline << outer_space;

      if (levels[i].size() > 0) {
        branchline << joint;
      }

      bool isline = true;
      for (size_t j = 1; j < H_levels[i].size(); ++j, isline = !isline) {
        if(isline)
          branchline << branch << joint;
        else
          branchline << inner_space << std::setfill(' ') <<
              std::setw(H_tab_width) << joint;
      }

      branchline << outer_space;
      H_text.push_back(branchline.str());
    }

    if (H_levels[i].size() > 0) {
      line << std::setfill(' ') << std::setw(H_tab_width) << H_levels[i][0];
    }

    for (size_t j = 1; j < H_levels[i].size(); ++j) {
      line << inner_space << std::setfill(' ') <<
          std::setw(H_tab_width) << H_levels[i][j];
    }

    line << outer_space;
    H_text.push_back(line.str());
  }

  //EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
  int E_magnitude = (abs(E[0]) <= 1 ? 1 : abs(E[0]));
  size_t E_tab_width = (size_t)floor(log(double(E_magnitude)) / log(10.0)) + 1;
  // size_t longest_line = LineLength(levels_number - 1, tab_width, tab_width);
  std::vector<std::string> E_text;
  text.reserve(E_levels_number * 2 - 1);

  // Do the aligned output to the strings array
  for (size_t i = 0; i < E_levels_number; ++i) {
    size_t outer_space_width = IntegerPower2(E_levels_number - 1 - i) - 1;
    size_t inner_space_width = outer_space_width * 2 + 1;
    std::string outer_space(outer_space_width * tab_width, ' ');
    std::string inner_space(inner_space_width * tab_width, ' ');

    std::ostringstream line;
    line << outer_space;

    if (i > 0) {
      std::ostringstream branchline;
      std::string joint(tab_width, '|');
      std::string branch(inner_space_width * tab_width, '-');

      branchline << outer_space;

      if (levels[i].size() > 0) {
        branchline << joint;
      }

      bool isline = true;
      for (size_t j = 1; j < E_levels[i].size(); ++j, isline = !isline) {
        if(isline)
          branchline << branch << joint;
        else
          branchline << inner_space << std::setfill(' ') <<
              std::setw(E_tab_width) << joint;
      }

      branchline << outer_space;
      E_text.push_back(branchline.str());
    }

    if (E_levels[i].size() > 0) {
      line << std::setfill(' ') << std::setw(E_tab_width) << E_levels[i][0];
    }

    for (size_t j = 1; j < E_levels[i].size(); ++j) {
      line << inner_space << std::setfill(' ') <<
          std::setw(E_tab_width) << E_levels[i][j];
    }

    line << outer_space;
    E_text.push_back(line.str());
  }


  // Output the text
  for (auto& i : text)
    std::cout << i << std::endl;
  for (auto& i : H_text)
    std::cout << i << std::endl;
  for (auto& i : E_text)
    std::cout << i << std::endl;

  return 0;
}
