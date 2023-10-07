// #include "Actor.hpp"
// #include "Object.hpp"
// #include "GameObject.hpp"
// #include "Timeline.hpp"
// #include <thread>
// #include <chrono>
// #include <mutex>
// #include <condition_variable>
// #include <iostream>



// int moldmain()
// {
//     std::mutex m;
//     std::condition_variable cv;
//     // Create a window
//     sf::RenderWindow window(sf::VideoMode(800, 600), "Engine Window");
//     window.setFramerateLimit(60);


//     float moveSpeed = 2;
//     bool hitboxActive = false;

//     // create time stuff
//     Timeline timeline = Timeline();
//     Timeline characterTimeline = Timeline(&timeline, 1);
//     int64_t currTime = 0;
//     int64_t lastTime = 0;
//     int64_t deltaTime = 0;
//     bool grounded = true;

//     // Create a circle shape
//     sf::CircleShape newActorCircle(35.f);

//     sf::RectangleShape line(sf::Vector2f(800.f, 500.f));
//     sf::RectangleShape movingLine(sf::Vector2f(250.f, 30.f));

//     // Create a an Actor
//     Actor character = Actor(newActorCircle, sf::Vector2f(100, 500), "hamster.png");
//     GameObject floor = GameObject(line, sf::Vector2f(0, 550), "");
//     GameObject platform = GameObject(movingLine, sf::Vector2f(600, 550), "");

//     // shape.setPosition(400.f, 400.f);

//     // Run the program as long as the window is open
//     while (window.isOpen())
//     {

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

//         currTime = timeline.getTime();
//         deltaTime = currTime - lastTime;
//         printf("Currtime: %ld\n", currTime);
//         printf("last time: %ld\n", lastTime);
//         printf("deltatime: %ld\n", deltaTime);

//         lastTime = currTime;
//         double sentTime = deltaTime;

//         // compute grounded
//         if (character.isTouchingFloor(floor.getShape()) || character.isTouchingFloor(platform.getShape()))
//         {
//             grounded = true;
//         }
//         else
//         {
//             grounded = false;
//         }
//         // Gravity
//         if (character.isTouchingFloor(floor.getShape()) || character.isTouchingFloor(platform.getShape()))
//         {
//             character.velocityY = 0;
//         }

//         // Turn hit boxes on or off
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
//         {
//             hitboxActive = !hitboxActive;
//         }
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
//         {
//             if (!timeline.isPaused())
//             {
//                 printf("paused pressed");
//                 timeline.pause();
//             }
//             else
//             {
//                 timeline.unpause();
//             }
//         }
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
//         {
//             printf("one pressed");
//             timeline.changeTic(4);
//         }
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
//         {
//             printf("two pressed");
//             timeline.changeTic(2);
//         }
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
//         {
//             printf("three pressed");
//             timeline.changeTic(1);
//         }

//         // Controls
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
//         {
//             // Left key is pressed: move our character to the left
//             character.moveLeft(moveSpeed);
//         }
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
//         {
//             // Right key is pressed: move our character to the right
//             character.moveRight(moveSpeed);
//         }
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
//         {
//             if (grounded)
//             {
//                 character.jump();
//             }
//         }

//         // Clear the window with white color
//         window.clear(sf ::Color::White);

//         platform.hoverY(200, 550, .5);
//         // platform.hoverX(200, 550, .5);

//         // printf("%ld\n", deltaTime);
//         if (!timeline.isPaused())
//         {
//             printf("sentTime: %lf\n", sentTime);
//             character.update(sentTime, grounded);
//         }
//         // Draw the character
//         character.draw(window, hitboxActive);
//         floor.draw(window, hitboxActive);
//         platform.draw(window, hitboxActive);

//         // Display the updated frame
//         window.display();
//     }

//     return EXIT_SUCCESS;
// }
