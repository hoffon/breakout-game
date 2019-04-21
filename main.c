#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cp_functions.h"

#define True  1
#define False 0
#define WindowTitle  "Breakout 61"
#define WindowWidth  910 //ขนาดหน้าจอ กว้าง
#define WindowHeight 650 //ขนาดหน้าจอ สูง

Sound hit_paddle_sound, hit_brick_sound , metal_sound ,allu_sound;
Sound hit_top_sound, end_sound , win_sound ,red_sound ,gold_sound;
Music background_sound ;
Texture paddle_texture, ball_texture , ball2_texture;
Texture brick_texture, background_texture , brick2_texture, allu_texture ,brick3_texture,bomb_texture,brickball_texture;
Font big_font, small_font, life_font ,position_ball_font,position_paddle_font,win_font , bomb_font;

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
    win_sound = cpLoadSound("win.wav") ;
    metal_sound = cpLoadSound("metal_sound.wav") ;
    allu_sound = cpLoadSound("allu_sound.wav") ;
    red_sound = cpLoadSound("red.wav");
    background_sound = cpLoadMusic("background_sound.mp3");
    gold_sound = cpLoadSound("20brick.wav") ;

    paddle_texture = cpLoadTexture("paddle.png");
    ball_texture = cpLoadTexture("ball2.png");
    brick_texture = cpLoadTexture("brick.png");
    brick2_texture = cpLoadTexture("brick2.jpg"); // ลักษณะอิฐ ที่ต้องทำลาย 4 ครั้ง
    allu_texture = cpLoadTexture("alluminium.jpg") ; // ลักษณะอิฐ ที่ต้อวทำลาย 3 ครั้ง
    background_texture = cpLoadTexture("backgrounded.png");
    brick3_texture = cpLoadTexture("brick3.jpg");
    bomb_texture = cpLoadTexture("bomb.jpg") ;
    brickball_texture = cpLoadTexture("brickball.jpeg") ; 
    ball2_texture = cpLoadTexture("ball.png") ;

    big_font = cpLoadFont("THSarabun.ttf", 60);
    small_font = cpLoadFont("THSarabun.ttf", 30);
    life_font = cpLoadFont("THSarabun.ttf",30);
    position_ball_font = cpLoadFont("THSarabun.ttf", 20);
    position_paddle_font = cpLoadFont("THSarabun.ttf", 20);
    win_font = cpLoadFont("THSarabun.ttf", 60) ;
    bomb_font = cpLoadFont("THSarabun.ttf", 30) ;

    if (hit_paddle_sound == NULL || hit_brick_sound == NULL ||
        hit_top_sound == NULL || end_sound == NULL ||
        paddle_texture == NULL || ball_texture == NULL ||
        brick_texture == NULL || background_texture == NULL ||
        big_font == NULL || small_font == NULL || /*background_sound == NULL*/ 
        win_font == NULL || win_sound == NULL || metal_sound == NULL || allu_sound == NULL || 
        brick3_texture ==NULL)
        return False;
    return True;
}

