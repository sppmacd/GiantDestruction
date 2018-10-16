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
            else if(j < hh+3)
            {
                b.blockType = 2; // Dirt;
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

    if(player.getPosition().y > 40.f)
        respawnPlayer(5.f, 0.f);
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

bool World::isCollidedWithPlayer(int x, int y)
{
    return player.getRect().intersects(blocks[x][y].getRect(x,y));
}

void World::movePlayer(float x, float y)
{
    player.move(x,y);
}

void World::placeBlock(int x, int y)
{
    if((GameSettings::world.getBlock(x+1, y).blockType != 0 ||
       GameSettings::world.getBlock(x-1, y).blockType != 0 ||
       GameSettings::world.getBlock(x, y+1).blockType != 0 ||
       GameSettings::world.getBlock(x, y-1).blockType != 0 ||
       GameSettings::world.getBlock(x+1, y+1).blockType != 0 ||
       GameSettings::world.getBlock(x-1, y+1).blockType != 0 ||
       GameSettings::world.getBlock(x+1, y-1).blockType != 0 ||
       GameSettings::world.getBlock(x-1, y-1).blockType != 0)
        && !GameSettings::world.isCollidedWithPlayer(x, y)
       )
    {
        World::Block block;

        if(GameSettings::world.getBlock(x, y).blockType == 0)
        {
            block.blockType = 1;
            block.flags = 0;
        }
        else
        {
            block.blockType = 0;
            block.flags |= BlockFlags::WORLD_BACK_LAYER;
        }

        GameSettings::world.setBlock(x, y, block);
    }
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

float ScreenSettings::zoom;

void World::respawnPlayer(float x, float y)
{
    player.respawn();
    player.setPosition(x,y);
}

void World::draw(RenderWindow& wnd)
{
    float bsize = ScreenSettings::getBlockSize();
    int startX = player.getScreenPosition().x/bsize;
    int startY = player.getScreenPosition().y/bsize;

    for(float i = startX - 24*ScreenSettings::zoom; i < startX + 24*ScreenSettings::zoom; i++)
    for(float j = startY - 14*ScreenSettings::zoom; j < startY + 14*ScreenSettings::zoom; j++)
    {
        World::Block block = getBlock(i,j);
        ScreenRenderer::drawBlock(wnd, block, i, j);
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

void World::getPlayer()
{
    return player;
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
