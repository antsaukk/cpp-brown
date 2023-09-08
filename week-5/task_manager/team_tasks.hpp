#include <iostream>

#include <string>
#include <map>
#include <unordered_map>

// Перечислимый тип для статуса задачи
enum class TaskStatus {
  NEW,          // новая
  IN_PROGRESS,  // в разработке
  TESTING,      // на тестировании
  DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = std::map<TaskStatus, int>;

class TeamTasks {
public:
  // Получить статистику по статусам задач конкретного разработчика
  const TasksInfo& GetPersonTasksInfo(const std::string& person) const
  {
    return info_.at(person);
  }
  
  // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
  void AddNewTask(const std::string& person)
  {
    ++info_[person][TaskStatus::NEW];
  }
  
  // Обновить статусы по данному количеству задач конкретного разработчика,
  // подробности см. ниже
  std::tuple<TasksInfo, TasksInfo> PerformPersonTasks(
      const std::string& person, int task_count)
  {
    TasksInfo& developer_tasks = info_[person];

    TasksInfo old = developer_tasks;
    TasksInfo updated;

    int doneTasksToUpdate = 0;

    for (auto it = developer_tasks.begin();
      it != developer_tasks.end() && task_count > 0;
      it++
    )
    {
      if (it->first != TaskStatus::DONE)
      {
        int to_be_completed = it->second < task_count ? it->second : task_count;

        old[it->first] -= to_be_completed;
          
        updated[GetNextTask(it->first)] = to_be_completed;

        task_count -= to_be_completed;
      }
    }

    developer_tasks[TaskStatus::DONE] -= doneTasksToUpdate;

    for (auto it = updated.begin(); it != updated.end(); it++)
    {
      developer_tasks[GetPreviousTask(it->first)] -= it->second;
      developer_tasks[it->first] += it->second;
    }

    if (old.count(TaskStatus::DONE)) {
      old.erase(TaskStatus::DONE);
    }

    // clear updated zero tasks
    ClearCollectionFromZeros(updated);

    ClearCollectionFromZeros(old);

    ClearCollectionFromZeros(developer_tasks);

    return std::tie(updated, old);
  }

private:
  std::unordered_map<std::string, TasksInfo> info_;

  TaskStatus GetNextTask(TaskStatus status)
  {
    if (status == TaskStatus::NEW) return TaskStatus::IN_PROGRESS;
    else if (status == TaskStatus::IN_PROGRESS) return TaskStatus::TESTING;
    else return TaskStatus::DONE;
  }

  TaskStatus GetPreviousTask(TaskStatus status)
  {
    if (status == TaskStatus::IN_PROGRESS) return TaskStatus::NEW;
    else if (status == TaskStatus::TESTING) return TaskStatus::IN_PROGRESS;
    else return TaskStatus::TESTING;
  }

  void ClearCollectionFromZeros(TasksInfo& tasks)
  {
    if (tasks[TaskStatus::NEW] == 0) {
      tasks.erase(TaskStatus::NEW);
    }

    if (tasks[TaskStatus::IN_PROGRESS] == 0) {
      tasks.erase(TaskStatus::IN_PROGRESS);
    }

    if (tasks[TaskStatus::TESTING] == 0) {
      tasks.erase(TaskStatus::TESTING);
    }

    if (tasks[TaskStatus::DONE] == 0) {
      tasks.erase(TaskStatus::DONE);
    }
  }
};