program solution;

{$APPTYPE CONSOLE}

uses
  SysUtils;

function check(x1,y1,x2,y2,x3,y3: Extended): Boolean;
var a,b,c: Extended;
begin
  a:=sqrt(sqr(x1-x2)+sqr(y1-y2));
  b:=sqrt(sqr(x1-x3)+sqr(y1-y3));
  c:=sqrt(sqr(x3-x2)+sqr(y3-y2));
  result:= ( (sqr(b)+sqr(c)-sqr(a))/(2*b*c) > -1e-3 ) and
           ( (sqr(a)+sqr(c)-sqr(b))/(2*a*c) > -1e-3 ) and
           ( (sqr(a)+sqr(b)-sqr(c))/(2*a*b) > -1e-3 );
end;

function max(x,y: Extended): Extended;
begin
  if (x>y) then
    result:=x
  else
    result:=y;
end;

function solve(x1,y1,x2,y2,x3,y3: Extended): Extended;
var xa,ya,xb,yb: Extended;
    a1,a2,b1,b2,c1,c2: Extended;
    x,y: Extended;
    a,b,c,d: Extended;
begin
  if (check(x1,y1,x2,y2,x3,y3)) then
    begin
      xa:=(x1+x2)/2;
      ya:=(y1+y2)/2;
      xb:=xa+(y2-y1);
      yb:=ya+(x1-x2);
      a1:=yb-ya;
      b1:=xa-xb;
      c1:=-a1*xa-b1*ya;

      xa:=(x1+x3)/2;
      ya:=(y1+y3)/2;
      xb:=xa+(y3-y1);
      yb:=ya+(x1-x3);
      a2:=yb-ya;
      b2:=xa-xb;
      c2:=-a2*xa-b2*ya;

      y:=(c1*a2-c2*a1)/(a1*b2-b1*a2);
      x:=(c1*b2-c2*b1)/(a2*b1-a1*b2);
      //Writeln(x:0:3,' ',y:0:3);
      result:=sqrt(sqr(x-x1)+sqr(y-y1));
    end
  else
    begin
      a:=sqrt(sqr(x1-x2)+sqr(y1-y2));
      b:=sqrt(sqr(x1-x3)+sqr(y1-y3));
      c:=sqrt(sqr(x3-x2)+sqr(y3-y2));
      if (a>c) then
        begin
          d:=a;
          a:=c;
          c:=d;
        end;
      if (a>b) then
        begin
          d:=a;
          a:=b;
          b:=d;
        end;
      if (b>c) then
        begin
          d:=c;
          c:=b;
          b:=d;
        end;
      result:=max(a*a*c/(a*a+c*c-b*b), b*b*c/(b*b+c*c-a*a));
    end;
end;

var x1,y1,x2,y2,x3,y3: Integer;
begin
  try
    Readln(x1,y1,x2,y2,x3,y3);
    Writeln(solve(x1,y1,x2,y2,x3,y3):0:7);
  except
    on E:Exception do
      Writeln(E.Classname, ': ', E.Message);
  end;
end.
