#include <iostream>
#include <random>
#include <vector>
#include <fstream>
#include <sstream>

#include "../includes/agenda.hpp"
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

namespace diary
{
  void clear_screen()
  {
    std::cout << "\033[2J\033[1;1H";
  }
  void print_error(const std::string &msg)
  {
    std::cerr << RED << "Erreur : " << msg << RESET << std::endl;
  }
  std::string getUID(std::size_t len)
  {
    // Génère un ID aléatoire pour les évènements.
    std::vector<char> x = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    std::string uid(len, '\0');
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dis(0, x.size() - 1);
    for (char &c : uid)
    {
      c = x[dis(gen)];
    }
    return uid;
  }
  bool diary_exist(const std::string &diary_name)
  {
    std::ifstream file(diary_name + ".txt");
    return file.is_open();
  }

  // Input
  void getUserInput(char &c, const std::string &msg)
  {
    std::cout << msg;
    std::cin >> c;

    // Cela évite la répétition du msg en vidant le buffer de std::cin.
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << '\n';
  }
  std::string create_title(const std::string &msg)
  {
    std::string title;
    do
    {
      std::cout << MAGENTA << msg << RESET << " > ";
      std::getline(std::cin, title);
      if (title.empty())
      {
        print_error("Erreur : saisissez un titre non vide !");
      }
      else if (title.find('/') != -1)
      {
        print_error("Erreur : saisissez un titre sans '/' !");
      }
    } while (title.empty() || title.find('/') != -1);
    return title;
  }
  std::string create_description(const std::string &msg)
  {
    std::string line = "";
    std::string description = "";

    std::cout << MAGENTA << msg << RESET << " > ";
    while (std::getline(std::cin, line))
    {
      if (line == ".")
      {
        if (!description.empty())
        {
          description.pop_back();
        }
        return description;
      }
      description += line + '\n';
    }
    return description;
  }
  bool ask_same_date()
  {
    print_choice_menu("Voulez-vous que la date de début soit également la date de fin ?");
    char c;
    do
    {
      getUserInput(c, "> ");
      switch (c)
      {
      case '1':
        return true;
      case '2':
        return false;
      default:
        print_error("ce n'est pas une réponse valide !");
        break;
      }
    } while (true);
  }
  bool ask_load_diary(Global &global)
  {
    std::string diary_name;
    char selection;

    do
    {
      std::cout << BLUE << "Donnez le nom de l'agenda à charger (ou /exit pour retourner au menu principale)." << RESET << " > ";
      std::getline(std::cin, diary_name);

      if (diary_name == "/exit")
      {
        return false;
      }
      else if (!diary_exist(diary_name))
      {
        // Si l'agenda n'est pas trouvé on affiche une erreur.
        print_error("l'agenda demandé n'existe pas.");
      }
      else
      {
        // Si l'agenda est trouvé on le charge.
        global.diary = load_diary(diary_name);
        return true;
      }
    } while (true);
  }

