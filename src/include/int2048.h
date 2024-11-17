#pragma once
#ifndef SJTU_BIGINTEGER
#define SJTU_BIGINTEGER

// Integer 1:
// 实现一个有符号的大整数类，只需支持简单的加减

// Integer 2:
// 实现一个有符号的大整数类，支持加减乘除，并重载相关运算符

// 请不要使用除了以下头文件之外的其它头文件
#include <complex>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>

// 请不要使用 using namespace std;

namespace sjtu {
  constexpr long long p = 1e4;
  constexpr long long base = 4;

  class int2048 {
    std::vector<long long> digit;
    bool symbol = false; //true=negative
    int length = 0;

  public:
    // 构造函数
    int2048();

    int2048(long long);

    int2048(const std::string &);

    int2048(const int2048 &);


    // 以下给定函数的形式参数类型仅供参考，可自行选择使用常量引用或者不使用引用
    // 如果需要，可以自行增加其他所需的函数
    // ===================================
    // Integer1
    // ===================================

    // 读入一个大整数
    void read(const std::string &);

    // 输出储存的大整数，无需换行
    void print();

    // 加上一个大整数
    int2048 &add(const int2048 &);

    // 返回两个大整数之和
    friend int2048 add(int2048, const int2048 &);

    // 减去一个大整数
    int2048 &minus(const int2048 &);

    // 返回两个大整数之差
    friend int2048 minus(int2048, const int2048 &);

    friend bool isAbsLess(const int2048 &, const int2048 &);

    // ===================================
    // Integer2
    // ===================================

    int2048 operator+() const;

    int2048 operator-() const;

    int2048 &operator=(const int2048 &);

    int2048 &operator+=(const int2048 &);

    friend int2048 operator+(const int2048 &, const int2048 &);

    int2048 &operator-=(const int2048 &);

    friend int2048 operator-(const int2048 &, const int2048 &);

    int2048 &operator*=(const int2048 &);

    friend int2048 operator*(int2048, const int2048 &);

    int2048 &operator/=(const int2048 &);

    friend int2048 operator/(int2048, const int2048 &);

    int2048 &operator%=(const int2048 &);

    friend int2048 operator%(int2048, const int2048 &);

    friend std::istream &operator>>(std::istream &, int2048 &);

    friend std::ostream &operator<<(std::ostream &, const int2048 &);

    friend bool operator==(const int2048 &, const int2048 &);

    friend bool operator!=(const int2048 &, const int2048 &);

    friend bool operator<(const int2048 &, const int2048 &);

    friend bool operator>(const int2048 &, const int2048 &);

    friend bool operator<=(const int2048 &, const int2048 &);

    friend bool operator>=(const int2048 &, const int2048 &);

    int2048 &operator<<=(long long);

    int2048 &operator>>=(long long);
  };

  inline int2048::int2048() {
    length = 1;
    digit.clear();
    digit.push_back(0);
  }

  inline int2048::int2048(long long num) {
    digit.clear();
    if (num < 0) {
      symbol = true;
      num = -num;
    }
    while (num) {
      digit.push_back(num % p);
      num /= p;
      length++;
    }
    if (length == 0) {
      length++;
      digit.push_back(0);
    }
  }

  inline int2048 &int2048::operator<<=(long long num) {
    digit.resize(++length);
    digit[0]*=10;
    for (int i = 1;i<length;++i) {
      digit[i]*=10;
      if(digit[i-1]>=p) {
        digit[i]+=digit[i]/p;
        digit[i-1]%=p;
      }
    }
    while (!digit[length - 1] && length > 1) {
      length--;
    }
    return *this;
  }

  inline int2048 &int2048::operator>>=(long long num) {
    for (int i = length - 1; i >= 1; --i) {
      digit[i-1]+=digit[i]%10*p;
      digit[i] /=10;
    }
    digit[0]/=10;
    while(!digit[length - 1] && length > 1) {
      length--;
    }
    return *this;
  }

  inline int2048::int2048(const std::string &str) {

    read(str);
  }

  inline int2048::int2048(const int2048 &rhs) {
    *this=rhs;
  };

