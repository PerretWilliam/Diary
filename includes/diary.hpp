#include <string>
#include <vector>
#include <functional>
#include <forward_list>
#include <filesystem>
#include <map>

#include "../includes/date.hpp"

namespace fs = std::filesystem;

namespace diary
{
  enum struct STATE
  {
    LAUNCH,
    MENU,
    CREATE_diary,
    LOAD_diary,
    DEL_diary,
    ADD_EVENT,
    DEL_EVENT,
    EXIT
  };

  struct Event
  {
    std::string id;
    std::string title;
    std::string description;
    date::Date date_begin;
    date::Date date_end;
  };

  struct Diary
  {
    std::string title;
    std::string description;
    std::forward_list<Event> events;
    bool is_modify;
  };

  // We create a struct Global before set is information because the entry needs the global and the menu needs the entry.
  struct Global;
  struct Entry
  {
    std::string label;
    std::function<void(Global &)> launch;
  };

  struct Menu
  {
    std::string label;
    std::map<char, Entry> entrys;
  };

  struct Global
  {
    STATE state;
    Diary diary;
    fs::path export_path = fs::current_path() / "export";
    std::vector<Menu> menus;
  };

  void clear_screen();
  void print_error(const std::string &msg);
  std::string getUID(std::size_t len);

  // File
  bool diary_exist(const fs::path &diary_path);
  void open_file(const std::string &file_name);
  void create_export_directory(const Global &global);
  void delete_diary(const fs::path &diary_path);
  void print_saved_diary(const fs::path &export_path);

  // Inputs
  void getUserInput(char &c, const std::string &msg);
  std::string create_title(const std::string &msg);
  std::string create_description(const std::string &msg);
  bool ask_same_date();
  bool ask_load_diary(Global &global);
  bool ask_delete_diary(Global &global);

  // Events
  void print_events(const Global &global);
  Event create_event();
  void add_event(Global &global);
  void delete_event(Global &global);

  // Diary
  void print_diary(const Global &global);
  Diary create_diary();

  // HTML export
  void export_description_HTML(std::ofstream &file, const std::string description);
  void export_event_HTML(std::ofstream &file, const Event &event);
  void export_diary_HTML(const Global &global);

  // Save
  void save_event(std::ofstream &file, Event event);
  void save_diary(const Global global);
  void confirm_save(const Global &global);

  // Load
  std::string load_description(std::ifstream &file);
  Diary load_diary(const fs::path &diary_path);

  // Menus
  void launch_entry(Global &global, size_t menu_index,  char entry_index);
  std::vector<diary::Menu> initialize_menu();
  void show_menu(const Menu &menu);
  void print_choice_menu(const std::string &msg);
  void menu(Global &global, std::size_t menu_index);

  // Launch the diary
  void launch_diary();
}