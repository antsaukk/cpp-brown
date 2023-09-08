#include <iostream>

#include "team_tasks.hpp"

using namespace std;

// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь
void PrintTasksInfo(TasksInfo tasks_info)
{
  cout << tasks_info[TaskStatus::NEW] << " new tasks" <<
      ", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" <<
      ", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested" <<
      ", " << tasks_info[TaskStatus::DONE] << " tasks are done" << endl;
}

void PrintTasksInfoStructured(const TasksInfo& tasks_info) {
    if (tasks_info.count(TaskStatus::NEW)) {
        std::cout << "NEW: " << tasks_info.at(TaskStatus::NEW) << " ";
    }
    if (tasks_info.count(TaskStatus::IN_PROGRESS)) {
        std::cout << "IN_PROGRESS: " << tasks_info.at(TaskStatus::IN_PROGRESS) << " ";
    }
    if (tasks_info.count(TaskStatus::TESTING)) {
        std::cout << "TESTING: " << tasks_info.at(TaskStatus::TESTING) << " ";
    }
    if (tasks_info.count(TaskStatus::DONE)) {
        std::cout << "DONE: " << tasks_info.at(TaskStatus::DONE) << " ";
    }
}

void PrintTasksInfo(const TasksInfo& updated_tasks, const TasksInfo& untouched_tasks)
{
    std::cout << "Updated: [";
    PrintTasksInfoStructured(updated_tasks);
    std::cout << "] ";
 
    std::cout << "Untouched: [";
    PrintTasksInfoStructured(untouched_tasks);
    std::cout << "] ";
 
    std::cout << std::endl;
}

void TestNaive()
{
  TeamTasks tasks;
  tasks.AddNewTask("Ilia");
  for (int i = 0; i < 3; ++i) {
    tasks.AddNewTask("Ivan");
  }
  cout << "Ilia's tasks: ";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
  cout << "Ivan's tasks: ";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));

  std::cout << "\n";
  
  TasksInfo updated_tasks, untouched_tasks;
  
  tie(updated_tasks, untouched_tasks) =
      tasks.PerformPersonTasks("Ivan", 2);
  cout << "Updated Ivan's tasks: ";
  PrintTasksInfo(updated_tasks);
  cout << "Untouched Ivan's tasks: ";
  PrintTasksInfo(untouched_tasks);

  std::cout << "\n";
  
  tie(updated_tasks, untouched_tasks) =
      tasks.PerformPersonTasks("Ivan", 2);
  cout << "Updated Ivan's tasks: ";
  PrintTasksInfo(updated_tasks);
  cout << "Untouched Ivan's tasks: ";
  PrintTasksInfo(untouched_tasks);

  std::cout << "\n";
}


void TestTwo()
{
  TeamTasks tasks;
  tasks.AddNewTask("Ilia");

  cout << "Ilia's tasks: ";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));

  std::cout << "\n";

  TasksInfo updated_tasks, untouched_tasks;

  tie(updated_tasks, untouched_tasks) =
      tasks.PerformPersonTasks("Ilia", 5);

  cout << "Updated Ilia's tasks: ";
  PrintTasksInfo(updated_tasks);
  cout << "Untouched Ilia's tasks: ";
  PrintTasksInfo(untouched_tasks);

  std::cout << "\n";
}

/*

Alice
Updated: [IN_PROGRESS: 5 ] Untouched: [] 
Updated: [TESTING: 5 ] Untouched: [] 
Updated: [DONE: 1 ] Untouched: [TESTING: 4 ] 
Updated: [IN_PROGRESS: 2 ] Untouched: [NEW: 3 TESTING: 4 ] 
NEW: 3 IN_PROGRESS: 2 TESTING: 4 DONE: 1 
Updated: [IN_PROGRESS: 3 TESTING: 1 ] Untouched: [IN_PROGRESS: 1 TESTING: 4 ] 
IN_PROGRESS: 4 TESTING: 5 DONE: 1 

*/
void TestThree()
{
  TeamTasks tasks;
  TasksInfo updated_tasks;
  TasksInfo untouched_tasks;

  std::cout << "Alice" << std::endl;
 
  for (auto i = 0; i < 5; ++i) {
    tasks.AddNewTask("Alice");
  }
  tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Alice", 5);
  PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 5}, {}]
 
  tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Alice", 5);
  PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"TESTING": 5}, {}]
 
  tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Alice", 1);
  PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"DONE": 1}, {"TESTING": 4}]
 
  for (auto i = 0; i < 5; ++i) {
    tasks.AddNewTask("Alice");
  }
  tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Alice",
                                                                   2);  // [{"IN_PROGRESS": 2}, {"NEW": 3, "TESTING": 4}]
  PrintTasksInfo(updated_tasks, untouched_tasks);
 
  PrintTasksInfoStructured(tasks.GetPersonTasksInfo("Alice"));  // {"NEW": 3, "IN_PROGRESS": 2, "TESTING": 4, "DONE": 1}
  std::cout << std::endl;
 
  tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Alice", 4);
  PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 3, "TESTING": 1}, {"IN_PROGRESS": 1, "TESTING": 4}]
 
  PrintTasksInfoStructured(tasks.GetPersonTasksInfo("Alice"));  // {"IN_PROGRESS": 4, "TESTING": 5, "DONE": 1}
  std::cout << std::endl;
}

