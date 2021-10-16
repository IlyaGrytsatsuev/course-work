
#include <stack>
#include <queue>
#include "Wave_finder.hpp"
#include <fstream>
#include <iostream>
WayMatrix::WayMatrix(unsigned aLines, unsigned aColumns)
{
  mMatrix.reserve(aLines * aColumns);
  mLines   = aLines;
  mColumns = aColumns;
}

void WayMatrix::push(eCell aCell)
{
  if(mMatrix.size() <= mLines * mColumns)
    mMatrix.push_back(aCell);
}

WayMatrix::WayMatrix(char*str)
{
    
    
    std::ifstream file{str};

      if(file.is_open())
      {
        unsigned lines   = 0;
        unsigned columns = 0;
        file >> lines;
        file >> columns;
          
          mMatrix.reserve(lines * columns);
          mLines   = lines;
          mColumns = columns;

       // mpMatrix = new WayMatrix(lines, columns);

        for(unsigned l = 0; l < lines; ++l)
        {
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            for(unsigned c = 0; c < columns; ++c)
            {
                switch (file.get())
                {
                  case '1':
                  {
                    mMatrix.push_back(eCell::START);
                    break;
                  }
                  case '2':
                  {
                      mMatrix.push_back(eCell::FINISH);
                    break;
                  }
                  case '3':
                  {
                      mMatrix.push_back(eCell::FREE);
                    break;
                  }
                  case '4':
                  {
                      mMatrix.push_back(eCell::UNFREE);
                    break;
                  }
                }
            }
        }
}
    else
        std::cout<<"File is not open";
}

const unsigned &WayMatrix::lines()const
{
  return mLines;
}

const unsigned &WayMatrix::columns()const
{
  return mColumns;
}

eCell WayMatrix::get(unsigned aLine, unsigned aColumn)const
{
  if((aLine * aColumn) < mMatrix.size())
      return mMatrix[aLine * mColumns + aColumn];
        return eCell::UNFREE;
}



std::pair<point, point> finder::find_start_finish(const WayMatrix &a)
{
  std::pair<point, point> res{{0,0},{0,0}};

  for(unsigned l = 0; l < a.lines();l++)
    for(unsigned c = 0; c < a.columns(); c++)
    {
      if(a.get(l, c) == eCell::START)
        res.first = {l,c};
      if(a.get(l, c) == eCell::FINISH)
        res.second = {l,c};
    }
  return res;
}

finder::finder(const WayMatrix &aMatrix)
:mMatrix(aMatrix)
{
  std::pair<point, point> res;
  res = find_start_finish(aMatrix);
  mStart  = res.first;
  mFinish = res.second;
  mMaxCellCost = aMatrix.columns() * aMatrix.lines();
}

void finder::find()
{
  std::vector<unsigned> arr;
  arr.resize(mMatrix.lines() * mMatrix.columns(),mMaxCellCost);

  //mFindType = aType;

      arr[mStart.first * mMatrix.columns() + mStart.second] = 0;

      mIsFound = false;

      _find_neumann(arr);
      if(mIsFound)
        _patch_building(arr);
}

const bool &finder::isFound()const
{
  return mIsFound;
}

const std::stack<eDirection> &finder::get()const
{
  return mPath;
}

std::pair<bool, point> finder::is_moving_up(std::vector<unsigned> &aArr,
                                            unsigned aL,
                                            unsigned aC)
{
  std::pair<bool, point> res{false,{0,0}};

    if(aL>0){
  auto l   = aL - 1;
  auto val = aArr[aL*mMatrix.columns() + aC] + 1;

  if(mMatrix.get(l, aC) == eCell::FINISH)
    mIsFound = true;

  if(mMatrix.get(l, aC) == eCell::FREE)
  {
    if(aArr[l*mMatrix.columns() + aC] == mMaxCellCost)
    {
      res.first         = true;
      res.second.first  = l;
      res.second.second = aC;
      aArr[l*mMatrix.columns() + aC] = val;
    }
  }
    }
  return res;
}

std::pair<bool, point> finder::is_moving_left(std::vector<unsigned> &aArr,
                                              unsigned aL,
                                              unsigned aC)
{
  std::pair<bool, point> res{false,{0,0}};
    if(aC>0){
  auto c   = aC - 1;
  auto val = aArr[aL*mMatrix.columns() + aC] + 1;

  if(mMatrix.get(aL, c) == eCell::FINISH)
    mIsFound = true;

  if(mMatrix.get(aL, c) == eCell::FREE)
  {
    if(aArr[aL*mMatrix.columns() + c] == mMaxCellCost)
    {
      res.first         = true;
      res.second.first  = aL;
      res.second.second = c;
      aArr[aL*mMatrix.columns() + c] = val;
    }
  }
    }
  return res;
}


