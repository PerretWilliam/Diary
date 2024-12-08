#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <limits>

#include "../includes/date.hpp"

#define RESET "\033[0m"
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

namespace date
{
  bool is_leap_year(unsigned int year)
  {
    // Check if it is a leap year
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
  }

  unsigned int is_day_valid(const Date &date)
  {
    // We return the maximum day for March it's 31, for April it's 30, ect...
    unsigned int max_day;
    if (date.month == 1 || date.month == 3 || date.month == 5 || date.month == 7 || date.month == 8 || date.month == 10 || date.month == 12)
      max_day = 31;
    else if (date.month == 4 || date.month == 6 || date.month == 9 || date.month == 11)
      max_day = 30;
    else if (date.month == 2)
      // Pour le mois de février, si l'année est bissextile, on prend 29 sinon 28.
      max_day = is_leap_year(date.year) ? 29 : 28;

    return max_day;
  }

  std::string print_date(const Date &date)
  {
    // We print the date and we fill it with 0 to have this format : 01/01/2000 00:00
    std::ostringstream chaine;
    chaine << std::setw(2) << std::setfill('0') << date.day
           << '/' << std::setw(2) << std::setfill('0') << date.month
           << '/' << date.year
           << ' ' << std::setw(2) << std::setfill('0') << date.hour
           << ':' << std::setw(2) << std::setfill('0') << date.minute << '\n';
    return chaine.str();
  }

  bool is_valid(const Date &date)
  {
    // We check if the date is valid
    return !(date.year == 0 || date.month < 1 || date.month > 12 || date.day < 1 || date.day > is_day_valid(date) || date.hour < 0 || date.hour > 23 || date.minute < 0 || date.minute > 59);
  }

  Date create_date(const std::string &msg)
  {
    /* The date have to respect this format : jj/mm/aaaa hh:mm*/
    Date date;
    std::string entry;
    bool valid = false;

    do
    {
      std::cout << MAGENTA << msg << RESET << " > ";
      getline(std::cin, entry);

      std::istringstream ss(entry);
      char sep_date, sep_heure;
      ss >> date.day >> sep_date >> date.month >> sep_date >> date.year >> date.hour >> sep_heure >> date.minute;

      // Check if the separators are correct
      if (sep_date != '/' || sep_heure != ':')
      {
        std::cerr << RED << "Erreur : format de la date incorrect !" << RESET << std::endl;
        continue;
      }

      // Check if the date is valid
      valid = is_valid(date);
      if (!valid)
      {
        std::cerr << RED << "Erreur : date non valide !" << RESET << std::endl;
        continue;
      }
    } while (!valid);

    return date;
  }

  bool is_reversed(const Date &date_begin, const Date &date_end)
  {
    // check if the dates are reversed
    if (date_end.year != date_begin.year)
      return date_end.year < date_begin.year;
    if (date_end.month != date_begin.month)
      return date_end.month < date_begin.month;
    if (date_end.day != date_begin.day)
      return date_end.day < date_begin.day;
    if (date_end.hour != date_begin.hour)
      return date_end.hour < date_begin.hour;
    return date_end.minute < date_begin.minute;
  }

  Date read_date(std::istream &current_date)
  {
    Date date;

    // We save the date in a string to séparate them after
    std::string date_str;
    std::getline(current_date, date_str);

    std::stringstream new_date(date_str);
    char sep_date, sep_time;

    // We set the day, the month and the year with the separator
    new_date >> date.day >> sep_date >> date.month >> sep_date >> date.year;

    // Avoid the space between the year and the hour
    new_date.get();

    // Set oh the hour and minute
    new_date >> date.hour >> sep_time >> date.minute;
    return date;
  }
}