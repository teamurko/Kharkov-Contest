type
  vect = record
    x, y, z : extended;
  end;

var
  v1, v2, v3 : vect;
  p1, p2, p3, p4 : vect;
  d, dt1, dt2, t1, t2, ans, cur : extended;

function sc(var v1, v2 : vect) : extended;
begin
  sc := v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
end;

procedure mvect(var v1, v2, r : vect);
begin
  r.x := v2.x - v1.x;
  r.y := v2.y - v1.y;
  r.z := v2.z - v1.z;
end;

procedure pr(var t : extended);
begin
  if t < 1e-17 then t := 0;
  if t > 1 - 1e-17 then t := 1;
end;

function dst(var p1, p3, v1, v2 : vect; t1, t2 : extended) : extended;
begin
  dst := sqrt(sqr(p1.x + t1 * v1.x - p3.x - t2 * v2.x) + sqr(p1.y + t1 * v1.y - p3.y - t2 * v2.y) + sqr(p1.z + t1 * v1.z - p3.z - t2 * v2.z));
end;

function solve(var p1, p2, p3, p4 : vect) : extended;
var
  ans : extended;
begin
  mvect(p1, p2, v1);
  mvect(p3, p4, v2);
  mvect(p3, p1, v3);
  d := sc(v1, v1) * sc(v2, v2) - sqr(sc(v1, v2));
  dt1 := sc(v2, v3) * sc(v1, v2) - sc(v2, v2) * sc(v1, v3);
  dt2 := -sc(v1, v2) * sc(v1, v3) + sc(v1, v1) * sc(v2, v3);
  if Abs(d) > 1e-17 then
  begin
    t1 := dt1 / d;
    t2 := dt2 / d;
    pr(t1);
    pr(t2);
    ans := dst(p1, p3, v1, v2, t1, t2);
  end else
  begin
    t1 := 0;
    t2 := 0;
    if sc(v1, v2) <> 0 then
      t2 := (sc(v1, v3) + sc(v1, v1) * t1) / sc(v1, v2);
    if sc(v2, v2) <> 0 then
      t2 := (sc(v2, v3) + sc(v1, v2) * t1) / sc(v2, v2);
    pr(t2);
    ans := dst(p1, p3, v1, v2, t1, t2);
    t1 := 1;
    t2 := 0;
    if sc(v1, v2) <> 0 then
      t2 := (sc(v1, v3) + sc(v1, v1) * t1) / sc(v1, v2);
    if sc(v2, v2) <> 0 then
      t2 := (sc(v2, v3) + sc(v1, v2) * t1) / sc(v2, v2);
    pr(t2);
    cur := dst(p1, p3, v1, v2, t1, t2);
    if ans > cur + 1e-17 then ans := cur;
  end;
  solve := ans;
end;

begin
  read(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, p3.x, p3.y, p3.z, p4.x, p4.y, p4.z);
  ans := solve(p1, p2, p3, p4);
  cur := solve(p3, p4, p1, p2);
  if ans > cur + 1e-17 then ans := cur;
  writeln(ans : 0 : 10);
end.