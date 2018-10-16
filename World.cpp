#include "World.h"

#include "Settings.h"

namespace GameSettings
{
    const int WORLD_SIZE_X = 48;
    const int WORLD_SIZE_Y = 18;
}

namespace BlockFlags
{
    const int WORLD_BACK_LAYER = 0x1;
    const int WORLD_HARVESTABLE = 0x2;
    const int WORLD_UPDATEABLE = 0x4;
    const int WORLD_PHYSICAL = 0x8;
}


World::World()
{
    int hh = GameSettings::WORLD_SIZE_Y / 2;
    for(int i = 0; i < GameSettings::WORLD_SIZE_X; i++)
    {
        hh += rand() % 3 - 1;

        if(hh > GameSettings::WORLD_SIZE_Y - 3)
            hh -= 2;
        else if(hh < 5)
            hh += 2;

        for(int j = 0; j < GameSettings::WORLD_SIZE_Y; j++)
        {
            Block b;
            b.heightType = 0;
            b.meta = 0;
            b.flags = 0;

            if(j < hh)
            {
                b.blockType = 0;
                b.flags |= BlockFlags::WORLD_BACK_LAYER;
            }
            else if(j == hh)
            {
                b.blockType = 3; //Grass
            }
            else
            {
                b.blockType = 1; //Stone
            }

            setBlock(i,j,b);
            //cout << blocks[i][j].flags << endl;
        }
    }
}

void World::update()
{
    ScreenSettings::currentWorldView.setCenter(player.getScreenPosition());
    acceleratePlayer(0.f, 0.0106f);
    player.update();
}

void World::acceleratePlayer(float x, float y)
{
    player.velocity += Vector2f(x,y);
}

void World::jump()
{
    if(!player.jumping)
    {
        acceleratePlayer(0.f, -0.195f);
        player.jumping = true;
    }
}

bool World::isCollided(float x, float y, float sx, float sy)
{
    bool collide = false;
    for(int i = 0; i < GameSettings::WORLD_SIZE_X; i++)
    for(int j = 0; j < GameSettings::WORLD_SIZE_Y; j++)
    {
        //cout << player.getRect().left << "," << player.getRect().top << endl;
        //cout << blocks[i][j].getRect(i,j).left << "," << blocks[i][j].getRect(i,j).top << endl;
        //cout << blocks[i][j].flags << endl;
        if(blocks[i][j].getRect(i,j).intersects(FloatRect(x,y,sx,sy)) && !(blocks[i][j].flags & BlockFlags::WORLD_BACK_LAYER))
        {
            collide = true;
            break;
        }
    }
    //Update the player!
    return collide;
}
void World::setBlock(int _x, int _y, World::Block& block)
{
    int x = max(0, min(_x, GameSettings::WORLD_SIZE_X));
    int y = max(0, min(_y, GameSettings::WORLD_SIZE_Y));

    blocks[x][y].blockType = block.blockType;
    blocks[x][y].flags = block.flags;
    blocks[x][y].heightType = block.heightType;
    blocks[x][y].meta = block.meta;
}

void World::movePlayer(float x, float y)
{
    player.move(x,y);
}

World::Block World::getBlock(int _x, int _y)
{
    int x = max(-1, min(_x, GameSettings::WORLD_SIZE_X+1));
    int y = max(-1, min(_y, GameSettings::WORLD_SIZE_Y+1));

    if(x == -1 || x == GameSettings::WORLD_SIZE_X+1 || y == -1 || y == GameSettings::WORLD_SIZE_Y+1)
        return World::Block{0,0,0,0x1}; //air
    else
        return blocks[x][y];
}

void World::draw(RenderWindow& wnd)
{
    float bsize = ScreenSettings::getBlockSize();
    int startX = player.getScreenPosition().x/bsize;
    int startY = player.getScreenPosition().y/bsize;

    for(float i = startX - 16; i < startX + 16; i++)
    for(float j = startY - 9; j < startY + 9; j++)
    {
        World::Block block = getBlock(i,j);
        RectangleShape rs(Vector2f(bsize, bsize));

        int alpha = block.hasFlag(BlockFlags::WORLD_BACK_LAYER) ? 100 : 255;

        switch(block.blockType)
        {
            case 0: rs.setFillColor(Color(0,0,0,alpha)); break;
            case 1: rs.setFillColor(Color(128,128,128,alpha)); break;
            case 3: rs.setFillColor(Color(0,128,0,alpha)); break;
            default: rs.setFillColor(Color(255,255,255,alpha)); break;
        }

        rs.setPosition(ScreenSettings::b2PosToScreen(Vector2f(i,j)));
        wnd.draw(rs);
    }

    //player
    RectangleShape rs(Vector2f(bsize, bsize*3.f));
    rs.setFillColor(Color(100,0,0));
    rs.setPosition(player.getScreenPosition());
    wnd.draw(rs);
}

bool World::Block::hasFlag(int flag)
{
    return flags & flag;
}

void World::Block::setFlag(int flag)
{
    flags |= flag;
}

void World::Block::unsetFlag(int flag)
{
    flags ^= flag;
}

FloatRect World::Block::getRect(int x, int y)
{
    return FloatRect(x,y,1,1);
}
