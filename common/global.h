#ifndef GLOBAL_HHHH_
#define GLOBAL_HHHH_
#include <iostream>
#include <map>
#include <deque>

using namespace std; 

#define TEST_PORT 4567
#define TEST_IP "127.0.0.1"
#define STOCK_COUNT 10000
#define GROUP_STOCK 20
#define SERVER_STOCK_COUNT 500*LOOP_COUNT/4 //*20
#define SNIFF_DEVICE "lo" //"eth0"
#define LOOP_COUNT 100
#define SERVER_STOCK 400

#define PRICE_LIST_SIZE 4096 
typedef map<int, deque<int> >  stockPriceMap; 


// debug function
inline void print_memory(const unsigned char * start,  unsigned int length)
{
    unsigned int row,  col,  i = 0; 
    for(row = 0;  (i + 1) < length;  row++)
    {
        for(col = 0;  col<16;  col++)
        {
            i = row*16+col; 
            if(col == 8)
                printf(" "); 
            if(i<length)
                printf("%02X",  (unsigned char)start[i]); 
            else
                printf("  "); 
            printf(" "); 
        }   

        printf(" "); 
        printf("\n"); 
    }   
} 



class stock{
    public:
        stock(){}
        stock(int id, int open, int close, int max, int min)
            :id_(id), open_price_(open), close_price_(close), max_price_(max), min_price_(min){}
        void print(const string& msg=""){
            cout << msg << "  stock id:"<<id_ << " open:" << open_price_ << " close:"
                 << close_price_ << " max:" << max_price_ << " min:" << min_price_ << endl; 
        }
       // void next(){
       //     open_price_ = close_price_; 
       //     min_price_= open_price_ - (int)(open_price_*0.1*rand()/(RAND_MAX+1.0)); 
       //     max_price_= open_price_ + (int)(open_price_*0.1*rand()/(RAND_MAX+1.0)); 
       //     close_price_ = open_price_ -(int)(open_price_*0.1*rand()/(RAND_MAX+1.0)); 
       //     close_price_ = close_price_<min_price_?min_price_:close_price_; 
       // }

        unsigned char* encode(){
            current_ = buffer_; 
            writeInt(id_); 
            writeInt(open_price_); 
            writeInt(close_price_); 
            writeInt(max_price_); 
            writeInt(min_price_); 
            //print_memory(buffer_,  20); 
            return buffer_; 
        }

        void decode(const unsigned char* msg){
            for(int i=0;  i<20; ++i){
                buffer_[i] = msg[i]; 
            }
            current_ = buffer_; 
            id_ = readInt(); 
            open_price_ = readInt(); 
            close_price_ = readInt(); 
            max_price_ = readInt(); 
            min_price_ = readInt(); 
        }

        int id_; 
        int open_price_; 
        int close_price_; 
        int max_price_; 
        int min_price_; 
        int time_; 

        unsigned char buffer_[20]; 
        unsigned char* current_; 
    private:
        void writeInt (int val) {
            writeByte(val  >>  24); 
            writeByte(val  >>  16); 
            writeByte(val  >>   8); 
            writeByte(val); 
        }

        void writeByte (char ch) { 
            *(current_++) = ch; 
        }

        int readInt(){
            int  val = (current_[0]  <<  24 | current_[1]  <<  16 | current_[2]  <<  8 | current_[3]);   
            current_ +=  4;
            return val;                                                                                        
       } 
}; 



#endif
