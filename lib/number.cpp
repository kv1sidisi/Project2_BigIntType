#include "number.h"
#include <cstring>

Consts consts;

int2023_t from_int(int32_t decimal_number) {
  int2023_t result;
  int bait_index = consts.max_bait;

  if (decimal_number >= 0) {
    for (int j = 0; j < sizeof(uint32_t); ++j) {
      result.data[bait_index--] = decimal_number & 0xFF;
      decimal_number >>= consts.shift;
    }

  } else {
    decimal_number = abs(decimal_number);
    for (int j = 0; j < sizeof(uint32_t); ++j) {
      result.data[bait_index--] = decimal_number & 0xFF;
      decimal_number >>= consts.shift;
    }

    for (int i = consts.max_bait; i >= 0; i--) {
      result.data[i] = ~result.data[i];
    }
    result = result + from_int(1);
  }

  return result;
}

void operator+=(int2023_t& result, const int2023_t& add){
  int overflow = 0;
  for (int i = consts.max_bait; i >= 0; i--) {
    result.data[i] = result.data[i] + add.data[i] + overflow;
    overflow = (result.data[i] + add.data[i] - 1 > result.data[i]) ? 1 : 0;
  }
}


//int2023_t from_string(const char* buff) {
//  int2023_t result;
//  int number_from_string;
//  int length_of_string = std::strlen(buff);
//  int2023_t tenth_grade = from_int(1);
//  int2023_t ten = from_int(10);
//  bool sign = 0;
//
//  if (buff[0] == '-') {
//    sign = 1;
//  }
//
//  for (int i = length_of_string - 1; i >= sign; i--) {
//    number_from_string = buff[i] - '0';
//    result = result + from_int(number_from_string) * tenth_grade;
//    tenth_grade = tenth_grade * ten;
//  }
//
//  if (sign) {
//    for (unsigned char& i : result.data) {
//      i = ~i;
//    }
//    result = result + from_int(1);
//  }
//
//  return result;
//}


int2023_t from_string(const char* buff) {
  int2023_t result;

  size_t size = strlen(buff);
  bool isNegative = buff[0] == '-';
  char number[size + 1];
  strncpy(number, isNegative ? buff + 1 : buff, size);
  number[size] = '\0';

  int bitPosition = 2023;
  while (std::strlen(number) > 0 && number[0] != '0') {
    int carry = 0;
    int n = std::strlen(number);
    for (int i = 0; i < n; ++i) {
      int num = carry * 10 + (number[i] - '0');
      carry = num % 2;
      number[i] = (num / 2) + '0';
    }
    if (number[0] == '0' && n > 1) {
      std::memmove(number, number + 1, n);
      number[n-1] = '\0';
    }
    if (bitPosition >= 0){
      result.data[bitPosition / 8] |= (carry << (7 - bitPosition % 8));

    }
    bitPosition--;
  }

  if (isNegative) {
    bool carry = true;
    for (int i = 253 - 1; i >= 0; --i) {
      result.data[i] = ~result.data[i];
      if (carry) {
        if (result.data[i] == 255) {
          result.data[i] = 0;
        } else {
          result.data[i]++;
          carry = false;
        }
      }
    }
  }

  return result;
}



int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs) {
  uint32_t overflow = 0;
  int2023_t result;

  for (int i = consts.max_bait; i >= 0; i--) {
    result.data[i] = lhs.data[i] + rhs.data[i] + overflow;
    overflow = (lhs.data[i] + rhs.data[i] - 1 > result.data[i]) ? 1 : 0;
  }

  return result;
}

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs) {
  int2023_t result;
  int2023_t current = lhs;

  for (int i = consts.max_bait; i >= 0; i--) {
    for (int j = 0; j < 8; j++) {
      bool t = ((rhs.data[i] >> j) & 1);
      if (t) {
        result = result + current;
      }

      for (int k = 0; k < consts.max_bait; k++) {
        bool m = ((current.data[k + 1] >> 7) & 1);
        current.data[k] <<= 1;
        current.data[k] |= m;
      }
      current.data[consts.max_bait] <<= 1;
    }
  }

  return result;
}

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs) {
  int2023_t result;
  int2023_t invert;

  for (int i = consts.max_bait; i >= 0; i--) {
    invert.data[i] = ~rhs.data[i];
  }

  invert = invert + from_int(1);
  result = lhs + invert;

  return result;
}



