#include "Canvas.hpp"
#include "Platform.hpp"
#include "stdio.h"
#include "MathUtil.hpp"
#include <random>
#include <vector>
#include <queue>

#define MAX_LVL 5
#define PART_COUNT 5
struct Particle
{
    float x,y;
    int rad;
    float velX,velY;
};



struct Node
{
    static std::queue<Node*> NodePool;

    int x,y;
    int w,h;
    int lvl;

    Node(int _x, int _y, int _w, int _h, int _lvl) : x(_x), y(_y), w(_w), h(_h), lvl(_lvl)
    {
        
    }

    std::vector<Node*> children;
    std::vector<Particle> particles;


    void Add(Particle _particle)
    {

        if(particles.size() < 4 || lvl == MAX_LVL)
        {
            particles.push_back(_particle);
            //printf("[%d,%d] Added to %d, %d, %d, %d\n", _particle.x, _particle.y, x,y,w,h);
        }
        else
        {
            if(children.size() == 0)
            {
                Node* n1 = NodePool.front();
                NodePool.pop();
                Node* n2 = NodePool.front();
                NodePool.pop();
                Node* n3 = NodePool.front();
                NodePool.pop();
                Node* n4 = NodePool.front();
                NodePool.pop();

                n1->Set(x,y, w/2, h/2, lvl+1);
                children.push_back(n1);
                n2->Set(x+w/2,y, w/2, h/2,lvl+1);
                children.push_back(n2);
                n3->Set(x,y+h/2, w/2, h/2,lvl+1);
                children.push_back(n3);
                n4->Set(x+w/2,y+h/2, w/2, h/2,lvl+1);
                children.push_back(n4);


                for(auto _p : particles)
                {
                    for(auto _node : children)
                    {
                        if(_node->Inside(_p))
                        {
                            _node->Add(_p);
                            break;
                        }
                    }
                }
            }
            
             for(auto _node : children)
            {
                if(_node->Inside(_particle))
                {
                    _node->Add(_particle);
                    break;
                }
            }
            
        }
        
    }


    int Inside(Particle& _particle)
    {

        float _px0 = _particle.x-_particle.rad;
        float _py0 = _particle.y-_particle.rad;
        float _px1 = _particle.x+_particle.rad;
        float _py1 = _particle.y+_particle.rad;
        
        
        if( (_px0 >= x && _px0 <= (x+w)) || (_px1 >= x && _px1 <= (x+w)) &&
            (_py0 >= y && _py0 <= (y+h)) || (_py1 >= y && _py1 <= (y+h)) )
        //if(_particle.x >= x && _particle.x <= x+w && _particle.y >= y && _particle.y <= y+h)
            return 1;
        else
            return 0;
    }

    void Print()
    {
        if(children.size() == 0)
        {
            for(auto& p : particles)
            {
                printf("[%d, %d, %d, %d] -> [%d, %d]\n", x,y,w,h, p.x, p.y);
            }
        }
        else
        {
            for(auto n : children)
                n->Print();
        }
        
    }

    void Draw(Canvas& _canvas, Color& _colPoint,  Color& _colGrid)
    {
        if(children.size() == 0)
        {
            for(auto& p : particles)
            {
                _canvas.DrawCircle((int)p.x, (int)p.y, p.rad, _colPoint);
                //_canvas.DrawPixel((int)p.x, (int)p.y, _colPoint);
                _canvas.DrawRectangle(x,y,w,h, _colGrid);
                //printf("[%d, %d, %d, %d] -> [%d, %d]\n", x,y,w,h, p.x, p.y);
            }
        }
        else
        {
            for(auto n : children)
                n->Draw(_canvas, _colPoint, _colGrid);
        }
    }

    void Set(int _x, int _y, int _w, int _h, int _lvl)
    {
        for(auto c : children)
            NodePool.push(c);
        children.clear();
        particles.clear();
        x = _x;
        y = _y;
        w = _w;
        h = _h;
        lvl = _lvl;
    }

};

std::queue<Node*> Node::NodePool;


Int_32 h = 128;
Int_32 w = 128;
Canvas canvas("Canvas Demo", w, h, 8, false);

Color colors[3] = { Color(0xffff0000), Color(0xff00ff00), Color(0xff0000ff)};

Float_32 rot = 0;
Mat3x3 rotMat;
Mat3x3 sclMat;
Mat3x3 transMat;
std::vector<Particle> particles;

void init()
{

    for(int i=0; i<1024; i++)
    {
        Node::NodePool.push(new Node(0,0,0,0,0));
    }
    
    for(int i=0; i<PART_COUNT; i++)
    {
        particles.push_back(Particle{ (float)(rand()%w), (float)(rand()%h), 5, (float)(rand()%3)-1, (float)(rand()%3)-1});
    }
   


    // print tree
    

    printf("Initialized\n");
    //root->Print();

}