  // Events
  void print_events(const Global &global)
  {
    // Affichage des informations des évènements s'il en existe.
    if (!global.diary.events.empty())
      for (const Event &event : global.diary.events)
      {
        std::cout << '\n';
        std::cout << date::print_date(event.date_begin);
        std::cout << event.title << std::endl;
        std::cout << event.description << std::endl;
        std::cout << date::print_date(event.date_end);
      }
    else
    {
      std::cout << '\n';
      std::cout << YELLOW << "Aucun évènement." << RESET << std::endl;
    }
  }
  Event create_event()
  {
    Event event;
    event.id = getUID(10);
    event.title = create_title("Saisissez le titre de l'évènement");
    event.description = create_description("Saisissez la description de l'évènement (laissez un '.' pour quitter)");

    bool condition = false;
    do
    {
      event.date_begin = date::create_date("Saisissez la date de début (jj/mm/aaaa hh:mm)");
      std::cout << '\n';

      bool is_same_date = ask_same_date();

      if (!is_same_date)
      {
        event.date_end = date::create_date("Saisissez la date de fin (jj/mm/aaaa hh:mm)");
      }
      else
      {
        event.date_end = event.date_begin;
      }

      if (date::is_reversed(event.date_begin, event.date_end))
      {
        print_error("les dates sont inversées !");
      }
      else
      {
        condition = true;
      }
    } while (!condition);
    return event;
  }
  void add_event(Global &global)
  {
    global.diary.events.push_front(create_event());
  }
  void delete_event(Global &global)
  {
    bool is_found = false;
    std::string event_title = create_title("Donnez le titre de l'évènement à supprimer");
    std::forward_list<Event>::iterator before = global.diary.events.before_begin();
    std::forward_list<Event>::iterator it = global.diary.events.begin();

    while (it != global.diary.events.end())
    {
      if (it->title == event_title)
      {
        if (is_found)
        {
          std::cout << "ID : " << YELLOW << it->id << RESET << " - Description : " << it->description << std::endl;
        }
        else
        {
          is_found = true;
          std::cout << "Il existe un/plusieurs évènement(s) : " << YELLOW << it->title << RESET << ", voici son/leur ID : " << std::endl;
          std::cout << "ID : " << YELLOW << it->id << RESET << " - Description : " << it->description << std::endl;
        }
      }
      ++before;
      ++it;
    }

    if (!is_found)
    {
      print_error("aucun évènement ne comporte ce nom !");
      return;
    }

    do
    {
      before = global.diary.events.before_begin();
      it = global.diary.events.begin();
      std::string id = create_title("Donnez l'ID de l'évènement à supprimer (ou 'r' pour retourner au menu)");
      is_found = false;

      if (id == "r")
      {
        global.state == STATE::MENU;
        return;
      }

      while (it != global.diary.events.end())
      {
        if (it->id == id)
        {
          global.diary.events.erase_after(before);
          std::cout << "L'évènement avec l'ID " << YELLOW << id << RESET << " a bien été supprimé." << std::endl;
          global.diary.is_modify = true;
          is_found = true;
          break;
        }
        ++before;
        ++it;
      }

      if (!is_found)
      {
        print_error("l'ID " + id + " n'existe pas !");
      }
    } while (!is_found);
  }

  // Diary
  void print_diary(const Global &global)
  {
    clear_screen();
    // Show diary.
    std::cout << global.diary.title << std::endl;
    std::cout << global.diary.description << std::endl;
    print_events(global);

    std::cout << MAGENTA << "Taper entrée pour quitter...";
    std::cin.get();
  }
  Diary create_diary()
  {
    Diary diary;
    diary.title = create_title("Saisissez le titre de l'agenda");
    diary.description = create_description("Saisissez la description de l'agenda (laissez un '.' pour quitter)");
    return diary;
  }

  // HTML export
  void export_description_HTML(std::ofstream &file, const std::string description)
  {
    for (char c : description)
    {
      if (c == '\n')
      {
        file << "<br>\n";
      }
      else
      {
        file << c;
      }
    }
  }
  void export_event_HTML(std::ofstream &file, const Event &event)
  {
    file << "<tr\n>";
    file << "<td>" << event.id << "</td>\n";
    file << "<td>" << event.title << "</td>\n";

    file << "<td><p>";
    export_description_HTML(file, event.description);
    file << "</p></td>\n";

    file << "<td>" << date::print_date(event.date_begin) << "</td>\n";
    file << "<td>" << date::print_date(event.date_end) << "</td>\n";

    file << "</tr>";
  }
  void export_diary_HTML(const Global &global)
  {
    std::string file_name = global.diary.title + ".html";
    std::ofstream file(file_name);

    if (!file.is_open())
    {
      print_error("impossible d'exporter l'agenda " + file_name + " !");
      return;
    }

    // Head of HTML file
    file << "<!DOCTYPE html>\n<html lang=\"fr\">\n<head>\n";
    file << "<meta charset=\"UTF-8\">\n<title>" << global.diary.title << "</title>\n";
    file << "</head>\n<body>\n";

    // Title & description of diary
    file << "<h1>" << global.diary.title << "</h1>\n";

    file << "<p>";
    export_description_HTML(file, global.diary.description);
    file << "</p>\n";

    file << "<h2>Événements</h2>\n";
    if (global.diary.events.empty())
    {
      file << "<p>Aucun évènement dans l'agenda.</p>\n";
    }
    else
    {
      file << "<table>\n";
      file << "<tr>\n";
      file << "<th>ID</th>\n";
      file << "<th>Titre</th>\n";
      file << "<th>Description</th>\n";
      file << "<th>Date de début</th>\n";
      file << "<th>Date de fin</th>\n";
      file << "</tr>\n";
      for (const Event &event : global.diary.events)
      {
        export_event_HTML(file, event);
      }
      file << "</table>\n";
    }

    file << "</body>\n</html>\n";
    file.close();

    std::cout << GREEN << "L'agenda a été exporté dans le fichier : " << CYAN << file_name << GREEN << '.' << std::endl;
  }

