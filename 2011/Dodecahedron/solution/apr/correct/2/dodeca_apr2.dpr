{
  @problem  "Dodecaedron" LOO 2005-2006 (2nd tour)
  @author   Alexey Preobrazhensky
}
{$apptype console}
{$q-,r-,o+}
var
  n: integer;
                         
begin
  reset(input, 'dodeca.in');
  rewrite(output, 'dodeca.out');

  read(n);
  writeln(7-2*n);
end.