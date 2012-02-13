#include <cmath>
#include <iostream>
using namespace std;

int main()
{
    double x11,y11,z11,x12,y12,z12;// координаты точек первой прямой
    double x21,y21,z21,x22,y22,z22;// координаты точек второй прямой
    double a1,b1,c1;// координаты и скорость первого муравья
    double a2,b2,c2,v;
    cin >> x11 >> y11 >> z11 >> x12 >> y12 >> z12;
    cin >> x21 >> y21 >> z21 >> x22 >> y22 >> z22;
    cin >> a1 >> b1 >> c1;
    cin >> a2 >> b2 >> c2;
    cin >> v;
    bool f11=false,f21=false,f12=false,f22=false;
    double t;   
    double A1,A2,A3,A4,B1,B3,D2,D4,C1,C2,C3,C4;
    A1= y12-y11;
    A2=z12-z11;
    A3=y22-y21;
    A4=z22-z21;
    B1=-x12+x11;
    B3=-x22+x21;
    D2=B1;
    D4=B3;
    C1=-x11*y12+x12*y11;
    C2=-x11*z12+x12*z11;
    C3=-x21*y22+x22*y21;
    C4=-x21*z22+x22*z21;
    if(A1*a1+B1*b1+C1==0&&A2*a1+D2*c1+C2==0)f11=true;
    if(A3*a1+B3*b1+C3==0&&A4*a1+D4*c1+C4==0)f21=true;
    if(A1*a2+B1*b2+C1==0&&A2*a2+D2*c2+C2==0)f12=true;
    if(A3*a2+B3*b2+C3==0&&A4*a2+D4*c2+C4==0)f22=true;
    if(f11==true&&f12==true||f21==true&&f22==true||(f11&&f12&&f21&&f22))
    {
        if(a1==a2&&b1==b2&&c1==c2) t=0;
        else t=sqrt((a1-a2)*(a1-a2)+(b1-b2)*(b1-b2)+(c1-c2)*(c1-c2))/v;
    }
    else
    {
        double delta,deltaX,deltaY,deltaZ,X,Y,Z;
        delta=-D2*(A1*B3-A3*B1);
        deltaX=-D2*(-C1*B3+C3*B1);
        deltaY=-D2*(-A1*C3+A3*C1);
        deltaZ=-(A2*(-B1*C3+B3*C1)-C2*(A1*B3-A3*B1));
        X=deltaX/delta;Y=deltaY/delta;Z=deltaZ/delta;
        double u=A4*X+D4*Z+C4;
        if(A4*X+D4*Z+C4==0)
        {
             t=(sqrt((a1-X)*(a1-X)+(b1-Y)*(b1-Y)+(c1-Z)*(c1-Z))+sqrt((a2-X)*(a2-X)+(b2-Y)*(b2-Y)+(c2-Z)*(c2-Z)))/v;
        }
        else t=-1;
    }
    if(t!=-1) printf("%.5f",t);
    else printf("-1");
    return 0;
}