  inline void int2048::read(const std::string &str) {
    digit.clear();
    symbol = false;
    long long starting=0,ending=str.length()-1;
    while((str[starting]<='0'||str[starting]>'9')&&starting<=ending) {
      if(str[starting]=='-') {
        symbol^=1;
      }
      starting++;
    }
    if(starting>ending) {
      *this=0;
      return;
    }
    length=(ending-starting)/base+1;
    digit.resize(length);
    for (int i = 0; i < length - 1; ++i) {
      for (int j = base - 1; j >= 0; --j) {
        digit[i] = digit[i] * 10 + (str[ending - j] - '0');
      }
      ending -= base;
    }
    for (int i = starting; i <= ending; ++i) {
      digit[length - 1] = digit[length - 1] * 10 + (str[i] - '0');
    }
  }

  inline void int2048::print() {
    if (symbol) {
      std::cout << '-';
    }
    std::cout<<digit[length-1];
    for (int i = length - 2; i >= 0; --i) {
      std::cout<<std::setw(base)<<std::setfill('0')<<digit[i];
    }
  }


  inline int2048 &int2048::add(const int2048 &other) {
    if (symbol == other.symbol) {
      long long res = 0;
      length = std::max(length, other.length);
      digit.resize(length);
      for (int i = 0; i < other.length; ++i) {
        digit[i] = digit[i] + other.digit[i] + res;
        res = digit[i] / p;
        digit[i] %= p;
      }
      for (int i = other.length; i < length; ++i) {
         if (!res)break;
        digit[i] = digit[i] + res;
        res = digit[i] / p;
        digit[i] %= p;
      }
      if (res) {
        digit.push_back(res);
        length++;
      }
    } else {
      bool status = isAbsLess(*this, other); //true:other > this

      if (status) {
        //symbol=true:negative+positive positive result
        //symbol=false:positive+negative negative result
        int res = 0;
        digit.resize(other.length);
        length = other.length;
        symbol ^= 1;
        for (int i = 0; i < length; ++i) {
          digit[i] = (other.digit[i] - digit[i] - res);
          res = 0;
          if (digit[i] < 0) {
            res = 1;
            digit[i] += p;
          }
        }
        while (digit[length - 1] == 0 && length > 1) {
          length--;
        }
      } else {
        //symbol==true:negative+positive negative result
        //symbol==false:positive+negative positive result
        int res = 0;
        for (int i = 0; i < other.length; ++i) {
          digit[i] = (digit[i] - other.digit[i] - res);
          res = 0;
          if (digit[i] < 0) {
            res = 1;
            digit[i] += p;
          }
        }
        for (int i = other.length; i < length; ++i) {
          if (!res)break;
          digit[i] = digit[i] - res;
          res = 0;
          if (digit[i] < 0) {
            res = 1;
            digit[i] += p;
          }
        }
        while (digit[length - 1] == 0 && length > 1) {
          length--;
        }
        ///@note:this function MIGHT produce zero as result.
        if (length == 0) {
          length = 1;
          symbol = false;
        }
      }
    }
    return *this;
  }



  inline int2048 add(int2048 first, const int2048 &second) {
    return first.add(second);
  }


  inline int2048 &int2048::minus(const int2048 &other) {
    if (symbol != other.symbol) {
      //negative-positive=negative+negative: negative
      //positive-negative=positive+positive: positive
      long long res = 0;
      length = std::max(length, other.length);
      digit.resize(length);
      for (int i = 0; i < other.length; ++i) {
        digit[i] = digit[i] + other.digit[i] + res;
        res = digit[i] / p;
        digit[i] %= p;
      }
      for (int i = other.length; i < length; ++i) {
        if (!res)break;
        digit[i] = digit[i] + res;
        res = digit[i] / p;
        digit[i] %= p;
      }
      if (res) {
        digit.push_back(res);
        length++;
      }
    } else {
      bool status = isAbsLess(*this, other); //true:this < other
      if (status) {
        //symbol=true:negative-negative positive result
        //symbol=false:positive-positive negative result
        long long res = 0;
        digit.resize(other.length);
        length = other.length;
        symbol = symbol ^ 1;
        for (int i = 0; i < length; ++i) {
          digit[i] = (other.digit[i] - digit[i] - res);
          res = 0;
          if (digit[i] < 0) {
            res = 1;
            digit[i] += p;
          }
        }
        while (digit[length - 1] == 0 && length > 1) {
          length--;
        }
        if(length==1&&digit[0]==0) {
          return *this=0;
        }
      } else {
        //symbol=true:negative-negative negative result
        //symbol=false:positive-positive positive result
        int res = 0;
        for (int i = 0; i < other.length; ++i) {
          digit[i] = (digit[i] - other.digit[i] - res);
          res = 0;
          if (digit[i] < 0) {
            res = 1;
            digit[i] += p;
          }
        }
        for (int i = other.length; i < length; ++i) {
          if (!res)break;
          digit[i] = digit[i] - res;
          res = 0;
          if (digit[i] < 0) {
            res = 1;
            digit[i] += p;
          }
        }
        while (digit[length - 1] == 0 && length > 1) {
          length--;
        }
        ///@note:this function MIGHT produce zero as result.
        if(length==1&&digit[0]==0) {
          return *this=0;
        }
      }
    }
    return *this;
  }

