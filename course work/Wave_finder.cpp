

#include <stack>
#include <queue>
#include "Wave_finder.hpp"
#include <fstream>
#include <iostream>
   

    WayMatrix::WayMatrix()
    {
          mMatrix.reserve(1);
          mLines   = 0;
          mColumns = 0;
    }

    WayMatrix::~WayMatrix()
    {
        mMatrix.clear();
    }

    void WayMatrix::insert(unsigned aLine,unsigned aColumn, eCell aCell)
    {
        if(aLine >= mLines || aLine < 0 || aColumn > mColumns || aColumn < 0)
            throw "ERROR!!! DEFINITION IS OUT OF RANGE!!";
        
        mMatrix[aLine*mColumns + aColumn] = aCell;
    }


    void WayMatrix::get_data(char*str)
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

                  file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
                  throw "File is not open";
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
          if(aLine >= mLines || aColumn > mColumns)
              throw "ERROR!!! DEFINITION IS OUT OF RANGE!!";
            
            return mMatrix[aLine * mColumns + aColumn];
               // return eCell::UNFREE;
    }



    std::pair<point, point> finder::find_start_finish()
    {
          std::pair<point, point> res{{0,0},{0,0}};

          for(unsigned l = 0; l < mat.lines();l++)
                for(unsigned c = 0; c < mat.columns(); c++)
                {
                    if(mat.get(l, c) == eCell::START)
                        res.first = {l,c};
                    
                    if(mat.get(l, c) == eCell::FINISH)
                        res.second = {l,c};
                }
        
          return res;
    }


    finder::finder(WayMatrix &aMatrix)
        :mat(aMatrix)
    {
          std::pair<point, point> res;
          res = find_start_finish();
          mStart  = res.first;
          mFinish = res.second;
          mMaxCellCost = aMatrix.columns() * aMatrix.lines();
          wave.resize(mat.lines() * mat.columns(),mMaxCellCost);
    }


    void finder::find()
    {

        wave[mStart.first * mat.columns() + mStart.second] = 0;
          
        mIsFound = false;

        generate_wave();
          
        if(mIsFound)
              _patch_building();
        else
              throw "WAY IS NOT FOUND !!!";
    
    }


    void finder:: file_output( char* str)
    {
            std::ofstream out{str};

              if(out.is_open())
              {

                    for(unsigned l = 0; l < mat.lines(); ++l)
                    {
                            out<<"\n";
                        
                            for(unsigned c = 0; c < mat.columns(); ++c)
                            {
            
                                    switch (mat.get(l,c))
                                    {
                                        case START:
                                        {
                                            out<<"1 ";
                                            break;
                                        }
                                        case FINISH:
                                        {
                                            out<<"2 ";
                                            break;
                                        }
                                        case FREE:
                                        {
                                            out<<"3 ";
                                            break;
                                        }
                                        case UNFREE: {
                                            out<<"4 ";
                                            break;
                                        }
                                        case WAY: {
                                            out<<"* ";
                                            break;
                                        }
                                    }
                            }
                    }
              }
        
                else
                    std::cout<<"File is not open";
            
        }


    const bool &finder::isFound()const
    {
        return mIsFound;
    }



    std::pair<bool, point> finder::is_moving_up (unsigned aL, unsigned aC)
    {
        std::pair<bool, point> res{false,{0,0}};

        if(aL>0)
        {
            auto l   = aL - 1;
            auto val = wave[aL*mat.columns() + aC] + 1;

            if(mat.get(l, aC) == eCell::FINISH)
                wave[l*mat.columns() + aC] = val;
                mIsFound = true;

            if(mat.get(l, aC) == eCell::FREE)
            {
                if(wave[l*mat.columns() + aC] == mMaxCellCost)
                {
                    res.first         = true;
                    res.second.first  = l;
                    res.second.second = aC;
                    wave[l*mat.columns() + aC] = val;
                }
            }
        }
      return res;
    }

    std::pair<bool, point> finder::is_moving_up_right (unsigned aL, unsigned aC)
    {
        std::pair<bool, point> res{false,{0,0}};

        if(aL>0 && (aC < mat.columns() - 1) )
        {
            auto l = aL - 1;
            auto c = aC + 1;
            auto val = wave[aL*mat.columns() + aC] + 1;

            if(mat.get(l, c) == eCell::FINISH)
                wave[l*mat.columns() + c] = val;
                mIsFound = true;

            if(mat.get(l, c) == eCell::FREE)
            {
                if(wave[l*mat.columns() + c] == mMaxCellCost)
                {
                    res.first         = true;
                    res.second.first  = l;
                    res.second.second = c;
                    wave[l*mat.columns() + c] = val;
                }
            }
        }
      return res;
    }


    std::pair<bool, point> finder::is_moving_left(unsigned aL, unsigned aC)
    {
        std::pair<bool, point> res{false,{0,0}};
        if(aC>0)
        {
            auto c   = aC - 1;
            auto val = wave[aL*mat.columns() + aC] + 1;

            if(mat.get(aL, c) == eCell::FINISH)
                wave[aL*mat.columns() + c] = val;
                mIsFound = true;

            if(mat.get(aL, c) == eCell::FREE)
            {
                if(wave[aL*mat.columns() + c] == mMaxCellCost)
                {
                    res.first         = true;
                    res.second.first  = aL;
                    res.second.second = c;
                    wave[aL*mat.columns() + c] = val;
                }
            }
        }
      return res;
    }

    std::pair<bool, point> finder::is_moving_up_left (unsigned aL, unsigned aC)
    {
        std::pair<bool, point> res{false,{0,0}};

        if(aL>0 && aC>0)
        {
            auto l = aL - 1;
            auto c = aC - 1;
            auto val = wave[aL*mat.columns() + aC] + 1;

            if(mat.get(l, c) == eCell::FINISH)
                wave[l*mat.columns() + c] = val;
                mIsFound = true;

            if(mat.get(l, c) == eCell::FREE)
            {
                if(wave[l*mat.columns() + c] == mMaxCellCost)
                {
                    res.first = true;
                    res.second.first  = l;
                    res.second.second = c;
                    wave[l*mat.columns() + c] = val;
                }
            }
        }
      return res;
    }

    std::pair<bool, point> finder::is_moving_right(unsigned aL, unsigned aC)
    {
        std::pair<bool, point> res{false,{0,0}};
        if(aC<mat.columns() - 1)
        {
            auto c   = aC + 1;
            auto val = wave[aL*mat.columns() + aC] + 1;

            if(mat.get(aL, c) == eCell::FINISH)
                wave[aL*mat.columns() + c] = val;
                mIsFound = true;

            if(mat.get(aL, c) == eCell::FREE)
            {
                if(wave[aL*mat.columns() + c] == mMaxCellCost)
                {
                    res.first = true;
                    res.second.first  = aL;
                    res.second.second = c;
                    wave[aL*mat.columns() + c] = val;
                }
            }
        }
      return res;
    }

    std::pair<bool, point> finder::is_moving_down_right (unsigned aL, unsigned aC)
    {
        std::pair<bool, point> res{false,{0,0}};

        if( (aL<mat.lines()-1) && (aC<mat.columns() - 1) )
        {
            auto l = aL + 1;
            auto c = aC + 1;
            auto val = wave[aL*mat.columns() + aC] + 1;

            if(mat.get(l, c) == eCell::FINISH)
                wave[l*mat.columns() + c] = val;
                mIsFound = true;

            if(mat.get(l, c) == eCell::FREE)
            {
                if(wave[l*mat.columns() + c] == mMaxCellCost)
                {
                    res.first = true;
                    res.second.first  = l;
                    res.second.second = c;
                    wave[l*mat.columns() + c] = val;
                }
            }
        }
      return res;
    }

    std::pair<bool, point> finder::is_moving_down(unsigned aL,unsigned aC)
    {
        std::pair<bool, point> res{false,{0,0}};
        if(aL<mat.lines()-1)
        {
            auto l   = aL + 1;
            auto val = wave[aL*mat.columns() + aC] + 1;

            if(mat.get(l, aC) == eCell::FINISH)
                wave[l*mat.columns() + aC] = val;
                mIsFound = true;

            if(mat.get(l, aC) == eCell::FREE)
            {
                if(wave[l*mat.columns() + aC] == mMaxCellCost)
                {
                    res.first = true;
                    res.second.first  = l;
                    res.second.second = aC;
                    wave[l*mat.columns() + aC] = val;
                }
            }
        }
      return res;
    }

    std::pair<bool, point> finder::is_moving_down_left (unsigned aL, unsigned aC)
    {
        std::pair<bool, point> res{false,{0,0}};

        if( (aL<mat.lines()-1) && aC>0 )
        {
            auto l = aL + 1;
            auto c = aC - 1;
            auto val = wave[aL*mat.columns() + aC] + 1;

            if(mat.get(l, c) == eCell::FINISH)
                wave[l*mat.columns() + c] = val;
                mIsFound = true;

            if(mat.get(l, c) == eCell::FREE)
            {
                if(wave[l*mat.columns() + c] == mMaxCellCost)
                {
                    res.first = true;
                    res.second.first  = l;
                    res.second.second = c;
                    wave[l*mat.columns() + c] = val;
                }
            }
        }
      return res;
    }

    void finder::generate_wave()
    {
        std::queue<point> _queque;
        _queque.push({mStart.first, mStart.second});

        std::pair<bool, point> res;

        while(_queque.size() > 0)
        {
            point tmp = _queque.front();
            _queque.pop();

            res = is_moving_up( tmp.first, tmp.second);
            if(res.first == true)
                _queque.push(res.second);
            
            res = is_moving_up_right( tmp.first, tmp.second);
            if(res.first == true)
                _queque.push(res.second);

            res = is_moving_right( tmp.first, tmp.second);
            if(res.first == true)
                _queque.push(res.second);
            
            res = is_moving_up_left( tmp.first, tmp.second);
            if(res.first == true)
                _queque.push(res.second);

            res = is_moving_down( tmp.first, tmp.second);
            if(res.first == true)
                _queque.push(res.second);
            
            res = is_moving_down_right( tmp.first, tmp.second);
            if(res.first == true)
                _queque.push(res.second);

            res = is_moving_left( tmp.first, tmp.second);
            if(res.first == true)
                _queque.push(res.second);
            
            res = is_moving_down_left( tmp.first, tmp.second);
            if(res.first == true)
                _queque.push(res.second);
        }
    }

    

    void finder::_patch_building()
    {
        auto ln  = mFinish.first;
        auto cn  = mFinish.second;
        auto val = wave[ln*mat.columns() + cn];
        auto index = 0;
        auto dl = 0;
        auto dc = 0;
        
        auto up = val;
        auto down = val;
        auto right = val;
        auto left = val;
        auto up_right = val;
        auto up_left = val;
        auto down_right = val;
        auto down_left = val;
        

        for(;;)
        {
            index = 0;
            
            if((ln-1) < 0||cn > mat.columns()-1)
                up = val;
            else
                up = wave[ (ln - 1)*mat.columns() + cn];
            
            if((ln-1) < 0 ||(cn+1) > mat.columns()-1)
                up_right = val;
            else
                up_right = wave[(ln - 1)*mat.columns() + (cn + 1)];
            
            if((cn+1) > mat.columns()-1)
                right = val;
            else
                right = wave[ ln*mat.columns() + (cn + 1)];
            
            if((ln+1) > mat.lines()-1||(cn+1) > mat.columns()-1)
                down_right = val;
            else
                down_right = wave[(ln + 1)*mat.columns() + (cn + 1)];
            
       
            if((ln+1) > mat.lines()-1)
                down = val;
            else
                down = wave[(ln + 1)*mat.columns() + cn];
            
            if((ln+1) > mat.lines()-1||(cn-1) < 0)
                down_left = val;
            else
                down_left  = wave[(ln + 1)*mat.columns() + (cn - 1)];
       
            if((cn-1) < 0)
                left = val;
            else
                left = wave[ ln*mat.columns() + (cn - 1)];
            
            if((ln-1) < 0 ||(cn-1) < 0)
                up_left = val;
            else
                up_left = wave[(ln - 1)*mat.columns() + (cn - 1)];
       

            if(down == val-1) { val = down; index = 1; }
            if(left  == val-1) { val = left; index = 2;  }
            if(up  == val-1) { val = up; index = 3; }
            if(right  == val-1) { val = right; index = 4; }
            if(up_right  == val-1) { val = up_right; index = 5; }
            if(up_left  == val-1) { val = up_left; index = 6; }
            if(down_right  == val-1) { val = down_right; index = 7; }
            if(down_left  == val-1) { val = down_left; index = 8; }
            if(val == 0) { index = 0; }

            
            switch(index)
            {
                case 0:
                {
                    return;
                }
                case 1:
                {
                    dl = 1;
                    mat.insert(ln+dl, cn+dc, eCell::WAY);
                    break;
                }
          
                case 2:
                {
                    dc = -1;
                    mat.insert(ln+dl, cn+dc, eCell::WAY);
                    break;
                }
          
                case 3:
                {
                    dl = -1;
                    mat.insert(ln+dl, cn+dc, eCell::WAY);
                    break;
                }
          
                case 4:
                {
                    dc = 1;
                    mat.insert(ln+dl, cn+dc, eCell::WAY);
                    break;
                }
                    
                case 5:
                {
                    dl = -1;
                    dc = 1;
                    mat.insert(ln+dl, cn+dc, eCell::WAY);
                    break;
                }
                
                case 6:
                {
                    dl = -1;
                    dc = -1;
                    mat.insert(ln+dl, cn+dc, eCell::WAY);
                    break;
                }
                    
                case 7:
                {
                    dl = 1;
                    dc = 1;
                    mat.insert(ln+dl, cn+dc, eCell::WAY);
                    break;
                }
          
                case 8:
                {
                    dl = 1;
                    dc = -1;
                    mat.insert(ln+dl, cn+dc, eCell::WAY);
                    break;
                }
            }
            
            ln += dl;
            cn += dc;
            dl = 0;
            dc = 0;
        }
    }