int main(int argc, char *args[])
{
    enum { BALL_VEL_Y = -5, PADDLE_VEL_X = 7 };
    int running, n_bricks = 64, n_hits = 0, score = 0 , lifepoint = 3, n_hits27 = 0,n_hits28 = 0 ;
    int n_hits35 = 0,n_hits36 = 0 , n_hits18 = 0 , n_hits21 = 0 , n_hits42 = 0 , n_hits45 = 0;
    int n_hits9 = 0,n_hits14 = 0,n_hits49 = 0,n_hits54 = 0 ,b = 0,life_bomb = 5,j = 0;
    char msg[80];
    char life[100] ;
    char position_ball[50] ;
    char position_paddle[50] ;
    char win[50] ;
    char bomb_[50] ;
    Object bricks[n_bricks];
    Object ball = {WindowWidth/2-12, 350, 24, 24, 0, BALL_VEL_Y, False};
    Object ball1 = {ball.x, 145, 24, 24, 0, 5, True};
    Object ball2 = {ball.x, 145, 24, 24, 0, 5, False};
    Object ball3 = {ball.x, 145, 24, 24, 0, 5, False};
    Object ball4 = {ball.x, 145, 24, 24, 0, 5, False};
    Object ball5 = {ball.x, 145, 24, 24, 0, 5, False};
    Object ball6 = {ball.x, 145, 24, 24, 0, 5, False};
    Object ball7 = {ball.x, 145, 24, 24, 0, 5, False};
    Object ball8 = {ball.x, 145, 24, 24, 0, 5, False};
    Object ball9 = {ball.x, 145, 24, 24, 0, 5, False};
    Object ball10 = {ball.x, 145, 24, 24, 0, 5, False};
    Object ball11 = {ball.x, 145, 24, 24, 0, 5, False};
    Object ball12 = {ball.x, 145, 24, 24, 0, 5, False};
    Object ball13 = {ball.x, 145, 24, 24, 0, 5, False};
    Object ball14 = {ball.x, 145, 24, 24, 0, 5, False};
    Object ball15 = {ball.x, 145, 24, 24, 0, 5, False};
    Object bomb = {WindowWidth/2-12, WindowHeight-80, 20, 30, 0, 0, False}; //ระเบิด
    Object paddle = {WindowWidth/2-62, WindowHeight-50, 124, 18, 0, 0, False};
    Event event;

    if (cpInit(WindowTitle, WindowWidth, WindowHeight) == False) {
        fprintf(stderr, "Window initialization failed!\n");
        exit(1);
    }

    if (game_init() == False) {
        fprintf(stderr, "Game initialization failed!\n"); // จะแจ้งใน msy64 เมื่อมีปัญหา เช่น โหลดรูปไม่สำเร็จ
        exit(1);
    }

    for (int n = 0, x = 100, y = 80 , num = 0 ; n < n_bricks; n++) { // สร้างแผ่นไม้
        bricks[n].width = 70; // ความยาวแผ่นไม้
        bricks[n].height = 18; // ความสูงแผ่นไม้
        bricks[n].x = x; // ตำแหน่งที่สร้างไม้ แกน x แนวนอน
        bricks[n].y = y; // ตำแหน่งที่สร้างไม้ แกน y แนวตั้ง
        bricks[n].destroyed = False; 
        x += bricks[n].width+20; // ตำแหน่งที่สร้างไม้ แกน x แนวนอน จะบวกต่อจากขนาดของแผ่นไม้ที่สร้างแล้ว
        num++ ; 
        if (num == 8) // สร้างไม้บรรทัดใหม่
        {
            x = 100 ;
            y += 30 ;
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
        cpDrawTexture(255, 255, 255,
            bomb.x, bomb.y, bomb.width, bomb.height, bomb_texture);
        cpDrawTexture(255, 255, 255,
            150, 40, bomb.width, bomb.height, bomb_texture);
        sprintf(msg, "คะแนน %d", score);
        cpDrawText(255, 255, 255, 3, 3, msg, small_font, 0);
        sprintf(life, "ชีวิตที่เหลือ : %d",lifepoint); // แสดงคำว่า ชีวิตที่เหลือ : 3 บนหน้าต่างเกมของเรา
        cpDrawText(255, 255, 255, 3, 40, life,life_font, 0);
        sprintf(position_ball, "Position_ball x = %.3f, y = %.3f", ball.x, ball1.y);
        cpDrawText(255, 255, 255, 750, 20, position_ball, position_ball_font, 1); // แสดงตำแหน่งของลูกบอล
        sprintf(position_paddle, "Position_paddle x = %.3f, y = %.3f", paddle.x, paddle.y);
        cpDrawText(255, 255, 255, 750, 40, position_paddle, position_paddle_font, 1); // แสดงตำแหน่งของไม้
        sprintf(bomb_, "x %d", life_bomb);
        cpDrawText(255, 255, 255, 180, 40, bomb_, bomb_font, 0); // แสดงจำนวนของระเบิดที่ยังใช้ได้
        
        
        if (j > 0 ){
        ball1.destroyed = False ;
        cpDrawTexture(255, 255, 255,
                ball1.x, ball1.y, ball1.width, ball1.height, ball2_texture);
        cpDrawTexture(255, 255, 255,
                ball2.x, ball2.y, ball2.width, ball2.height, ball2_texture);
        cpDrawTexture(255, 255, 255,
                ball3.x, ball3.y, ball3.width, ball3.height, ball2_texture);
        cpDrawTexture(255, 255, 255,
                ball4.x, ball4.y, ball4.width, ball4.height, ball2_texture);
        cpDrawTexture(255, 255, 255,
                ball5.x, ball5.y, ball5.width, ball5.height, ball2_texture);
        cpDrawTexture(255, 255, 255,
                ball6.x, ball6.y, ball6.width, ball6.height, ball2_texture);
        cpDrawTexture(255, 255, 255,
                ball7.x, ball7.y, ball7.width, ball7.height, ball2_texture);
        cpDrawTexture(255, 255, 255,
                ball8.x, ball8.y, ball8.width, ball8.height, ball2_texture);
        cpDrawTexture(255, 255, 255,
                ball9.x, ball9.y, ball9.width, ball9.height, ball2_texture);
        cpDrawTexture(255, 255, 255,
                ball10.x, ball10.y, ball10.width, ball10.height, ball2_texture);
        cpDrawTexture(255, 255, 255,
                ball11.x, ball11.y, ball11.width, ball11.height, ball2_texture);
        cpDrawTexture(255, 255, 255,
                ball12.x, ball12.y, ball12.width, ball12.height, ball2_texture);
        cpDrawTexture(255, 255, 255,
                ball13.x, ball13.y, ball13.width, ball13.height, ball2_texture);
        cpDrawTexture(255, 255, 255,
                ball14.x, ball14.y, ball14.width, ball14.height, ball2_texture);
        cpDrawTexture(255, 255, 255,
                ball15.x, ball15.y, ball15.width, ball15.height, ball2_texture);
        }
        for (int n = 0; n < n_bricks; n++) {
            if (!bricks[n].destroyed)
            {
                if (n == 27 || n == 28 || n == 35 || n == 36) // สร้างลักษณะอิฐใหม่ให้มีการชน 4 ครั้งถึงจะถูกทำลาย 
                    cpDrawTexture(255, 255, 255,
                    bricks[n].x, bricks[n].y, bricks[n].width, bricks[n].height,
                    brick2_texture);
                else if (n == 20)
                    cpDrawTexture(255, 255, 255,
                    bricks[n].x, bricks[n].y, bricks[n].width, bricks[n].height,
                    brickball_texture);
                else if (n == 18 || n == 21 || n == 42 || n == 45)
                    cpDrawTexture(255, 255, 255,
                    bricks[n].x, bricks[n].y, bricks[n].width, bricks[n].height,
                    allu_texture);
                else if (n == 9 || n == 14 || n == 49 || n == 54)
                    cpDrawTexture(255, 255, 255,
                    bricks[n].x, bricks[n].y, bricks[n].width, bricks[n].height,
                    brick3_texture);
                else cpDrawTexture(255, 255, 255,
                    bricks[n].x, bricks[n].y, bricks[n].width, bricks[n].height,
                    brick_texture);
            }
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
            if (life_bomb > 0 && bomb.y == WindowHeight-80){ //ถ้ายังใช้ระเบิดได้และ ยังมีชีวิตเหลือ
            bomb.x = WindowWidth/2-12 ; // ระเบิดกลับมายังจุดเริ่มต้น
            bomb.y = WindowHeight-80 ;} //  ระเบิดกลับมายังจุดเริ่มต้น
            
        }
        if (n_hits == n_bricks) // เมื่อลเล่นเกมแล้วพังแผ่นไม้ได้หมด จะแสดงคำว่า ยินดีด้วยคุณชนะ
        {
            cpPlaySound(win_sound) ;
            cpDrawText(255, 255, 0, 450, 350, "ยินดีด้วยคุณชนะ", win_font, 1);
            cpSwapBuffers();
            while (1) {
                cbEventListener(&event);
                if (event.type == QUIT ||
                    event.type == KEYUP && event.key.keysym.sym == K_ESCAPE) {
                    running = False;
                    break;
                }
            }
            //break ;
            //running = False ;
        }
        
        if (lifepoint < 0 ) {
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
                if (event.key.keysym.sym == K_LEFT){ //รับแป้นซ้ายจากkeyboard
                    paddle.vel_x = -abs(PADDLE_VEL_X); //ไม้เคลื่อนที่ไปทางซ้าย (ติดลบ)
                    bomb.vel_x = -abs(PADDLE_VEL_X);
                }
                if (event.key.keysym.sym == K_RIGHT){ //รับแป้นขวาจากkeyboard
                    paddle.vel_x = abs(PADDLE_VEL_X); //ไม้เคลื่อนที่ไปทางขวา
                    bomb.vel_x = abs(PADDLE_VEL_X);
                } 
                if (event.key.keysym.sym == K_DOWN) // รับแป้นลงจาก keyboard
                    cpPlayMusic(background_sound); // เปิดเพลงขณะเล่น
                if (event.key.keysym.sym == K_UP){
                    b++;
                    bomb.vel_y = BALL_VEL_Y ;
                }
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
        bomb.y += bomb.vel_y ;
        if (b == 0)
        bomb.x += paddle.vel_x ;  //ต่ำแหน่งของระเบิดที่จะเคลื่อนที่ 
        

        if (paddle.x < 0){ // ไม้จะติดขอบด้านซ้าย 
            paddle.x = 0;
            if (b == 0)
                bomb.x = paddle.width/2 + paddle.x -10 ;
        }
        if (paddle.x + paddle.width > WindowWidth){ // ไม้จะติดขอบด้านขวา
            paddle.x = WindowWidth - paddle.width;
            if (b == 0)
            bomb.x =paddle.width/2 + paddle.x -10 ;}

        ball.x += ball.vel_x;//ต่ำแหน่งของบอลที่จะเคลื่อนที่ แกน x แนวนอน  
        ball.y += ball.vel_y;//ต่ำแหน่งของบอลที่จะเคลื่อนที่ แกน y แนวตั้ง
        if (j > 0){
        ball1.x += ball1.vel_x ;
        ball1.y += ball1.vel_y ;
        ball2.x += ball2.vel_x ;
        ball2.y += ball2.vel_y ;
        ball3.x += ball3.vel_x ;
        ball3.y += ball3.vel_y ;
        ball4.x += ball4.vel_x ;
        ball4.y += ball4.vel_y ;
        ball5.x += ball5.vel_x ;
        ball5.y += ball5.vel_y ;
        ball6.x += ball6.vel_x ;
        ball6.y += ball6.vel_y ;
        ball7.x += ball7.vel_x ;
        ball7.y += ball7.vel_y ;
        ball8.x += ball8.vel_x ;
        ball8.y += ball8.vel_y ;
        ball9.x += ball9.vel_x ;
        ball9.y += ball9.vel_y ;
        ball10.x += ball10.vel_x ;
        ball10.y += ball10.vel_y ;
        ball11.x += ball11.vel_x ;
        ball11.y += ball11.vel_y ;
        ball12.x += ball12.vel_x ;
        ball12.y += ball12.vel_y ;
        ball13.x += ball13.vel_x ;
        ball13.y += ball13.vel_y ;
        ball14.x += ball14.vel_x ;
        ball14.y += ball14.vel_y ;
        ball15.x += ball15.vel_x ;
        ball15.y += ball15.vel_y ;
        }
        


        if (ball.x < 0 || ball.x + ball.width > WindowWidth) // เมื่อบอลกระทบกับหน้าต่างเกมซ้ายและขวาจะเด้งกลับไม่เลยจอออกไป
            ball.vel_x = -ball.vel_x;
        
        if (ball1.x < 0 || ball1.x + ball1.width > WindowWidth)
        ball1.vel_x = -ball1.vel_x ;
        if (ball2.x < 0 || ball2.x + ball2.width > WindowWidth)
        ball2.vel_x = -ball2.vel_x ;
        if (ball3.x < 0 || ball3.x + ball3.width > WindowWidth)
        ball3.vel_x = -ball3.vel_x ;
        if (ball4.x < 0 || ball4.x + ball4.width > WindowWidth)
        ball4.vel_x = -ball4.vel_x ;
        if (ball5.x < 0 || ball5.x + ball5.width > WindowWidth)
        ball5.vel_x = -ball5.vel_x ;
        if (ball6.x < 0 || ball6.x + ball6.width > WindowWidth)
        ball6.vel_x = -ball6.vel_x ;
        if (ball7.x < 0 || ball7.x + ball7.width > WindowWidth)
        ball7.vel_x = -ball7.vel_x ;
        if (ball8.x < 0 || ball8.x + ball8.width > WindowWidth)
        ball8.vel_x = -ball8.vel_x ;
        if (ball9.x < 0 || ball9.x + ball9.width > WindowWidth)
        ball9.vel_x = -ball9.vel_x ;
        if (ball10.x < 0 || ball10.x + ball10.width > WindowWidth)
        ball10.vel_x = -ball10.vel_x ;
        if (ball11.x < 0 || ball11.x + ball11.width > WindowWidth)
        ball11.vel_x = -ball11.vel_x ;
        if (ball12.x < 0 || ball12.x + ball12.width > WindowWidth)
        ball12.vel_x = -ball12.vel_x ;
        if (ball13.x < 0 || ball13.x + ball13.width > WindowWidth)
        ball13.vel_x = -ball13.vel_x ;
        if (ball14.x < 0 || ball14.x + ball14.width > WindowWidth)
        ball14.vel_x = -ball14.vel_x ;
        if (ball15.x < 0 || ball15.x + ball15.width > WindowWidth)
        ball15.vel_x = -ball15.vel_x ;
        

        if (bomb.y < 0 ) //  ถ้าระเบิดกระทบจุดสูงสุดของหน้าต่างเกม
        {
            cpPlaySound(end_sound); //เสียงเมื่อระเบิดชนกับอิฐ
            bomb.destroyed = True ;// ทำลายระเบิด
            if (life_bomb > 0){ //ถ้ายังใช้ระเบิดได้ 
                bomb.destroyed = False ;
                bomb.x = paddle.width/2 + paddle.x -10 ; // ระเบิดกลับมายังจุดเริ่มต้น
                bomb.y = WindowHeight-80 ;
                bomb.vel_y = 0 ;
                b = 0 ;
            }
        }

        if (life_bomb  < 1) // เมื่อใช้ระเบิดหมดแล้วระเบิดจะหายไปจากเกม
        bomb.y = 1000 ;

        if (ball.y < 0) { //ถ้าบอลกระทบจุดสูงสุดของหน้าต่างเกม
            cpPlaySound(hit_top_sound);
            ball.vel_y = -ball.vel_y; //บอลเด้งกลับลงมา ไม่งั้นจะค้างข้างบน
        }

        if (ball1.y < 0){
        cpPlaySound(hit_top_sound);
        ball1.vel_y = -ball1.vel_y ;
        }
        if (ball2.y < 0){
        cpPlaySound(hit_top_sound);
        ball2.vel_y = -ball2.vel_y ;
        }
        if (ball3.y < 0){
        cpPlaySound(hit_top_sound);
        ball3.vel_y = -ball3.vel_y ;
        }
        if (ball4.y < 0){
        cpPlaySound(hit_top_sound);
        ball4.vel_y = -ball4.vel_y ;
        }
        if (ball5.y < 0){
        cpPlaySound(hit_top_sound);
        ball5.vel_y = -ball5.vel_y ;
        }
        if (ball6.y < 0){
        cpPlaySound(hit_top_sound);
        ball6.vel_y = -ball6.vel_y ;
        }
        if (ball7.y < 0){
        cpPlaySound(hit_top_sound);
        ball7.vel_y = -ball7.vel_y ;
        }
        if (ball8.y < 0){
        cpPlaySound(hit_top_sound);
        ball8.vel_y = -ball8.vel_y ;
        }
        if (ball9.y < 0){
        cpPlaySound(hit_top_sound);
        ball9.vel_y = -ball9.vel_y ;
        }
        if (ball10.y < 0){
        cpPlaySound(hit_top_sound);
        ball10.vel_y = -ball10.vel_y ;
        }
        if (ball11.y < 0){
        cpPlaySound(hit_top_sound);
        ball11.vel_y = -ball11.vel_y ;
        }
        if (ball12.y < 0){
        cpPlaySound(hit_top_sound);
        ball12.vel_y = -ball12.vel_y ;
        }
        if (ball13.y < 0){
        cpPlaySound(hit_top_sound);
        ball13.vel_y = -ball13.vel_y ;
        }
        if (ball14.y < 0){
        cpPlaySound(hit_top_sound);
        ball14.vel_y = -ball14.vel_y ;
        }
        if (ball15.y < 0){
        cpPlaySound(hit_top_sound);
        ball15.vel_y = -ball15.vel_y ;
        }
        for (int n = 0; n < n_bricks; n++) {
            if (!bricks[n].destroyed &&
                collide(bomb, bricks[n]) == True && !bomb.destroyed){
                    cpPlaySound(end_sound); //เสียงเมื่อระเบิดชนกับอิฐ
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    bomb.destroyed = True ;// ทำลายระเบิด
                    bomb.y = -1000 ;
                    life_bomb-- ;
                    if (life_bomb > 0){ //ถ้ายังใช้ระเบิดได้
                        bomb.destroyed = False ;
                        bomb.x = paddle.width/2 + paddle.x -10 ; // ระเบิดกลับมายังจุดเริ่มต้น
                        bomb.y = WindowHeight-80 ;
                        bomb.vel_y = 0 ;
                        b = 0 ;
                    }
                    if (n == 27 || n == 28 || n == 35 || n == 36){
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40;} //คะแนนเพิ่ม
                    
                    else if (n == 18 || n == 21 || n == 42 || n == 45){
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; }//คะแนนเพิ่ม
                    
                    else if (n == 9 || n == 14 || n == 49 || n == 54){
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20;} //คะแนนเพิ่ม
                    
                    else if (n == 20){
                        cpPlaySound(gold_sound);
                        n_hits++ ;
                        score += 10 ;
                        j++;}
                    else{
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 10;} //คะแนนเพิ่ม
                }
        


            if (n == 27 ){ // อิฐก้อนที่ 27 ต้อง ทำลาย 4 ครั้ง ถึงจะได้แต้ม 40 
            if (!bricks[n].destroyed &&
                collide(ball, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball.vel_y = -ball.vel_y;
                n_hits27++ ;
                if (n_hits27 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break ;
                }
            else if (!bricks[n].destroyed &&
                collide(ball1, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball1.vel_y = -ball1.vel_y;
                n_hits27++ ;
                if (n_hits27 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball2, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball2.vel_y = -ball2.vel_y;
                n_hits27++ ;
                if (n_hits27 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball3, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball3.vel_y = -ball3.vel_y;
                n_hits27++ ;
                if (n_hits27 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball4, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball4.vel_y = -ball4.vel_y;
                n_hits27++ ;
                if (n_hits27 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball5, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball5.vel_y = -ball5.vel_y;
                n_hits27++ ;
                if (n_hits27 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball6, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball6.vel_y = -ball6.vel_y;
                n_hits27++ ;
                if (n_hits27 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball7, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball7.vel_y = -ball7.vel_y;
                n_hits27++ ;
                if (n_hits27 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball8, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball8.vel_y = -ball8.vel_y;
                n_hits27++ ;
                if (n_hits27 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball9, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball9.vel_y = -ball9.vel_y;
                n_hits27++ ;
                if (n_hits27 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball10, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball10.vel_y = -ball10.vel_y;
                n_hits27++ ;
                if (n_hits27 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball11, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball11.vel_y = -ball11.vel_y;
                n_hits27++ ;
                if (n_hits27 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball12, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball12.vel_y = -ball12.vel_y;
                n_hits27++ ;
                if (n_hits27 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball13, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball13.vel_y = -ball13.vel_y;
                n_hits27++ ;
                if (n_hits27 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball14, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball14.vel_y = -ball14.vel_y;
                n_hits27++ ;
                if (n_hits27 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball15, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball15.vel_y = -ball15.vel_y;
                n_hits27++ ;
                if (n_hits27 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }///////////////////////////////////////////////////////////////
            }
            else if (n == 28 ){ // อิฐก้อนที่ 28 ต้อง ทำลาย 4 ครั้ง ถึงจะได้แต้ม 40 
            if (!bricks[n].destroyed &&
                collide(ball, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball.vel_y = -ball.vel_y;
                n_hits28++ ;
                if (n_hits28 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break ;
                }
            else if (!bricks[n].destroyed &&
                collide(ball1, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball1.vel_y = -ball1.vel_y;
                n_hits28++ ;
                if (n_hits28 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball2, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball2.vel_y = -ball2.vel_y;
                n_hits28++ ;
                if (n_hits28 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball3, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball3.vel_y = -ball3.vel_y;
                n_hits28++ ;
                if (n_hits28 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball4, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball4.vel_y = -ball4.vel_y;
                n_hits28++ ;
                if (n_hits28 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball5, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball5.vel_y = -ball5.vel_y;
                n_hits28++ ;
                if (n_hits28 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball6, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball6.vel_y = -ball6.vel_y;
                n_hits28++ ;
                if (n_hits28 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball7, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball7.vel_y = -ball7.vel_y;
                n_hits28++ ;
                if (n_hits28 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball8, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball8.vel_y = -ball8.vel_y;
                n_hits28++ ;
                if (n_hits28 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball9, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball9.vel_y = -ball9.vel_y;
                n_hits28++ ;
                if (n_hits28 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball10, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball10.vel_y = -ball10.vel_y;
                n_hits28++ ;
                if (n_hits28 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball11, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball11.vel_y = -ball11.vel_y;
                n_hits28++ ;
                if (n_hits28 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball12, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball12.vel_y = -ball12.vel_y;
                n_hits28++ ;
                if (n_hits28 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball13, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball13.vel_y = -ball13.vel_y;
                n_hits28++ ;
                if (n_hits28 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball14, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball14.vel_y = -ball14.vel_y;
                n_hits28++ ;
                if (n_hits28 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball15, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball15.vel_y = -ball15.vel_y;
                n_hits28++ ;
                if (n_hits28 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }///////////////////////////////////////////////////////////////
            }
            else if (n == 35 ){ // อิฐก้อนที่ 35 ต้อง ทำลาย 4 ครั้ง ถึงจะได้แต้ม 40 
            if (!bricks[n].destroyed &&
                collide(ball, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball.vel_y = -ball.vel_y;
                n_hits35++ ;
                if (n_hits35 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break ;
                }
            else if (!bricks[n].destroyed &&
                collide(ball1, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball1.vel_y = -ball1.vel_y;
                n_hits35++ ;
                if (n_hits35 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball2, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball2.vel_y = -ball2.vel_y;
                n_hits35++ ;
                if (n_hits35 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball3, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball3.vel_y = -ball3.vel_y;
                n_hits35++ ;
                if (n_hits35 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball4, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball4.vel_y = -ball4.vel_y;
                n_hits35++ ;
                if (n_hits35 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball5, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball5.vel_y = -ball5.vel_y;
                n_hits35++ ;
                if (n_hits35 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball6, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball6.vel_y = -ball6.vel_y;
                n_hits35++ ;
                if (n_hits35 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball7, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball7.vel_y = -ball7.vel_y;
                n_hits35++ ;
                if (n_hits35 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball8, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball8.vel_y = -ball8.vel_y;
                n_hits35++ ;
                if (n_hits35 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball9, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball9.vel_y = -ball9.vel_y;
                n_hits35++ ;
                if (n_hits35 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball10, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball10.vel_y = -ball10.vel_y;
                n_hits35++ ;
                if (n_hits35 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball11, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball11.vel_y = -ball11.vel_y;
                n_hits35++ ;
                if (n_hits35 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball12, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball12.vel_y = -ball12.vel_y;
                n_hits35++ ;
                if (n_hits35 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball13, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball13.vel_y = -ball13.vel_y;
                n_hits35++ ;
                if (n_hits35 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball14, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball14.vel_y = -ball14.vel_y;
                n_hits35++ ;
                if (n_hits35 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball15, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball15.vel_y = -ball15.vel_y;
                n_hits35++ ;
                if (n_hits35 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }///////////////////////////////////////////////////////////////
            }
            else if (n == 36 ){ // อิฐก้อนที่ 36 ต้อง ทำลาย 4 ครั้ง ถึงจะได้แต้ม 40 
            if (!bricks[n].destroyed &&
                collide(ball, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball.vel_y = -ball.vel_y;
                n_hits36++ ;
                if (n_hits36 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break ;
                }
            else if (!bricks[n].destroyed &&
                collide(ball1, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball1.vel_y = -ball1.vel_y;
                n_hits36++ ;
                if (n_hits36 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball2, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball2.vel_y = -ball2.vel_y;
                n_hits36++ ;
                if (n_hits36 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball3, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball3.vel_y = -ball3.vel_y;
                n_hits36++ ;
                if (n_hits36 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball4, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball4.vel_y = -ball4.vel_y;
                n_hits36++ ;
                if (n_hits36 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball5, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball5.vel_y = -ball5.vel_y;
                n_hits36++ ;
                if (n_hits36 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball6, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball6.vel_y = -ball6.vel_y;
                n_hits36++ ;
                if (n_hits36 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball7, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball7.vel_y = -ball7.vel_y;
                n_hits36++ ;
                if (n_hits36 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball8, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball8.vel_y = -ball8.vel_y;
                n_hits36++ ;
                if (n_hits36 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball9, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball9.vel_y = -ball9.vel_y;
                n_hits36++ ;
                if (n_hits36 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball10, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball10.vel_y = -ball10.vel_y;
                n_hits36++ ;
                if (n_hits36 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball11, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball11.vel_y = -ball11.vel_y;
                n_hits36++ ;
                if (n_hits36 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball12, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball12.vel_y = -ball12.vel_y;
                n_hits36++ ;
                if (n_hits36 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball13, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball13.vel_y = -ball13.vel_y;
                n_hits36++ ;
                if (n_hits36 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball14, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball14.vel_y = -ball14.vel_y;
                n_hits36++ ;
                if (n_hits36 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball15, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball15.vel_y = -ball15.vel_y;
                n_hits36++ ;
                if (n_hits36 >= 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
                }///////////////////////////////////////////////////////////////
            }
            else if (n == 18 ){ // อิฐก้อนที่ 18 ต้อง ทำลาย 3 ครั้ง ถึงจะได้แต้ม 30 
            if (!bricks[n].destroyed &&
                collide(ball, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball.vel_y = -ball.vel_y;
                n_hits18++ ;
                if (n_hits18 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break ;
                }
            else if (!bricks[n].destroyed &&
                collide(ball1, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball1.vel_y = -ball1.vel_y;
                n_hits18++ ;
                if (n_hits18 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball2, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball2.vel_y = -ball2.vel_y;
                n_hits18++ ;
                if (n_hits18 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball3, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball3.vel_y = -ball3.vel_y;
                n_hits18++ ;
                if (n_hits18 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball4, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball4.vel_y = -ball4.vel_y;
                n_hits18++ ;
                if (n_hits18 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball5, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball5.vel_y = -ball5.vel_y;
                n_hits18++ ;
                if (n_hits18 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball6, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball6.vel_y = -ball6.vel_y;
                n_hits18++ ;
                if (n_hits18 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball7, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball7.vel_y = -ball7.vel_y;
                n_hits18++ ;
                if (n_hits18 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball8, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball8.vel_y = -ball8.vel_y;
                n_hits18++ ;
                if (n_hits18 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball9, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball9.vel_y = -ball9.vel_y;
                n_hits18++ ;
                if (n_hits18 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball10, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball10.vel_y = -ball10.vel_y;
                n_hits18++ ;
                if (n_hits18 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball11, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball11.vel_y = -ball11.vel_y;
                n_hits18++ ;
                if (n_hits18 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball12, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball12.vel_y = -ball12.vel_y;
                n_hits18++ ;
                if (n_hits18 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball13, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball13.vel_y = -ball13.vel_y;
                n_hits18++ ;
                if (n_hits18 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball14, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball14.vel_y = -ball14.vel_y;
                n_hits18++ ;
                if (n_hits18 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball15, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball15.vel_y = -ball15.vel_y;
                n_hits18++ ;
                if (n_hits18 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }///////////////////////////////////////////////////////////////
            }
            else if (n == 21 ){ // อิฐก้อนที่ 21 ต้อง ทำลาย 3 ครั้ง ถึงจะได้แต้ม 30 
            if (!bricks[n].destroyed &&
                collide(ball, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball.vel_y = -ball.vel_y;
                n_hits21++ ;
                if (n_hits21 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break ;
                }
            else if (!bricks[n].destroyed &&
                collide(ball1, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball1.vel_y = -ball1.vel_y;
                n_hits21++ ;
                if (n_hits21 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball2, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball2.vel_y = -ball2.vel_y;
                n_hits21++ ;
                if (n_hits21 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball3, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball3.vel_y = -ball3.vel_y;
                n_hits21++ ;
                if (n_hits21 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball4, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball4.vel_y = -ball4.vel_y;
                n_hits21++ ;
                if (n_hits21 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball5, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball5.vel_y = -ball5.vel_y;
                n_hits21++ ;
                if (n_hits21 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball6, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball6.vel_y = -ball6.vel_y;
                n_hits21++ ;
                if (n_hits21 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball7, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball7.vel_y = -ball7.vel_y;
                n_hits21++ ;
                if (n_hits21 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball8, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball8.vel_y = -ball8.vel_y;
                n_hits21++ ;
                if (n_hits21 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball9, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball9.vel_y = -ball9.vel_y;
                n_hits21++ ;
                if (n_hits21 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball10, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball10.vel_y = -ball10.vel_y;
                n_hits21++ ;
                if (n_hits21 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball11, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball11.vel_y = -ball11.vel_y;
                n_hits21++ ;
                if (n_hits21 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball12, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball12.vel_y = -ball12.vel_y;
                n_hits21++ ;
                if (n_hits21 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball13, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball13.vel_y = -ball13.vel_y;
                n_hits21++ ;
                if (n_hits21 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball14, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball14.vel_y = -ball14.vel_y;
                n_hits21++ ;
                if (n_hits21 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball15, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball15.vel_y = -ball15.vel_y;
                n_hits21++ ;
                if (n_hits21 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }///////////////////////////////////////////////////////////////
            }
            else if (n == 42 ){ // อิฐก้อนที่ 42 ต้อง ทำลาย 3 ครั้ง ถึงจะได้แต้ม 30 
            if (!bricks[n].destroyed &&
                collide(ball, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball.vel_y = -ball.vel_y;
                n_hits42++ ;
                if (n_hits42 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break ;
                }
            else if (!bricks[n].destroyed &&
                collide(ball1, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball1.vel_y = -ball1.vel_y;
                n_hits42++ ;
                if (n_hits42 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball2, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball2.vel_y = -ball2.vel_y;
                n_hits42++ ;
                if (n_hits42 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball3, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball3.vel_y = -ball3.vel_y;
                n_hits42++ ;
                if (n_hits42 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball4, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball4.vel_y = -ball4.vel_y;
                n_hits42++ ;
                if (n_hits42 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball5, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball5.vel_y = -ball5.vel_y;
                n_hits42++ ;
                if (n_hits42 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball6, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball6.vel_y = -ball6.vel_y;
                n_hits42++ ;
                if (n_hits42 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball7, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball7.vel_y = -ball7.vel_y;
                n_hits42++ ;
                if (n_hits42 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball8, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball8.vel_y = -ball8.vel_y;
                n_hits42++ ;
                if (n_hits42 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball9, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball9.vel_y = -ball9.vel_y;
                n_hits42++ ;
                if (n_hits42 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball10, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball10.vel_y = -ball10.vel_y;
                n_hits42++ ;
                if (n_hits42 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball11, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball11.vel_y = -ball11.vel_y;
                n_hits42++ ;
                if (n_hits42 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball12, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball12.vel_y = -ball12.vel_y;
                n_hits42++ ;
                if (n_hits42 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball13, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball13.vel_y = -ball13.vel_y;
                n_hits42++ ;
                if (n_hits42 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball14, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball14.vel_y = -ball14.vel_y;
                n_hits42++ ;
                if (n_hits42 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball15, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball15.vel_y = -ball15.vel_y;
                n_hits42++ ;
                if (n_hits42 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }///////////////////////////////////////////////////////////////
            }
            else if (n == 45 ){ // อิฐก้อนที่ 45 ต้อง ทำลาย 3 ครั้ง ถึงจะได้แต้ม 30 
            if (!bricks[n].destroyed &&
                collide(ball, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball.vel_y = -ball.vel_y;
                n_hits45++ ;
                if (n_hits45 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break ;
                }
            else if (!bricks[n].destroyed &&
                collide(ball1, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball1.vel_y = -ball1.vel_y;
                n_hits45++ ;
                if (n_hits45 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball2, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball2.vel_y = -ball2.vel_y;
                n_hits45++ ;
                if (n_hits45 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball3, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball3.vel_y = -ball3.vel_y;
                n_hits45++ ;
                if (n_hits45 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball4, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball4.vel_y = -ball4.vel_y;
                n_hits45++ ;
                if (n_hits45 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball5, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball5.vel_y = -ball5.vel_y;
                n_hits45++ ;
                if (n_hits45 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball6, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball6.vel_y = -ball6.vel_y;
                n_hits45++ ;
                if (n_hits45 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball7, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball7.vel_y = -ball7.vel_y;
                n_hits45++ ;
                if (n_hits45 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball8, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball8.vel_y = -ball8.vel_y;
                n_hits45++ ;
                if (n_hits45 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball9, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball9.vel_y = -ball9.vel_y;
                n_hits45++ ;
                if (n_hits45 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball10, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball10.vel_y = -ball10.vel_y;
                n_hits45++ ;
                if (n_hits45 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball11, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball11.vel_y = -ball11.vel_y;
                n_hits45++ ;
                if (n_hits45 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball12, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball12.vel_y = -ball12.vel_y;
                n_hits45++ ;
                if (n_hits45 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball13, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball13.vel_y = -ball13.vel_y;
                n_hits45++ ;
                if (n_hits45 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball14, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball14.vel_y = -ball14.vel_y;
                n_hits45++ ;
                if (n_hits45 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball15, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball15.vel_y = -ball15.vel_y;
                n_hits45++ ;
                if (n_hits45 >= 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
                }///////////////////////////////////////////////////////////////
            }
            else if (n == 9 ){ // อิฐก้อนที่ 9 ต้อง ทำลาย 2 ครั้ง ถึงจะได้แต้ม 20 
            if (!bricks[n].destroyed &&
                collide(ball, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball.vel_y = -ball.vel_y;
                n_hits9++ ;
                if (n_hits9 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break ;
                }
            else if (!bricks[n].destroyed &&
                collide(ball1, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball1.vel_y = -ball1.vel_y;
                n_hits9++ ;
                if (n_hits9 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball2, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball2.vel_y = -ball2.vel_y;
                n_hits9++ ;
                if (n_hits9 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball3, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball3.vel_y = -ball3.vel_y;
                n_hits9++ ;
                if (n_hits9 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball4, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball4.vel_y = -ball4.vel_y;
                n_hits9++ ;
                if (n_hits9 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball5, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball5.vel_y = -ball5.vel_y;
                n_hits9++ ;
                if (n_hits9 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball6, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball6.vel_y = -ball6.vel_y;
                n_hits9++ ;
                if (n_hits9 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball7, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball7.vel_y = -ball7.vel_y;
                n_hits9++ ;
                if (n_hits9 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball8, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball8.vel_y = -ball8.vel_y;
                n_hits9++ ;
                if (n_hits9 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball9, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball9.vel_y = -ball9.vel_y;
                n_hits9++ ;
                if (n_hits9 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball10, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball10.vel_y = -ball10.vel_y;
                n_hits9++ ;
                if (n_hits9 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball11, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball11.vel_y = -ball11.vel_y;
                n_hits9++ ;
                if (n_hits9 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball12, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball12.vel_y = -ball12.vel_y;
                n_hits9++ ;
                if (n_hits9 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball13, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball13.vel_y = -ball13.vel_y;
                n_hits9++ ;
                if (n_hits9 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball14, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball14.vel_y = -ball14.vel_y;
                n_hits9++ ;
                if (n_hits9 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball15, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball15.vel_y = -ball15.vel_y;
                n_hits9++ ;
                if (n_hits9 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }///////////////////////////////////////////////////////////////
            }
            else if (n == 14 ){ // อิฐก้อนที่ 14 ต้อง ทำลาย 2 ครั้ง ถึงจะได้แต้ม 20 
            if (!bricks[n].destroyed &&
                collide(ball, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball.vel_y = -ball.vel_y;
                n_hits14++ ;
                if (n_hits14 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break ;
                }
            else if (!bricks[n].destroyed &&
                collide(ball1, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball1.vel_y = -ball1.vel_y;
                n_hits14++ ;
                if (n_hits14 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball2, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball2.vel_y = -ball2.vel_y;
                n_hits14++ ;
                if (n_hits14 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball3, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball3.vel_y = -ball3.vel_y;
                n_hits14++ ;
                if (n_hits14 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball4, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball4.vel_y = -ball4.vel_y;
                n_hits14++ ;
                if (n_hits14 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball5, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball5.vel_y = -ball5.vel_y;
                n_hits14++ ;
                if (n_hits14 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball6, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball6.vel_y = -ball6.vel_y;
                n_hits14++ ;
                if (n_hits14 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball7, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball7.vel_y = -ball7.vel_y;
                n_hits14++ ;
                if (n_hits14 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball8, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball8.vel_y = -ball8.vel_y;
                n_hits14++ ;
                if (n_hits14 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball9, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball9.vel_y = -ball9.vel_y;
                n_hits14++ ;
                if (n_hits14 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball10, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball10.vel_y = -ball10.vel_y;
                n_hits14++ ;
                if (n_hits14 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball11, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball11.vel_y = -ball11.vel_y;
                n_hits14++ ;
                if (n_hits14 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball12, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball12.vel_y = -ball12.vel_y;
                n_hits14++ ;
                if (n_hits14 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball13, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball13.vel_y = -ball13.vel_y;
                n_hits14++ ;
                if (n_hits14 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball14, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball14.vel_y = -ball14.vel_y;
                n_hits14++ ;
                if (n_hits14 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball15, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball15.vel_y = -ball15.vel_y;
                n_hits14++ ;
                if (n_hits14 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }///////////////////////////////////////////////////////////////
            }
            else if (n == 49 ){ // อิฐก้อนที่ 49 ต้อง ทำลาย 2 ครั้ง ถึงจะได้แต้ม 20 
            if (!bricks[n].destroyed &&
                collide(ball, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball.vel_y = -ball.vel_y;
                n_hits49++ ;
                if (n_hits49 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break ;
                }
            else if (!bricks[n].destroyed &&
                collide(ball1, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball1.vel_y = -ball1.vel_y;
                n_hits49++ ;
                if (n_hits49 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball2, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball2.vel_y = -ball2.vel_y;
                n_hits49++ ;
                if (n_hits49 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball3, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball3.vel_y = -ball3.vel_y;
                n_hits49++ ;
                if (n_hits49 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball4, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball4.vel_y = -ball4.vel_y;
                n_hits49++ ;
                if (n_hits49 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball5, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball5.vel_y = -ball5.vel_y;
                n_hits49++ ;
                if (n_hits49 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball6, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball6.vel_y = -ball6.vel_y;
                n_hits49++ ;
                if (n_hits49 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball7, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball7.vel_y = -ball7.vel_y;
                n_hits49++ ;
                if (n_hits49 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball8, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball8.vel_y = -ball8.vel_y;
                n_hits49++ ;
                if (n_hits49 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball9, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball9.vel_y = -ball9.vel_y;
                n_hits49++ ;
                if (n_hits49 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball10, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball10.vel_y = -ball10.vel_y;
                n_hits49++ ;
                if (n_hits49 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball11, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball11.vel_y = -ball11.vel_y;
                n_hits49++ ;
                if (n_hits49 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball12, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball12.vel_y = -ball12.vel_y;
                n_hits49++ ;
                if (n_hits49 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball13, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball13.vel_y = -ball13.vel_y;
                n_hits49++ ;
                if (n_hits49 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball14, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball14.vel_y = -ball14.vel_y;
                n_hits49++ ;
                if (n_hits49 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball15, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball15.vel_y = -ball15.vel_y;
                n_hits49++ ;
                if (n_hits49 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }///////////////////////////////////////////////////////////////
            }
            else if (n == 54 ){ // อิฐก้อนที่ 54 ต้อง ทำลาย 2 ครั้ง ถึงจะได้แต้ม 20 
            if (!bricks[n].destroyed &&
                collide(ball, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball.vel_y = -ball.vel_y;
                n_hits54++ ;
                if (n_hits54 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break ;
                }
            else if (!bricks[n].destroyed &&
                collide(ball1, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball1.vel_y = -ball1.vel_y;
                n_hits54++ ;
                if (n_hits54 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball2, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball2.vel_y = -ball2.vel_y;
                n_hits54++ ;
                if (n_hits54 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball3, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball3.vel_y = -ball3.vel_y;
                n_hits54++ ;
                if (n_hits54 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball4, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball4.vel_y = -ball4.vel_y;
                n_hits54++ ;
                if (n_hits54 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball5, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball5.vel_y = -ball5.vel_y;
                n_hits54++ ;
                if (n_hits54 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball6, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball6.vel_y = -ball6.vel_y;
                n_hits54++ ;
                if (n_hits54 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball7, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball7.vel_y = -ball7.vel_y;
                n_hits54++ ;
                if (n_hits54 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball8, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball8.vel_y = -ball8.vel_y;
                n_hits54++ ;
                if (n_hits54 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball9, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball9.vel_y = -ball9.vel_y;
                n_hits54++ ;
                if (n_hits54 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball10, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball10.vel_y = -ball10.vel_y;
                n_hits54++ ;
                if (n_hits54 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball11, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball11.vel_y = -ball11.vel_y;
                n_hits54++ ;
                if (n_hits54 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball12, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball12.vel_y = -ball12.vel_y;
                n_hits54++ ;
                if (n_hits54 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball13, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball13.vel_y = -ball13.vel_y;
                n_hits54++ ;
                if (n_hits54 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball14, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball14.vel_y = -ball14.vel_y;
                n_hits54++ ;
                if (n_hits54 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball15, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball15.vel_y = -ball15.vel_y;
                n_hits54++ ;
                if (n_hits54 >= 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
                }///////////////////////////////////////////////////////////////
            }
            
            else if (n == 20 ){ 
            if (!bricks[n].destroyed &&
                collide(ball, bricks[n]) == True ) {
                cpPlaySound(gold_sound);
                ball.vel_y = -ball.vel_y;
                bricks[n].destroyed = True; //ทำลายอิฐ
                n_hits++; //จำนวนทีอิฐ่ทำลาย
                score += 10; //คะแนนเพิ่ม
                j = 1 ;
                break;
            }
            }
            else if(!bricks[n].destroyed &&
                collide(ball, bricks[n]) == True ) {
                cpPlaySound(hit_brick_sound);
                ball.vel_y = -ball.vel_y;
                bricks[n].destroyed = True; //ทำลายอิฐ
                n_hits++; //จำนวนทีอิฐ่ทำลาย
                score += 10; //คะแนนเพิ่ม
                break;
            }
            else if(!bricks[n].destroyed &&
                collide(ball1, bricks[n]) == True ) {
                cpPlaySound(hit_brick_sound);
                ball1.vel_y = -ball1.vel_y;
                bricks[n].destroyed = True; //ทำลายอิฐ
                n_hits++; //จำนวนทีอิฐ่ทำลาย
                score += 10; //คะแนนเพิ่ม
                break;
            }
            else if (!bricks[n].destroyed &&
                collide(ball2, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball2.vel_y = -ball2.vel_y;
                n_hits54++ ;
                bricks[n].destroyed = True; //ทำลายอิฐ
                n_hits++; //จำนวนทีอิฐ่ทำลาย
                score += 10; //คะแนนเพิ่ม
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball3, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball3.vel_y = -ball3.vel_y;
                n_hits54++ ;
                bricks[n].destroyed = True; //ทำลายอิฐ
                n_hits++; //จำนวนทีอิฐ่ทำลาย
                score += 10; //คะแนนเพิ่ม
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball4, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball4.vel_y = -ball4.vel_y;
                n_hits54++ ;
                bricks[n].destroyed = True; //ทำลายอิฐ
                n_hits++; //จำนวนทีอิฐ่ทำลาย
                score += 10; //คะแนนเพิ่ม
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball5, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball5.vel_y = -ball5.vel_y;
                n_hits54++ ;
                bricks[n].destroyed = True; //ทำลายอิฐ
                n_hits++; //จำนวนทีอิฐ่ทำลาย
                score += 10; //คะแนนเพิ่ม
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball6, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball6.vel_y = -ball6.vel_y;
                n_hits54++ ;
                bricks[n].destroyed = True; //ทำลายอิฐ
                n_hits++; //จำนวนทีอิฐ่ทำลาย
                score += 10; //คะแนนเพิ่ม
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball7, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball7.vel_y = -ball7.vel_y;
                n_hits54++ ;
                bricks[n].destroyed = True; //ทำลายอิฐ
                n_hits++; //จำนวนทีอิฐ่ทำลาย
                score += 10; //คะแนนเพิ่ม
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball8, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball8.vel_y = -ball8.vel_y;
                n_hits54++ ;
                bricks[n].destroyed = True; //ทำลายอิฐ
                n_hits++; //จำนวนทีอิฐ่ทำลาย
                score += 10; //คะแนนเพิ่ม
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball9, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball9.vel_y = -ball9.vel_y;
                n_hits54++ ;
                bricks[n].destroyed = True; //ทำลายอิฐ
                n_hits++; //จำนวนทีอิฐ่ทำลาย
                score += 10; //คะแนนเพิ่ม
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball10, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball10.vel_y = -ball10.vel_y;
                n_hits54++ ;
                bricks[n].destroyed = True; //ทำลายอิฐ
                n_hits++; //จำนวนทีอิฐ่ทำลาย
                score += 10; //คะแนนเพิ่ม
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball11, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball11.vel_y = -ball11.vel_y;
                n_hits54++ ;
                bricks[n].destroyed = True; //ทำลายอิฐ
                n_hits++; //จำนวนทีอิฐ่ทำลาย
                score += 10; //คะแนนเพิ่ม
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball12, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball12.vel_y = -ball12.vel_y;
                n_hits54++ ;
                bricks[n].destroyed = True; //ทำลายอิฐ
                n_hits++; //จำนวนทีอิฐ่ทำลาย
                score += 10; //คะแนนเพิ่ม
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball13, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball13.vel_y = -ball13.vel_y;
                n_hits54++ ;
                bricks[n].destroyed = True; //ทำลายอิฐ
                n_hits++; //จำนวนทีอิฐ่ทำลาย
                score += 10; //คะแนนเพิ่ม
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball14, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball14.vel_y = -ball14.vel_y;
                n_hits54++ ;
                bricks[n].destroyed = True; //ทำลายอิฐ
                n_hits++; //จำนวนทีอิฐ่ทำลาย
                score += 10; //คะแนนเพิ่ม
                break;
                }
            else if (!bricks[n].destroyed &&
                collide(ball15, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball15.vel_y = -ball15.vel_y;
                n_hits54++ ;
                bricks[n].destroyed = True; //ทำลายอิฐ
                n_hits++; //จำนวนทีอิฐ่ทำลาย
                score += 10; //คะแนนเพิ่ม
                break;
                }///////////////////////////////////////////////////////////////
        }

        if (collide(ball, paddle) == True  ) 
        {
            cpPlaySound(hit_paddle_sound);
            if (ball.y >= paddle.y || ball.y >= 590)
                ball.vel_y = ball.vel_y ; //  เมื่อ ball.y อยู่ตำแหน่งเดียวกับ  paddle.y บอลจะไม่ค้างอยู่ที่ไม้ 
            else if (ball.x >= paddle.x + paddle.width / 2 && 
                ball.x < paddle.x + paddle.width && 
                ball.y + ball.height >= paddle.y )
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
        if (collide(ball1, paddle) == True)
        {
            cpPlaySound(hit_paddle_sound);
             if (ball1.x >= paddle.x + paddle.width / 2 && 
                ball1.x < paddle.x + paddle.width && 
                ball1.y + ball1.height >= paddle.y)
                {
                ball1.vel_y = -ball1.vel_y;
                ball1.vel_x = -ball1.vel_y;
                }
            else {
                ball1.vel_y = -ball1.vel_y;
                ball1.vel_x = ball1.vel_y;
            }
        }
        if (collide(ball2, paddle) == True)
        {
            cpPlaySound(hit_paddle_sound);
             if (ball2.x >= paddle.x + paddle.width / 2 && 
                ball2.x < paddle.x + paddle.width && 
                ball2.y + ball2.height >= paddle.y)
                {
                ball2.vel_y = -ball2.vel_y;
                ball2.vel_x = -ball2.vel_y;
                }
            else {
                ball2.vel_y = -ball2.vel_y;
                ball2.vel_x = ball2.vel_y;
            }
        }
        if (collide(ball3, paddle) == True)
        {
            cpPlaySound(hit_paddle_sound);
             if (ball3.x >= paddle.x + paddle.width / 2 && 
                ball3.x < paddle.x + paddle.width && 
                ball3.y + ball3.height >= paddle.y)
                {
                ball3.vel_y = -ball3.vel_y;
                ball3.vel_x = -ball3.vel_y;
                }
            else {
                ball3.vel_y = -ball3.vel_y;
                ball3.vel_x = ball3.vel_y;
            }
        }
        if (collide(ball4, paddle) == True)
        {
            cpPlaySound(hit_paddle_sound);
             if (ball4.x >= paddle.x + paddle.width / 2 && 
                ball4.x < paddle.x + paddle.width && 
                ball4.y + ball4.height >= paddle.y)
                {
                ball4.vel_y = -ball4.vel_y;
                ball4.vel_x = -ball4.vel_y;
                }
            else {
                ball4.vel_y = -ball4.vel_y;
                ball4.vel_x = ball4.vel_y;
            }
        }
        if (collide(ball5, paddle) == True)
        {
            cpPlaySound(hit_paddle_sound);
             if (ball5.x >= paddle.x + paddle.width / 2 && 
                ball5.x < paddle.x + paddle.width && 
                ball5.y + ball5.height >= paddle.y)
                {
                ball5.vel_y = -ball5.vel_y;
                ball5.vel_x = -ball5.vel_y;
                }
            else {
                ball5.vel_y = -ball5.vel_y;
                ball5.vel_x = ball5.vel_y;
            }
        }
        if (collide(ball6, paddle) == True)
        {
            cpPlaySound(hit_paddle_sound);
             if (ball6.x >= paddle.x + paddle.width / 2 && 
                ball6.x < paddle.x + paddle.width && 
                ball6.y + ball6.height >= paddle.y)
                {
                ball6.vel_y = -ball6.vel_y;
                ball6.vel_x = -ball6.vel_y;
                }
            else {
                ball6.vel_y = -ball6.vel_y;
                ball6.vel_x = ball6.vel_y;
            }
        }
        if (collide(ball7, paddle) == True)
        {
            cpPlaySound(hit_paddle_sound);
             if (ball7.x >= paddle.x + paddle.width / 2 && 
                ball7.x < paddle.x + paddle.width && 
                ball7.y + ball7.height >= paddle.y)
                {
                ball7.vel_y = -ball7.vel_y;
                ball7.vel_x = -ball7.vel_y;
                }
            else {
                ball7.vel_y = -ball7.vel_y;
                ball7.vel_x = ball7.vel_y;
            }
        }
        if (collide(ball8, paddle) == True)
        {
            cpPlaySound(hit_paddle_sound);
             if (ball8.x >= paddle.x + paddle.width / 2 && 
                ball8.x < paddle.x + paddle.width && 
                ball8.y + ball8.height >= paddle.y)
                {
                ball8.vel_y = -ball8.vel_y;
                ball8.vel_x = -ball8.vel_y;
                }
            else {
                ball8.vel_y = -ball8.vel_y;
                ball8.vel_x = ball8.vel_y;
            }
        }
        if (collide(ball9, paddle) == True)
        {
            cpPlaySound(hit_paddle_sound);
             if (ball9.x >= paddle.x + paddle.width / 2 && 
                ball9.x < paddle.x + paddle.width && 
                ball9.y + ball9.height >= paddle.y)
                {
                ball9.vel_y = -ball9.vel_y;
                ball9.vel_x = -ball9.vel_y;
                }
            else {
                ball9.vel_y = -ball9.vel_y;
                ball9.vel_x = ball9.vel_y;
            }
        }
        if (collide(ball10, paddle) == True)
        {
            cpPlaySound(hit_paddle_sound);
             if (ball10.x >= paddle.x + paddle.width / 2 && 
                ball10.x < paddle.x + paddle.width && 
                ball10.y + ball10.height >= paddle.y)
                {
                ball10.vel_y = -ball10.vel_y;
                ball10.vel_x = -ball10.vel_y;
                }
            else {
                ball10.vel_y = -ball10.vel_y;
                ball10.vel_x = ball10.vel_y;
            }
        }
        if (collide(ball11, paddle) == True)
        {
            cpPlaySound(hit_paddle_sound);
             if (ball11.x >= paddle.x + paddle.width / 2 && 
                ball11.x < paddle.x + paddle.width && 
                ball11.y + ball11.height >= paddle.y)
                {
                ball11.vel_y = -ball11.vel_y;
                ball11.vel_x = -ball11.vel_y;
                }
            else {
                ball11.vel_y = -ball11.vel_y;
                ball11.vel_x = ball11.vel_y;
            }
        }
        if (collide(ball12, paddle) == True)
        {
            cpPlaySound(hit_paddle_sound);
             if (ball12.x >= paddle.x + paddle.width / 2 && 
                ball12.x < paddle.x + paddle.width && 
                ball12.y + ball12.height >= paddle.y)
                {
                ball12.vel_y = -ball12.vel_y;
                ball12.vel_x = -ball12.vel_y;
                }
            else {
                ball12.vel_y = -ball12.vel_y;
                ball12.vel_x = ball12.vel_y;
            }
        }
        if (collide(ball13, paddle) == True)
        {
            cpPlaySound(hit_paddle_sound);
             if (ball13.x >= paddle.x + paddle.width / 2 && 
                ball13.x < paddle.x + paddle.width && 
                ball13.y + ball13.height >= paddle.y)
                {
                ball13.vel_y = -ball13.vel_y;
                ball13.vel_x = -ball13.vel_y;
                }
            else {
                ball13.vel_y = -ball13.vel_y;
                ball13.vel_x = ball13.vel_y;
            }
        }
        if (collide(ball14, paddle) == True)
        {
            cpPlaySound(hit_paddle_sound);
             if (ball14.x >= paddle.x + paddle.width / 2 && 
                ball14.x < paddle.x + paddle.width && 
                ball14.y + ball14.height >= paddle.y)
                {
                ball14.vel_y = -ball14.vel_y;
                ball14.vel_x = -ball14.vel_y;
                }
            else {
                ball14.vel_y = -ball14.vel_y;
                ball14.vel_x = ball14.vel_y;
            }
        }
        if (collide(ball15, paddle) == True)
        {
            cpPlaySound(hit_paddle_sound);
             if (ball15.x >= paddle.x + paddle.width / 2 && 
                ball15.x < paddle.x + paddle.width && 
                ball15.y + ball15.height >= paddle.y)
                {
                ball15.vel_y = -ball15.vel_y;
                ball15.vel_x = -ball15.vel_y;
                }
            else {
                ball15.vel_y = -ball15.vel_y;
                ball15.vel_x = ball15.vel_y;
            }
        }

        }


        cpDelay(10);// ความเร็วตอนเล่น
    
    cpCleanUp();
    return 0;
}