  inline int2048 minus(int2048 first, const int2048 &second) {
    return first.minus(second);
  }

  inline bool isAbsLess(const int2048 &first, const int2048 &second) {
    if (first.length < second.length) {
      return true;
    }
    if (first.length > second.length) {
      return false;
    }
    for (int i = first.length - 1; i >= 0; --i) {
      if (first.digit[i] < second.digit[i]) {
        return true;
      }
      if (first.digit[i] > second.digit[i]) {
        return false;
      }
    }
    return false;
  }


  inline int2048 int2048::operator+() const {
    return *this;
  }

  inline int2048 int2048::operator-() const {
    int2048 result = *this;
    if(result==0)return result;
    result.symbol ^= 1;
    return result;
  }

  inline std::istream &operator>>(std::istream &in, int2048 &number) {
    std::string s;
    in >> s;
    number.read(s);
    return in;
  }

  inline std::ostream &operator<<(std::ostream &out, const int2048 &number) {
    if (number.symbol) {
      out << '-';
    }
    out<<number.digit[number.length - 1];
    for (int i = number.length - 2; i >= 0; --i) {
      out << std::setw(base) << std::setfill('0') << number.digit[i];
    }
    return out;
  }

  inline int2048 &int2048::operator=(const int2048 &rhs) = default;

  inline int2048 operator+(const int2048 &lhs, const int2048 &rhs) {
    return add(lhs, rhs);
  }

  inline int2048 operator-(const int2048 &lhs, const int2048 &rhs) {
    return minus(lhs, rhs);
  }

  inline int2048 &int2048::operator+=(const int2048 &rhs) {
    return add(rhs);
  }

  inline int2048 &int2048::operator-=(const int2048 &rhs) {
    return minus(rhs);
  }

  inline bool operator==(const int2048 &lhs, const int2048 &rhs) {
    if (lhs.symbol != rhs.symbol) {
      return false;
    }
    if (lhs.length != rhs.length) {
      return false;
    }
    for (int i = lhs.length - 1; i >= 0; --i) {
      if(lhs.digit[i]!=rhs.digit[i]) {
        return false;
      }
    }
    return true;
  }

  inline bool operator!=(const int2048 &lhs, const int2048 &rhs) {
    return 1^operator==(lhs, rhs) ;
  }

  inline bool operator<(const int2048 &lhs, const int2048 &rhs) {
    if (lhs.symbol != rhs.symbol) {
      //symbol=true: negative
      return lhs.symbol;
    }
    if (lhs.symbol) {
      return isAbsLess(rhs,lhs);
    }
    return isAbsLess(lhs, rhs);
  }

  inline bool operator>(const int2048 &lhs, const int2048 &rhs) {
    return operator<(rhs, lhs);
  }

  inline bool operator<=(const int2048 &lhs, const int2048 &rhs) {
    return operator>(lhs, rhs) ^ 1;
  }

  inline bool operator>=(const int2048 &lhs, const int2048 &rhs) {
    return operator<(lhs, rhs) ^ 1;
  }


