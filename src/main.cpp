#include <iostream>
#include <vector>

#include "../includes/agenda.hpp"

#define RESET "\033[0m"
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

int main()
{
  diary::Global global = {diary::STATE::LAUNCH};

  diary::clear_screen();

  std::vector<diary::Menu> menus = diary::initialize_menu();
  diary::start_menu(global, menus);

  return EXIT_SUCCESS;
}