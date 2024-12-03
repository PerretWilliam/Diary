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
    // Vérifie si l'année est bissextile.
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
  }

  unsigned int is_day_valid(const Date &date)
  {
    // On cherche quel est le jour maximum grâce au mois saisit précédemment.
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
    // On affiche la date et on ajoute des 0 si nécessaire pour avoir ce format là : 01/01/2000 00:00
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
    // On vérifie si la date saisie est valide en regardant l'année, le mois, le jour, les heures et les minutes
    return !(date.year == 0 || date.month < 1 || date.month > 12 || date.day < 1 || date.day > is_day_valid(date) || date.hour < 0 || date.hour > 23 || date.minute < 0 || date.minute > 59);
  }

  Date create_date(const std::string &msg)
  {
    Date date;
    std::string entry;
    bool valid = false;

    do
    {
      std::cout << BLUE << msg << RESET << " > ";
      getline(std::cin, entry);

      // Vérifier la présence de séparateurs pour ajuster la lecture
      if (entry.find('/') != std::string::npos || entry.find(':') != std::string::npos)
      {
        // Cas avec séparateurs
        std::istringstream ss(entry);
        char sep_date, sep_heure;
        ss >> date.day >> sep_date >> date.month >> sep_date >> date.year >> date.hour >> sep_heure >> date.minute;

        // Vérifier que les séparateurs sont corrects
        if (sep_date != '/' || sep_heure != ':')
        {
          std::cerr << RED << "Erreur : format de la date incorrect !" << RESET << std::endl;
          continue;
        }
      }
      else
      {
        try
        {
          date.day = std::stoi(entry.substr(0, 2));
          date.month = std::stoi(entry.substr(3, 2));
          date.year = std::stoi(entry.substr(6, 4));
          date.hour = std::stoi(entry.substr(11, 2));
          date.minute = std::stoi(entry.substr(14, 2));
        }
        catch (const std::invalid_argument &e)
        {
          std::cerr << RED << "Erreur : format de la date incorrect !" << RESET << std::endl;
          continue;
        }
        catch (const std::out_of_range &e)
        {
          std::cerr << RED << "Erreur : format de la date incorrect !" << RESET << std::endl;
          continue;
        }
      }

      // Vérifie la validité de la date
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
    // Permet de vérifier si la date de fin de l'évènement est plus petite que celle de début.
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

    // On stocke la date dans une chaîne de charactère pour la séparer ensuite.
    std::string date_str;
    std::getline(current_date, date_str);

    std::stringstream new_date(date_str);
    char sep_date, sep_time;

    // Lecture de la date.
    new_date >> date.day >> sep_date >> date.month >> sep_date >> date.year;

    // Permet de sauter l'espace entre la date et l'heure.
    new_date.get();

    // Lecture du temps.
    new_date >> date.hour >> sep_time >> date.minute;
    return date;
  }
}