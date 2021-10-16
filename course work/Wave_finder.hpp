
#ifndef Wave_finder_hpp
#define Wave_finder_hpp

#include <stdio.h>

#include <vector>
#include <stack>
#include <initializer_list>
#include <utility>



enum class eCell
{
  START
 ,FINISH
 ,FREE
 ,UNFREE
};

enum class eDirection
{
  UP
 ,RIGHT
 ,DOWN
 ,LEFT
};


    class WayMatrix
{
    private:
    unsigned mLines;
    unsigned mColumns;
    std::vector<eCell> mMatrix;
  
public:
    //WayMatrix(std::initializer_list<eCell>, unsigned aColumns);
  WayMatrix(unsigned aLines, unsigned aColumns);
  ~WayMatrix() = default;
  WayMatrix(char* str);
  //WayMatrix(const WayMatrix &) = delete;
 // WayMatrix &operator=(const WayMatrix &) = delete;
  //WayMatrix(const WayMatrix &&) = delete;
 // WayMatrix &operator=(const WayMatrix &&) = delete;

  eCell get(unsigned, unsigned)const;

  void push(eCell);

  const unsigned &lines()const;
  const unsigned &columns()const;


};


using point = std::pair<unsigned, unsigned>;

//typedef void (*pf_next)();

class finder
{
private:
  unsigned mMaxCellCost;
  bool mIsFound{false};
  //eFindType               mFindType{eFindType::NEUMANN};
  point mStart{0,0};
  point mFinish{0,0};
  const WayMatrix &mMatrix;
  std::stack<eDirection>  mPath;
  
    
public:
    finder(const WayMatrix&);
  //finder(const finder &) = delete;
  //finder &operator=(const finder &) = delete;

  void find();

  const bool &isFound()const;

  const std::stack<eDirection> &get()const;

//private:
  void _find_neumann(std::vector<unsigned>&);
  void _patch_building(std::vector<unsigned>&);

  std::pair<bool, point> is_moving_up(std::vector<unsigned>&, unsigned, unsigned);
  std::pair<bool, point> is_moving_left(std::vector<unsigned>&, unsigned, unsigned);
  std::pair<bool, point> is_moving_right(std::vector<unsigned>&, unsigned, unsigned);
  std::pair<bool, point> is_moving_down(std::vector<unsigned>&, unsigned, unsigned);

    std::pair<point, point> find_start_finish(const WayMatrix &aMatrix);
};



#endif /* Wave_finder_hpp */
