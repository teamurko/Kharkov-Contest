(*
	������ "���������".
	���-2006. 2-�� ���.
	����� �������: ����� �������.
*)

{$apptype console}
var
  n : integer;
begin
  reset(input, 'dodeca.in');
  rewrite(output, 'dodeca.out');

  read(n);
  assert((n >= 0) and (n <= 3));

  writeln(7 - 2 * n);
end.