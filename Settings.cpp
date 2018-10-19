#include "Settings.h"

map<string,Texture> ScreenSettings::textures;
View ScreenSettings::currentGUIView;

/*const unsigned short DEFAULT_WORLD[][9][18]=
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
    };*/

const int blockHTV[][4][4] =
{
    {{0,29,0,0},{0,32,0,64},{32,32,64,64},{32,29,64,0}}, //PLATE    0
    {{0,32,0,0},{32,32,0,64},{32,32,64,64},{32,0,64,0}}, //45U       1
    {{0,0,0,0},{0,32,0,64},{0,32,64,64},{32,32,64,0}}, //45D        2
    {{0,0,0,0},{0,32,0,64},{32,32,64,64},{32,0,64,0}}, //FULL       3
    {{0,32,0,0},{32,32,0,64},{32,32,64,64},{32,16,64,0}}, //225UD   4
    {{0,16,0,0},{0,32,0,64},{32,32,64,64},{32,0,64,0}}, //225UU     5
    {{0,0,0,0},{0,32,0,64},{32,32,64,64},{32,16,64,0}}, //225DU     6
    {{0,16,0,0},{0,32,0,64},{0,32,64,64},{32,32,64,0}}, //225DD     7
    {{0,32,0,0},{32,32,0,64},{32,0,64,64},{16,0,64,0}}, //675UD     8 v
    {{16,32,0,0},{32,32,0,64},{32,32,64,64},{32,0,64,0}}, //675UU   9
    {{0,0,0,0},{0,32,0,64},{0,32,64,64},{16,32,64,0}}, //675DU      a
    {{16,0,0,0},{0,0,0,64},{0,32,64,64},{32,32,64,0}}, //675DD      b v
    {{8,0,0,0},{24,0,0,64},{24,32,64,64},{24,0,64,0}}, //VPLIIAR    c v
    {{0,8,0,0},{0,24,0,64},{32,24,64,64},{32,8,64,0}}, //HPILLAR    d
    {{8,8,0,0},{8,24,0,64},{24,24,64,64},{24,8,64,0}}, //CPILLAR    e
    {{0,0,0,0},{0,0,0,64},{0,0,64,64},{0,0,64,0}} //STRUCTURE f
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

Font ScreenSettings::font;

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

    //debug info

    ScreenSettings::window.setView(ScreenSettings::currentWorldView);
    Vector2f pos = ScreenSettings::screenPosToB2(ScreenSettings::window.mapPixelToCoords(Mouse::getPosition(ScreenSettings::window)));
    ScreenSettings::window.setView(ScreenSettings::currentGUIView);

    World::Block block = GameSettings::world.getBlock(pos.x,pos.y);
    Text text(string("Giant Destruction v1.0 [beta] [build ")+string(__DATE__)+" "+string(__TIME__)+string("]\n")+
          "Mx: "+to_string(Mouse::getPosition().x) + "\n"+
          "My: "+to_string(Mouse::getPosition().y) + "\n"+
          "Bx: "+to_string(int(pos.x)) + "\n"+
          "By: "+to_string(int(pos.y)) + "\n"+
          string("Bdata:")+
            " T: "+to_string(block.blockType) +
            " HT: "+to_string(block.heightType) +
            " M: "+to_string(block.meta) +
            " F: "+to_string(block.flags),
          ScreenSettings::font,15
          );
    text.setPosition(10.f, 40.f);
    ScreenSettings::window.draw(text);
}

float ScreenSettings::getBlockSize()
{
    return 32.f;
}

void ScreenSettings::loadTextures()
{
    loadTexture("terrain");
    ScreenSettings::font.loadFromFile("res/arial.ttf");
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

    varr[1].position = Vector2f(ScreenSettings::getBlockSize() * GameSettings::WORLD_SIZE_X, -300.f);
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
    //float bsize = ScreenSettings::getBlockSize();
    Vector2f bpos = ScreenSettings::b2PosToScreen(Vector2f(x,y));
    //int arr[4][4] = ;
    int alpha = block.hasFlag(BlockFlags::WORLD_BACK_LAYER) ? 100 : 255;
    auto arr = blockHTV[block.heightType];

    VertexArray varr(Quads, 4);

    varr.append(sf::Vertex(Vector2f(arr[0][0]+bpos.x,arr[0][1]+bpos.y), Color(255,255,255,alpha),
                           Vector2f(arr[0][2]+block.blockType*64,arr[0][3]+block.meta*64)));
    varr.append(sf::Vertex(Vector2f(arr[1][0]+bpos.x,arr[1][1]+bpos.y), Color(255,255,255,alpha),
                           Vector2f(arr[1][2]+block.blockType*64,arr[1][3]+block.meta*64)));
    varr.append(sf::Vertex(Vector2f(arr[2][0]+bpos.x,arr[2][1]+bpos.y), Color(255,255,255,alpha),
                           Vector2f(arr[2][2]+block.blockType*64,arr[2][3]+block.meta*64)));
    varr.append(sf::Vertex(Vector2f(arr[3][0]+bpos.x,arr[3][1]+bpos.y), Color(255,255,255,alpha),
                           Vector2f(arr[3][2]+block.blockType*64,arr[3][3]+block.meta*64)));

    wnd.draw(varr, &ScreenSettings::getTexture("terrain"));
}
