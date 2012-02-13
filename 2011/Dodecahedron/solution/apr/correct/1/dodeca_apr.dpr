{
  @problem  "Dodecaedron" LOO 2005-2006 (2nd tour)
  @author   Alexey Preobrazhensky
}
{$apptype console}
{$q-,r-,o+}
uses
  math;

const
  next: array[1..12, 1..5] of integer = (( 2, 3, 4, 5, 6),
                                         ( 1, 3, 6, 7, 8),
                                         ( 1, 4, 2, 8, 9),
                                         ( 1, 5, 3, 9,10),
                                         ( 1, 6, 4,10,11),
                                         ( 1, 2, 5,11, 7),
                                         (12, 8,11, 6, 2),
                                         (12, 9, 7, 2, 3),
                                         (12,10, 8, 3, 4),
                                         (12,11, 9, 4, 5),
                                         (12, 7,10, 5, 6),
                                         ( 7, 8, 9,10,11));
const
  _police = 0;
  _zubair = 1;

var
  n: integer;
  d: array[1..12, 1..12, 1..12, _police.._zubair] of integer;
  g: array[1..12, 1..12] of integer;

procedure readdata;
begin
  read(n);
end;

procedure solve;
var
  inf, mv, i, j, k, t, m: integer;
  chg: boolean;
begin
  fillchar(g, sizeof(g), 63);
  inf := g[1, 1];
  for i := 1 to 12 do
    g[i, i] := 0;

  for i := 1 to 12 do
    for j := 1 to 5 do begin
      g[i, next[i, j]] := 1;
      g[next[i, j], i] := 1;
    end;

  for i := 1 to 12 do
    for j := 1 to 12 do
      for k := 1 to 12 do
        if (g[j, i] < inf) and (g[i, k] < inf) and (g[j, k] > g[j, i] + g[i, k]) then
          g[j, k] := g[j, i] + g[i, k];

  fillchar(d, sizeof(d), -1);
  for i := 1 to 12 do
    for j := 1 to 12 do
      for k := 1 to 12 do
        for t := _police to _zubair do
          if ((i = k) or (j = k)) then
            d[i, j, k, t] := 0;

  chg := true;
  while (chg) do begin
    chg := false;
    for i := 1 to 12 do
      for j := 1 to 12 do
        for k := 1 to 12 do
          for t := _police to _zubair do if (d[i, j, k, t] = -1) then begin
            case t of
              _police: begin
                mv := inf;          
                for m := 1 to 5 do begin
                  if (d[next[i, m], j, k, 1 - t] <> -1) then mv := min(mv, d[next[i, m], j, k, 1 - t] + 1);
                  if (d[i, next[j, m], k, 1 - t] <> -1) then mv := min(mv, d[i, next[j, m], k, 1 - t] + 1);
                end;
                if (mv <> inf) then begin
                  d[i, j, k, t] := mv;
                  chg := true;
                end;
              end;

              _zubair: begin
                mv := -1;
                for m := 1 to 5 do begin
                  if (d[i, j, next[k, m], 1 - t] = -1) then begin
                    mv := -1;
                    break;
                  end else mv := max(mv, d[i, j, next[k, m], 1 - t] + 1);
                end;
                if (mv <> -1) then begin
                  d[i, j, k, t] := mv;
                  chg := true;
                end;
              end;
            end;
          end;
  end;

  mv := 0;
  for i := 1 to 12 do
    for j := 1 to 12 do
      if (g[i, j] = n) then begin
        mv := 0;
        for k := 1 to 12 do
          mv := max(mv, d[i, j, k, _police]);
      end;
  writeln(mv);
end;

begin
  reset(input, 'dodeca.in');
  rewrite(output, 'dodeca.out');

  readdata;
  solve;
end.