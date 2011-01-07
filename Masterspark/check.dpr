{Checker: standard}
{Checker details: compare one 80-bit real}
{Checker details: additive error at most eps}
{Origin: testlib-kitten, chkreal.pas}
{$I trans.inc}
program macro;
uses testlib;
const eps=1.1e-4;
var jury, cont:extended;
begin
  jury:=ans.readreal;
  cont:=ouf.readreal;
  if not ouf.seekeof then quit (_pe, 'EOF expected');
  if abs (jury-cont)>eps then quit (_wa, 'Required: '+r2s (jury)+', got: '+r2s (cont));
  quit (_ok, '�⢥�: ' + r2s (cont));
end.