/*

Jack
Updated: [IN_PROGRESS: 1 ] Untouched: [] 
Updated: [IN_PROGRESS: 1 TESTING: 1 ] Untouched: [] 
NEW: 1 IN_PROGRESS: 1 TESTING: 1 
Updated: [IN_PROGRESS: 1 TESTING: 1 DONE: 1 ] Untouched: [] 
IN_PROGRESS: 1 TESTING: 1 DONE: 1 

*/
void TestFour()
{
  TeamTasks tasks;
  TasksInfo updated_tasks;
  TasksInfo untouched_tasks;

  std::cout << "\nJack" << std::endl;
 
  tasks.AddNewTask("Jack");
 
  tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Jack", 1);
  PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 1}, {}]
 
  tasks.AddNewTask("Jack");
 
  tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Jack", 2);
  PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 1, "TESTING": 1}, {}]
 
  tasks.AddNewTask("Jack");
 
  PrintTasksInfoStructured(tasks.GetPersonTasksInfo("Jack"));  // {"NEW": 1, "IN_PROGRESS": 1, "TESTING": 1, "DONE": 0}
  std::cout << std::endl;
 
  tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Jack", 3);
  PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 1, "TESTING": 1, "DONE": 1}, {}]
 
  PrintTasksInfoStructured(tasks.GetPersonTasksInfo("Jack"));  // {"IN_PROGRESS": 1, "TESTING": 1, "DONE": 1}
  std::cout << std::endl;
}

/*

Lisa
Updated: [IN_PROGRESS: 5 ] Untouched: [] 
Updated: [TESTING: 5 ] Untouched: [] 
Updated: [DONE: 1 ] Untouched: [TESTING: 4 ] 
Updated: [IN_PROGRESS: 2 ] Untouched: [NEW: 3 TESTING: 4 ] 
NEW: 3 IN_PROGRESS: 2 TESTING: 4 DONE: 1 
Updated: [IN_PROGRESS: 3 TESTING: 1 ] Untouched: [IN_PROGRESS: 1 TESTING: 4 ] 
IN_PROGRESS: 4 TESTING: 5 DONE: 1 
Updated: [TESTING: 4 DONE: 1 ] Untouched: [TESTING: 4 ] 
TESTING: 8 DONE: 2 
Updated: [DONE: 8 ] Untouched: [] 
DONE: 10 
Updated: [] Untouched: [] 
DONE: 10 
NEW: 1 DONE: 10 
Updated: [IN_PROGRESS: 1 ] Untouched: [] 
IN_PROGRESS: 1 DONE: 10 
Updated: [] Untouched: []

*/
void TestFive()
{
  TeamTasks tasks;
  TasksInfo updated_tasks;
  TasksInfo untouched_tasks;

  std::cout << "\nLisa" << std::endl;
 
  for (auto i = 0; i < 5; ++i) {
    tasks.AddNewTask("Lisa");
  }
 
  tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 5);
  PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 5}, {}]
 
  tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 5);
  PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"TESTING": 5}, {}]
 
  tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 1);
  PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"DONE": 1}, {"TESTING": 4}]
 
  for (auto i = 0; i < 5; ++i) {
    tasks.AddNewTask("Lisa");
  }
 
  tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 2);
  PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 2}, {"NEW": 3, "TESTING": 4}]
 
  PrintTasksInfoStructured(tasks.GetPersonTasksInfo("Lisa"));  // {"NEW": 3, "IN_PROGRESS": 2, "TESTING": 4, "DONE": 1}
  std::cout << std::endl;
 
  tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 4);
  PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 3, "TESTING": 1}, {"IN_PROGRESS": 1, "TESTING": 4}]
 
  PrintTasksInfoStructured(tasks.GetPersonTasksInfo("Lisa"));  // {"IN_PROGRESS": 4, "TESTING": 5, "DONE": 1}
  std::cout << std::endl;
 
  tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 5);
  PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"TESTING": 4, "DONE": 1}, {"TESTING": 4}]
 
  PrintTasksInfoStructured(tasks.GetPersonTasksInfo("Lisa"));  // {"TESTING": 8, "DONE": 2}
  std::cout << std::endl;
 
  tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 10);
  PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"DONE": 8}, {}]
 
  PrintTasksInfoStructured(tasks.GetPersonTasksInfo("Lisa"));  // {"DONE": 10}
  std::cout << std::endl;
 
  tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 10);
  PrintTasksInfo(updated_tasks, untouched_tasks);  // [{}, {}]
 
  PrintTasksInfoStructured(tasks.GetPersonTasksInfo("Lisa"));  // {"DONE": 10}
  std::cout << std::endl;
 
  tasks.AddNewTask("Lisa");
 
  PrintTasksInfoStructured(tasks.GetPersonTasksInfo("Lisa"));  // {"NEW": 1, "DONE": 10}
  std::cout << std::endl;
 
  tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Lisa", 2);
  PrintTasksInfo(updated_tasks, untouched_tasks);  // [{"IN_PROGRESS": 1}, {}]
 
  PrintTasksInfoStructured(tasks.GetPersonTasksInfo("Lisa"));  // {"IN_PROGRESS": 1, "DONE": 10}
  std::cout << std::endl;
 
  tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Bob", 3);
  PrintTasksInfo(updated_tasks, untouched_tasks);  // [{}, {}]
}

int main()
{
  TestNaive();
  TestTwo();
  TestThree();
  TestFour();
  TestFive();

  return 0;
}