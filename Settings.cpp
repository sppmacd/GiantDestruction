#include "Settings.h"

map<string,Texture> ScreenSettings::textures;
View ScreenSettings::currentGUIView;

const unsigned short DEFAULT_WORLD[][9][18]=
    {//FFFF TYPE,HEIGHT,META,FLAGS
        {
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0x3300,0x2300,0x1300},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0x3600,0x2300,0x1300},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0x3700,0x3330,0x2300},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,0x9301,0x8c01,0x8c01,0x3300,0x2300},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0x3600,0x2300},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,0x9301,0x8c01,0x3700,0x3330},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,0x9301,0x8c01,0x8c01,0x3300},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0x3400,0x3320},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,0x9301,0x8c01,0x3500,0x2300}
        },
        {
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0x3300,0x2300},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0x3100,0x3300,0x2300},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,0x3900,0x3800,0x3300,0x2300,0x1300},
            {1,1,1,1,1,1,1,1,1,0x4400,1,1,0x3100,0x3340,0x2300,0x1300,0x1300,0x1300},
            {1,1,1,1,1,1,1,1,1,0x4500,0xa300,0xb300,0x3300,0x2300,0x1300,0x1300,0x1300,0x1300},
            {1,1,1,1,1,1,1,1,1,0x4600,0xa300,0xb300,0x3300,0x2300,0x1300,0x1300,0x1300,0x1300},
            {1,1,1,1,1,1,1,1,1,0x4700,1,1,0x3600,0x2300,0x1300,0x1300,0x1300,0x1300},
            {1,1,1,1,1,1,1,1,1,1,1,1,0x3700,0x3330,0x2300,0x2300,0x1300,0x1300},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,0x3a00,0x3b00,0x3300,0x2300,0x1300}
        }
    };
void GameSettings::saveDefaultWorld()
{
    /*for(int i=0; i<2; i++)
    {//chunk
        ofstream file("res/world/chunk_"+to_string(i)+".chunk", ios::binary);
        for(int j=0; j<9; j++)
        {//vert.layer
            for(int k=0; k<18; k++)
            {//block
                string str;
                cout << hex << "0x" << DEFAULT_WORLD[i][j][k] << ":";
                str += (unsigned char)((DEFAULT_WORLD[i][j][k] & 0b1111111100000000) >> 8);
                str += (unsigned char)(DEFAULT_WORLD[i][j][k] & 0b11111111);
                file << (str);
            }
            cout << endl;
        }
        file.close();
    }*/
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

void ScreenRenderer::drawGUI()
{
    //health bar
    RectangleShape rsFilledHB(Vector2f(200.f, 20.f));
    rsFilledHB.setFillColor(Color(200,200,200));
    rsFilledHB.setOutlineColor(Color(50,50,50));
    rsFilledHB.setOutlineThickness(1.f);
    rsFilledHB.setPosition(Vector2f(10.f,10.f));
    ScreenSettings::window.draw(rsFilledHB);

    RectangleShape rsHealthBar(Vector2f(GameSettings::world.getPlayer().health * 200.f / 120.f, 20.f));
    int htc = GameSettings::world.getPlayer().health * 255/120;
    rsHealthBar.setFillColor(Color(255-htc,htc,0));
    rsHealthBar.setPosition(Vector2f(10.f,10.f));
    ScreenSettings::window.draw(rsHealthBar);

    /////////////
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
    // Sky
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

    // Blocks
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

    rs.setFillColor(Color(255,255,255,alpha));
    rs.setTexture(&ScreenSettings::getTexture("terrain"), false);
    rs.setTextureRect(IntRect(block.blockType*64,block.meta*64,64,64));
    rs.setPosition(ScreenSettings::b2PosToScreen(Vector2f(x,y)));
    wnd.draw(rs);
}
