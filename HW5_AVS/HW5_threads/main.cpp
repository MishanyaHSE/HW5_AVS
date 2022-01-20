#include <iostream>
#include <queue>
#include <mutex>
#include <thread>

using namespace std;

// Объявление очередей для хранения задач на проверку каждого программиста
queue<pair<int, int>> tasks_for_check_first;
queue<pair<int, int>> tasks_for_check_second;
queue<pair<int, int>> tasks_for_check_third;

// Объявление переменных для хранения информации о количестве задач
int number_first_tasks;
int number_second_tasks;
int number_third_tasks;
int all_tasks;

// Переменные для определения возможности делать следующую задачу каждым из программистов
bool can_do_next_task_first = true;
bool can_do_next_task_second = true;
bool can_do_next_task_third = true;

// Мьютекс для синхронизации работы потоков
mutex mtx;

// Функция, моделирующая работу первого программиста
void first_programmer(){
    while(all_tasks > 0) {
        mtx.lock();
        // Проверяем, нужно программисту писать программу и может ли он это сделать
        if(number_first_tasks > 0 && can_do_next_task_first) {
            number_first_tasks--;
            // Выбираем, кому на проверку отправится задача
            if (rand() % 2 == 0) {
                //Отправляем готовую задачу в соответствующую очередь
                tasks_for_check_second.push(make_pair(1, rand() % 10));
                cout << "Programmer 1 has sent program to programmer 2 for check. ";
                cout << "Tasks left: " << number_first_tasks << endl;
            } else {
                tasks_for_check_third.push(make_pair(1, rand() % 10));
                cout << "Programmer 1 has sent program to programmer 3 for check. ";
                cout << "Tasks left: " << number_first_tasks << endl;
            }
            // Забираем у программиста возможность писать программы, пока отправленная программа не будет проверена
            can_do_next_task_first = false;
        }
        // Выясняем, какой из программистов прислал задачу на проверку
        if(!tasks_for_check_first.empty() && tasks_for_check_first.front().first == 2) {
            // Проверяем задачу
            if(tasks_for_check_first.front().second % 2 == 0) {
                tasks_for_check_first.pop();
                cout << "Programmer 1 has checked program from programmer 2. It was done correctly."<< endl;
                all_tasks--;
            } else {
                tasks_for_check_first.pop();
                cout << "Programmer 1 has checked program from programmer 2. It was done not correctly."<< endl;
                number_second_tasks++;
            }
            // Разрешаем программисту написать еще одну программу
            can_do_next_task_second = true;
        } else if(!tasks_for_check_first.empty() && tasks_for_check_first.front().first == 3) {
            if(tasks_for_check_first.front().second % 2 == 0) {
                tasks_for_check_first.pop();
                cout << "Programmer 1 has checked program from programmer 3. It was done correctly."<< endl;
                all_tasks--;
            } else {
                tasks_for_check_first.pop();
                cout << "Programmer 1 has checked program from programmer 3. It was done not correctly."<< endl;
                number_third_tasks++;
            }
            can_do_next_task_third = true;
        }
        mtx.unlock();
        this_thread::sleep_for(chrono::microseconds(500));
    }
}

// Функция, моделирующая работу второго программиста
void second_programmer(){
    while(all_tasks > 0) {
        mtx.lock();
        // Проверяем, нужно программисту писать программу и может ли он это сделать
        if(number_second_tasks > 0 && can_do_next_task_second) {
            number_second_tasks--;
            // Выбираем, кому на проверку отправится задача
            if (rand() % 2 == 0) {
                //Отправляем готовую задачу в соответствующую очередь
                tasks_for_check_first.push(make_pair(2, rand() % 10));
                cout << "Programmer 2 has sent program to programmer 1 for check. ";
                cout << "Tasks left: " << number_second_tasks << endl;
            } else {
                tasks_for_check_third.push(make_pair(2, rand() % 10));
                cout << "Programmer 2 has sent program to programmer 3 for check. ";
                cout << "Tasks left: " << number_second_tasks << endl;
            }
            // Забираем у программиста возможность писать программы, пока отправленная программа не будет проверена
            can_do_next_task_second = false;
        }
        if(!tasks_for_check_second.empty() && tasks_for_check_second.front().first == 1) {
            if(tasks_for_check_second.front().second % 2 == 0) {
                tasks_for_check_second.pop();
                cout << "Programmer 2 has checked program from programmer 1. It was done correctly."<< endl;
                all_tasks--;
            } else {
                tasks_for_check_second.pop();
                cout << "Programmer 2 has checked program from programmer 1. It was done not correctly."<< endl;
                number_first_tasks++;
            }
            can_do_next_task_first = true;
        } else if(!tasks_for_check_second.empty() && tasks_for_check_second.front().first == 3) {
            if(tasks_for_check_second.front().second % 2 == 0) {
                tasks_for_check_second.pop();
                cout << "Programmer 2 has checked program from programmer 3. It was done correctly."<< endl;
                all_tasks--;
            } else {
                tasks_for_check_second.pop();
                cout << "Programmer 2 has checked program from programmer 3. It was done not correctly."<< endl;
                number_third_tasks++;
            }
            can_do_next_task_third = true;
        }
        mtx.unlock();
        this_thread::sleep_for(chrono::microseconds(500));
    }
}

