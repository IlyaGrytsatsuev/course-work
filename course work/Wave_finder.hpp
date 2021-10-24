
#ifndef Wave_finder_hpp
#define Wave_finder_hpp

#include <stdio.h>

#include <vector>
#include <stack>
#include <initializer_list>
#include <utility>



    enum  eCell
    {
        START
        ,FINISH
        ,FREE
        ,UNFREE
        ,WAY
    };

    enum  eDirection
    {
        UP
        ,RIGHT
        ,DOWN
        ,LEFT
    };

using point = std::pair<unsigned, unsigned>;

        class WayMatrix
        {
            private:
            
                unsigned mLines;
                unsigned mColumns;
                std::vector<eCell> mMatrix;
      
            public:
                
                
                WayMatrix();
                ~WayMatrix(); //= default;
                void get_data(char* str);
                eCell get(unsigned, unsigned)const;
                void insert(unsigned aLine,unsigned aColumn, eCell aCell);
                const unsigned &lines()const;
                const unsigned &columns()const;
    };

    using point = std::pair<unsigned, unsigned>;
    
    class finder
    {
        private:
            
            unsigned mMaxCellCost;
            bool mIsFound{false};
            point mStart{0,0};
            point mFinish{0,0};
            WayMatrix &mat;
            std::vector<unsigned> wave;

        public:
            
            finder(WayMatrix& aMatrix);
            void find();
            std::pair<point, point> find_start_finish();
            const bool &isFound()const;
            void generate_wave();
            void _patch_building();
            void file_output(const char* str);

            std::pair<bool, point> is_moving_up(unsigned aL, unsigned aC);
            std::pair<bool, point> is_moving_left( unsigned aL, unsigned aC);
            std::pair<bool, point> is_moving_right( unsigned aL, unsigned aC);
            std::pair<bool, point> is_moving_down( unsigned aL, unsigned aC);

    };

        


#endif /* Wave_finder_hpp */
