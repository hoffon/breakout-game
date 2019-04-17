#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cp_functions.h"

#define True  1
#define False 0
#define WindowTitle  "Breakout 61"
#define WindowWidth  910 //ขนาดหน้าจอ กว้าง
#define WindowHeight 650 //ขนาดหน้าจอ สูง

Sound hit_paddle_sound, hit_brick_sound;
Sound hit_top_sound, end_sound;
Texture paddle_texture, ball_texture;
Texture brick_texture, background_texture;
Font big_font, small_font, life_font ,position_ball_font,position_paddle_font;

// Structure for storing info for objects, i.e. Paddle, Brick, Ball.
typedef struct
{
    float x, y;
    float width, height;
    float vel_x, vel_y;
    int destroyed;
} Object;

// Collision Detection between two objects a and b
int collide(Object a, Object b)
{
    if (a.x + a.width  < b.x || b.x + b.width  < a.x ||
        a.y + a.height < b.y || b.y + b.height < a.y)
        return False;
    else
        return True;
}

// Initial routine to load sounds, textures, and fonts.
int game_init()
{
    hit_paddle_sound = cpLoadSound("hitDown.wav");
    hit_brick_sound = cpLoadSound("hitUp.wav");
    hit_top_sound = cpLoadSound("hitTop.wav");
    end_sound = cpLoadSound("theEnd.wav");

    paddle_texture = cpLoadTexture("paddle.png");
    ball_texture = cpLoadTexture("ball.png");
    brick_texture = cpLoadTexture("brick.png");
    background_texture = cpLoadTexture("background.png");

    big_font = cpLoadFont("THSarabun.ttf", 60);
    small_font = cpLoadFont("THSarabun.ttf", 30);
    life_font = cpLoadFont("THSarabun.ttf",30);
    position_ball_font = cpLoadFont("THSarabun.ttf", 20);
    position_paddle_font = cpLoadFont("THSarabun.ttf", 20);

    if (hit_paddle_sound == NULL || hit_brick_sound == NULL ||
        hit_top_sound == NULL || end_sound == NULL ||
        paddle_texture == NULL || ball_texture == NULL ||
        brick_texture == NULL || background_texture == NULL ||
        big_font == NULL || small_font == NULL)
        return False;
    return True;
}