// Функция, моделирующая работу третьего программиста
void third_programmer(){
    while(all_tasks > 0) {
        mtx.lock();
        // Проверяем, нужно программисту писать программу и может ли он это сделать
        if(number_third_tasks > 0 && can_do_next_task_third) {
            number_third_tasks--;
            // Выбираем, кому на проверку отправится задача
            if (rand() % 2 == 0) {
                //Отправляем готовую задачу в соответствующую очередь
                tasks_for_check_first.push(make_pair(3, rand() % 10));
                cout << "Programmer 3 has sent program to programmer 1 for check. ";
                cout << "Tasks left: " << number_third_tasks << endl;
            } else {
                tasks_for_check_second.push(make_pair(3, rand() % 10));
                cout << "Programmer 3 has sent program to programmer 2 for check. ";
                cout << "Tasks left: " << number_third_tasks << endl;
            }
            // Забираем у программиста возможность писать программы, пока отправленная программа не будет проверена
            can_do_next_task_third = false;
        }
        // Проверка работы
        if(!tasks_for_check_third.empty() && tasks_for_check_third.front().first == 1) {
            if(tasks_for_check_third.front().second % 2 == 0) {
                tasks_for_check_third.pop();
                cout << "Programmer 3 has checked program from programmer 1. It was done correctly."<< endl;
                all_tasks--;
            } else {
                tasks_for_check_third.pop();
                cout << "Programmer 3 has checked program from programmer 1. It was done not correctly."<< endl;
                number_first_tasks++;
            }
            can_do_next_task_first = true;
        } else if(!tasks_for_check_third.empty() && tasks_for_check_third.front().first == 2) {
            if(tasks_for_check_third.front().second % 2 == 0) {
                tasks_for_check_third.pop();
                cout << "Programmer 3 has checked program from programmer 2. It was done correctly."<< endl;
                all_tasks--;
            } else {
                tasks_for_check_third.pop();
                cout << "Programmer 3 has checked program from programmer 2. It was done not correctly."<< endl;
                number_second_tasks++;
            }
            can_do_next_task_second = true;
        }
        mtx.unlock();
        this_thread::sleep_for(chrono::microseconds(500));
    }
}

// Функция для проверки корректности введенных данных
int inputInt(int programmer_index, int m = INT_MIN, int M = INT_MAX)
{
    int N;
    for (;;) {
        std::cout << "Input number of tasks for "<< programmer_index << " programmer. Integer number from " << m << " to " << M << "): " << endl;
        if ((std::cin >> N).good() && (m <= N) && (N <= M)) return N;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cout << "Incorrect input(you should enter an integer number), try again." << endl;
        } else {
            std::cout << "Number out of bounds. Try again." << endl;
        }
        std::cin.ignore(100,'\n');
    }
}

int main() {
    // Получаем из консоли количество задач для каждого из программистов и считаем общее количество
    number_first_tasks = inputInt(1, 0, 100);
    number_second_tasks = inputInt(2, 0, 100);
    number_third_tasks = inputInt(3, 0, 100);
    all_tasks = number_first_tasks + number_second_tasks + number_third_tasks;

    // Запускаем потоки - по одному на каждого программиста
    thread first_programmer_thread(first_programmer);
    thread second_programmer_thread(second_programmer);
    thread third_programmer_thread(third_programmer);

    // Ждем завершения работы потоков
    first_programmer_thread.join();
    second_programmer_thread.join();
    third_programmer_thread.join();

    cout << "All tasks were done.";
    return 0;
}
