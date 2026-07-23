#include "/home/codeleaded/System/Static/Library/WindowEngine.h"
#include "/home/codeleaded/System/Static/Library/Splines.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#include "/home/codeleaded/System/Static/Library/Geometry.h"

#define SHAPE_NONE      0
#define SHAPE_RECT      1
#define SHAPE_LINE      2
#define SHAPE_CIRCLE    3
#define SHAPE_SPLINE    4
#define SHAPE_TRIANGLE  5

unsigned int SHAPE_POINTS[] = {
    0U,2U,2U,2U,3U,3U
};

Rect ScreenRect;
TransformedView tv;
Vector shapes;
Vec2* Selected = NULL;


typedef struct Shape {
    unsigned int type;
    unsigned int points;
    Vec2* Memory;
} Shape;

Shape Shape_New(unsigned int type,Vec2* Memory){
    Shape s;
    s.type = type;
    s.points = SHAPE_POINTS[type];
    s.Memory = malloc(sizeof(Vec2) * s.points);
    memcpy(s.Memory,Memory,sizeof(Vec2) * s.points);
    return s;
}
Vec2* Shape_Update(Shape* s,Vec2 p){
    for(int i = 0;i<s->points;i++){
        const Vec2 delta = Vec2_Sub(s->Memory[i],p);
        const float len = Vec2_Mag(delta);

        if(len < 1.0f){
            return s->Memory + i;
        }
    }

    return NULL;
}
Vector Shape_Collision(Shape* s,Shape* other){
    if(s->type == SHAPE_RECT){
        if(other->type == SHAPE_RECT){
            const Vec2 rp = { F32_Min(s->Memory[0].x,s->Memory[1].x),F32_Min(s->Memory[0].y,s->Memory[1].y) };
            const Vec2 rd = { F32_Abs(s->Memory[0].x-s->Memory[1].x),F32_Abs(s->Memory[0].y-s->Memory[1].y) };
            const Vec2 rpo = { F32_Min(other->Memory[0].x,other->Memory[1].x),F32_Min(other->Memory[0].y,other->Memory[1].y) };
            const Vec2 rdo = { F32_Abs(other->Memory[0].x-other->Memory[1].x),F32_Abs(other->Memory[0].y-other->Memory[1].y) };

            return Intersections_Rect_Rect(
                Rect_New(rp,rd),
                Rect_New(rpo,rdo)
            );
        }else if(other->type == SHAPE_LINE){
            const Vec2 rp = { F32_Min(s->Memory[0].x,s->Memory[1].x),F32_Min(s->Memory[0].y,s->Memory[1].y) };
            const Vec2 rd = { F32_Abs(s->Memory[0].x-s->Memory[1].x),F32_Abs(s->Memory[0].y-s->Memory[1].y) };

            return Intersections_Rect_Line(
                Rect_New(rp,rd),
                Line_New(other->Memory[0],other->Memory[1])
            );
        }else if(other->type == SHAPE_CIRCLE){
            const Vec2 rp = { F32_Min(s->Memory[0].x,s->Memory[1].x),F32_Min(s->Memory[0].y,s->Memory[1].y) };
            const Vec2 rd = { F32_Abs(s->Memory[0].x-s->Memory[1].x),F32_Abs(s->Memory[0].y-s->Memory[1].y) };
            const float rad = Vec2_Mag(Vec2_Sub(other->Memory[1],other->Memory[0]));

            return Intersections_Rect_Circle(
                Rect_New(rp,rd),
                Circle_New(other->Memory[0],rad)
            );
        }else if(other->type == SHAPE_SPLINE){

        }else if(other->type == SHAPE_TRIANGLE){
            const Vec2 rp = { F32_Min(s->Memory[0].x,s->Memory[1].x),F32_Min(s->Memory[0].y,s->Memory[1].y) };
            const Vec2 rd = { F32_Abs(s->Memory[0].x-s->Memory[1].x),F32_Abs(s->Memory[0].y-s->Memory[1].y) };

            return Intersections_Rect_Triangle(
                Rect_New(rp,rd),
                Triangle_New(other->Memory[0],other->Memory[1],other->Memory[2])
            );
        }
    }else if(s->type == SHAPE_LINE){
        if(other->type == SHAPE_RECT){
            const Vec2 rp = { F32_Min(other->Memory[0].x,other->Memory[1].x),F32_Min(other->Memory[0].y,other->Memory[1].y) };
            const Vec2 rd = { F32_Abs(other->Memory[0].x-other->Memory[1].x),F32_Abs(other->Memory[0].y-other->Memory[1].y) };

            return Intersections_Rect_Line(
                Rect_New(rp,rd),
                Line_New(s->Memory[0],s->Memory[1])
            );
        }else if(other->type == SHAPE_LINE){
            return Vector_MBuild(sizeof(Vec2),1,(Vec2[]){
                Intersections_Line_Line(
                    Line_New(s->Memory[0],s->Memory[1]),
                    Line_New(other->Memory[0],other->Memory[1])
                )
            });
        }else if(other->type == SHAPE_CIRCLE){
            const float rad = Vec2_Mag(Vec2_Sub(s->Memory[1],s->Memory[0]));

            return Intersections_Circle_Line(
                Circle_New(other->Memory[0],rad),
                Line_New(s->Memory[0],s->Memory[1])
            );
        }else if(other->type == SHAPE_SPLINE){

        }else if(other->type == SHAPE_TRIANGLE){
            return Intersections_Triangle_Line(
                Triangle_New(other->Memory[0],other->Memory[1],other->Memory[2]),
                Line_New(s->Memory[0],s->Memory[1])
            );
        }
    }else if(s->type == SHAPE_CIRCLE){
        if(other->type == SHAPE_RECT){
            const Vec2 rp = { F32_Min(other->Memory[0].x,other->Memory[1].x),F32_Min(other->Memory[0].y,other->Memory[1].y) };
            const Vec2 rd = { F32_Abs(other->Memory[0].x-other->Memory[1].x),F32_Abs(other->Memory[0].y-other->Memory[1].y) };
            const float rad = Vec2_Mag(Vec2_Sub(s->Memory[1],s->Memory[0]));

            return Intersections_Circle_Rect(
                Circle_New(s->Memory[0],rad),
                Rect_New(rp,rd)
            );
        }else if(other->type == SHAPE_LINE){
            const float rad = Vec2_Mag(Vec2_Sub(s->Memory[1],s->Memory[0]));

            return Intersections_Circle_Line(
                Circle_New(s->Memory[0],rad),
                Line_New(other->Memory[0],other->Memory[1])
            );
        }else if(other->type == SHAPE_CIRCLE){
            const float rad = Vec2_Mag(Vec2_Sub(s->Memory[1],s->Memory[0]));
            const float rado = Vec2_Mag(Vec2_Sub(other->Memory[1],other->Memory[0]));

            return Intersections_Circle_Circle(
                Circle_New(s->Memory[0],rad),
                Circle_New(other->Memory[0],rado)
            );
        }else if(other->type == SHAPE_SPLINE){

        }else if(other->type == SHAPE_TRIANGLE){
            const float rad = Vec2_Mag(Vec2_Sub(s->Memory[1],s->Memory[0]));

            return Intersections_Circle_Triangle(
                Circle_New(s->Memory[0],rad),
                Triangle_New(other->Memory[0],other->Memory[1],other->Memory[2])
            );
        }
    }else if(s->type == SHAPE_SPLINE){
        
    }else if(s->type == SHAPE_TRIANGLE){
        if(other->type == SHAPE_RECT){
            const Vec2 rp = { F32_Min(other->Memory[0].x,other->Memory[1].x),F32_Min(other->Memory[0].y,other->Memory[1].y) };
            const Vec2 rd = { F32_Abs(other->Memory[0].x-other->Memory[1].x),F32_Abs(other->Memory[0].y-other->Memory[1].y) };

            return Intersections_Triangle_Rect(
                Triangle_New(s->Memory[0],s->Memory[1],s->Memory[2]),
                Rect_New(rp,rd)
            );
        }else if(other->type == SHAPE_LINE){
            return Intersections_Triangle_Line(
                Triangle_New(s->Memory[0],s->Memory[1],s->Memory[2]),
                Line_New(other->Memory[0],other->Memory[1])
            );
        }else if(other->type == SHAPE_CIRCLE){
            const float rad = Vec2_Mag(Vec2_Sub(other->Memory[1],other->Memory[0]));

            return Intersections_Triangle_Circle(
                Triangle_New(s->Memory[0],s->Memory[1],s->Memory[2]),
                Circle_New(other->Memory[0],rad)
            );
        }else if(other->type == SHAPE_SPLINE){

        }else if(other->type == SHAPE_TRIANGLE){
            return Intersections_Triangle_Triangle(
                Triangle_New(s->Memory[0],s->Memory[1],s->Memory[2]),
                Triangle_New(other->Memory[0],other->Memory[1],other->Memory[2])
            );
        }
    }
}
void Shape_Render(Shape* s){
    for(int i = 0;i<s->points;i++){
        const Vec2 p = TransformedView_WorldScreenPos(&tv,s->Memory[i]);
        const Vec2 d = TransformedView_WorldScreenLength(&tv,(Vec2){ 1.0f,1.0f });
        Circle_R_RenderWire(WINDOW_STD_ARGS,p,d,RED,1.0f);
    }

    if(s->type == SHAPE_RECT){
        const Vec2 rp = { F32_Min(s->Memory[0].x,s->Memory[1].x),F32_Min(s->Memory[0].y,s->Memory[1].y) };
        const Vec2 rd = { F32_Abs(s->Memory[0].x-s->Memory[1].x),F32_Abs(s->Memory[0].y-s->Memory[1].y) };
        const Vec2 sp = TransformedView_WorldScreenPos(&tv,rp);
        const Vec2 sd = TransformedView_WorldScreenLength(&tv,rd);
        Rect_RenderXWire(WINDOW_STD_ARGS,sp,sd,WHITE,1.0f);
    }else if(s->type == SHAPE_LINE){
        const Vec2 p1 = TransformedView_WorldScreenPos(&tv,s->Memory[0]);
        const Vec2 p2 = TransformedView_WorldScreenPos(&tv,s->Memory[1]);
        Line_RenderX(WINDOW_STD_ARGS,p1,p2,WHITE,1.0f);
    }else if(s->type == SHAPE_CIRCLE){
        const float rad = Vec2_Mag(Vec2_Sub(s->Memory[1],s->Memory[0]));
        const Vec2 sp = TransformedView_WorldScreenPos(&tv,s->Memory[0]);
        const Vec2 sd = TransformedView_WorldScreenLength(&tv,(Vec2){ rad,rad });
        Circle_R_RenderWire(WINDOW_STD_ARGS,sp,sd,WHITE,1.0f);
    }else if(s->type == SHAPE_SPLINE){
        
    }else if(s->type == SHAPE_TRIANGLE){
        const Vec2 p1 = TransformedView_WorldScreenPos(&tv,s->Memory[0]);
        const Vec2 p2 = TransformedView_WorldScreenPos(&tv,s->Memory[1]);
        const Vec2 p3 = TransformedView_WorldScreenPos(&tv,s->Memory[2]);
        Triangle_RenderXWire(WINDOW_STD_ARGS,p1,p2,p3,WHITE,1.0f);
    }
}
void Shape_Free(Shape* s){
    if(s->Memory) free(s->Memory);
    s->Memory = NULL;
}

