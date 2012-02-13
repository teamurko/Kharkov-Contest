{$apptype console}
uses
    testlib, sysutils;
type integer=longint;
var
    ja, pa: string;
    jea,pea:extended;
    code:integer;
begin
    ja := ans.readstring;
    pa := ouf.readstring;
    pa := trim(pa);

    if pa = '' then begin
        quit(_pe, 'Empty file');
    end;

    if not (pa[1] in ['-', '.','e','E','0'..'9']) then
        quit(_pe, 'invalid character in answer');
    {for i := 2 to length(pa) do
        if not (pa[i] in ['0'..'9']) then
            quit(_pe, 'invalid character in answer');}
    val(ja,jea,code);
    val(pa,pea,code);
    if code<>0 then
      quit(_pe,'invalid real number');
    if abs(pea-jea)>1e-6 then
{    if ja <> pa then}
        quit(_wa, 'expected: "' + ja + '", found: "' + pa + '"');

    quit(_ok, pa);
end.