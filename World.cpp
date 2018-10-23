#include "World.h"

#include "Settings.h"
#include "WorldBlockSettings.h"

namespace GameSettings
{
    const int WORLD_SIZE_X = 5*9;
    const int WORLD_SIZE_Y = 18;
}

namespace BlockFlags
{
    const int WORLD_BACK_LAYER = 0x1;
    const int WORLD_HARVESTABLE = 0x2;
    const int WORLD_UPDATEABLE = 0x4;
    const int WORLD_PHYSICAL = 0x8;
}

void World::init()
{
    loadFromFile(0);
}

void World::loadFromFile(int chunkId)
{
    ScreenRenderer::drawLoadingProgress("Loading world", "Loading chunk: " + to_string(chunkId));
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

        //cout << endl;

        while(!file.eof())
        {
            unsigned char a,b;
            file.read((char*)&a,1);
            file.read((char*)&b,1);
            unsigned short blockCode = ((a << 8) | b) & 0xFFFF;

            //cout << "[" << blockX << "," << blockY << "]: A:" << int(a) << " B:" << int(b) << " C: 0x" << hex << blockCode << "; ";
            Block block(blockCode);
            chunk->setBlock(blockX, blockY, block); //nie dziala powyzej 8!

            blockY++;
            if(blockY >= 18)
            {
                blockY = 0;
                blockX++;
                //cout << endl;
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
    for(auto entity : entities)
    {
        delete entity;
    }
}

World::World()
{
    Player* pl = new Player; //PL not P1
    spawnEntity(pl);
    player = pl;

    for(int i = 0; i < 20; i++)
    {
        Entity* e1 = new Entity(Vector2f(rand() % GameSettings::WORLD_SIZE_X, 9.f)); //E1 not EL
        spawnEntity(e1);
    }
}

void World::update()
{
    ScreenSettings::currentWorldView.setCenter(player->getScreenPosition());

    for(unsigned int i = 0; i < entities.size(); i++)
    {
        Entity* entity = entities[i];
        entity->update();
        entity->velocity += Vector2f(0.f, 0.0106f);
    }

    if(player->getPosition().y > 40.f)
        respawnPlayer(5.f, 15.f);
}

void World::acceleratePlayer(float x, float y)
{
    player->velocity += Vector2f(x,y);
}

void World::jump()
{
    player->jump();
}

bool World::isCollided(float x, float y, float sx, float sy, bool includeHouses)
{
    bool collide = false;
    for(int i = int(x)-3; i < int(x)+3; i++)
    for(int j = 0; j < GameSettings::WORLD_SIZE_Y+1; j++)
    {
        //cout << player->getRect().left << "," << player->getRect().top << endl;
        //cout << blocks[i][j].getRect(i,j).left << "," << blocks[i][j].getRect(i,j).top << endl;
        //cout << blocks[i][j].flags << endl;
        Block b = getBlock(i,j);
        bool t = b.getRect(i,j).intersects(FloatRect(x,y,sx,sy)) && !(b.flags & BlockFlags::WORLD_BACK_LAYER);

        if(includeHouses && (b.blockType == BlockType::TYPE_DOORS || b.blockType == BlockType::TYPE_WINDOWS || b.blockType == BlockType::TYPE_BRICK))
            t = false;

        if(t)
        {
            collide = true;
            break;
        }
    }
    return collide;
}
void World::setBlock(int _x, int _y, World::Block& block)
{
    if(_x < 0 || _y < 0)
        return;

    int x = _x;
    int y = _y;

    if(chunks.count((x-(x%9))/9))
    {
        Chunk* chunk = chunks[(x-(x%9))/9];
        chunk->setBlock(x%9,y,block);
    }
    else
        loadFromFile((x-(x%9))/9);
}

bool World::isCollidedWithPlayer(int x, int y)
{
    return player->getRect().intersects(getBlock(x,y).getRect(x,y));
}

void World::movePlayer(float x, float y, bool disableReset)
{
    player->move(x,y,disableReset);
}

void World::placeBlock(int x, int y)
{
    if((getBlock(x+1, y).blockType != 0 ||
       getBlock(x-1, y).blockType != 0 ||
       getBlock(x, y+1).blockType != 0 ||
       getBlock(x, y-1).blockType != 0 ||
       getBlock(x+1, y+1).blockType != 0 ||
       getBlock(x-1, y+1).blockType != 0 ||
       getBlock(x+1, y-1).blockType != 0 ||
       getBlock(x-1, y-1).blockType != 0 ||
       getBlock(x, y).blockType != 0)
       )
    {
        World::Block block(0);
        World::Block block2 = getBlock(x, y);
        World::Block blockCeil = getBlock(x, y - 1);

        if(block2.blockType == 0 && !isCollidedWithPlayer(x, y) && player->inventory.getItem(player->currentBlock,0).id != 0)
        {
            block.blockType = player->inventory.getItem(player->currentBlock,0).id;
            block.flags = 0;
            block.heightType = BlockHeightType::HT_FULL_BlOCK;
            block.meta = BlockMetadata::META_DEFAULT;
            setBlock(x, y, block);
        }
        else
        {
            block.blockType = 0;
            block.flags |= BlockFlags::WORLD_BACK_LAYER;

            if((block2.blockType == BlockType::TYPE_WOOD && block2.heightType == BlockHeightType::HT_VERTICAL_PILLAR))
            {
                for(int i = y; getBlock(x,i+1).blockType != BlockType::TYPE_LEAVES && i > 0; i--)
                {
                    setBlock(x, i, block);
                }
            }
            else if((blockCeil.blockType == BlockType::TYPE_WOOD && blockCeil.heightType == BlockHeightType::HT_VERTICAL_PILLAR))
            {
                for(int i = y-1; getBlock(x,i+1).blockType != BlockType::TYPE_LEAVES && i > 0; i--)
                {
                    setBlock(x, i, block);
                }
            }
            else
            {
                setBlock(x, y, block);
            }
        }
    }
}

void World::spawnEntity(Entity* entity)
{
    entities.push_back(entity);
}

World::Block World::getBlock(int _x, int _y)
{
    if(_x < 0 || _y < 0)
        return World::Block();

    int x = _x;
    int y = _y;

    if(chunks.count((x-(x%9))/9))
    {
        if(x < 0 || x >= GameSettings::WORLD_SIZE_X || y < 0)
            return World::Block(); //air
        else if(y >= 18)
            return World::Block(0x13F0); //stone
        else
            return chunks[(x-(x%9))/9]->getBlock(x%9,y);
    }
    else
    {
        loadFromFile((x-(x%9))/9);
        return World::Block();
    }
}

float ScreenSettings::zoom;

void World::respawnPlayer(float x, float y)
{
    player->respawn();
    player->setPosition(x,y);
}

void World::draw(RenderWindow& wnd)
{
    float bsize = ScreenSettings::getBlockSize();
    int startX = player->getScreenPosition().x/bsize;
    //int startY = player->getScreenPosition().y/bsize;

    float iend = startX + 24;

    for(float i = startX - 24; i < iend; i++)
    for(float j = 0; j < 31; j++)
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
    /*RectangleShape rs(Vector2f(bsize, bsize*3.f));
    rs.setOrigin(bsize/2, bsize*3.f);
    rs.setPosition(player->getScreenPosition());
    rs.setTexture(&ScreenSettings::getTexture("player"));
    wnd.draw(rs);*/

    for(unsigned int i = 0; i < entities.size(); i++)
    {
        Entity* entity = entities[i];

        if(entity->shown)
        {
            RectangleShape rs(Vector2f(entity->getRect().width * (bsize+0.1f), entity->getRect().height * (bsize+0.1f)));
            rs.setOrigin(entity->getRect().width * (bsize+0.1f) / 2, entity->getRect().height * (bsize+0.1f));
            rs.setPosition(entity->getScreenPosition());
            rs.setTexture(&ScreenSettings::getTexture("player"));
            wnd.draw(rs);
        }
    }

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
    return *player;
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
    switch(heightType)
    {
        case 0: return FloatRect(x,y+0.9,1.0,0.1);
        case 1: return FloatRect(x+0.5,y+0.5,0.5,0.5);
        case 2: return FloatRect(x,y+0.5,0.5,0.5);
        case 3: return FloatRect(x,y,1.0,1.0);
        case 4: return FloatRect(x,y+0.75,1.0,0.25);
        case 5: return FloatRect(x,y+0.25,1.0,0.75);
        case 6: return FloatRect(x,y+0.25,1.0,0.75);
        case 7: return FloatRect(x,y+0.75,1.0,0.25);
        case 8: return FloatRect(x+0.25,y,0.75,1.0);
        case 9: return FloatRect(x+0.75,y,0.25,1.0);
        case 10: return FloatRect(x,y,0.25,1.0);
        case 11: return FloatRect(x,y,0.75,1.0);
        case 12: return FloatRect(x+0.25,y,0.5,1.0);
        case 13: return FloatRect(x,y+0.25,1.0,0.5);
        case 14: return FloatRect(x+0.25,y+0.25,0.5,0.5);
        case 15: return FloatRect(x,y,0.0,0.0);
    }
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
