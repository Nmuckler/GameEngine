// #include "Actor.hpp"
// #include "Object.hpp"
// #include "GameObject.hpp"
// #include "Timeline.hpp"
// #include "GameManager.hpp"
// #include <thread>
// #include <chrono>
// #include <mutex>
// #include <iostream>
// class ThreadRunner
// {
//     int i;                                        // an identifier
//     ThreadRunner *other;                          // a reference to the "other" thread in this example
//     std::mutex *_mutex;                           // the object for mutual exclusion of execution
//     GameManager *manager;

// public:
//     ThreadRunner(int i, ThreadRunner *other, std::mutex *_mutex, GameManager *gamemanager)
//     {
//         this->i = i; // set the id of this thread

//         this->_mutex = _mutex;
//         this->manager = gamemanager;
//     }

//     void run(sf::RenderWindow &window)
//     {

//         if (i == 0)
//         {
//             try
//             {
//                 std::unique_lock<std::mutex> cv_lock(*this->_mutex);
//                 //...

//                 // compute deltatime from game manager
//                 manager->updateDeltaTime();

//                 // process inputs from game manager
//             }
//             catch (...)
//             {
//                 std::cerr << "Thread " << i << " caught exception." << std::endl;
//             }
//         }

//         else // id == 1
//         {

//             try
//             {
//                 std::unique_lock<std::mutex> cv_lock(*this->_mutex);

//                 // manager->checkInputs();

//             }
//             catch (...)
//             {
//                 std::cerr << "Thread " << i << " caught exception." << std::endl;
//             }

//         }
//     }
// };

// /**
//  * Wrapper function because threads can't take pointers to member functions.
//  */
// void run_wrapper(ThreadRunner *fe, sf::RenderWindow &window)
// {
//     fe->run(window);
// }

// int main()
// {
//     std::mutex m;
//     // Create a window
//     sf::RenderWindow window(sf::VideoMode(800, 600), "Engine Window");
//     window.setFramerateLimit(60);
//     GameManager *manager = GameManager().getInstance();
//     manager->initialize(); // initialize

//     ThreadRunner t1(0, NULL, &m, manager);
//     ThreadRunner t2(1, &t1, &m, manager);

//     // Run the program as long as the window is open
//     while (window.isOpen())
//     {
//         // manager->updateDeltaTime();

//         // Check all the window's events that were triggered since the last iteration of the loop
//         sf::Event event;

//         while (window.pollEvent(event))
//         {
//             // "Close requested" event: we close the window
//             if (event.type == sf::Event::Closed)
//             {
//                 window.close();
//             }
//         }
//         // manager->checkInputs();

//         std::thread first(run_wrapper, &t1, std::ref(window));
//         std::thread second(run_wrapper, &t2, std::ref(window));

//         first.join();
//         second.join();

//         manager->render(window);
//     }
//     return EXIT_SUCCESS;
// }