# SAÉ S1.01 - Implementation of Client Needs

> For this SAÉ, diary management had to be programmed.

---

## Content
In the extracted folder, you have three directories:
  - *src* containing the C++ code files: *main.cpp*, *date.cpp*, and *diary.cpp*.
  - *includes* containing the header files: *date.hpp* and *diary.hpp*.
  - *tests* containing the test cases that we will detail below.
  - *CMakeLists.txt* to create the necessary files for compiling the program.

---

## Compilation  

A CMake file is available to create all the files necessary for compiling the program.

For Windows and Linux:
```bash
$ cmake .
```

For Linux, you must then compile the program with the Makefile that will be created.

```bash
$ make
```

> This will create an executable named **diary**.

For Windows, you must open the **diary.sln** file with Visual Studio and compile the program.

---  

### The Welcome Menu
> When the program starts, an export folder will be created at the root of the project if it does not exist (in case you delete it by mistake), it will contain everything exported by the program.

When you start the program, you will arrive at this menu:

Welcome, what would you like to do?
[1] Create a new diary
[2] Load an diary
[3] Delete an diary
[q] Quit

> When creating the title of your diary, you are not allowed to use '/'.

### The Main Menu
Here is the menu you will be in after creating or loading the diary.

Welcome to the diary.
[1] Edit the diary
[2] View the diary
[3] Add an event
[4] Delete an event
[5] Export to HTML format
[6] Save the diary
[q] Quit

When you edit the diary, it will only be saved if you do so.

> If you quit the application after making changes, you will be asked whether or not you want to save the diary, press '1' to save.

### Dates

> You must follow this format for the date: dd/mm/yyyy hh:mm

Enter the start date (dd/mm/yyyy hh:mm) > 21042006 21:59
> If your input is incorrect, the program will notify you.
"Error: incorrect date format!"

Enter the start date (dd/mm/yyyy hh:mm) > 21/04/2006 21:59

---

## Test Cases  
First of all, it is important that you are in the test directory to run the tests.

```bash
cd tests
```

You have at your disposal, test cases made with *expect*.

In a *tests* directory, three tests will be available to you:
  - *test_menu.exp*: For testing the menus.
  - *test_event.exp*: For testing the events.
  - *test_file.exp*: For testing file management (saving, loading, and deleting).

> All tests will create a {test name}.log file in a created 'log' directory.
> Note that the export file will be created in the 'tests' directory.

To run the tests, you have a menu available, do:

```bash
./launch.sh
```

> If the tests or the bash script do not have execution rights, do:

```bash
chmod +x launch.sh test_menu.exp test_event.exp test_file.exp
```