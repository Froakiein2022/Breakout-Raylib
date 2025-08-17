#include <iostream> //Using the iostream library for keyboard input
#include <raylib.h> //Using the raylib library for displaying graphics for the game

using namespace std;

//Global variables and classes:

Color Background_Color = Color{20, 160, 133, 255};//The color of the background
Color Ball_Color = Color{243, 213, 91, 255};//The color of the ball

//The colors of the bricks based on its row (Red, Orange, Yellow, Green, Blue, Purple):
Color Brick_Colors[6] = {Color{235, 93, 93, 255}, Color{253, 135, 76, 255}, Color{255, 240, 9, 255}, Color{64, 198, 64, 255}, Color{85, 24, 206, 255}, Color{229, 50, 255, 255}};

int lives = 5;//Number of lives remaining
int score = 0;//Current score
int round_number = 1;//Current round number
int number_of_bricks = 0;//Number of bricks remaining

//Classes:
//Ball class:
class Ball
{
    public:
    float x, y; //The x-position and y-position of the center of the ball
    int speed_x, speed_y; //The horizontal and vertical speeds of the ball
    int radius; //The radius of the ball

    void Draw() //Displays the ball:
    {
        DrawCircle(x, y, radius, Ball_Color);
    }

    void Update() 
    {
        //Updates the x-position and y-position of the ball based on its horizontal and vertical speeds:
        x += speed_x;
        y += speed_y;

        if(y - radius <= 0 && speed_y < 0)//If the ball hits the top of the screen, rebound it in the y-direction towards the bottom of the screen
        {
            speed_y *= -1;
        }
        if (y + radius >= GetScreenHeight())//If the ball reaches the bottom of the screen and gone past the paddle:
        {
            lives--;//Lower the number of lives left by 1
            ResetBall();//Reset the ball's position and speed
        }

        if((x + radius >= GetScreenWidth() && speed_x > 0)|| (x - radius <= 0 && speed_x < 0))//If the ball hits either the left or right boundary of the screen, rebound it in the opposite x-direction
        {
            speed_x *= -1;
        }
    }

    void ResetBall()
    {
        y = GetScreenHeight()/2;

        int speed_choices[4] = {-4, -2, 2, 4};
        speed_x = speed_choices[GetRandomValue(0, 3)];
        speed_y = 5;
    }
};

//Paddle class:
class Paddle
{
    public:
    float x, y; //The x-position and y-position of the top left corner of the paddle
    float width, height; //The width and height of the paddle
    int speed;//The speed of the paddle

    void Draw() //Displays the paddle
    {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
    }

    void Update() //Moves the paddle when one of the arrow keys is pressed:
    {
        if(IsKeyDown(KEY_LEFT))//Moves the paddle left when the left arrow key is pressed:
        {
            x -= speed;
        }
        if(IsKeyDown(KEY_RIGHT))//Moves the paddle right when the right arrow key is pressed:
        {
            x += speed;
        }

        //Keeps the paddle within bounds of the game window
        if(x <= 0)//If the paddle hits the left boundary of the game window
        {
            x = 0;//Keeps the paddle from moving past the left boundary
        }
        if(x + width >= GetScreenWidth())//If the paddle hits the right boundary of the game window
        {
            x = GetScreenWidth() - width;//Keeps the paddle from moving past the right boundary
        }
    }
};

//Brick class:
class Brick 
{
    public:
    float x, y; //The x-position and y-position of the top left corner of the brick
    float width, height; //The width and height of the brick
    bool brick_hit; //Determines if the brick has been hit and thus can be removed
    Color brick_color; //The color of the brick

    void Draw() 
    {
        if(!brick_hit)//If the brick has not been hit, display it:
        {
            DrawRectangle(x, y, width, height, brick_color);
            DrawRectangleLines(x, y, width, height, BLACK);
        }
    }
};

Ball ball;//Ball used in the game
Paddle paddle;//Paddle used in the game
Brick bricks[6][5];//Array of bricks in the game