std::pair<bool, point> finder::is_moving_right(std::vector<unsigned> &aArr,
                                               unsigned aL,
                                               unsigned aC)
{
  std::pair<bool, point> res{false,{0,0}};
    if(aC<mMatrix.columns() - 1){
  auto c   = aC + 1;
  auto val = aArr[aL*mMatrix.columns() + aC] + 1;

  if(mMatrix.get(aL, c) == eCell::FINISH)
    mIsFound = true;

  if(mMatrix.get(aL, c) == eCell::FREE)
  {
    if(aArr[aL*mMatrix.columns() + c] == mMaxCellCost)
    {
      res.first         = true;
      res.second.first  = aL;
      res.second.second = c;
      aArr[aL*mMatrix.columns() + c] = val;
    }
  }
    }
  return res;
}

std::pair<bool, point> finder::is_moving_down(std::vector<unsigned> &aArr,
                                              unsigned aL,
                                              unsigned aC)
{
  std::pair<bool, point> res{false,{0,0}};
    if(aL<mMatrix.lines()-1){
  auto l   = aL + 1;
  auto val = aArr[aL*mMatrix.columns() + aC] + 1;

  if(mMatrix.get(l, aC) == eCell::FINISH)
    mIsFound = true;

  if(mMatrix.get(l, aC) == eCell::FREE)
  {
    if(aArr[l*mMatrix.columns() + aC] == mMaxCellCost)
    {
      res.first         = true;
      res.second.first  = l;
      res.second.second = aC;
      aArr[l*mMatrix.columns() + aC] = val;
    }
  }
}
  return res;
}

void finder::_find_neumann(std::vector<unsigned> &aArr)
{
  std::queue<point> _queque;
  _queque.push({mStart.first, mStart.second});

  std::pair<bool, point> res;

  while(_queque.size() > 0)
  {
    point tmp = _queque.front();
    _queque.pop();

    res = is_moving_up(aArr, tmp.first, tmp.second);
    if(res.first == true)
      _queque.push(res.second);

    res = is_moving_right(aArr, tmp.first, tmp.second);
    if(res.first == true)
      _queque.push(res.second);

    res = is_moving_down(aArr, tmp.first, tmp.second);
    if(res.first == true)
      _queque.push(res.second);

    res = is_moving_left(aArr, tmp.first, tmp.second);
    if(res.first == true)
      _queque.push(res.second);
  }
}

void finder::_patch_building(std::vector<unsigned> &aArr)
{
  auto ln   = mFinish.first ;
  auto cn   = mFinish.second ;
  auto val = mMaxCellCost;
  auto index = 0;
  auto dl = 0;
  auto dc = 0;
    
    auto up = val;
    auto down = val;
    auto right = val;
    auto left = val;

  while(!mPath.empty())
      mPath.pop();

  for(;;)
  {
    index = 0;
    if((ln-1) > mMatrix.lines()-1||cn > mMatrix.columns()-1)
         up = val;
      else
     up = aArr[ (ln - 1)*mMatrix.columns() + cn];
   // auto up_right   = aArr[ (ln - 1)*mMatrix.columns() + (cn + 1)];
      if(ln > mMatrix.lines()-1||(cn+1) > mMatrix.columns()-1)
           right = val;
      else
       right = aArr[ ln*mMatrix.columns() + (cn + 1)];
    //auto down_right = aArr[ (ln + 1)*mMatrix.columns() + (cn + 1)];
      if((ln+1) > mMatrix.lines()-1||cn > mMatrix.columns()-1)
           down = val;
      else
       down = aArr[ (ln + 1)*mMatrix.columns() + cn];
   // auto down_left  = aArr[ (ln + 1)*mMatrix.columns() + (cn - 1)];
      if(ln > mMatrix.lines()-1||(cn-1) > mMatrix.columns()-1)
           left = val;
      else
       left = aArr[ ln*mMatrix.columns() + (cn - 1)];
    //auto up_left    = aArr[ (ln - 1)*mMatrix.columns() + (cn - 1)];

    if(down < val)    { val = down;   index = 1; }
    if(left < val) { val = left; index = 3; }
    if(up < val)  { val = up; index = 5; }
    if(right < val)  { val = right; index = 7; }


    switch(index)
    {
      case 0:
      {
        return;
      }
      case 1: // up
      {
        dl = 1;
        mPath.push(eDirection::DOWN);
        break;
      }
      
      case 3: // right
      {
        dc = -1;
        mPath.push(eDirection::LEFT);
        break;
      }
      
      case 5: // down
      {
        dl = -1;
        mPath.push(eDirection::UP);
        break;
      }
      
      case 7: // left
      {
        dc = 1;
        mPath.push(eDirection::RIGHT);
        break;
      }
      
    }
    ln += dl;
    cn += dc;
    dl = 0;
    dc = 0;
  }
}
