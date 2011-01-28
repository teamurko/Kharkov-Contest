var
  x1, y1, z1, x2, y2, z2, x, y, z, dx, dy, dz, t, ans : extended;
begin
  read(x1, y1, z1, x2, y2, z2, x, y, z);
  dx := x2 - x1;
  dy := y2 - y1;
  dz := z2 - z1;
  if (dx <> 0) or (dy <> 0) or (dz <> 0) then
    t := ((x - x1) * dx + (y - y1) * dy + (z - z1) * dz) / (sqr(dx) + sqr(dy) + sqr(dz))
  else
    t := 0;
  if t < 1e-17 then t := 0;
  if t > 1 - 1e-17 then t := 1;
  x1 := x1 + dx * t;
  y1 := y1 + dy * t;
  z1 := z1 + dz * t;
  ans := sqrt(sqr(x - x1) + sqr(y - y1) + sqr(z - z1));
  writeln(ans : 0 : 10);
end.