#ifndef DATE_HPP
#define DATE_HPP

#include <string>

namespace date
{
  struct Date
  {
    int year;
    unsigned int day, month, hour, minute;
  };

  bool is_leap_year(unsigned int annee);

  unsigned int is_day_valid(const Date &date);

  std::string print_date(const Date &date);

  Date create_date(const std::string &msg = "");

  bool is_reversed(const Date &date_debut, const Date &date_fin);

  Date read_date(std::istream &current_date);
}
#endif
