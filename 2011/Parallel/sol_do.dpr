

program solution;
const
 nmax = 7;
 inf = 1e100;
 eps = 1e-8;
 cinfile = 'input.txt';
 coutfile = 'output.txt';
type
 int = longint;
 real = extended;
 tVector = array [1..nmax] of real;
 tmatrix = array [1..nmax,1..nmax] of real;
var
 n :int;
 k :int;
 a :tvector;
 p :array [1..nmax] of tvector;
 c :array [1..nmax] of int;
 used :array [1..nmax] of boolean;
 ans :real;

procedure init;
begin
 fillchar (n,integer(@ans) - integer(@n),0);
 ans := inf;
end;

procedure readdata;
var i,j :int;
begin
 assign (input,cinfile);
 reset (input);
 read (n,k);
 for i := 1 to n do
  read (a[i]);
 for i := 1 to k do
  for j := 1 to n do
   read (p[i][j]);  
 close (input);
end;

procedure outdata;
var i,j :int;
begin
 assign (output,coutfile);
 rewrite (output);
 writeln (ans:0:18  );
 close (output);
end;

function scalar (const a,b :tvector):real;
var i :int;
begin
 result := 0;
 for i := 1 to n do
  result := result + a[i]*b[i];
end;

procedure swapReal (var a,b :real);
var tmp :real;
begin
 tmp := a;
 a := b;
 b := tmp;
end;

procedure solveLinear (var M :tmatrix; var b :tvector; n :int);
var i,j,k :int;
    max :real;
    t :real;
    cur :int;
begin
 for i := 1 to n do
  begin
   max := 0;
   cur := 0;
   for j := i to n do
    if abs(M[i][j]) > max then
     begin
      max := abs(M[i][j]);
      cur := j;
     end;
   for k := i to n do
    swapReal(M[i][k],M[cur][k]);
   swapReal(b[cur],b[i]);
   if M[i][i] < 0 then
    max := -max;
   for k := i to n do
    M[i][k] := M[i][k] / max;
   b[i] := b[i] / max;
   for j := i+1 to n do
    begin
     t := M[j][i];
     for k := i to n do
      M[j][k] := M[j][k] - t*M[i][k];
     b[j] := b[j] - t*b[i]; 
    end;
  end;
 for i := n downto 1 do
  for j := i-1 downto 1 do
   b[j] := b[j] - M[j][i]*b[i];
end;

procedure calculate;
var i,j :int;
    u :tvector;
    h :tvector;
    M :tmatrix;
    B :tvector;
    l :int;
    v :array [1..nmax] of tvector;
    t :real;
begin
 fillchar (u,sizeof(u),0);
 fillchar (m,sizeof(m),0);
 fillchar (h,sizeof(h),0);
 for i := 1 to n do
  u[i] := -a[i];
 l := 0;
 for i := 1 to k do
  begin
   if c[i] = 1 then
    for j := 1 to n do
     u[j] := u[j] - p[i][j];
   if c[i] = 2 then
    begin
     inc (l);
     v[l] := p[i];
    end
  end;
 for i := 1 to l do
  for j := 1 to l do
   M[i,j] := scalar (v[i],v[j]);
 for i := 1 to l do
  B[i] := scalar (u,v[i]);
 solveLinear(M,b,l);
 for i := 1 to l do
  if (b[i] < -eps) or (b[i] > 1 + eps) then
   exit;
 h := u;
 for i := 1 to l do
  for j := 1 to n do
   h[j] := h[j] - b[i]*v[i][j];
 t := sqrt(scalar(h,h));
 if t < ans then
  ans := t;
end;

procedure rec (pos :int);
begin
 if pos > k then
  begin
   calculate;
   exit;
  end;
 c[pos] := 0;
 rec (pos+1);
 c[pos] := 1;
 rec (pos+1);
 c[pos] := 2;
 rec (pos+1);
 c[pos] := 0;
end;

procedure solve;
var i,j :int;
begin
 rec (1);
end;

begin
 init;
 readdata;
 solve;
 outdata;
end.