void Setup(AlxWindow* w){
    tv = TransformedView_Make(
        (Vec2){ GetWidth(),GetHeight() },
        (Vec2){ 0.0f,0.0f },
        (Vec2){ 0.025f,0.025f },
        (float)GetWidth() / (float)GetHeight()
    );
    shapes = Vector_New(sizeof(Shape));
}
void Update(AlxWindow* w){
    TransformedView_HandlePanZoom(&tv,window.Strokes,GetMouse());
	ScreenRect = TransformedView_ScreenWorldRect(&tv,(Rect){ 0.0f,0.0f,GetWidth(),GetHeight() });
    const Vec2 w_mouse = TransformedView_ScreenWorldPos(&tv,GetMouse());

    if(Stroke(ALX_MOUSE_L).PRESSED){
        Selected = NULL;

        for(int i = 0;i<shapes.size;i++){
            Shape* s = (Shape*)Vector_Get(&shapes,i);
            Vec2* const p = Shape_Update(s,w_mouse);
            Selected = p;

            if(Selected) break;
        }
    }else if(Stroke(ALX_MOUSE_L).RELEASED){
        Selected = NULL;
    }else if(Selected){
        *Selected = w_mouse;
    }

    if(Stroke(ALX_KEY_1).PRESSED){
        Vector_Push(&shapes,(Shape[]){ Shape_New(SHAPE_RECT,(Vec2[]){ w_mouse,w_mouse }) });
    }else if(Stroke(ALX_KEY_2).PRESSED){
        Vector_Push(&shapes,(Shape[]){ Shape_New(SHAPE_LINE,(Vec2[]){ w_mouse,w_mouse }) });
    }else if(Stroke(ALX_KEY_3).PRESSED){
        Vector_Push(&shapes,(Shape[]){ Shape_New(SHAPE_CIRCLE,(Vec2[]){ w_mouse,w_mouse }) });
    }else if(Stroke(ALX_KEY_4).PRESSED){
        Vector_Push(&shapes,(Shape[]){ Shape_New(SHAPE_SPLINE,(Vec2[]){ w_mouse,w_mouse,w_mouse }) });
    }else if(Stroke(ALX_KEY_5).PRESSED){
        Vector_Push(&shapes,(Shape[]){ Shape_New(SHAPE_TRIANGLE,(Vec2[]){ w_mouse,w_mouse,w_mouse }) });
    }

    Clear(DARK_BLUE);

    for(int i = 0;i<shapes.size;i++){
        Shape* s = (Shape*)Vector_Get(&shapes,i);
        Shape_Render(s);

        for(int j = i + 1;j<shapes.size;j++){
            Shape* other = (Shape*)Vector_Get(&shapes,j);
            Vector Ips = Shape_Collision(s,other);
            
            for(int i = 0;i<Ips.size;i++){
                const Vec2 sp = TransformedView_WorldScreenPos(&tv,*(Vec2*)Vector_Get(&Ips,i));
                const Vec2 sd = TransformedView_WorldScreenLength(&tv,(Vec2){ 0.5f,0.5f });
                Circle_R_RenderWire(WINDOW_STD_ARGS,sp,sd,YELLOW,1.0f);
            }
            
            Vector_Free(&Ips);
        }
    }
}
void Delete(AlxWindow* w){
    for(int i = 0;i<shapes.size;i++){
        Shape* s = (Shape*)Vector_Get(&shapes,i);
        Shape_Free(s);
    }
    Vector_Free(&shapes);
}

int main(){
    if(Create("Collision Shapes",800,600,2,2,Setup,Update,Delete))
        Start();
    return 0;
}