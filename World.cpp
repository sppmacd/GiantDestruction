#include "World.h"

#include "Settings.h"

namespace GameSettings
{
    const int WORLD_SIZE_X = 18;
    const int WORLD_SIZE_Y = 18;
}

namespace BlockFlags
{
    const int WORLD_BACK_LAYER = 0x1;
    const int WORLD_HARVESTABLE = 0x2;
    const int WORLD_UPDATEABLE = 0x4;
    const int WORLD_PHYSICAL = 0x8;
}

void World::loadFromFile(int chunkId)
{
    Chunk* chunk = new Chunk;

    if(!chunks.count(chunkId))
    {
        ifstream file;

        file.open("res/world/chunk_"+to_string(chunkId)+".chunk", ios::binary);
        if(!file.good())
        {
            chunks.insert(make_pair(chunkId,chunk));
            return;
        }

        int blockX = 0;
        int blockY = 0;

        cout << endl;

        while(!file.eof())
        {
            char a,b;
            file.read(&a,1);
            file.read(&b,1);
            short blockCode = ((a << 8) | b) & 0xFFFF;

            cout << "[" << blockX << "," << blockY << "]: 0x" << hex << blockCode << "; ";
            Block block(blockCode);
            chunk->setBlock(blockX, blockY, block); //nie dziala powyzej 8!

            blockY++;
            if(blockY >= 18)
            {
                blockY = 0;
                blockX++;
                cout << endl;
            }
        }
    }
    //addChunk(chunk,chunkId);
    chunks.insert(make_pair(chunkId,chunk));
}

void World::addChunk(Chunk* chunk, int id)
{
    chunks.insert(make_pair(id,chunk));
}

World::Block::Block(unsigned short code)
{
    this->blockType = (code & 0xF000) >> 12;
    this->heightType = (code & 0xF00) >> 8;
    this->meta = (code & 0xF0) >> 4;
    this->flags = code & 0xF;
}

World::~World()
{
    for(auto ch : chunks)
    {
        delete ch.second;
    }
}

World::World()
{
    //int hh = GameSettings::WORLD_SIZE_Y / 2;
    GameSettings::saveDefaultWorld();
    loadFromFile(0);
    /*for(int i = 0; i < GameSettings::WORLD_SIZE_X; i++)
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
    }*/

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
    player.jump();
}

bool World::isCollided(float x, float y, float sx, float sy)
{
    bool collide = false;
    for(int i = int(x)-int(x)%9; i < int(x)-int(x)%9+9; i++)
    for(int j = 0; j < GameSettings::WORLD_SIZE_Y; j++)
    {
        //cout << player.getRect().left << "," << player.getRect().top << endl;
        //cout << blocks[i][j].getRect(i,j).left << "," << blocks[i][j].getRect(i,j).top << endl;
        //cout << blocks[i][j].flags << endl;
        Block b = getBlock(i,j);
        bool t = b.getRect(i,j).intersects(FloatRect(x,y,sx,sy)) && !(b.flags & BlockFlags::WORLD_BACK_LAYER);
        if(t)
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
    if(_x < 0 || _y < 0)
        return;

    int x = _x;
    int y = _y;

    if(chunks.count(x/9))
    {
        Chunk* chunk = chunks[x/9];
        chunk->setBlock(x%9,y,block);
    }
    else
        loadFromFile(x/9);
}

bool World::isCollidedWithPlayer(int x, int y)
{
    return player.getRect().intersects(getBlock(x,y).getRect(x,y));
}

void World::movePlayer(float x, float y, bool disableReset)
{
    player.move(x,y,disableReset);
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
       GameSettings::world.getBlock(x-1, y-1).blockType != 0 ||
       GameSettings::world.getBlock(x, y).blockType != 0)
        && !GameSettings::world.isCollidedWithPlayer(x, y)
       )
    {
        World::Block block(0);

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
    if(_x < 0 || _y < 0)
        return World::Block();

    int x = _x;
    int y = _y;

    if(chunks.count(x/9))
    {
        if(x == -1 || x == GameSettings::WORLD_SIZE_X+1 || y == -1 || y == GameSettings::WORLD_SIZE_Y+1)
            return World::Block(); //air
        else
            return chunks[x/9]->getBlock(x%9,y);
    }
    else
    {
        loadFromFile(x/9);
        return World::Block();
    }
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
    //int startY = player.getScreenPosition().y/bsize;

    float iend = startX + 24*ScreenSettings::zoom;

    for(float i = startX - 24*ScreenSettings::zoom; i < iend; i++)
    for(float j = 0; j < 18; j++)
    {
        if(i >= 0 && j >= 0)
        {
            //cout << "startdraw " << i << "," << j << endl;
            World::Block block = getBlock(i,j);
            ScreenRenderer::drawBlock(wnd, block, i, j);
            //cout << "enddraw " << i << "," << j << endl;
        }
    }

    //player
    RectangleShape rs(Vector2f(bsize, bsize*3.f));
    rs.setFillColor(Color(100,0,0));
    rs.setPosition(player.getScreenPosition());
    wnd.draw(rs);

    ////////////////////////////
    // block info and outline //
    ////////////////////////////

    Vector2f pos = ScreenSettings::screenPosToB2(ScreenSettings::window.mapPixelToCoords(Mouse::getPosition(ScreenSettings::window)));

    RectangleShape rs2(Vector2f(ScreenSettings::getBlockSize(), ScreenSettings::getBlockSize()));
    rs2.setPosition(ScreenSettings::b2PosToScreen(Vector2f(int(pos.x),int(pos.y))));
    rs2.setFillColor(Color::Transparent);
    rs2.setOutlineColor(Color::Red);
    rs2.setOutlineThickness(1.f);
    ScreenSettings::window.draw(rs2);
}

bool World::Block::hasFlag(unsigned flag)
{
    return flags & flag;
}

Player& World::getPlayer()
{
    return player;
}

void World::Block::setFlag(unsigned flag)
{
    flags |= flag;
}

void World::Block::unsetFlag(unsigned flag)
{
    flags ^= flag;
}

FloatRect World::Block::getRect(int x, int y)
{
    return FloatRect(x,y,1,1);
}


Chunk::Chunk()
{
}

void Chunk::setBlock(int x, int y, World::Block& block)
{
    if(x >=0 && x < 9 && y >= 0 && y < 18)
        blocks[x][y] = block;
}
World::Block Chunk::getBlock(int x, int y)
{
    if(x >= 0 && x < 9 && y >= 0 && y < 18)
        return blocks[x][y];
    else
        return World::Block();
}
