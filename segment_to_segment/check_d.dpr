{$A+,B-,C+,D+,E-,F-,G+,H+,I+,J-,K-,L+,M-,N+,O-,P+,Q+,R+,S+,T-,U-,V+,W-,X+,Y+,Z1}
{$MINSTACKSIZE $00004000}
{$MAXSTACKSIZE $00100000}
{$IMAGEBASE $00400000}
{$APPTYPE CONSOLE}
uses
    testlib, sysutils;

const
    EPS = 1E-5;

var
    a, b : extended;

begin
    DecimalSeparator := '.';
  a := ouf.ReadReal;
  b := ans.ReadReal;

    if (abs(a - b) < EPS) then
    begin
        writeln('Ok');
        quit(_ok, '');
    end
    else
        quit(_wa, format('a = %.10f, b = %.10f, delta = %.10f', [a, b, abs(b - a)]));
end.
