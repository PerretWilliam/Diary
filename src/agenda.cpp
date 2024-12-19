#include <iostream>
#include <random>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
#include <sys/stat.h>

#include "../includes/agenda.hpp"
#include "../includes/date.hpp"
#include "../includes/color.hpp"

#define FILE_EXTENTION ".txt"

namespace fs = std::filesystem;

// Create of a namespace
namespace diary
{
  void clear_screen()
  {
    // This clear the screen
    std::cout << "\033[2J\033[1;1H";
  }
  void print_error(const std::string &msg)
  {
    // Print an error with the message given
    std::cerr << RED << "Erreur : " << msg << RESET << std::endl;
  }
  std::string getUID(std::size_t len)
  {
    // Generate an random ID for the events with ten character between [1,9] and [a,f] example : a59ed87bf4
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

  // File
  bool diary_exist(const fs::path &diary_path)
  {
    // Check if the diary exists, if not return false
    std::ifstream file(diary_path.string() + FILE_EXTENTION);
    return file.is_open();
  }
  void open_file(const std::string &file_name)
  {
    // When we export an file like HTML, we open it
    std::string command;

    // Check on which operating system is the user and do the right command
#ifdef _WIN32
    command = "start " + file_name;
#elif __APPLE__
    chmod(file_name.c_str(), 0644);
    command = "open " + file_name;
#elif __linux__
    chmod(file_name.c_str(), 0644);
    command = "xdg-open " + file_name + " > /dev/null 2>&1";
#else
    print_error("ouverture non supportée sur ce système.")
#endif

    // If the command can be done for any reason, we print an error
    if (std::system(command.c_str()) != 0)
    {
      print_error("impossible d'ouvrir le fichier.");
    }
  }
  void create_export_directory(const Global &global)
  {
    if (!fs::exists(global.export_path))
    {
      if (fs::create_directory(global.export_path))
      {
        std::cout << GREEN << "Le dossier 'export' a été crée avec succès à l'emplacement : " << YELLOW << fs::absolute(global.export_path) << GREEN << '.' << RESET << std::endl;
      }
      else
      {
        print_error("impossible de créer le dossier.");
      }
    }
  }
  void delete_diary(const fs::path &diary_path)
  {
    // If we can remove the diary, then we print a message to confirm it, else, we print an error
    if (fs::remove(diary_path.string() + FILE_EXTENTION))
    {
      std::cout << GREEN << "L'agenda à l'emplacement " << BLUE << diary_path << GREEN << " a bien été supprimé" << RESET << std::endl;
    }
    else
    {
      print_error("impossible de supprimer l'agenda.");
    }
  }
  void print_saved_diary(const fs::path &export_path)
  {
    bool has_diary = false;

    // We print all the diary saved in the export folder with .txt extension
    for (const auto &entry : fs::directory_iterator(export_path))
    {
      if (entry.path().extension() == FILE_EXTENTION)
      {
        if (!has_diary)
        {
          std::cout << MAGENTA << "Voici les agendas sauvegardés dans le dossier 'export' : " << RESET << std::endl;
          has_diary = true;
        }
        std::cout << "- " << entry.path().stem() << std::endl;
      }
    }
    if (!has_diary)
    {
      std::cout << "Aucun agenda sauvegardé dans le dossier 'export'." << std::endl;
    }
  }

  // Input
  void getUserInput(char &c, const std::string &msg)
  {
    std::cout << msg;
    std::cin >> c;

    // Prevent some bugs, for example, the message appears two times without those lines
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << '\n';
  }
  std::string create_title(const std::string &msg)
  {
    std::string title;
    do
    {
      // The title cannot be empty or have a '/', print an error if this is the case
      std::cout << MAGENTA << msg << RESET << " > ";
      std::getline(std::cin, title);
      if (title.empty())
      {
        print_error("saisissez un titre non vide !");
      }
      else if (title.find('/') != -1)
      {
        print_error("saisissez un titre sans '/' !");
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
      // While the user didn't just put '.' on a line, we add the line else we return the description
      if (line == ".")
      {
        // We check if the description is not empty because if it is, the pop_back() throw an error
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
    char c;
    do
    {
      // We ask to the user if he wants date_begin = date_end 1. for yes and 2. for no
      print_choice_menu("Voulez-vous que la date de début soit également la date de fin ?");
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
    /* This function return a bool because we want to know if the user load the diary
    if true, we go to the menu of the diary, in the other case, we stay on the first menu */
    std::string diary_name;
    char selection;

    do
    {
      /* The user can load an diary or just return to the menu with /exit
      This is why we don't want a '/' in the name of the diary */
      print_saved_diary(global.export_path);
      std::cout << MAGENTA << "Donnez le nom de l'agenda à charger (ou /exit pour retourner au menu principal)" << RESET << " > ";
      std::getline(std::cin, diary_name);

      if (diary_name == "/exit")
      {
        clear_screen();
        return false;
      }
      else if (!diary_exist(global.export_path / diary_name))
      {
        clear_screen();
        // If the diary can't be found, we print an error
        print_error("l'agenda demandé n'existe pas.");
      }
      else
      {
        clear_screen();
        // Si l'agenda est trouvé on le charge.
        global.diary = load_diary(global.export_path / diary_name);
        return true;
      }
    } while (true);
  }
  bool ask_delete_diary(Global &global)
  {
    // This is like the ask_load_diary() function, we delete the diary if 1. is selected
    std::string diary_name;
    char selection;

    do
    {
      print_saved_diary(global.export_path);
      std::cout << MAGENTA << "Donnez le nom de l'agenda à supprimer (ou /exit pour retourner au menu principal)" << RESET << " > ";
      std::getline(std::cin, diary_name);

      if (diary_name == "/exit")
      {
        clear_screen();
        return false;
      }
      else if (!diary_exist(global.export_path / diary_name))
      {
        clear_screen();
        // Si l'agenda n'est pas trouvé on affiche une erreur.
        print_error("l'agenda demandé n'existe pas.");
      }
      else
      {
        clear_screen();
        // Si l'agenda est trouvé on le supprime.
        delete_diary(global.export_path / diary_name);
        return true;
      }
    } while (true);
  }

  // Events
  void print_events(const Global &global)
  {
    // We print the events if they exist
    if (!global.diary.events.empty())
    {
      // If the events is not empty, we print all the events
      for (const Event &event : global.diary.events)
      {
        std::cout << '\n';
        std::cout << date::print_date(event.date_begin);
        std::cout << event.title << std::endl;
        std::cout << event.description << std::endl;
        std::cout << date::print_date(event.date_end);
      }
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

    // We start by generate his ID
    event.id = getUID(10);

    // Like for the diary, we create his title and description
    event.title = create_title("Saisissez le titre de l'évènement");
    event.description = create_description("Saisissez la description de l'évènement (laissez un '.' pour quitter)");

    bool condition = false;
    do
    {
      /* The user has to give a date in this format (jj/mm/aaa hh:mm), we print an error he doesn't.
      We check if the dates are valid or if they are not reversed */
      event.date_begin = date::create_date("Saisissez la date de début (jj/mm/aaaa hh:mm)");
      std::cout << '\n';

      // If the user wants to have the same date then date_end = date_begin
      bool is_same_date = ask_same_date();

      if (!is_same_date)
      {
        event.date_end = date::create_date("Saisissez la date de fin (jj/mm/aaaa hh:mm)");
      }
      else
      {
        event.date_end = event.date_begin;
      }

      // Check if the dates are not reversed
      if (date::is_reversed(event.date_begin, event.date_end))
      {
        print_error("les dates sont inversées !");
      }
      else
      {
        condition = true;
      }
    } while (!condition);
    clear_screen();
    return event;
  }
  void add_event(Global &global)
  {
    // We add the event to the diary events
    global.diary.events.push_front(create_event());
    global.diary.is_modify = true;
  }
  void delete_event(Global &global)
  {
    bool is_found = false;

    // We start by ask the title of the event to delete
    std::string event_title = create_title("Donnez le titre de l'évènement à supprimer");

    // We iterate on all the list
    std::forward_list<Event>::iterator before = global.diary.events.before_begin();
    std::forward_list<Event>::iterator it = global.diary.events.begin();

    while (it != global.diary.events.end())
    {
      // If at least one title is equal to the title the user asks then we show their ID and their description
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

    // If we don't found any events with the name given, we return to the menu
    if (!is_found)
    {
      clear_screen();
      print_error("aucun évènement ne comporte ce nom !");
      return;
    }

    do
    {
      before = global.diary.events.before_begin();
      it = global.diary.events.begin();

      // We ask for the ID of the event to delete or 'r' to cancel
      std::string id = create_title("Donnez l'ID de l'évènement à supprimer (ou 'r' pour retourner au menu)");
      is_found = false;

      if (id == "r")
      {
        clear_screen();
        // Return to the menu
        global.state == STATE::MENU;
        return;
      }

      while (it != global.diary.events.end())
      {
        if (it->id == id)
        {
          // While we didn't find the event that match the id given, we continue
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
        clear_screen();
        // If the id is equal to no event, we print an error and we ask again
        print_error("l'ID " + id + " n'existe pas !");
      }
    } while (!is_found);
  }

  // Diary
  void print_diary(const Global &global)
  {
    // Print the diary.
    std::cout << global.diary.title << std::endl;
    std::cout << global.diary.description << std::endl;

    // Print the diary events
    print_events(global);

    std::cout << MAGENTA << "Tapez entrée pour quitter...";
    std::cin.get();
  }
  Diary create_diary()
  {
    Diary diary;

    // We just ask for his title and description
    diary.title = create_title("Saisissez le titre de l'agenda");
    diary.description = create_description("Saisissez la description de l'agenda (laissez un '.' pour quitter)");
    return diary;
  }

  // HTML export
  void export_description_HTML(std::ofstream &file, const std::string description)
  {
    // We search all the '\n' in the description to convert them to <br> for the HTML
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
    // We create an entry in the table
    file << "<tr\n>";

    // We put the ID, the title and the description
    file << "<td>" << event.id << "</td>\n";
    file << "<td>" << event.title << "</td>\n";

    file << "<td><p>";
    export_description_HTML(file, event.description);
    file << "</p></td>\n";

    // We put the date too
    file << "<td>" << date::print_date(event.date_begin) << "</td>\n";
    file << "<td>" << date::print_date(event.date_end) << "</td>\n";

    file << "</tr>";
  }
  void export_diary_HTML(const Global &global)
  {
    // We create all the HTML diary in the export folder
    std::string file_name = global.diary.title + ".html";
    std::ofstream file(global.export_path / file_name);

    if (!file.is_open())
    {
      // If the file cannot be open, print an error
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

    // Events in the diary if the diary has no event, put "no event in the diary"
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
      // Close the table
      file << "</table>\n";
    }

    // End of the HTML file and close the file
    file << "</body>\n</html>\n";
    file.close();

    // Confirm that the file has been export and open it
    std::cout << GREEN << "L'agenda a été exporté dans le fichier : " << CYAN << file_name << GREEN << '.' << YELLOW << std::endl;
    open_file((global.export_path / file_name).string());
  }

  // Save
  void save_event(std::ofstream &file, Event event)
  {
    // Save the event in this order : id, title, description with a '.' to mark his end, date_begin and date_end
    file << event.id << '\n';
    file << event.title << '\n';
    file << event.description << '\n';
    file << ".\n";
    file << date::print_date(event.date_begin);
    file << date::print_date(event.date_end);
  }
  void save_diary(const Global global)
  {
    // Create the diary file in 'txt' format
    std::string file_name = global.diary.title + FILE_EXTENTION;
    std::ofstream file(global.export_path / file_name);

    if (!file.is_open())
    {
      // If the file cannot be open, print an error
      print_error("impossible d'enregistrer l'agenda : " + file_name + " !");
      return;
    }

    // We save the title and description and the events if we have to
    file << global.diary.title << '\n';
    file << global.diary.description << '\n';
    file << ".\n";
    for (const Event event : global.diary.events)
    {
      save_event(file, event);
    }

    // Close the file and show a confirm message
    file.close();
    std::cout << GREEN << "L'agenda a été enregistré dans le fichier " << CYAN << file_name << GREEN << '.' << RESET << std::endl;
  }
  void confirm_save(const Global &global)
  {
    char selection;
    do
    {
      /* When the user quits the program, if there are modifications in the diary not saved,
      we ask if the user wants to save it */
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

    // While we can read a line in the file
    while (std::getline(file, line))
    {
      /* If the line contains only '.' then return the description.
      This is why we put only a '.' in the save description of the diary and events */
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
  Diary load_diary(const fs::path &diary_path)
  {
    Diary diary;
    std::ifstream file(diary_path.string() + FILE_EXTENTION);

    if (!file.is_open())
    {
      // If we can't open the file, we throw an error
      print_error("impossible d'ouvrir le fichier " + diary_path.string() + FILE_EXTENTION);
      return {};
    }

    // We load the title and the description
    std::getline(file, diary.title);
    diary.description = load_description(file);

    Event event;
    std::string line;

    // While we find an id, we add it to the event && if this id is not empty
    while (std::getline(file, event.id) && !event.id.empty())
    {
      // We load the title and the description
      std::getline(file, event.title);
      event.description = load_description(file);

      // We load the dates
      std::getline(file, line);
      std::istringstream date_begin(line);
      event.date_begin = date::read_date(date_begin);

      std::getline(file, line);
      std::istringstream date_end(line);
      event.date_end = date::read_date(date_end);

      // We add the loaded event to the diary events
      diary.events.push_front(event);
    }

    // We close the file, we show a confirm message and we return the diary loaded
    file.close();
    std::cout << GREEN << "L'agenda a été chargé." << RESET << std::endl;
    return diary;
  }

  // Menus
  void launch_entry(Global &global, size_t menu_index, char entry_index)
  {
    // We check if the entry selected is not empty
    if (!(global.menus[menu_index].entrys.empty()))
    {
      clear_screen();
      global.menus[menu_index].entrys[entry_index].launch(global);
    }
    else
    {
      print_error("impossible d'ouvrir le menu ou l'entrée !");
    }
  }
  std::vector<Menu> initialize_menu()
  {
    /* Each menu has a label and a list of entrys.
    The entrys can be found with their key, one label and an associated function, we launch this function if the entry is selected
    */

    clear_screen();
    // To show the accents on Windows
#ifdef _WIN32
    system("chcp 65001");
#endif

    // Start menu.
    return {{{"\033[35mBienvenue, que souhaitez-vous faire ?\033[0m", {
      {'1', {"Crée un nouvel agenda", [](Global &global) -> void 
      {
        global.state = STATE::CREATE_AGENDA;
        global.diary = create_diary();
        global.state = STATE::MENU;
        menu(global, 1);
      }
      }},
      {'2', {"Charger un agenda", [](Global &global) -> void
      {
        global.state = STATE::LOAD_AGENDA;
        bool is_loaded = ask_load_diary(global);
        if (is_loaded)
        {
        global.state = STATE::MENU;
        menu(global, 1);
        }
        else
        {
        global.state = STATE::MENU;
        }
      }
      }},
      {'3', {"Supprimer un agenda", [](Global &global) -> void
      {
        global.state = STATE::DEL_AGENDA;
        ask_delete_diary(global);
        global.state = STATE::MENU;
      }
      }},
      {'q', {"Quitter", [](Global &global) -> void
      { global.state = STATE::EXIT; }
      }}
    }},
         // Main menu
    {"\033[35mBienvenue dans l'agenda.\033[0m", {
      {'1', {"Modifier l'agenda", [](Global &global) -> void
      {
        global.state = STATE::CREATE_AGENDA;
        global.diary.title = create_title("Saisissez le titre de l'agenda");
        global.diary.description = create_description("Saisissez la description de l'agenda (laissez un '.' pour quitter)");
        global.diary.is_modify = true;
        global.state = STATE::MENU;
      }
      }},
      {'2', {"Afficher l'agenda", [](Global &global) -> void
      {
        print_diary(global);
      }
      }},
      {'3', {"Ajouter un événement", [](Global &global) -> void
      {
        global.state = STATE::ADD_EVENT;
        add_event(global);
        global.state = STATE::MENU;
      }
      }},
      {'4', {"Supprimer un événement", [](Global &global) -> void
      {
        global.state = STATE::DEL_EVENT;
        delete_event(global);
        global.state = STATE::MENU;
      }
      }},
      {'5', {"Exporter au format HTML", [](Global &global) -> void
      {
        export_diary_HTML(global);
      }
      }},
      {'6', {"Enregistrer l'agenda", [](Global &global) -> void
      {
        save_diary(global);
        global.diary.is_modify = false;
      }
      }},
      {'q', {"Quitter", [](Global &global) -> void
      {
        if (global.diary.is_modify)
        {
          confirm_save(global);
        }
        global.state = STATE::EXIT;
      }
      }}
    }}}};
  }
  void print_choice_menu(const std::string &msg)
  {
    // This is a yes or no choice with the message given
    std::cout << MAGENTA << msg << std::endl
              << GREEN << "1. Oui" << std::endl
              << RED << "2. Non" << RESET << std::endl;
  }
  void show_menu(const Menu &menu)
  {
    // We show the label of the menu
    std::cout << menu.label << std::endl;

    // We show the entrys with their input and their label
    for (const auto &entry : menu.entrys)
    {
      std::cout << '[' << entry.first << "] " << entry.second.label << std::endl;
    }
  }
  void menu(Global &global, std::size_t menu_index)
  {
    // When the user select an action, we always check if the entry is valid else, we print an error
    if(global.menus[menu_index].entrys.empty())
    {
      print_error("le menu est vide !");
      return;
    }

    char selection;
    do
    {
      // Create the diary, we don't need to check if the entry is empty here because the menu is not empty
      show_menu(global.menus[menu_index]);
      getUserInput(selection, "> ");
      if(auto search = global.menus[menu_index].entrys.find(selection); search != global.menus[menu_index].entrys.end())
      {
        launch_entry(global, menu_index, selection);
      }
      else
      {
        print_error("aucune action ne correspond à la sélection !");
      }
    } while (global.state != diary::STATE::EXIT);
  }

  // Launch the diary
  void launch_diary()
  {
    Global global;
    global.state = diary::STATE::LAUNCH;

    global.menus = diary::initialize_menu();
    diary::create_export_directory(global);
    
    diary::STATE::MENU;
    diary::menu(global, 0);
  }
}