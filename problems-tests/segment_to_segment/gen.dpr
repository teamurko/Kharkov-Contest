const
  NMAX = 1000;
var
  z, l, i, x, y, n : integer;
begin
  randomize;
  x := random(NMAX);
  y := random(NMAX);
  z := random(NMAX);
  writeln(x, ' ', y, ' ', z);
  x := random(NMAX);
  y := random(NMAX);
  z := random(NMAX);
  writeln(x, ' ', y, ' ', z);
  x := random(NMAX);
  y := random(NMAX);
  z := random(NMAX);
  writeln(x, ' ', y, ' ', z);
  x := random(NMAX);
  y := random(NMAX);
  z := random(NMAX);
  writeln(x, ' ', y, ' ', z);
end.