void ConstructQuadTree(Node* root, float DeltaTime)
{
    //particles.clear();
    srand(20);
    root->Set(0,0,w,h,0);

    
    for(int i=0; i<PART_COUNT; i++)
    {
        //particles[i].x += particles[i].velX*DeltaTime;
        //particles[i].y += particles[i].velY*DeltaTime;
        if(particles[i].x + particles[i].velX <= 0 || particles[i].x + particles[i].velX >w)
            particles[i].velX = -particles[i].velX;
        if(particles[i].y + particles[i].velY <= 0 || particles[i].y + particles[i].velY >h)
            particles[i].velY = -particles[i].velY;

        //particles.push_back(Particle{rand()%w, rand()%h, 1,});
        root->Add(particles[i]);
    }
}

int load = 0;

void update()
{

    // for(int x=0; x<w; x++)
    // {
    //     for(int y=0; y<h; y++)
    //     {
    //         Color c = colors[rand()%3];
    //         canvas.DrawPixel(x,y,c);
    //     }
    // }

    Color r = colors[0];
    Color g = colors[1];
    Color b = colors[2];

    Color red(0xff, 0,0,0xff, BGRA);
    Color blue(0, 0,0xff,0xff, BGRA);
    Color green(0, 0xff,0, 0xff, BGRA);

    Vec2f p0(canvas.Width/2,canvas.Height/2);
    Vec2f p1(canvas.Width/1.3,canvas.Height/2);
    
    rot -= canvas.DeltaTime * 0.001f;
    rotMat = Mat3x3::Identity();
    rotMat(0,0) = cosf(rot);
    rotMat(0,1) = -sinf(rot);
    rotMat(1,0) = sinf(rot);
    rotMat(1,1) = cosf(rot);
    
    sclMat = Mat3x3::Identity();
    sclMat(0,0) = 2;
    sclMat(1,1) = 2;


    p1 -= p0;
    p1 = rotMat * p1;
    p1 += p0;

    // canvas.DrawFilledCircle(p0.x, p0.y, canvas.Width/4, red);
    // canvas.DrawCircle(p0.x, p0.y, canvas.Width/4, blue);

    static Uint_32 _id1  = 0;
    if(!load)
    {
        load = 1;
        _id1 = canvas.LoadImage("App/light.jpg");
    }

    Image* _img = canvas.GetImageById(_id1);

    static Float_32 _x = 0,_y=0;


    static Node* root = new Node(0,0,0,0,0);

    ConstructQuadTree(root, canvas.DeltaTime*0.0075f);
    //root->Draw(canvas, red, blue);

    Node _node1(0,0,w/2, h/2, 0);
    Node _node2(0,h/2,w/2, h/2, 0);
    Particle p{20,(float)20,5,0,0};

    canvas.DrawRectangle(0, 0, w/2,h/2,blue);
    canvas.DrawRectangle(0, h/2, w/2,h/2,blue);
    
    if(_node1.Inside(p))
    {
        canvas.DrawRectangle(0, 0, w/2,h/2,green);
    }
    if(_node2.Inside(p))
    {
        canvas.DrawRectangle(0, h/2, w/2,h/2,green);
    }
    
    canvas.DrawCircle(p.x,p.y, p.rad, blue);


    // for(auto p : particles)
    // {
    //     canvas.DrawPixel(p.x, p.y, red);
    // }


    //canvas.BlitImage(_img, _x, _y);

    //Vec2i trans{w/2, h/2};
    // Vec2f transA{(Float_32) w/2, (Float_32)h/2};
    // Vec2f transB{(Float_32) w/2-50, (Float_32)h/2-50};
    // Vec2i origin{_img->Width/2, _img->Height/2};

    // Float_32 osc = abs(sinf(rot))+0.5f;
    // Vec2f scale(2*osc,2*osc);
    // canvas.BlitImage(_img, origin, rotMat, transA, scale, LINEAR);
    //canvas.BlitImage(_img, origin, rotMat, transA, scale, LINEAR);
    //canvas.BlitImage(_img, origin, transA, scale, NEAREST);
   //canvas.BlitImage(_img, origin, transB, scale, LINEAR);
    //canvas.BlitImage(_img, trans, origin);
    //canvas.DrawLine(p0.x, p0.y, p1.x, p1.y, r);

    //canvas.DrawRectangle(p0.x - canvas.Width/4, p0.y - canvas.Height/4, canvas.Width/2, canvas.Height/2, r);

    //canvas.DrawLine(p0.x, p0.y, p3.x, p3.y, c);
    //printf("Frame time: %f ms\n", canvas.DeltaTime);


}

void close()
{
    printf("Closed\n");
}

int main()
{
    canvas.SetInitFunc(init);
    canvas.SetUpdateFunc(update);
    canvas.SetCloseFunc(close);

    canvas.Start();

    return 0;
}