#include "Settings.h"

map<string,Texture> ScreenSettings::textures;

const short DEFAULT_WORLD[9][9][9]=
    {//FFFF TYPE,HEIGHT,META,FLAGS
        {
            {1,1,1,1,1,1,0x3300,0x2300,0x1300},
            {1,1 1,1,1,1,0x3600,0x2300,0x1300},
            {1,1,1,1,1,1,0x3700,0x3330,0x2300},
            {1,1,1,1,0x9300,0x8c00,0x8c00,0x3300,0x2300},
            {1,1,1,1,1,1,1,0x3600,0x2300},
            {1,1,1,1,1,0x9300,0x8c00,0x3700,0x3330},
            {1,1,1,1,1,0x9300,0x8c00,0x8c00,0x3300},
            {1,1,1,1,1,1,1,0x3400,0x3320},
            {1,1,1,1,1,0x9300,0x8c00,0x3500,0x2300}
        }
    }
void GameSettings::saveDefaultWorld()
{
    string str = "MSD";
    for(int i=0; i<9; i++)
    {//chunk
        str="MSD\255";
        for(int j=0; j<9; j++)
        {//vert.layer
            str+=0xFE;
            for(int k=0; k<9; k++)
            {//block
                str+=DEFAULT_WORLD[i][j][k];
            }

        }
        ofstream fl("res/world/w"+to_string(i));
        fl<<str;
        f1.close();
    }
}
Vector2f ScreenSettings::b2PosToScreen(Vector2f vec)
{
    float x = ScreenSettings::getBlockSize()*vec.x;
    float y = ScreenSettings::getBlockSize()*vec.y;
    return Vector2f(x,y);
}

Vector2f ScreenSettings::screenPosToB2(Vector2f vec)
{
    float x = vec.x/ScreenSettings::getBlockSize();
    float y = vec.y/ScreenSettings::getBlockSize();
    return Vector2f(x,y);
}

float ScreenSettings::getBlockSize()
{
    return 32.f;
}

void ScreenSettings::loadTextures()
{
    loadTexture("terrain");
}
void ScreenSettings::loadTexture(string name)
{
    Texture texture;
    texture.loadFromFile("res/"+name+".png");
    ScreenSettings::textures.insert(make_pair(name,texture));
}
Texture& ScreenSettings::getTexture(string name)
{
    return ScreenSettings::textures[name];
}

void ScreenRenderer::drawWorld()
{
    VertexArray varr(Quads, 4);
    varr[0].position = Vector2f(0.f, -600.f);
    varr[0].color = Color(0,0,0);

    varr[1].position = Vector2f(ScreenSettings::getBlockSize() * GameSettings::WORLD_SIZE_X, -600.f);
    varr[1].color = Color(0,0,0);

    varr[2].position = Vector2f(ScreenSettings::getBlockSize() * GameSettings::WORLD_SIZE_X, ScreenSettings::getBlockSize() * GameSettings::WORLD_SIZE_Y);
    varr[2].color = Color(40,255,255);

    varr[3].position = Vector2f(0.f, ScreenSettings::getBlockSize() * GameSettings::WORLD_SIZE_Y);
    varr[3].color = Color(40,255,255);

    ScreenSettings::window.draw(varr);

    GameSettings::world.draw(ScreenSettings::window);
}

void ScreenRenderer::drawBlock(RenderWindow& wnd, World::Block block, int x, int y)
{
    float bsize = ScreenSettings::getBlockSize();
    RectangleShape rs(Vector2f(bsize, bsize));

    int alpha = block.hasFlag(BlockFlags::WORLD_BACK_LAYER) ? 100 : 255;

    if(FloatRect(ScreenSettings::b2PosToScreen(Vector2f(x,y)), Vector2f(bsize, bsize)).contains(wnd.mapPixelToCoords(Mouse::getPosition(wnd))))
    {
        rs.setOutlineColor(Color::Red);
        rs.setOutlineThickness(1.f);
    }

    rs.setTexture(&ScreenSettings::getTexture("terrain"), false);
    rs.setTextureRect(IntRect(block.blockType*64,block.meta*64,64,64));
    rs.setPosition(ScreenSettings::b2PosToScreen(Vector2f(x,y)));
    wnd.draw(rs);
}
