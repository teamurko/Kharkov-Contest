{$apptype console}
{$o-}
type
  vector=array[1..10] of extended;
  vectorset=array[1..10] of vector;
var
  n,k:integer;
  v1:vector;
  edges:vectorset;
  ans:extended;
  nearestvec:vector;
  currentvec:vector;
  optmask:integer;
  optmask2:integer;
function scal(var x,y:vector):extended;
var
  i:integer;
  res:extended;
begin
  res:=0;
  for i:=1 to n do
    res:=res+x[i]*y[i];
  scal:=res;
end;
procedure readdata;
var
  i,j:integer;
begin
  read(n,k);
  for i:=1 to n do
    read(v1[i]);
  for i:=1 to k do
    for j:=1 to n do
      read(edges[i][j]);
  ans:=100000000;
end;
function dist(var v1:vector;var edges:vectorset;nedges:integer;
              var newv:vector):extended;
var
  i,j,l:integer;
  matr:array[1..10,0..10] of extended;
  coff:extended;
begin
  for i:=1 to nedges do
    for j:=1 to nedges do
      matr[i,j]:=scal(edges[i],edges[j]);
  for i:=1 to nedges do
    matr[i,0]:=scal(v1,edges[i]);
  {solving equations - matrix is positive}
  for i:=1 to nedges-1 do
  begin
    for j:=i+1 to nedges do
    begin
      coff:=matr[j,i]/matr[i,i];
      for l:=i to nedges do
        matr[j,l]:=matr[j,l]-matr[i,l]*coff;
      matr[j,0]:=matr[j,0]-matr[i,0]*coff;
    end;
  end;
  newv:=v1;
  for i:=nedges downto 1 do
  begin
    for j:=i+1 to nedges do
      matr[i,0]:=matr[i,0]-matr[j,0]*matr[i,j];
    matr[i,0]:=matr[i,0]/matr[i,i];
  end;
  for i:=1 to nedges do
  begin
    for j:=1 to n do
      newv[j]:=newv[j]-matr[i,0]*edges[i,j];
  end;
  dist:=scal(newv,newv);
  for i:=1 to nedges do
    newv[i]:=-matr[i,0];
end;
function isinside(nedges:integer;var newv:vector):boolean;
var
  i:integer;
begin
  isinside:=true;
  for i:=1 to nedges do
  begin
    if (newv[i]<-1e-10) or (newv[i]>1+1e-10) then
    begin
      isinside:=false;
      exit;
    end;
  end;
end;
var
  vmask:integer;
  vmask2:integer;
procedure check(var startv:vector;vset:integer);
var
  edges1:vectorset;
  i,j:integer;
  nedges:integer;
  distance:extended;
  newv:vector;
begin
  nedges:=0;
  for i:=1 to k do
  begin
    if odd(vset) then
    begin
      inc(nedges);
      for j:=1 to n do
        edges1[nedges][j]:=edges[i][j];
    end;
    vset:=vset div 2;
  end;
  if nedges=0 then
  begin
    distance:=scal(startv,startv);
    if (ans>distance) then
    begin
      ans:=distance;
      optmask:=vmask;
    end;
  end
  else
  begin
    distance:=dist(startv,edges1,nedges,newv);
    if isinside(nedges,newv) and (ans>distance) then
    begin
      ans:=distance;
      optmask:=vmask;
      for i:=1 to nedges do
        nearestvec[i]:=newv[i];
    end;
  end;
end;
function nbits(i:integer):integer;
var
  res:integer;
begin
  res:=0;
  while (i>0) do
  begin
    if odd(i) then inc(res);
    i:=i div 2;
  end;
  nbits:=res;
end;
procedure add(var v1,v2:vector);
var
  i:integer;
begin
  for i:=1 to n do
    v1[i]:=v1[i]+v2[i];
end;
procedure solve;
var
  i,j,l,u,v:integer;
  startv:vector;
  bits:integer;
begin
  for i:=0 to (1 shl k)-1 do {set}
  begin
    bits:=nbits(i);
    bits:=k-bits;
    for j:=0 to (1 shl bits)-1 do
    begin
      startv:=v1;
      u:=i;v:=j;
      for l:=1 to k do
      begin
        if not odd(u) then
        begin
          if odd(v) then
            add(startv,edges[l]);
          v:=v div 2;
        end;
        u:=u div 2;
      end;
      vmask:=i;
      vmask2:=j;
      check(startv,i);
    end;
  end;
end;
begin
  {assign(input,'tests\12');
  reset(input); {}
  readdata;
  solve;
  ans:=sqrt(ans);
  writeln(ans:0:18);
  {writeln(optmask);
  writeln(optmask2);}
end.
