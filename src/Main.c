#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/Splines.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#include "/home/codeleaded/System/Static/Library/ObjectNotation.h"
#include "/home/codeleaded/System/Static/Library/Geometry.h"

#define SHAPE_NONE      0
#define SHAPE_RECT      1
#define SHAPE_LINE      2
#define SHAPE_CIRCLE    3
#define SHAPE_SPLINE    4

typedef struct Shape {
    int Type;
    void* Memory;
    size_t ElementSize;
    void (*Update)(void*);
    void (*Render)(void*);
} Shape;

Shape Shape_New(int Type,void* Memory,size_t ElementSize,void (*Update)(void*),void (*Render)(void*)){
    Shape s;
    s.Type = Type;
    s.Memory = malloc(ElementSize);
    memcpy(s.Memory,Memory,ElementSize);
    s.Update = Update;
    s.Render = Render;
    return s;
}

void Shape_Free(Shape* s){
    if(s->Memory) free(s->Memory);
    s->Memory = NULL;
}

Rect ScreenRect;
TransformedView tv;
Vector Points;
Vec2* Selected = NULL;

void Setup(AlxWindow* w){
    tv = TransformedView_New((Vec2){ GetWidth(),GetHeight() });
	//TransformedView_Focus(&tv,&Me.p,Me.d);
    TransformedView_Offset(&tv,(Vec2){ -0.5f,-0.5f });
    TransformedView_Zoom(&tv,(Vec2){ 1.0f,1.0f });

    Points = Vector_New(sizeof(Vec2));
    Vector_PushCount(&Points,(Vec2[14]){ 0 },14);
}
void Update(AlxWindow* w){
    TransformedView_HandlePanZoom(&tv,window.Strokes,(Vec2){ GetMouse().x,GetMouse().y });
	ScreenRect = TransformedView_ScreenWorldRect(&tv,(Rect){ 0.0f,0.0f,GetWidth(),GetHeight() });

    if(Stroke(ALX_MOUSE_L).PRESSED){
        Selected = NULL;
        for(int i = 0;i<Points.size;i++){
            Vec2* p = (Vec2*)Vector_Get(&Points,i);
            if(Vec2_Mag(Vec2_Sub(TransformedView_WorldScreenPos(&tv,*p),GetMouse()))<10.0f){
                Selected = p;
            }
        }
    }
    if(Stroke(ALX_MOUSE_L).RELEASED){
        Selected = NULL;
    }
    if(Selected){
        *Selected = TransformedView_ScreenWorldPos(&tv,GetMouse());
    }

    Clear(DARK_BLUE);

    for(int i = 0;i<Points.size;i++){
        Vec2* p = (Vec2*)Vector_Get(&Points,i);
        RenderCircleWire(TransformedView_WorldScreenPos(&tv,*p),10.0f,GREEN,1.0f);
    }
    
    {
        Vec2 p = TransformedView_WorldScreenPos(&tv,*(Vec2*)Vector_Get(&Points,0));
        Vec2 d = TransformedView_WorldScreenLength(&tv,Vec2_Sub(*(Vec2*)Vector_Get(&Points,1),*(Vec2*)Vector_Get(&Points,0)));
        RenderRectWire(p.x,p.y,d.x,d.y,RED,1.0f);
    }

    {
        //Vec2 s = TransformedView_WorldScreenPos(&tv,*(Vec2*)Vector_Get(&Points,2));
        //Vec2 e = TransformedView_WorldScreenPos(&tv,*(Vec2*)Vector_Get(&Points,3));
        //RenderLine(s,e,RED,1.0f);
        Vec2 p = TransformedView_WorldScreenPos(&tv,*(Vec2*)Vector_Get(&Points,2));
        Vec2 d = TransformedView_WorldScreenLength(&tv,Vec2_Sub(*(Vec2*)Vector_Get(&Points,3),*(Vec2*)Vector_Get(&Points,2)));
        RenderRectWire(p.x,p.y,d.x,d.y,RED,1.0f);
    }

    {
        Vec2 s = TransformedView_WorldScreenPos(&tv,*(Vec2*)Vector_Get(&Points,0));
        Vec2 e = TransformedView_WorldScreenPos(&tv,*(Vec2*)Vector_Get(&Points,1));
        RenderLine(s,e,RED,1.0f);
    }

    {
        Vec2 p = TransformedView_WorldScreenPos(&tv,*(Vec2*)Vector_Get(&Points,4));
        Vec2 r = TransformedView_WorldScreenLength(&tv,Vec2_Sub(*(Vec2*)Vector_Get(&Points,4),*(Vec2*)Vector_Get(&Points,5)));
        RenderCircleWire(p,Vec2_Mag(r),RED,1.0f);
    }

    {
        Vec2 p1 = TransformedView_WorldScreenPos(&tv,*(Vec2*)Vector_Get(&Points,6));
        Vec2 p2 = TransformedView_WorldScreenPos(&tv,*(Vec2*)Vector_Get(&Points,7));
        Vec2 p3 = TransformedView_WorldScreenPos(&tv,*(Vec2*)Vector_Get(&Points,8));
        Vec2 p4 = TransformedView_WorldScreenPos(&tv,*(Vec2*)Vector_Get(&Points,9));

        Poly_RenderXWire(WINDOW_STD_ARGS,(Vec2[]){ p1,p2,p3,p4 },4,RED,1.0f);
        //RenderTriangleWire(p1,p2,p3,RED,1.0f);
    }

    {
        Vec2 p1 = TransformedView_WorldScreenPos(&tv,*(Vec2*)Vector_Get(&Points,9));
        Vec2 p2 = TransformedView_WorldScreenPos(&tv,*(Vec2*)Vector_Get(&Points,10));
        Vec2 p3 = TransformedView_WorldScreenPos(&tv,*(Vec2*)Vector_Get(&Points,11));
        RenderTriangleWire(p1,p2,p3,RED,1.0f);
    }

    {
        Vec2 p = TransformedView_WorldScreenPos(&tv,*(Vec2*)Vector_Get(&Points,12));
        Vec2 r = TransformedView_WorldScreenLength(&tv,Vec2_Sub(*(Vec2*)Vector_Get(&Points,12),*(Vec2*)Vector_Get(&Points,13)));
        RenderCircleWire(p,Vec2_Mag(r),RED,1.0f);
    }

    //Vector Ips = Intersections_Circle_Line((Circle){ *(Vec2*)Vector_Get(&Points,4),Vec2_Mag(Vec2_Sub(*(Vec2*)Vector_Get(&Points,4),*(Vec2*)Vector_Get(&Points,5))) },(Line){ *(Vec2*)Vector_Get(&Points,2),*(Vec2*)Vector_Get(&Points,3) });
    //Vector Ips = Intersections_Triangle_Line((Triangle){ *(Vec2*)Vector_Get(&Points,6),*(Vec2*)Vector_Get(&Points,7),*(Vec2*)Vector_Get(&Points,8) },(Line){ *(Vec2*)Vector_Get(&Points,2),*(Vec2*)Vector_Get(&Points,3) });
    //Vector Ips = Intersections_Triangle_Triangle((Triangle){ *(Vec2*)Vector_Get(&Points,6),*(Vec2*)Vector_Get(&Points,7),*(Vec2*)Vector_Get(&Points,8) },(Triangle){ *(Vec2*)Vector_Get(&Points,9),*(Vec2*)Vector_Get(&Points,10),*(Vec2*)Vector_Get(&Points,11) });
    //Vector Ips = Intersections_Circle_Triangle((Circle){ *(Vec2*)Vector_Get(&Points,4),Vec2_Mag(Vec2_Sub(*(Vec2*)Vector_Get(&Points,4),*(Vec2*)Vector_Get(&Points,5))) },(Triangle){ *(Vec2*)Vector_Get(&Points,9),*(Vec2*)Vector_Get(&Points,10),*(Vec2*)Vector_Get(&Points,11) });
    //Vector Ips = Intersections_Circle_Circle((Circle){ *(Vec2*)Vector_Get(&Points,4),Vec2_Mag(Vec2_Sub(*(Vec2*)Vector_Get(&Points,4),*(Vec2*)Vector_Get(&Points,5))) },(Circle){ *(Vec2*)Vector_Get(&Points,12),Vec2_Mag(Vec2_Sub(*(Vec2*)Vector_Get(&Points,12),*(Vec2*)Vector_Get(&Points,13))) });
    //Vector Ips = Intersections_Circle_Rect((Circle){ *(Vec2*)Vector_Get(&Points,4),Vec2_Mag(Vec2_Sub(*(Vec2*)Vector_Get(&Points,4),*(Vec2*)Vector_Get(&Points,5))) },(Rect){ *(Vec2*)Vector_Get(&Points,0),Vec2_Sub(*(Vec2*)Vector_Get(&Points,1),*(Vec2*)Vector_Get(&Points,0)) });
    //Vector Ips = Intersections_Triangle_Rect((Triangle){ *(Vec2*)Vector_Get(&Points,6),*(Vec2*)Vector_Get(&Points,7),*(Vec2*)Vector_Get(&Points,8) },(Rect){ *(Vec2*)Vector_Get(&Points,0),Vec2_Sub(*(Vec2*)Vector_Get(&Points,1),*(Vec2*)Vector_Get(&Points,0)) });
    
    Rect r1 = (Rect){ *(Vec2*)Vector_Get(&Points,0),Vec2_Sub(*(Vec2*)Vector_Get(&Points,1),*(Vec2*)Vector_Get(&Points,0)) };
    Rect r2 = (Rect){ *(Vec2*)Vector_Get(&Points,2),Vec2_Sub(*(Vec2*)Vector_Get(&Points,3),*(Vec2*)Vector_Get(&Points,2)) };
    //Resolve_Rect_Rect(&r1,r2);
    *(Vec2*)Vector_Get(&Points,0) = r1.p;
    
    Vector Ips = Intersections_Rect_Rect(r1,r2);
    for(int i = 0;i<Ips.size;i++){
        Vec2 ip = *(Vec2*)Vector_Get(&Ips,i);
        ip = TransformedView_WorldScreenPos(&tv,ip);
        RenderCircleWire(ip,10.0f,YELLOW,1.0f);
    }
    Vector_Free(&Ips);

    //Vec2 ip = Line_LineOverlap((Line){ *(Vec2*)Vector_Get(&Points,0),*(Vec2*)Vector_Get(&Points,1) },(Line){ *(Vec2*)Vector_Get(&Points,2),*(Vec2*)Vector_Get(&Points,3) });
    //if(Point_Valid(ip)){
    //    ip = TransformedView_WorldScreenPos(&tv,ip);
    //    RenderCircleWire(ip,10.0f,YELLOW,1.0f);
    //}

    //Vec2 ip = Line_LineOverlap(l1,l2);
    //ip = TransformedView_WorldScreenPos(&tv,ip);
    //RenderCircleWire(ip,10.0f,YELLOW,1.0f);
}
void Delete(AlxWindow* w){
    Vector_Free(&Points);
}

int main(){
    if(Create("Collision Shapes",800,600,2,2,Setup,Update,Delete))
        Start();
    return 0;
}