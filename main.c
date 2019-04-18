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
Sound hit_top_sound, end_sound , win_sound ,red_sound ;
Music background_sound ;
Texture paddle_texture, ball_texture;
Texture brick_texture, background_texture , brick2_texture, allu_texture ,brick3_texture,bomb_texture;
Font big_font, small_font, life_font ,position_ball_font,position_paddle_font,win_font;

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

    paddle_texture = cpLoadTexture("paddle.png");
    ball_texture = cpLoadTexture("ball2.png");
    brick_texture = cpLoadTexture("brick.png");
    brick2_texture = cpLoadTexture("brick2.jpg"); // ลักษณะอิฐ ที่ต้องทำลาย 4 ครั้ง
    allu_texture = cpLoadTexture("alluminium.jpg") ; // ลักษณะอิฐ ที่ต้อวทำลาย 3 ครั้ง
    background_texture = cpLoadTexture("backgrounded.png");
    brick3_texture = cpLoadTexture("brick3.jpg");
    bomb_texture = cpLoadTexture("bomb.jpg") ;

    big_font = cpLoadFont("THSarabun.ttf", 60);
    small_font = cpLoadFont("THSarabun.ttf", 30);
    life_font = cpLoadFont("THSarabun.ttf",30);
    position_ball_font = cpLoadFont("THSarabun.ttf", 20);
    position_paddle_font = cpLoadFont("THSarabun.ttf", 20);
    win_font = cpLoadFont("THSarabun.ttf", 60) ;

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
    int n_hits9 = 0,n_hits14 = 0,n_hits49 = 0,n_hits54 = 0 ,b = 0,life_bomb = 1;
    char msg[80];
    char life[100] ;
    char position_ball[50] ;
    char position_paddle[50] ;
    char win[50] ;
    Object bricks[n_bricks];
    Object ball = {WindowWidth/2-12, 350, 24, 24, 0, BALL_VEL_Y, False};
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
            {
                if (n == 27 || n == 28 || n == 35 || n == 36) // สร้างลักษณะอิฐใหม่ให้มีการชน 4 ครั้งถึงจะถูกทำลาย 
                    cpDrawTexture(255, 255, 255,
                    bricks[n].x, bricks[n].y, bricks[n].width, bricks[n].height,
                    brick2_texture);
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
            if (life_bomb == 1){
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
                collide(bomb, bricks[n]) == True && !bomb.destroyed){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    bomb.destroyed = True ;// ทำลายระเบิด
                    bomb.y = -1000 ;
                    life_bomb-- ;
                    if (n == 27 || n == 28 || n == 35 || n == 36)
                    {
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                    }
                    else if (n == 18 || n == 21 || n == 42 || n == 45)
                    {
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                    }
                    else if (n == 9 || n == 14 || n == 49 || n == 54)
                    {
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                    }
                    else
                    {
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 10; //คะแนนเพิ่ม
                    }
                }


            if (n == 27 ){ // อิฐก้อนที่ 27 ต้อง ทำลาย 4 ครั้ง ถึงจะได้แต้ม 40 
            if (!bricks[n].destroyed &&
                collide(ball, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball.vel_y = -ball.vel_y;
                n_hits27++ ;
                if (n_hits27 == 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
            }
            }
            else if (n == 28 ){ // อิฐก้อนที่ 28 ต้อง ทำลาย 4 ครั้ง ถึงจะได้แต้ม 40 
            if (!bricks[n].destroyed &&
                collide(ball, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball.vel_y = -ball.vel_y;
                n_hits28++ ;
                if (n_hits28 == 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
            }
            }
            else if (n == 35 ){ // อิฐก้อนที่ 35 ต้อง ทำลาย 4 ครั้ง ถึงจะได้แต้ม 40 
            if (!bricks[n].destroyed &&
                collide(ball, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball.vel_y = -ball.vel_y;
                n_hits35++ ;
                if (n_hits35 == 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
            }
            }
            else if (n == 36 ){ // อิฐก้อนที่ 36 ต้อง ทำลาย 4 ครั้ง ถึงจะได้แต้ม 40 
            if (!bricks[n].destroyed &&
                collide(ball, bricks[n]) == True) {
                cpPlaySound(metal_sound);
                ball.vel_y = -ball.vel_y;
                n_hits36++ ;
                if (n_hits36 == 4){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 40; //คะแนนเพิ่ม
                }
                break;
            }
            }
            else if (n == 18 ){ // อิฐก้อนที่ 18 ต้อง ทำลาย 3 ครั้ง ถึงจะได้แต้ม 30 
            if (!bricks[n].destroyed &&
                collide(ball, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball.vel_y = -ball.vel_y;
                n_hits18++ ;
                if (n_hits18 == 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
            }
            }
            else if (n == 21 ){ // อิฐก้อนที่ 21 ต้อง ทำลาย 3 ครั้ง ถึงจะได้แต้ม 30 
            if (!bricks[n].destroyed &&
                collide(ball, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball.vel_y = -ball.vel_y;
                n_hits21++ ;
                if (n_hits21 == 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
            }
            }
            else if (n == 42 ){ // อิฐก้อนที่ 42 ต้อง ทำลาย 3 ครั้ง ถึงจะได้แต้ม 30 
            if (!bricks[n].destroyed &&
                collide(ball, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball.vel_y = -ball.vel_y;
                n_hits42++ ;
                if (n_hits42 == 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
            }
            }
            else if (n == 45 ){ // อิฐก้อนที่ 45 ต้อง ทำลาย 3 ครั้ง ถึงจะได้แต้ม 30 
            if (!bricks[n].destroyed &&
                collide(ball, bricks[n]) == True) {
                cpPlaySound(allu_sound);
                ball.vel_y = -ball.vel_y;
                n_hits45++ ;
                if (n_hits45 == 3){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 30; //คะแนนเพิ่ม
                }
                break;
            }
            }
            else if (n == 9 ){ // อิฐก้อนที่ 9 ต้อง ทำลาย 2 ครั้ง ถึงจะได้แต้ม 20 
            if (!bricks[n].destroyed &&
                collide(ball, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball.vel_y = -ball.vel_y;
                n_hits9++ ;
                if (n_hits9 == 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
            }
            }
            else if (n == 14 ){ // อิฐก้อนที่ 14 ต้อง ทำลาย 2 ครั้ง ถึงจะได้แต้ม 20 
            if (!bricks[n].destroyed &&
                collide(ball, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball.vel_y = -ball.vel_y;
                n_hits14++ ;
                if (n_hits14 == 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
            }
            }
            else if (n == 49 ){ // อิฐก้อนที่ 49 ต้อง ทำลาย 2 ครั้ง ถึงจะได้แต้ม 20 
            if (!bricks[n].destroyed &&
                collide(ball, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball.vel_y = -ball.vel_y;
                n_hits49++ ;
                if (n_hits49 == 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
            }
            }
            else if (n == 54 ){ // อิฐก้อนที่ 54 ต้อง ทำลาย 2 ครั้ง ถึงจะได้แต้ม 20 
            if (!bricks[n].destroyed &&
                collide(ball, bricks[n]) == True) {
                cpPlaySound(red_sound);
                ball.vel_y = -ball.vel_y;
                n_hits54++ ;
                if (n_hits54 == 2){
                    bricks[n].destroyed = True; //ทำลายอิฐ
                    n_hits++; //จำนวนทีอิฐ่ทำลาย
                    score += 20; //คะแนนเพิ่ม
                }
                break;
            }
            }
            else if(!bricks[n].destroyed &&
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
            if (ball.y >= paddle.y || ball.y >= 590)
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