  // Save
  void save_event(std::ofstream &file, Event event)
  {
    file << event.id << '\n';
    file << event.title << '\n';
    file << event.description << '\n';
    file << ".\n";
    file << date::print_date(event.date_begin);
    file << date::print_date(event.date_end);
  }
  void save_diary(const Global global)
  {
    std::string file_name = global.diary.title + ".txt";
    std::ofstream file(file_name);

    if (!file.is_open())
    {
      print_error("impossible d'enregistrer l'agenda : " + file_name + " !");
      return;
    }

    file << global.diary.title << '\n';
    file << global.diary.description << '\n';
    file << ".\n";
    for (const Event event : global.diary.events)
    {
      save_event(file, event);
    }
    file.close();
    std::cout << GREEN << "L'agenda a été enregistré dans le fichier " << CYAN << file_name << GREEN << '.' << RESET << std::endl;
  }
  void confirm_save(const Global &global)
  {
    char selection;
    do
    {
      print_choice_menu("L'agenda a été modifié ! L'enregistrer ?");
      getUserInput(selection, "> ");
      switch (selection)
      {
      case '1':
        // Enregistrement de l'agenda.
        save_diary(global);
        return;

      case '2':
        // On enregistre pas l'agenda.
        return;

      default:
        print_error("réponse invalide !");
        break;
      }
    } while (true);
  }

  // Load
  std::string load_description(std::ifstream &file)
  {
    std::string line;
    std::string description;

    while (std::getline(file, line))
    {
      if (line != ".")
      {
        description += line + '\n';
      }
      else
      {
        description.pop_back();
        return description;
      }
    }
    return description;
  }
  Diary load_diary(const std::string &diary_name)
  {
    Diary diary;
    std::string file_name = diary_name + ".txt";
    std::ifstream file(file_name);

    if (!file.is_open())
    {
      print_error("impossible d'ouvrir le fichier " + file_name + '.');
      return {};
    }

    std::getline(file, diary.title);
    diary.description = load_description(file);

    Event event;
    std::string line;

    while (std::getline(file, event.id) && !event.id.empty())
    {
      std::getline(file, event.title);
      event.description = load_description(file);

      std::getline(file, line);
      std::istringstream date_begin(line);
      event.date_begin = date::read_date(date_begin);

      std::getline(file, line);
      std::istringstream date_end(line);
      event.date_end = date::read_date(date_end);

      diary.events.push_front(event);
    }

    file.close();
    std::cout << GREEN << "L'agenda a été chargé." << RESET << std::endl;
    return diary;
  }

