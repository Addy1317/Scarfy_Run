#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};
//---------------------------------------------------------------------------------------------------------
bool isOnGround(AnimData data, int windowHeighht)
{
    return data.pos.y >= windowHeighht - data.rec.height;
}
//---------------------------------------------------------------------------------------------------------
// Anim data for sarfy
AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    // update running time
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
        // update animation frame
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
}
//------------------------------------------------------------------------------------------------------------
int main()
{
    //------------------------------------------------------------------------------------------------------------
    // window Dimension using Array
    int windowDimensions[2];
    windowDimensions[0] = 512;
    windowDimensions[1] = 380;
    // initialize Window
    InitWindow(windowDimensions[0], windowDimensions[1], "RunnerUp");
    // acceleartion due to Gravity(pixels/s)/s
    const int gravity{1'000};

    //-------------------------------------------------------------------------------------------------------------
    // Nebula Variable
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    // size of nebula
    const int sizeofNebulae{10};
    // Array and Loop for Anim Data
    AnimData nebulae[sizeofNebulae]{};
    // for loop Nebula
    for (int i = 0; i < sizeofNebulae; i++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width / 8;
        nebulae[i].rec.height = nebula.height / 8;
        nebulae[i].pos.y = windowDimensions[1] - nebula.height / 8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 0.0;
        nebulae[i].pos.x = windowDimensions[0] + i * 300;
    }

    // finish line condition
    float finishLine{nebulae[sizeofNebulae - 1].pos.x};
    // nebula X Velocity (Pixels/seconds)
    int nebVel{-200};

    //------------------------------------------------------------------------------------------------------------
    // scarfy Vaiables
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.rec.width = scarfy.width / 6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;
    scarfyData.pos.x = windowDimensions[0] / 2 - scarfyData.rec.width / 2;
    scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;
    scarfyData.frame = 0;
    scarfyData.updateTime = 1.0 / 12.0;
    scarfyData.runningTime = 0.0;

    //-----------------------------------------------------------------------------------------------------------------------------
    // Is the Rectangle in Air
    bool IsinAir{};
    // JUMP VELOCITY pixels/sec
    const int jumpVel{-600};
    // jump velocity
    int velocity{0};

    //------------------------------------------------------------------------------------------------------------------------------
    // Ground textures
    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX{};
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX{};
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX{};

    bool collision{};
    
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        // delta time (time since last frame)
        const float dT{GetFrameTime()};
        //------------------------------------------------------------------------------------------------------------
        // Game Logic Begins
        BeginDrawing();
        ClearBackground(WHITE);

        //------------------------------------------------------------------------------------------------------------
        // scroll background
        bgX -= 20 * dT;
        if (bgX <= -background.width * 2)
        {
            bgX = 0.0;
        }
        // scroll midground
        mgX -= 40 * dT;
        if (mgX <= -midground.width * 2)
        {
            mgX = 0.0;
        }
        // scroll foreground
        fgX -= 80 * dT;
        if (fgX <= -foreground.width * 2)
        {
            fgX = 0.0;
        }
        // DRAW background
        Vector2 bg1Pos{bgX, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
        Vector2 bg2Pos{bgX + background.width * 2, 0.0};
        DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);

        // draw midground
        Vector2 mg1Pos{mgX, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
        Vector2 mg2Pos{mgX + midground.width * 2, 0.0};
        DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);

        // draw foreground
        Vector2 fg1Pos{fgX, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
        Vector2 fg2Pos{fgX + foreground.width * 2, 0.0};
        DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);

        //--------------------------------------------------------------------------------------------------------------
        if (isOnGround(scarfyData, windowDimensions[1]))
        {
            velocity = 0;
            IsinAir = false;
        }
        else
        {
            velocity += gravity * dT;
            IsinAir = true;
        }

        // Jump Check
        if (IsKeyPressed(KEY_SPACE) && !IsinAir)
        {
            velocity += jumpVel;
        }
        //-------------------------------------------------------------------------------------------------------------
        for (int i = 0; i < sizeofNebulae; i++)
        {
            // update the position of each Nebula
            nebulae[i].pos.x += nebVel * dT;
        }
        // update finish Line
        finishLine += nebVel * dT;
        //-----------------------------------------------------------------------------------------------------------
        // update scarfy position
        scarfyData.pos.y += velocity * dT;

        // update scarfy's animation frame
        if (!IsinAir)
        {
            scarfyData = updateAnimData(scarfyData, dT, 5);
        }
        //-----------------------------------------------------------------------------------------------------------
        for (int i = 0; i < sizeofNebulae; i++)
        {
            nebulae[i] = updateAnimData(nebulae[i], dT, 7);
        }
        //------------------------------------------------------------------------------------------------------------
        // collision condition
        for (AnimData nebula : nebulae)
        {
            float pad{50};
            Rectangle nebRec{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2 * pad,
                nebula.rec.height - 2 * pad
                };
            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
                };
            if (CheckCollisionRecs(nebRec, scarfyRec))
            {
                collision = true;
            }

        }
        
        if (collision)
        {
            DrawText("Game Over!", windowDimensions[0]/4,windowDimensions[1]/2, 40, RED);
        }
        else if(scarfyData.pos.x >= finishLine)
        {
            DrawText("You Win!", windowDimensions[0]/4,windowDimensions[1]/2, 40, RED);
        }
        else
        {
            // draw Nebula
            for (int i = 0; i < sizeofNebulae; i++)
            {
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
            }
             // Draw Scarfy
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        }
       
        // Game Logic Ends
        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);

    CloseWindow();
}