//----------------------------------------------------------------------------------------------------------------------------------------------------------
//Functions:
//Code for resetting the playfield based on the current round number:
void resetRound()
{
    //Initiaizing the number of bricks based on the current round number:
    for(int i = 0; i < round_number; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            //Sets each brick's width, height, x-position, y-position, and color:
            bricks[i][j].width = 160;
            bricks[i][j].height = 50;
            bricks[i][j].x = j*bricks[i][j].width;
            bricks[i][j].y = i*bricks[i][j].height + 60;
            bricks[i][j].brick_color = Brick_Colors[i];

            //Sets each brick to be displayed:
            bricks[i][j].brick_hit = false;
        }
    }
    number_of_bricks = round_number * 5;//Determines the number of bricks to clear in the current round:

    //Sets the ball to be in the middle of the screen and randomly determines the horizontal speed and x-direction of the ball:
    ball.x = GetScreenWidth()/2;
    ball.ResetBall();

    
    //Sets the paddle to the middle of the bottom of the screen:
    paddle.x = GetScreenWidth()/2 - paddle.width/2;
    paddle.y = GetScreenHeight() - paddle.height - 10;
}

//Main program:
int main () 
{
    cout << "Starting the game" << endl;
    //Initializing the game window:
    const int screen_width = 800;
    const int screen_height = 900;
    InitWindow(screen_width, screen_height, "Breakout");

    SetTargetFPS(60);//Sets the game's target FPS to 60.

    bool game_started = false;//Determines if the game has been started
    bool game_completed = false;//Determines if the game is completed

    ball.radius = 20;//Sets the ball's radius

    //Sets the paddle's width, height, and speed:
    paddle.width = 120;
    paddle.height = 25;
    paddle.speed = 6;

    while(!WindowShouldClose())//While the game window is open:
    {
        BeginDrawing();

        if(!game_started)//Start Screen:
        {
            ClearBackground(BLACK);
            //Displays the current score, round number, and the number of lives remaining:
            DrawText(TextFormat("Score: %i", score), 10, 20, 40, WHITE);
            DrawText(TextFormat("Round: %i", round_number), (screen_width/3)+50, 20, 40, WHITE);
            DrawText(TextFormat("Lives: %i", lives), screen_width - 160, 20, 40, WHITE);
            //Display title screen and instructions:
            DrawText("Breakout", screen_width/4, screen_height/3, 80, BLUE);
            DrawText("Use the arrow keys to move the paddle", 20, screen_height/2, 39, GREEN);

            DrawText("Press Enter to begin", (screen_width/4) - 20, 2*screen_height/3, 39, WHITE);//Prompts the user to press Enter if they want to start the game
            if(IsKeyPressed(KEY_ENTER))//If Enter is pressed, start the game at Round 1:
            {
                game_started = true;
                resetRound();
            }
        }
        else if (lives > 0 && !game_completed)//Main Game:
        {
            // Updating the ball and paddle:
            ball.Update();
            paddle.Update();

            // Checks if the ball touches the paddle:
            //If the ball touches the paddle, it rebounds:
            if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{paddle.x, paddle.y, paddle.width, paddle.height}) && ball.speed_y > 0)
            {
                ball.speed_y *= -1;//The ball rebounds towards the top of the screen.

                //Determining the x-direction and horizontal speed of the ball based on the position of the ball on the paddle:
                if(ball.x < paddle.x + (paddle.width/4))
                {
                    ball.speed_x = -4;
                }
                else if(ball.x < paddle.x + (1.7 * paddle.width/4))
                {
                    ball.speed_x = -2;
                }
                else if(ball.x < paddle.x + (2.3 * paddle.width/4))
                {
                    ball.speed_x = 0;
                }
                else if(ball.x < paddle.x + (3 * paddle.width/4))
                {
                    ball.speed_x = 2;
                }
                else
                {
                    ball.speed_x = 4;
                }
            }

            //Checks if the ball touches a brick:
            for(int i = 0; i < round_number; i++)
            {
                for(int j = 0; j < 5; j++)
                {
                    if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{bricks[i][j].x, bricks[i][j].y, bricks[i][j].width, bricks[i][j].height}) && !bricks[i][j].brick_hit)//If the ball touches a brick:
                    {
                        //Remove the brick from the screen and increase the score by 1:
                        bricks[i][j].brick_hit = true;
                        number_of_bricks--;
                        score++;

                        //Determining the x-direction and y-direction of the ball based on the position of the ball on the brick (the horizontal speed of the ball remains the same):
                        if(ball.x >= bricks[i][j].x && ball.x <= bricks[i][j].x + bricks[i][j].width && ((ball.y - ball.radius <= bricks[i][j].y + bricks[i][j].height && ball.speed_y < 0) || (ball.y + ball.radius >= bricks[i][j].y && ball.speed_y > 0)))
                        {
                            ball.speed_y *= -1;
                        }
                        else if (ball.y <= bricks[i][j].y + bricks[i][j].height && ball.y >= bricks[i][j].y && ((ball.x + ball.radius >= bricks[i][j].x && ball.speed_x > 0) || (ball.x - ball.radius <= bricks[i][j].x + bricks[i][j].width && ball.speed_x < 0)))
                        {
                            ball.speed_x *= -1;
                        }
                        else
                        {
                            ball.speed_y *= -1;
                            ball.speed_x *= -1;
                        }

                        if(number_of_bricks == 0)//If all bricks in the current round are cleared:
                        {
                            if (round_number < 6)//If less than 6 rounds are cleared, move to the next round:
                            {
                                round_number++;
                                resetRound();
                            }
                            else//If all 6 rounds are cleared, end the game:
                            {
                                game_completed = true;
                            }
                            break;
                        }
                    }
                }
            }

            // Drawing the ball, the paddle, and bricks:
            ClearBackground(Background_Color);
            ball.Draw();
            paddle.Draw();
            for(int i = 0; i < round_number; i++)
            {
                for(int j = 0; j < 5; j++)
                {
                    bricks[i][j].Draw();
                }
            }

            //Displays the current score, round number, and the number of lives remaining:
            DrawText(TextFormat("Score: %i", score), 10, 20, 40, WHITE);
            DrawText(TextFormat("Round: %i", round_number), (screen_width/3)+50, 20, 40, WHITE);
            DrawText(TextFormat("Lives: %i", lives), screen_width - 160, 20, 40, WHITE);
        }
        
        else//End Screen:
        {
            ClearBackground(BLACK);
            //Displays the current score, round number, and the number of lives remaining:
            DrawText(TextFormat("Score: %i", score), 10, 20, 40, WHITE);
            DrawText(TextFormat("Round: %i", round_number), (screen_width/3)+50, 20, 40, WHITE);
            DrawText(TextFormat("Lives: %i", lives), screen_width - 160, 20, 40, WHITE);

            if(!game_completed)//If all lives were lost, display a Game Over message:
            {
                DrawText("Game Over", screen_width/4, screen_height/3, 80, RED);
            }
            else//If all 6 rounds of the game are completed, display a Congratulations message:
            {
                DrawText("Congratulations!", 70, screen_height/3, 80, GREEN);
            }
            DrawText("Press Enter to play again", (screen_width/4) - 40, screen_height/2, 39, WHITE);//Prompts the user to press Enter if they want to play again
            
            if(IsKeyPressed(KEY_ENTER))//If Enter is pressed, restart the game:
            {
                //Resets the score, round number, and number of lives:
                score = 0;
                round_number = 1;
                lives = 5;
                //Starts the game back at Round 1:
                game_completed = false;
                resetRound();
            }
        }
        EndDrawing();
    }

    //If the player closes the window, end the program:
    CloseWindow();
    return 0;
}