  inline int2048 &int2048::operator*=(const int2048 &rhs) {
    if(*this==0||rhs==0)return *this;
    // if(rhs.length==1) {
    //   long long num=rhs.digit[0];
    //   digit.resize(length+1);
    //   for(int i=0;i<length;++i) {
    //     digit[i]*=num;
    //     digit[i+1]+=digit[i]/p;
    //     digit[i]%=p;
    //   }
    //   if(digit[length]) {
    //     length++;
    //   }
    //   symbol=(symbol ^ (rhs.symbol));
    //   return *this;
    // }
    int2048 result=0;
    result.symbol = (symbol ^ (rhs.symbol));
    result.length = length + rhs.length - 1;
    result.digit.resize(length + rhs.length + 2);
    for (int i = 0; i < result.length; ++i) {
      int starting = std::max(i - rhs.length + 1, 0);
      int ending = std::min(length - 1, i);
      for (int j = starting; j <= ending; ++j) {
        if(i-j<0||i-j>=rhs.length)continue;
        result.digit[i] += digit[j] * rhs.digit[i - j];
      }
      if (result.digit[i] >= p) {
        result.digit[i + 1] += result.digit[i] / p;
        result.digit[i] %= p;
      }
    }
    if (result.digit[result.length]) {
      result.length++;
    }
    while(result.digit[result.length-1]==0&&result.length>1)result.length--;
    *this = result;
    return *this;
  }

  inline int2048 operator*(int2048 lhs, const int2048 &rhs) {
    return lhs.operator*=(rhs);
  }


  inline int2048 &int2048::operator/=(const int2048 &rhs) {
    if(rhs==0)return *this=0;
   if(*this==0)return *this;

    // if(isAbsLess(*this,rhs)) {
    //   if(symbol==rhs.symbol) return *this=0;
    //   return *this=(-1);
    // }

    // if(rhs.length==1) {
    //   long long div=rhs.digit[0];
    //   int2048 tmp=*this;
    //   tmp.symbol=false;
    //   long long res=0;
    //   res=tmp.digit[tmp.length-1]%div;
    //   tmp.digit[tmp.length-1]/=div;
    //   for(int i=tmp.length-2;i>=0;--i) {
    //     tmp.digit[i]+=res*p;
    //     res=tmp.digit[i]%div;
    //     tmp.digit[i]/=div;
    //   }
    //   if (!tmp.digit[length - 1] && tmp.length > 1) {
    //     tmp.length--;
    //   }
    //   tmp.symbol = (symbol ^ (rhs.symbol));
    //   if(tmp.symbol&&res>0) {
    //     tmp-=1;
    //   }
    //   *this=tmp;
    //   return *this;
    // }


    int2048 result(0), tmp=*this, div = rhs, num(1);
    div.symbol = false;
    tmp.symbol = false;
    while (div.digit[div.length-1]==0)div.length--;
    while (tmp.digit[tmp.length-1]==0)tmp.length--;

    // if(tmp.length/div.length>100) {
    //   int2048 res=0,current=0;
    //   for(int i=tmp.length-1;i>=0;--i) {
    //     current=current*p+tmp.digit[i];
    //     result*=p;
    //     if(current>=div) {
    //       int2048 x=current/div;
    //       result+=x;
    //       current=current-x*div;
    //     }
    //   }
    //   result.symbol = (symbol ^ (rhs.symbol));
    //   if(result.symbol&&current>0) {
    //     result-=1;
    //   }
    //   *this = result;
    //   return *this;
    // }

    long long cnt=1;
    while(div<=tmp) {
      div*=10;
      num*=10;
      cnt++;
    }

    for(int i=1;i<=cnt;++i){
      if(div<=0||num<=0)break;
      while(tmp>=div) {
        tmp-=div;
        result+=num;
      }
      div>>=1;
      num>>=1;
    }

    result.symbol = (symbol ^ (rhs.symbol));
    if(result.symbol&&tmp>0) {
      result+=-1;
    }
    *this = result;
    return *this;
  }

  inline int2048 operator/(int2048 lhs, const int2048 &rhs) {
    return lhs.operator/=(rhs);
  }

  inline int2048 &int2048::operator%=(const int2048 &rhs) {

    *this=*this-(*this/rhs)*rhs;
    return *this;
  }

  inline int2048 operator%(int2048 lhs, const int2048 &rhs) {
    return lhs.operator%=(rhs);
  }
} // namespace sjtu

#endif