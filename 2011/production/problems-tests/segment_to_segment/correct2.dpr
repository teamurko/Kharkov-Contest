{16:54:18,86}
{$I-,Q-,R-,S-,O+,H-}
{$M 1000000, 10000000}
{$apptype console}

const
  cOutFile = '';
  cInFile = '';

  inf = maxlongint shr 1;
  eps = 1e-8;

type
  int = longint;
  real = extended;

  point = record
            x, y, z : int64;
          end;  

var
  n, k : int;

  ans : real;

  p1, p2 : point;
  v1, v2 : point;

procedure OutData;
var
  i, j : int;
begin
  writeln(ans : 0 : 10);
end;

procedure Solve;
var
  i, j : int;
  sv, kt, kt2, ktt2, ktkv, kt2kv : int64;
  f, s, z : int64;
  t1, t2, buf : real;
begin
  sv:= (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y) + (p1.z - p2.z)*(p1.z - p2.z);
  kt:= (p1.x - p2.x)*v1.x + (p1.y - p2.y)*v1.y + (p1.z - p2.z)*v1.z; kt:= kt * 2;
  kt2:= (p1.x - p2.x)*v2.x + (p1.y - p2.y)*v2.y + (p1.z - p2.z)*v2.z; kt2:= - 2 * kt2;
  ktt2:= v1.x*v2.x + v1.y*v2.y + v1.z*v2.z; ktt2:= - 2 * ktt2;
  ktkv:= v1.x*v1.x + v1.y*v1.y + v1.z*v1.z;
  kt2kv:= v2.x*v2.x + v2.y*v2.y + v2.z*v2.z;

  z:= 4 * ktkv * kt2kv - ktt2 * ktt2;
  f:= kt2 * ktt2 - 2 * kt * kt2kv;
  s:= ktt2 * kt - 2 * ktkv * kt2;
  if z = 0 then
  begin
    t1:= 0;
    t2:= 0;
    if ktt2 <> 0 then t2:= - kt / ktt2;
    if kt2kv <> 0 then t2:= - kt2 / 2 / kt2kv;
    if t1 > 1 - eps then t1:= 1 else
      if t1 < eps then t1:= 0;
    if t2 > 1 - eps then t2:= 1 else
      if t2 < eps then t2:= 0;
    buf:= sv + kt * t1 + kt2 * t2 + ktt2 * t1 * t2
      + ktkv * t1 * t1 + kt2kv * t2 * t2;
    if ans > buf then ans:= buf;
    t1:= 1;
    t2:= 1;
    if ktt2 <> 0 then t2:= (- kt - 2*ktkv) / ktt2;
    if kt2kv <> 0 then t2:= (- kt2 - ktt2) / 2 / kt2kv;
    if t1 > 1 - eps then t1:= 1 else
      if t1 < eps then t1:= 0;
    if t2 > 1 - eps then t2:= 1 else
      if t2 < eps then t2:= 0;
    buf:= sv + kt * t1 + kt2 * t2 + ktt2 * t1 * t2
      + ktkv * t1 * t1 + kt2kv * t2 * t2;
    if ans > buf then ans:= buf;
  end else
  begin
    t1:= f / z;
    t2:= s / z;
    if t1 > 1 - eps then t1:= 1 else
      if t1 < eps then t1:= 0;
    if t2 > 1 - eps then t2:= 1 else
      if t2 < eps then t2:= 0;
    buf:= sv + kt * t1 + kt2 * t2 + ktt2 * t1 * t2
      + ktkv * t1 * t1 + kt2kv * t2 * t2;
    if ans > buf then ans:= buf;
  end;
end;

procedure swap(var p1, p2 : point);
var
  x : point;
begin
  x:= p1;
  p1:= p2;
  p2:= x;
end;

procedure ReadData;
var
  i, j : int;
begin
  read(p1.x, p1.y, p1.z);
  read(v1.x, v1.y, v1.z);
  dec(v1.x, p1.x);  dec(v1.y, p1.y);  dec(v1.z, p1.z);
  read(p2.x, p2.y, p2.z);
  read(v2.x, v2.y, v2.z);
  dec(v2.x, p2.x);  dec(v2.y, p2.y);  dec(v2.z, p2.z);
  ans:= inf;
  Solve;
  swap(p1, p2); swap(v1, v2);
  solve;
  ans:= sqrt(ans); 
end;

begin
  assign(input,cinfile);
  assign(output,coutfile);
  reset(input);
  rewrite(output);
  ReadData;
  OutData;
  close(input);
  close(output);
end.
