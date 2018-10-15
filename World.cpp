#include "World.h"

#include "Settings.h"

namespace GameSettings
{
    const int WORLD_SIZE_X = 48;
    const int WORLD_SIZE_Y = 9;
}

namespace BlockFlags
{
    const int WORLD_BACK_LAYER = 0x1;
    const int WORLD_HARVESTABLE = 0x2;
    const int WORLD_UPDATEABLE = 0x4;
    const int WORLD_PHYSICAL = 0x8;
}


World::World()
: world(b2World(b2Vec2(0.f,-9.81f))), player(&world)
{
    b2BodyDef def;
    def.userData = this;
    terrainBody = world.CreateBody(&def);

    for(int i = 0; i < GameSettings::WORLD_SIZE_X; i++)
    for(int j = 0; j < GameSettings::WORLD_SIZE_Y; j++)
    {
        Block b = Block{0,rand()%16,0,0,NULL};

        if(rand()%2==0)
            b.setFlag(BlockFlags::WORLD_BACK_LAYER);

        setBlock(i,j,b);
    }
}

void World::update()
{
    ScreenSettings::currentWorldView.setCenter(player.getScreenPosition());
    //world.Step(1/60.f, 10, 10);
}

void World::setBlock(int _x, int _y, World::Block& block)
{
    int x = max(0, min(_x, GameSettings::WORLD_SIZE_X));
    int y = max(0, min(_y, GameSettings::WORLD_SIZE_Y));

    //Update the body stats

    b2Fixture* terr = terrainBody->GetFixtureList();
    if(terrainBody->GetFixtureCount() != 0)
    do
    {
        if(block.hasFlag(BlockFlags::WORLD_BACK_LAYER) && !getBlock(x,y).hasFlag(BlockFlags::WORLD_BACK_LAYER))
        {
            terrainBody->DestroyFixture(terr);
            break;
        }
        else if(!block.hasFlag(BlockFlags::WORLD_BACK_LAYER) && getBlock(x,y).hasFlag(BlockFlags::WORLD_BACK_LAYER))
        {
            b2PolygonShape shape;
            shape.SetAsBox(1.f, 1.f);
            terrainBody->CreateFixture(&shape, 1.f);
        }
    }while((terr = terr->GetNext()));

    // Update block
    blocks[x][y].blockType = block.blockType;
    blocks[x][y].flags = block.flags;
    blocks[x][y].heightType = block.heightType;
    blocks[x][y].meta = block.meta;
}

World::Block World::getBlock(int x, int y)
{
    return blocks[max(0, min(x, GameSettings::WORLD_SIZE_X))][max(0, min(y, GameSettings::WORLD_SIZE_Y))];
}

void World::draw(RenderWindow& wnd)
{
    float bsize = ScreenSettings::getBlockSize();
    int startX = player.getScreenPosition().x/bsize;
    int startY = player.getScreenPosition().y/bsize;

    for(int i = startX - 8; i < startX + 8; i++)
    for(int j = startY - 5; j < startY + 3; j++)
    {
        World::Block block = getBlock(i,j);
        RectangleShape rs(Vector2f(bsize, bsize*ScreenSettings::X_BY_Y));

        int c1 = block.blockType*16;
        int alpha = block.hasFlag(BlockFlags::WORLD_BACK_LAYER) ? 100 : 255;
        rs.setFillColor(Color(c1,c1,c1, alpha));
        rs.setPosition(bsize*i,bsize*j*ScreenSettings::X_BY_Y);
        rs.setOutlineColor(Color::White);
        rs.setOutlineThickness(1.f);
        wnd.draw(rs);
    }

    //player
    RectangleShape rs(Vector2f(bsize, bsize*ScreenSettings::X_BY_Y*2.f));
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