bool FirstBiggerOrEqualThanSecond(int2023_t& lhs, int2023_t& rhs) {
  int2023_t result;

  if (((lhs.data[0] >> 7) & 1) == 0 and ((rhs.data[0] >> 7) & 1) == 1) {
    return true;
  } else if (((lhs.data[0] >> 7) & 1) == 1 and ((rhs.data[0] >> 7) & 1) == 0) {
    return false;
  } else {
    result = lhs - rhs;
    if (result == from_int(0)) {
      return true;
    }
    if (((result.data[0] >> 7) & 1) == 0) {
      return true;
    } else {
      return false;
    }
  }
}

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs) {
  int2023_t result;
  int2023_t remainder;
  int2023_t divisor;
  int2023_t divinder;
  divisor = rhs;
  divinder = lhs;
  bool sign = false;

  if (((divinder.data[0] >> 7) & 1) == 1 && ((divisor.data[0] >> 7) & 1) == 1) {
    for (int i = consts.max_bait; i >= 0; i--) {
      divinder.data[i] = ~divinder.data[i];
    }
    divinder = divinder + from_int(1);

    for (int i = consts.max_bait; i >= 0; i--) {
      divisor.data[i] = ~divisor.data[i];
    }
    divisor = divisor + from_int(1);

    sign = false;
  } else if (((divinder.data[0] >> 7) & 1) == 1 && ((divisor.data[0] >> 7) & 1) == 0) {

    for (int i = consts.max_bait; i >= 0; i--) {
      divinder.data[i] = ~divinder.data[i];
    }
    divinder = divinder + from_int(1);
    sign = true;

  } else if (((divinder.data[0] >> 7) & 1) == 0 && ((divisor.data[0] >> 7) & 1) == 1) {

    for (int i = consts.max_bait; i >= 0; i--) {
      divisor.data[i] = ~divisor.data[i];
    }
    divisor = divisor + from_int(1);
    sign = true;

  }

  if (!FirstBiggerOrEqualThanSecond(divinder, divisor)) {
    if (divisor != divinder) {
      return result;
    }
  }

  if (divisor == divinder) {
    result = from_int(1);
    return result;
  }

  if (divinder == from_int(0) || divisor == from_int(0)) {
    return result;
  }

  for (int i = 0; i < consts.max_bait + 1; i++) {
    for (int j = 7; j >= 0; j--) {
      for (int k = 0; k < consts.max_bait; k++) {
        bool m = ((remainder.data[k + 1] >> 7) & 1);
        remainder.data[k] <<= 1;
        remainder.data[k] |= m;
      }
      remainder.data[consts.max_bait] <<= 1;

      remainder.data[consts.max_bait] |= ((divinder.data[i] >> j) & 1);
      if (FirstBiggerOrEqualThanSecond(remainder, divisor)) {
        remainder = remainder - divisor;
        result.data[i] |= (1 << j);
      }
    }
  }

  if (sign) {
    for (int i = consts.max_bait; i >= 0; i--) {
      result.data[i] = ~result.data[i];
    }
    result = result + from_int(1);
  }
  return result;
}

bool operator==(const int2023_t& lhs, const int2023_t& rhs) {
  int2023_t result = lhs - rhs;
  int count = 0;
  for (unsigned char i : result.data) {
    if (i == 0) {
      count++;
    }
  }
  if (count == consts.max_bait + 1) {
    return true;
  }
  return false;
}

bool operator!=(const int2023_t& lhs, const int2023_t& rhs) {
  int2023_t result = lhs - rhs;
  int count = 0;
  for (unsigned char i : result.data) {
    if (i == 0) {
      count++;
    }
  }
  if (count != consts.max_bait + 1) {
    return true;
  }
  return false;
}

std::ostream& operator<<(std::ostream& stream, const int2023_t& value) {
  for (unsigned char i : value.data) {
    for (int j = 7; j >= 0; j--) {
      std::cout << ((i >> j) & 1);
    }
  }
  return stream;
}