int main(int argc, char *args[])
{
    enum { BALL_VEL_Y = -5, PADDLE_VEL_X = 7 };
    int running, n_bricks = 120, n_hits = 0, score = 0 , lifepoint = 3;
    char msg[80];
    char life[100] ;
    char position_ball[50] ;
    char position_paddle[50] ;
    Object bricks[n_bricks];
    Object ball = {WindowWidth/2-12, 350, 24, 24, 0, BALL_VEL_Y, False};
    Object paddle = {WindowWidth/2-62, WindowHeight-50, 124, 18, 0, 0, False};
    Event event;

    if (cpInit(WindowTitle, WindowWidth, WindowHeight) == False) {
        fprintf(stderr, "Window initialization failed!\n");
        exit(1);
    }

    if (game_init() == False) {
        fprintf(stderr, "Game initialization failed!\n");
        exit(1);
    }

    for (int n = 0, x = 10, y = 80 , num = 0 ; n < n_bricks; n++) { // สร้างแผ่นไม้
        bricks[n].width = 50; // ความยาวแผ่นไม้
        bricks[n].height = 18; // ความสูงแผ่นไม้
        bricks[n].x = x; // ตำแหน่งที่สร้างไม้ แกน x แนวนอน
        bricks[n].y = y; // ตำแหน่งที่สร้างไม้ แกน y แนวตั้ง
        bricks[n].destroyed = False; 
        x += bricks[n].width+10; // ตำแหน่งที่สร้างไม้ แกน x แนวนอน จะบวกต่อจากขนาดของแผ่นไม้ที่สร้างแล้ว
        num++ ; 
        if (num == 15) // สร้างไม้บรรทัดใหม่
        {
            x = 10 ;
            y += 20 ;
            num = 0 ;
        }
    }


    running = True;
    while (running) {
        cpClearScreen();
        cpDrawTexture(255, 255, 255,
            0, 0, WindowWidth, WindowHeight, background_texture);
        cpDrawTexture(255, 255, 255,
            paddle.x, paddle.y, paddle.width, paddle.height, paddle_texture);
        cpDrawTexture(255, 255, 255,
            ball.x, ball.y, ball.width, ball.height, ball_texture);
        sprintf(msg, "คะแนน %d", score);
        cpDrawText(255, 255, 255, 3, 3, msg, small_font, 0);
        sprintf(life, "ชีวิตที่เหลือ : %d",lifepoint); // แสดงคำว่า ชีวิตที่เหลือ : 3 บนหน้าต่างเกมของเรา
        cpDrawText(255, 255, 255, 3, 40, life,life_font, 0);
        sprintf(position_ball, "Position_ball x = %.3f, y = %.3f", ball.x, ball.y);
        cpDrawText(255, 255, 255, 750, 20, position_ball, position_ball_font, 1); // แสดงตำแหน่งของลูกบอล
        sprintf(position_paddle, "Position_paddle x = %.3f, y = %.3f", paddle.x, paddle.y);
        cpDrawText(255, 255, 255, 750, 40, position_paddle, position_paddle_font, 1); // แสดงตำแหน่งของไม้

        for (int n = 0; n < n_bricks; n++) {
            if (!bricks[n].destroyed)
                cpDrawTexture(255, 255, 255,
                    bricks[n].x, bricks[n].y, bricks[n].width, bricks[n].height,
                    brick_texture);
        }

        if (ball.y + ball.width > WindowHeight) // ส่วนของชีวิตที่ยังคงสามารถเล่นได้ เมื่อบอลตก 
        {
            lifepoint-- ; //เมื่อบอลตก ชีวิตลดลงไป 1
            ball.x = WindowWidth/2-12 ; // บอลจะกลับมายังจุดเริ่มต้น
            ball.y = 350 ; // บอลกลับมายังจุดเริ่มต้น 
            ball.vel_x = 0 ; // บอลเคลื่อนที่ตามแกน x  เป็น 0
            ball.vel_y = 5 ; // บอลเคลื่อนที่ตามแกน y (เคลื่อนที่ลง) เป็น 0
            paddle.x = WindowWidth/2-62 ; // ไม้กลับมายังจุดเริ่มต้น
            paddle.y = WindowHeight-50 ; // ไม้กลับมายังจุดเริ่มต้น
            
        }
        if (lifepoint < 0 || n_hits == n_bricks) {
            cpPlaySound(end_sound);
            cpDrawText(255, 255, 0, 450, 350, "จบเกมจบกัน", big_font, 1);
            cpSwapBuffers();
            while (1) {
                cbEventListener(&event);
                if (event.type == QUIT ||
                    event.type == KEYUP && event.key.keysym.sym == K_ESCAPE) {
                    running = False;
                    break;
                }
            }
        }
        cpSwapBuffers();

        while (cbEventListener(&event)) {
            if (event.type == QUIT ||
                 event.type == KEYUP && event.key.keysym.sym == K_ESCAPE) {
                running = False;
                break;
            }

            if (event.type == KEYDOWN) {
                if (event.key.keysym.sym == K_LEFT)  //รับแป้นซ้ายจากkeyboard
                    paddle.vel_x = -abs(PADDLE_VEL_X); //ไม้เคลื่อนที่ไปทางซ้าย (ติดลบ)
                if (event.key.keysym.sym == K_RIGHT) //รับแป้นขวาจากkeyboard
                    paddle.vel_x = abs(PADDLE_VEL_X); //ไม้เคลื่อนที่ไปทางขวา 
            }
            else
            if (event.type == KEYUP) {
                if (event.key.keysym.sym == K_LEFT)
                    paddle.vel_x = 0;
                if (event.key.keysym.sym == K_RIGHT)
                    paddle.vel_x = 0;
            }
        }
        paddle.x += paddle.vel_x; //ต่ำแหน่งของไม้ที่จะเคลื่อนที่ 

        if (paddle.x < 0) // ไม้จะติดขอบด้านซ้าย 
            paddle.x = 0;
        if (paddle.x + paddle.width > WindowWidth) // ไม้จะติดขอบด้านขวา
            paddle.x = WindowWidth - paddle.width;

        ball.x += ball.vel_x;//ต่ำแหน่งของบอลที่จะเคลื่อนที่ แกน x แนวนอน  
        ball.y += ball.vel_y;//ต่ำแหน่งของบอลที่จะเคลื่อนที่ แกน y แนวตั้ง

        if (ball.x < 0 || ball.x + ball.width > WindowWidth) // เมื่อบอลกระทบกับหน้าต่างเกมซ้ายและขวาจะเด้งกลับไม่เลยจอออกไป
            ball.vel_x = -ball.vel_x;

        if (ball.y < 0) { //ถ้าบอลกระทบจุดสูงสุดของหน้าต่างเกม
            cpPlaySound(hit_top_sound);
            ball.vel_y = -ball.vel_y; //บอลเด้งกลับลงมา ไม่งั้นจะค้างข้างบน
        }

        for (int n = 0; n < n_bricks; n++) {
            if (!bricks[n].destroyed &&
                collide(ball, bricks[n]) == True) {
                cpPlaySound(hit_brick_sound);
                ball.vel_y = -ball.vel_y;
                bricks[n].destroyed = True; //ทำลายอิฐ
                n_hits++; //จำนวนทีอิฐ่ทำลาย
                score += 10; //คะแนนเพิ่ม
                break;
            }
        }

        if (collide(ball, paddle) == True) 
        {
            cpPlaySound(hit_paddle_sound);
            if (ball.y >= paddle.y)
                ball.vel_y = ball.vel_y ; //  เมื่อ ball.y อยู่ตำแหน่งเดียวกับ  paddle.y บอลจะไม่ค้างอยู่ที่ไม้ 
            else if (ball.x >= paddle.x + paddle.width / 2 && 
                ball.x < paddle.x + paddle.width && 
                ball.y + ball.height >= paddle.y)
            {
                ball.vel_y = -ball.vel_y;
                ball.vel_x = -ball.vel_y;//บอลโดนไม้ด้านขวา บอลจะเฉียงออกไปทางขวา
            }
            else if ( ball.x == paddle.x + 50  )
                ball.vel_y = -ball.vel_y ; // บอลโดนไม้ตรงกลาง บอลจะเด้งขึ้น
            else {
                ball.vel_y = -ball.vel_y ;
                ball.vel_x = ball.vel_y ; // บอลโดนไม้ด้านซ้าย บอลจะเฉียงออกไปทางซ้าย
            }

        }

        cpDelay(10);// ความเร็วตอนเล่น
    }
    cpCleanUp();
    return 0;
}
