#include "include/TimeManager.hpp"
RepeatingEvent TimeManager::re(150);
struct tm* TimeManager::t = NULL;
#include "include/AlarmManager.hpp"
AlarmManager AlarmManagerInstance::am;