  // Menus
  std::vector<diary::Menu> initialize_menu()
  {
#ifdef _WIN32
    system(chcp 65001);
#endif
    // Start menu.
    return {{{"\033[35mBienvene, que souhaitez-vous faire ?\033[0m", {{'1', "Crée un nouvel agenda", [](Global &global) -> void
                                                                       {
                                                                         global.state = STATE::CREATE_AGENDA;
                                                                         global.diary = create_diary();
                                                                         global.state = STATE::MENU;
                                                                       }},
                                                                      {'2', "Charger un agenda", [](Global &global) -> void
                                                                       {
                                                                         global.state = STATE::LOAD_AGENDA;
                                                                       }},
                                                                      {'q', "Quitter", [](Global &global) -> void
                                                                       { global.state = STATE::EXIT; }}}},
             // Main menu
             {"\033[35mBienvenue dans l'agenda.\033[0m", {{'1', "Modifier l'agenda", [](Global &global) -> void
                                                           {
                                                             global.state = STATE::CREATE_AGENDA;
                                                             global.diary.title = create_title("Saisissez le titre de l'agenda");
                                                             global.diary.description = create_description("Saisissez la description de l'agenda (laissez un '.' pour quitter)");
                                                             global.diary.is_modify = true;
                                                             global.state = STATE::MENU;
                                                           }},
                                                          {'2', "Afficher l'agenda", [](Global &global) -> void
                                                           {
                                                             print_diary(global);
                                                           }},
                                                          {'3', "Ajouter un événement", [](Global &global) -> void
                                                           {
                                                             global.state = STATE::ADD_EVENT;
                                                             add_event(global);
                                                             global.state = STATE::MENU;
                                                           }},
                                                          {'4', "Supprimer un événement", [](Global &global) -> void
                                                           {
                                                             global.state = STATE::DEL_EVENT;
                                                             delete_event(global);
                                                             global.state = STATE::MENU;
                                                           }},
                                                          {'5', "Exporter au format HTML", [](Global &global) -> void
                                                           {
                                                             export_diary_HTML(global);
                                                           }},
                                                          {'6', "Enregistrer l'agenda", [](Global &global) -> void
                                                           {
                                                             save_diary(global);
                                                             global.diary.is_modify = false;
                                                           }},
                                                          {'q', "Quitter", [](Global &global) -> void
                                                           {
                                                             if (global.diary.is_modify)
                                                             {
                                                               confirm_save(global);
                                                             }
                                                             global.state = STATE::EXIT;
                                                           }}}},
             // Add event menu
             {"Menu ajout événement", {{'r', "Retour", [](diary::Global &global) -> void
                                        { global.state = diary::STATE::MENU; }}}},

             // Delete event menu
             {"Menu suppression événement", {{'r', "Retour", [](diary::Global &global) -> void
                                              { global.state = diary::STATE::MENU; }}}}}};
  }
  void print_choice_menu(const std::string &msg)
  {
    std::cout << MAGENTA << msg << std::endl
              << GREEN << "1. Oui" << std::endl
              << RED << "2. Non" << RESET << std::endl;
  }
  void show_menu(const Menu &menu)
  {
    std::cout << menu.label << std::endl;

    for (Entry entrys : menu.entrys)
    {
      std::cout << '[' << entrys.input << "] " << entrys.label << std::endl;
    }
  }
  void start_menu(Global &global, const std::vector<diary::Menu> &menus)
  {
    char selection;
    do
    {
      show_menu(menus[0]);
      getUserInput(selection, "> ");
      if (selection == '1')
      {
        menus[0].entrys[0].launch(global);
        main_menu(global, menus);
      }
      else if (selection == '2')
      {
        menus[0].entrys[1].launch(global);
        bool is_loaded = ask_load_diary(global);
        if (is_loaded)
        {
          global.state = STATE::MENU;
          main_menu(global, menus);
        }
        else
        {
          global.state = STATE::MENU;
        }
      }
      else if (selection == 'q')
      {
        menus[0].entrys[2].launch(global);
      }
      else
      {
        print_error("aucune action ne correspond à la sélection.");
      }
    } while (global.state != diary::STATE::EXIT);
  }
  void main_menu(Global &global, const std::vector<diary::Menu> &menu)
  {
    char selection;
    do
    {
      show_menu(menu[1]);
      getUserInput(selection, "> ");
      if (selection == '1')
      {
        menu[1].entrys[0].launch(global);
      }
      else if (selection == '2')
      {
        menu[1].entrys[1].launch(global);
      }
      else if (selection == '3')
      {
        menu[1].entrys[2].launch(global);
      }
      else if (selection == '4')
      {
        menu[1].entrys[3].launch(global);
      }
      else if (selection == '5')
      {
        menu[1].entrys[4].launch(global);
      }
      else if (selection == '6')
      {
        menu[1].entrys[5].launch(global);
      }
      else if (selection == 'q')
      {
        menu[1].entrys[6].launch(global);
      }
      else
      {
        print_error("aucune action ne correspond à la sélection.");
      }
    } while (global.state != diary::STATE::EXIT);
  }
}
